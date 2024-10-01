/**
 * @file CharArray.h
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

#ifndef CHARARRAY_H
#define CHARARRAY_H

#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

/**
 * @brief Class for storing char-array (byte-array) together with size. The Array is stored on Heap-memory
 * and is extended by adding bytes with "+=" or by write-access to an index larger than the current size.
 */
class CharArray {
private:
    /// @brief pointer to first array-byte to the data stored on heap-memory
    char* data;

    /// @brief number of byte stored 
    size_t size;

    /**
     * @brief extends the size of the array to store more items, return
     * 
     * @param extendedSize new size for array
     * @return char* Pointer to the extended heap-memory 
     */
    void extendArray(size_t extendedSize);

public:
    /**
     * @brief Construct a new empty Char Array object (0 byte)
     * 
     */
    CharArray();


    /**
     * @brief Destroy the Char Array object
     * 
     */
    ~CharArray();

    /**
     * @brief  constructor for Arduino-String
     *
     * Initializes a new CharArray object with the content of the String-object.
     * 
     * @param other The String-Object
     */
    CharArray(const String& str);

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
    CharArray(const CharArray& other); 

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
    CharArray& operator=(const CharArray& other);

    /**
     * @brief Assignment operator for String-assignment
     *
     * This operator assigns the values from a String-object to the current object.
     * 
     * @param other The String object whose values will be assigned.
     * @return A reference to the current object after assignment.
     * 
     */
    CharArray& operator=(const String& other);

    /**
     * @brief Write-Access the element at the specified index.
     * 
     * Returns a reference to the element at the given index in the char array.
     * If the index is out of bounds, a the array will be extended.
     * 
     * @param index The index of the element to access.
     * @return char& A reference to the element at the specified index.
     */
    char& operator[](size_t index);

    /**
     * @brief Read-Access the element at the specified index.
     * 
     * Returns a reference to the element at the given index in the char array.
     * If the index is out of bounds, a refernce to a dummy nullterminator will be returned.
     * 
     * @param index The index of the element to access.
     * @return char& A reference to the element at the specified index.
     */
    const char& operator[](size_t index) const;


    /**
     * @brief += Operator to append a single character
     * 
     * @param c char to append
     * @return CharArray& 
     */
    CharArray& operator+=(char c);

    /**
     * @brief Equality Operator
     * 
     * @param other 
     * @return bool 
     */
    bool operator==(const CharArray& other) const;

    /**
     * @brief Inequality Operator
     * 
     * @param other 
     * @return bool 
     */
    bool operator!=(const CharArray& other) const;


    /**
     * @brief Retrieves the size of the char array.
     * 
     * Returns the number of elements in the char array.
     * 
     * @return size_t The size of the char array.
     */
    size_t getSize() const;

    /**
     * @brief Provides direct access to the internal char array.
     * 
     * Returns a pointer to the internal char array. This allows direct manipulation
     * of the data stored in the array.
     * 
     * @return char* A pointer to the internal char array.
     */
    const char* getData() const;

    /**
     * @brief Get the Hex-Values of the Array as a String
     * 
     * @return String 
     */
    String getHexString();
};



#endif  // CHARARRAY_H
