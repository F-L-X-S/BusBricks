/**
 * @file CharArray.cpp
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

#include "CharArray.h"

/**
 * @brief Construct a new empty Char Array object (0 byte)
 * 
 */
CharArray::CharArray() : data(nullptr), size(0) {}


/**
 * @brief Construct a new Char Array object with specified size
 * 
 * @param size 
 */
CharArray::CharArray(size_t size) : size(size) {
    data = new char[size];
}


/**
 * @brief Destroy the Char Array object
 * 
 */
CharArray::~CharArray() {
    delete[] data;
}

/**
 * @brief Read-Access the element at the specified index.
 * 
 * Returns a reference to the element at the given index in the char array.
 * If the index is out of bounds, a refernce to a dummy nullterminator will be returned.
 * 
 * @param index The index of the element to access.
 * @return char& A reference to the element at the specified index.
 */
const char& CharArray::operator[](size_t index) const {
    static char dummy = '\0';
    if (index >= size) {
        return dummy;
    }
    return data[index];
}

/**
 * @brief Write-Access the element at the specified index.
 * 
 * Returns a reference to the element at the given index in the char array.
 * If the index is out of bounds, a the array will be extended.
 * 
 * @param index The index of the element to access.
 * @return char& A reference to the element at the specified index.
 */
char& CharArray::operator[](size_t index){
    if (index >= size) {
        data = extendArray(index+1);
    }
    return data[index];
}

/**
 * @brief += Operator to append a single character
 * 
 * @param c char to append
 * @return CharArray& 
 */
CharArray& CharArray::operator+=(char c) {
    // Allocate a new array with one extra space
    data = extendArray(size +1);

    // Return the current object to allow chaining
    return *this;
}

char* CharArray::extendArray(size_t extendedSize){
    // Allocate a new array extended space
    char* newData = new char[extendedSize];
    // Copy existing data to new array
    for (size_t i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    // update the size
    size = extendedSize;
    // Delete old array
    delete[] data;
    return newData;
}

/**
 * @brief Retrieves the size of the char array.
 * 
 * Returns the number of elements in the char array.
 * 
 * @return size_t The size of the char array.
 */
size_t CharArray::getSize() const {
    return size;
}

/**
 * @brief Provides direct access to the internal char array.
 * 
 * Returns a pointer to the internal char array. This allows direct manipulation
 * of the data stored in the array.
 * 
 * @return char* A pointer to the internal char array.
 */
char* CharArray::getData() {
    return data;
}
