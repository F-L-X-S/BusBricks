/**
 * @file ErrorState.cpp
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

#include "ErrorState.h"

// Default constructor
ErrorState::ErrorState() : errorState(noError) {}

// Destructor
ErrorState::~ErrorState() {}

// Raises a new error by setting the given error code as the error state
void ErrorState::raiseError(errorCodes code) {
    errorState = code;
}

// Gets the currently active error code, returns 0 if no error is active
errorCodes ErrorState::getErrorState() {
    return errorState;
}

// Clears the current error state
void ErrorState::clearErrorState() {
    errorState = noError;
}
