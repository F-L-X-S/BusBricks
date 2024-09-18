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
 * @brief Destroy the Char Array object
 * 
 */
CharArray::~CharArray() {
    if (data != nullptr) delete[] data;
}


/**
 * @brief Copy constructor.
 *
 * Initializes a new CharArray object as a copy of another CharArray object.
 * 
 * @param other The other CharArray object to copy.
 * 
 * @details This constructor performs a deep copy of all the data from `other`, 
 * ensuring that the new object is an exact copy of the source object.
 */
CharArray::CharArray(const CharArray& other) : size(other.size) {
    data = new char[size];
    memcpy(data, other.data, size);
}

/**
 * @brief Assignment operator.
 *
 * This operator assigns the values from another object of the same type to the current object.
 * 
 * @param other The other CharArray object whose values will be assigned.
 * @return A reference to the current object after assignment.
 * 
 * @details This operator performs a deep copy of the data from `other`, ensuring that the current object 
 * becomes an exact copy of `other`.
 */
CharArray& CharArray::operator=(const CharArray& other) {
    if (this != &other) {  
        delete[] data;
        size = other.size;
        data = new char[size];
        memcpy(data, other.data, size);
    }
    return *this;
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
        extendArray(index+1);
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
    extendArray(size +1);
    // add c on last index
    data[size - 1] = c;
    // Return the current object to allow chaining
    return *this;
}

/**
 * @brief extends the size of the array to store more items, return
 * 
 * @param extendedSize new size for array
 * @return char* Pointer to the extended heap-memory 
 */
void CharArray::extendArray(size_t extendedSize){
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
    data = newData;
}

/**
 * @brief Equality Operator
 * 
 * @param other 
 * @return bool 
 */
bool CharArray::operator==(const CharArray& other) const {
    // Check if sizes are the same
    if (size != other.size) {
        return false;
    }

    // Check each element for equality
    for (size_t i = 0; i < size; i++) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Inequality Operator
 * 
 * @param other 
 * @return bool 
 */
bool CharArray::operator!=(const CharArray& other) const {
    return !(*this == other);
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
const char* CharArray::getData() const {
    return data;
}
