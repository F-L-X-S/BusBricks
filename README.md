# BusBricks 
Object-oriented building-blocks to customize bus-communication for different services and interfaces on Arduino-MCUs <br>

- [X] Checkout detailed documentation [BusBricks-documentation](https://f-l-x-s.github.io/BusBricks/)!

## Vision
Provide configurable software-modules to customize the communication between services, hosted on different MCUs. <br> 
BusBricks are templates to derive classes for mapping service-specific rules to different bus-systems. 
The communication between the hosting devices is therefore separated from the inter-service-communication. 
That means, the communication between the services can follow it's own (from the chosen communication-interface independent) rules. <br>

#### Example: Messenger-service 
Hosting a messenger on different host-devices. Some of those hosts support a serial commuication for modbus-RTU, some only support I2C.<br>
|*Service*|*Communication-interfaces*|
|----------|--------------------------|   
|Messenger|Modbus-RTU (SoftwareSerial)|
|         |I2C (Wire)|

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
Conversely, the format, the information has after applying the rules of the next level closer to "Layer-0" is called **representation**.<br>

### Classdiagram
<img src="https://github.com/F-L-X-S/BusBricks/raw/main/docs/classdiagram.png" alt="classdiagram" style="width:1700px;">

### Stackprocessing
<img src="https://github.com/F-L-X-S/BusBricks/raw/main/docs/stackProcessing.png" alt="stackProcessing" style="width:400px;">

## Deriving a custom Service 
- [x] [define content to handle](#define-the-content)
- [x] [define a service to handle the specified content](#define-the-service)

#### Define the content
Each [Service](https://f-l-x-s.github.io/BusBricks/class_service.html) has to know, how the conversion between the processable data structure (content) and the payload, that is sended or received by the [communication-layer](#communication-layer) (representation) is defined. This is done by 
- [x] deriving a content-class from the [content-template](lib/Service/Content.h) ([docs](https://f-l-x-s.github.io/BusBricks/class_content.html))
<br>

The mapping between content and representation (in this case `String`) is defined by:<br>

- [x] overwriting and implementing the template functions ```content_to_rep()``` and ```rep_to_content()``` of [```Content```](https://f-l-x-s.github.io/BusBricks/class_content.html)

- [x] adding specific functions to be used inside of the service (optional) (e.g. [string representation for printing](https://f-l-x-s.github.io/BusBricks/class_message.html#a8907afa8cc86db8a3623aea0caba0324) of the [Message-Class](https://f-l-x-s.github.io/BusBricks/class_message.html))

#### Define the service 
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

## License

This project is licensed under the GNU Affero General Public License v3.0. See the [LICENSE](https://github.com/F-L-X-S/BusBricks/blob/5e3d14aac293e28b3e3d8c17ed7d4e8bd7617505/LICENSE) file for details.

Copyright (c) 2024 Felix Schuelke