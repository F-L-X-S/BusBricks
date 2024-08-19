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
<img src="https://github.com/F-L-X-S/BusBricks/raw/main/docs/stackProcessing.png" alt="stackProcessing" style="width:600px;">



## License

This project is licensed under the GNU Affero General Public License v3.0. See the [LICENSE](https://github.com/F-L-X-S/BusBricks/blob/5e3d14aac293e28b3e3d8c17ed7d4e8bd7617505/LICENSE) file for details.

Copyright (c) 2024 Felix Schuelke