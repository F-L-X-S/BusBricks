/**
 * @file test_Error.cpp
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

#include <Error.h>

int main(){
    errorCodes code = unknownError;
    char instanceId = 'A';

    // create Error from representation
    String rep = "";
    rep += instanceId;
    rep += ":ERROR";
    rep += code;
    Error errorFromRep(&rep);
    Serial.print(errorFromRep.to_string());
    Serial.println(*errorFromRep.get_representation());

    // create Error from content
    ErrorContent_t content(instanceId, code);
    Error errorFromContent(&content);
    Serial.print(errorFromContent.to_string());
    Serial.println(*errorFromContent.get_representation());

    // create Error from code
    Error errorFromCode(instanceId,code);
    Serial.print(errorFromCode.to_string());
    Serial.println(*errorFromCode.get_representation());

}
