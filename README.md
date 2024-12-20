# BusBricks 
OSI-oriented modularization to customize communication between Layer-7-services on interchangeable Layer-1/2-interfaces. <br>
The architecture is usable for all MCUs of the Arduino-platform.
<br>
📖 [Checkout detailed Documentation](https://f-l-x-s.github.io/BusBricks/)

## Vision
Provide configurable software-modules to customize the communication between services, hosted on different MCUs. <br> 
BusBricks are templates to derive classes for mapping service-specific rules to different bus-systems. 
The communication between the hosting devices is therefore separated from the inter-service-communication. 
That means, the communication between the services can follow it's own (from the chosen communication-interface independent) rules. <br>

### example use-case: Messenger-service 
Hosting a messenger on different host-devices. Some of those hosts support a serial commuication for modbus-RTU, some only support I2C.<br>
#### Roadmap:
###### Message-Service implementation
- [x] [Message](lib/MessageService/Message.h) content and payload definition
- [x] Implementation of the [Messenger-Service](lib/MessageService/MessageService.h)

###### Modbus-RTU implementation
- [x] Implementation of the [Modbus-RTU communication-interface](lib/ModbusRTU/CommInterface_modbusRTU.h)
- [x] [Modbus-RTU frame](lib/ModbusRTU/Frame_modbusRTU.h) definition
- [x] Implementation of the [Modbus-RTU Service-interface](lib/ModbusRTU/ServiceInterface_modbusRTU.h)
- [x] Registration of [Messenger-Service](lib/MessageService/MessageService.h)-instance and [Error-Service](lib/ErrorService/ErrorService.h)-instance within an [service-cluster](lib/Service/ServiceCluster.h) in [main](src/main.cpp)
- [x] Implementation of [Modbus-RTU Service-Interface](lib/Interface/ServiceInterface.h) to process payload from/for the registered services and frames of from/for [Modbus-RTU communication-interface](lib/ModbusRTU/CommInterface_modbusRTU.h)
- [ ] Setup of the [Modbus-RTU Service-interface](lib/ModbusRTU/ServiceInterface_modbusRTU.h) for Master-mode (polling and message-forwarding)
- [ ] Response-Timeout for Message-Service

###### I2C implementation
- [ ] Implementation of a [communication-interface](lib/Interface/CommInterface.h) derived I2C-communication-interface
- [ ] Definition of a [Frame](lib/Frame/Frame.h) derived I2C-Frame-class
- [ ] Setup of an I2C service-interface
- [ ] Setup [service-template](lib/Service/Service.h) and [service-cluster](lib/Service/ServiceCluster.h) for association in multiple [service-interfaces](lib/Interface/ServiceInterface.h)
  
###### User-interaction
- [x] Setup of a tiny chat-interface in [main](src/main.cpp)

## Basic Concepts
### Service- and Communication-Layer  
The separation of inter-service- and inter-mcu-communication-rules requires a separation of the software modules. <br><br>

##### Service-Layer
**Services** are defined in the **service-layer**. All implementations in the service-layer are Service-specific and therefore they should not depend on the chosen communication-interface. That means, the service-layer-modules should be **reusable, independent which communication-interface the host-device supports**. 

##### Communication-Layer 
Modules being part of the Communication-Layer are specific for the chosen bus (e.g. I2C, OneWire, Musbus-rtu...) the host device provides. They are responsible for **applying the bus-protocol-specific rules (e.g. CRC-check and -calculation, communication-timeouts, framelength...) to the content (PDU)** provided by the services. <br>

### Content and representation
The cascading of processing information and the rules applied to it lead to the concept of *content- and representation*.<br>
In every iteration, an information is processed and the rules of the next level, closer to the physical layer are applied to it, the information closer to the format the service is able to process is called **Content**.<br> 
Conversely, the format, the information has after applying the rules of the next level closer to "Layer-0" is called **representation**.<br><br>
<img src="https://raw.githubusercontent.com/F-L-X-S/BusBricks/refs/heads/main/docs/Content_Representation.svg" alt="Content and Representation" style="width:100%;">


### Classdiagram
<img src="https://raw.githubusercontent.com/F-L-X-S/BusBricks/fa4a1927d184c56d71477509aa5243d7c93def95/docs/classdiagram.svg" alt="classdiagram" style="width:100%;">

### Protocoll-stack
<img src="https://raw.githubusercontent.com/F-L-X-S/BusBricks/refs/heads/main/docs/protocollstack.svg" alt="protocollstack" style="width:100%;">

### Stackprocessing
<img src="https://raw.githubusercontent.com/F-L-X-S/BusBricks/fa4a1927d184c56d71477509aa5243d7c93def95/docs/stackProcessing.svg" alt="stackProcessing" style="width:75%;">

### Error-handling
The Errors of a device are managed by a Service-derived [```ErrorService```](lib/ErrorService/ErrorService.h). Every component (no matter, if Service, ServiceInterface or CommInterface) can be enabled to raise errors by deriving the [```ErrorState```](lib/ErrorService/ErrorState.h) and calling ```raiseError()```. <br>
```cpp
class CommInterfaceBase: public ErrorState{
    public:
        CommInterfaceBase(): ErrorState(){};
}
```
The class-instance is now able to raise Errors and, if applicable, to handle them within the class itself. E.g.:<br>
```cpp
    // wait for Frame-timeout to ensure frame is complete, raise Error, if the silence-time is violated
    if((_clearRxBuffer()>0) & (receiveBuffer->getSize()!=0)){
      (numBytes>=MAXFRAMESIZE) ? raiseError(frameLengthError):raiseError(arbitrationError);
    } 

    // handle the Arbitration-Error with waiting for bus-silence
    if(getErrorState()==arbitrationError) while(_clearRxBuffer()!=0);
```

To handle the Error outside the instance, the Error-state of the components has to be processed within the [```ServiceInterface```](lib/Interface/ServiceInterface.h) by checking the ErrorState of the called instances and, if applicable raising the Error with the same Error-code within the ServiceInterface: <br>

```cpp
errorCodes commInterfaceErrorState = comm_interface->getErrorState();
if (commInterfaceErrorState!=noError) raiseError(commInterfaceErrorState);
```

Only Errors raised in [```ServiceInterface```](lib/Interface/ServiceInterface.h) are forwarded to an eventually registered [```ErrorService```](lib/ErrorService/ErrorService.h), that can execute error-code-specific actions (like printing the Error or broadcasting errors to the network). <br>
After handling the Error of the called instance, the ErrorState has to be cleared:<br>
```cpp
comm_interface->clearErrorState();
```

The Error-codes are enumerated in the Content-derived [Error-class](lib/ErrorService/Error.h). To add new Error-cases, define a new Error-code in ```enum ErrorCodes``` and add an Error-message to the ```getErrorMessage(errorCodes code)```function within the [Error-class](lib/ErrorService/Error.h). The Error-Service will display those messages after an Error is either raised by another component on the local device or an Error-Frame was received.


## Deriving a custom Service 
To derive a Layer-7-service, that can be used in combination with the predefined Layer-2-communication-interfaces, you have to
- [x] [define content to handle](#define-the-content)
- [x] [define a service to handle the specified content](#define-the-service)

### Define the content
Each [Service](https://f-l-x-s.github.io/BusBricks/class_service.html) has to know, how the conversion between the processable data structure (content) and the payload, that is sended or received by the [communication-layer](#communication-layer) (representation) is defined. This is done by 
- [x] deriving a content-class from the [content-template](lib/Service/Content.h) ([docs](https://f-l-x-s.github.io/BusBricks/class_content.html))
<br>

The mapping between content and representation (in this case `String`) is defined by:<br>

- [x] overwriting and implementing the template functions ```content_to_rep()``` and ```rep_to_content()``` of [```Content```](https://f-l-x-s.github.io/BusBricks/class_content.html)

- [x] adding specific functions to be used inside of the service (optional) (e.g. [string representation for printing](https://f-l-x-s.github.io/BusBricks/class_message.html#a8907afa8cc86db8a3623aea0caba0324) of the [Message-Class](https://f-l-x-s.github.io/BusBricks/class_message.html))

### Define the service 
A [Service](https://f-l-x-s.github.io/BusBricks/class_service.html) is meant to process incoming payload and eventually generate new payload to be send. Received Payload is added to the Service with [```impart_pdu```](https://f-l-x-s.github.io/BusBricks/class_service.html#ac0c60bc735c64547dee269c6c3a62767) and the services response is able to be picked up by calling [```get_response```](https://f-l-x-s.github.io/BusBricks/class_service.html#a601e6d2878ce2b292a3b7c6428f0d51f). Both functions of the [service-template](https://f-l-x-s.github.io/BusBricks/class_service.html) are using the representation (for services contents always String) of the Content.<br>
Defining a custom service is done by
- [x] deriving a service-class from the [service-template](lib/Service/Service.h)
- [x] defining a default Service-ID (unique for each service)
- [x] define construction with Service-ID and Instance-ID (unique for each instantiation of a service), e.g.:
```cpp
#define SERVICEID 'e'
ErrorService::ErrorService(uint8_t instance_id): Service<Error, STACKSIZE>(SERVICEID, instance_id){}
```
- [x] overwriting and implementing the template's [```stack_processing```](https://f-l-x-s.github.io/BusBricks/class_service_base.html#a6129f02c6e59212d0b5b6d4d451b30e9) 

The ```stack_processing``` function has to process all payloads from the receive-stack and add the payloads to be send to the send-stack. Check [```ErrorService```](lib/ErrorService/ErrorService.cpp) or [```MessageService```](lib/MessageService/MessageService.cpp) for example implementations.

- [x] adding functions to interact with the service besides imparting/picking-up payloads (e.g. [```sendMessage```](https://f-l-x-s.github.io/BusBricks/class_message__service.html#aa8d198ccf700400339e3240e1c9c86e4) of the [```MessageService```](lib/MessageService/MessageService.h)) (optional)

## Build environments and testing
### clang-build for local debugging
The project is implemented to be debugged on the local engineering-device (PC). Therefore [testing-functions](test/manual) are defined for the local execution with clang (possible with other compilers, but only tested with clang). <br>
Those functions are more thought as a "playground" for setting up scenarios to debug, than for approving complete functionality of the Program.<br>
The clang-build is customized in [tasks](.vscode/tasks.json).<br>
Because of the usage of Arduino-specific functions not supported by c++ natively, those functions are replaced by using the namespaces in [Arduino-Mocking](lib/mockArduino/mockArduino.h) and [Serial-Mocking](lib/mockSoftwareSerial/mockSoftwareSerial.h). 

### native environment 
The pio native-environment is setup in [platformio.ini](platformio.ini). All [automated tests](test/native/) are executed in this environment.

### uno and nano328p environment 
 The environments for the target-architecture are configured in [platformio.ini](platformio.ini).

## License
### GNU Affero General Public License v3.0
This project is licensed under the GNU Affero General Public License v3.0. See the [LICENSE](https://github.com/F-L-X-S/BusBricks/blob/5e3d14aac293e28b3e3d8c17ed7d4e8bd7617505/LICENSE) file for details.

Copyright (c) 2024 Felix Schuelke