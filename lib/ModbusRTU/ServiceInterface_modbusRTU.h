/**
 * @file ServiceInterface_modbusRTU.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SERVICEINTERFACE_MODBUSRTU
#define SERVICEINTERFACE_MODBUSRTU
#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <ServiceInterface.h>
#include <CommInterface_modbusRTU.h>
#include <Frame_modbusRTU.h>
#include <Message_service.h>

/**
 * @brief Service-Interface-class for Modbus-RTU:
 * Impart incoming frames from the CommInterface to the designated service of the service-cluster. 
 * Build outgoing modbus-RTU-frames from the payload provided by the services and impart them to the Communication-Interface.
 * The service-IDs of the associated services are mapped to the modbus-rtu-function-code. If a Frame is sent by a modbus-slave device,
 * the frame's slave-id is the same as the sender-id (and the local device-id). 
 * If a Frame is sent by a modbus-device in mastermode, the frame's slave-id is the receiver-id provided by the service, 
 * that provided the payload for the frame. 
 */
class ServiceInterface_modbusRTU: public ServiceInterface<CommInterface_modbusRTU, Frame_modbusRTU>{
    public:
        /**
         * @brief Construct a new ServiceInterface_modbusRTU object
         * 
         * @param services pointer to Service-cluster containing all services associated with the ServiceInterface
         * @param comm_interface pointer to the CommInterface_modbusRTU-instance to use for communication
         */
        ServiceInterface_modbusRTU(ServiceClusterBase* services, CommInterface_modbusRTU* comm_interface);

        /**
         * @brief Destroy the ServiceInterface_modbusRTU object
         * 
         */
        ~ServiceInterface_modbusRTU();

        /**
         * @brief Manages data transfer between the CommInterface and services.
         *
         * This function coordinates the entire communication process by:
         * - Retrieving incoming frames from the communication interface.
         * - Processing and adding received PDUs to the appropriate services.
         * - Updating communication stacks as needed.
         * - Executing the communication cycle for the interface.
         * 
         * @details
         * - Calls `processRecStack` to handle incoming frames and manage the receive stack.
         * - Uses `addPDU_to_services` to distribute PDUs to services based on the Modbus RTU function code.
         * - Executes `processServices` to perform any necessary actions or updates within the services.
         * - Retrieves PDUs from services and prepares them for sending using `getPDU_from_services`.
         * - Finally, calls `processSendStack` to manage and send the new frames to the communication interface.
         * 
         * @note
         * - The function orchestrates the communication between the CommInterface and services, ensuring data is properly transferred in both directions.
         */
        void communicate() override;

    private:
        /**
         * @brief Adds all PDUs from the services to the local sendStack, that contains the frames, that have to be sent by the CommInterface.
         *
         * This function iterates through all services in the service cluster and creates a Modbus RTU frame for each service's payload.
         * The created frames are then added to the `sendStack`. If the send stack becomes full during the process, the function exits early.
         * 
         * @details
         * - Iterates through the services provided by the `services` object.
         * - For each service with an available response, retrieves the response PDU and constructs a Modbus RTU frame.
         * - The device ID is determined based on the service's instance ID, with specific handling for message services where the destination device ID is obtained.
         * - The frame is built using the service's response PDU, device ID, and function code, and added to the `sendStack`.
         * - The service's response is cleared after processing.
         * 
         * @note
         * - The function handles different types of services and their specific response formats.
         * - The `sendStack` must have sufficient space to accommodate the frames; if it becomes full, the function will stop adding new frames.
         */
        void getPDU_from_services() override;

        /**
         * @brief Adds received PDUs from the receive stack to the corresponding services.
         *
         * This function processes all PDUs (Protocol Data Units) retrieved from the `recStack` and distributes them to the appropriate services. 
         * Each PDU is assigned to a service based on the Modbus RTU function code. If the destination service is not found, the frame is discarded.
         * 
         * @details
         * - The function first checks if there are any PDUs in the `recStack`. If the stack is empty, the function exits.
         * - For each PDU in the stack, it retrieves the Modbus RTU function code and finds the corresponding service using this code as the Service ID.
         * - If a valid service is found, the PDU is passed to the service's receive stack via the `impart_pdu` method.
         * - The frame is then removed from the `recStack` after being processed.
         * - If no corresponding service is found for a PDU, the frame is discarded and removed from the stack.
         * 
         * @note
         * - The function assumes that `recStack` contains valid Modbus RTU frames and that `services` provides a method to retrieve services by Service ID.
         */
        void addPDU_to_services() override;
};

#endif // SERVICEINTERFACE_MODBUSRTU