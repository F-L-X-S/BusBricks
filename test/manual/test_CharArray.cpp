/**
 * @file test_CharArray.cpp
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
#include <mockArduino.h>
#include <mockSoftwareSerial.h>
#include<CharArray.h>

void showCharArrayStats(CharArray buffer,String comment){
    // show comment
    Serial.print(comment);

    // show size
    Serial.print("\tSize:\t");
    Serial.print(buffer.getSize());

    // show data as string 
    const char* data = buffer.getData();
    Serial.print(":\tData as string:\t");
    if (data != nullptr) Serial.print(String(data).substr(0, buffer.getSize()));

    // show data as hex-array 
    Serial.print("\tData as hex-array:\t");
    for (int i = 0; i < buffer.getSize(); i++) {
        Serial.print("0x");
        if (data[i] < 16) {
            Serial.print("0");  
        }
        char hexCharBuffer[3];  
        sprintf(hexCharBuffer, "%02X", (unsigned char)data[i]);  
        Serial.print(String(hexCharBuffer));  
        Serial.print(" ");
    }
    
    // newline
    Serial.println();
}

void test_CharArray(){
    CharArray buffer;
    // check default constructor 
    showCharArrayStats(buffer,"Default constructor");
    // check insertion at non existing index (automated array-extension)
    buffer[3] = 'a';
    showCharArrayStats(buffer,"Added char a at position 3");
    // check appending
    buffer += 'b';
    showCharArrayStats(buffer,"Appended char b at last index");
    // check insertion at existing index
    buffer[0] = '1';
    showCharArrayStats(buffer,"Added char 1 at position 0");
}

void test_CharArrayComparison(){
    CharArray buffer;
    // check comparison for empty objects
    if (buffer == CharArray()) Serial.println("Empty buffer object equal to default constructor");

    // check comparison for objects with same content
    CharArray cmpBuffer = buffer; //assignment 
    buffer[3]='a';
    if (buffer != cmpBuffer) Serial.println("Compare-buffer not equal to buffer");
    cmpBuffer[3]='a';
    if (buffer == cmpBuffer) Serial.println("Compare-buffer equal to buffer");
}

int main(){
    test_CharArray();
    test_CharArrayComparison();
}