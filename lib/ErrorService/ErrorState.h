/**
 * @file ErrorState.h
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

#ifndef ERRORSTATE_H
#define ERRORSTATE_H

#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    #include <cstdio>
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <Error.h>

/**
 * @class ErrorState
 * @brief A class to manage and track error states using error codes.
 * 
 * The `ErrorState` class is designed to handle error management by keeping track
 * of the current error state using error codes. It provides methods to raise errors,
 * retrieve the current error state, and clear errors when necessary. The class maintains
 * an internal error code (`errorState`) to represent the current state that is defined by the enumeration 
 * in of the Error-class
 */
class ErrorState {
    public:
        /**
         * @brief Default constructor for `ErrorState`.
         * 
         * Initializes an empty ErrorState object
         */
        ErrorState();

        /**
         * @brief Destroy the ErrorState object
         * 
         */
        ~ErrorState();
        
        /**
         * @brief raises a new error by setting  the given error-code as errorState
         * 
         * @param code error-code of error to raise
         */
        void raiseError(errorCodes code);

        /**
         * @brief Get the currently active error-code
         * 
         * @return char error-code 
         */
        errorCodes getErrorState();

        /**
         * @brief sets the current error-state to "no Error" 
         * 
         */
        void clearErrorState();

    private:
        /// @brief currently active error-state
        errorCodes errorState;
};

#endif // ERRORSTATE_H