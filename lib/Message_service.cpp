#include "Message_service.h"

// Constructor for creating Message-service 
Message_service::Message_service(): Service<Message, STACKSIZE>(FUNCTIONCODE){}