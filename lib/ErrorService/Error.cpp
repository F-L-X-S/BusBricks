/**
 * @file Error.cpp
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright * Copyright (C) 2024 Felix Schuelke
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include "Error.h"

// Default constructor for `Error`
Error::Error() : Content<ErrorContent_t, String>(ErrorContent_t()) {
    Error::content_to_rep();
}

// Destructor for `Error`
Error::~Error() {}

// Constructor for `Error` from a string-formatted PDU
Error::Error(String* representation) : Content<ErrorContent_t, String>(*representation) {
    Error::rep_to_content();
}

// Constructor for `Error` from `ErrorContent_t`
Error::Error(ErrorContent_t* errorContent) : Content<ErrorContent_t, String>(*errorContent) {
    Error::content_to_rep();
}

// Constructor for `Error` from error-code
Error::Error(errorCodes code) : Content<ErrorContent_t, String>(ErrorContent_t(code)) {
    Error::content_to_rep();
}

// Converts the Error to a well formatted, printable string
String Error::to_string() {
    String string_rep;
    
    // Header
    string_rep = "---Error ";
    string_rep += content.code;
    string_rep += "---\n";

    // Error-message
    string_rep += content.message;
    string_rep += "\n";
    return string_rep;
}

// Converts the byte-representation (PDU) to error's content by initializing ErrorContent_t with Error-code
void Error::rep_to_content(){
    content = ErrorContent_t(errorCodes(representation[6]));
}

// Converts error's content to byte-representation (PDU)
void Error::content_to_rep() {
    representation = "ERROR:";
    representation += content.code;    // Add Error-code
}

