/**
 * @file Content.h
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


#ifndef CONTENT_H
#define CONTENT_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
    #include <cstring>
#endif

/**
 * @author Felix Schuelke 
 * @brief The cascading of processing information and the rules applied to it lead to the concept of content- and representation.
 * In every iteration, an information is processed and the rules of the next level, closer to the physical layer are applied to it, 
 * the information closer to the format the service is able to process is called Content.
 * Conversely, the format, the information has after applying the rules of the next level closer to "Layer-0" is called representation.
 * @tparam content_type defines the type for storing the information (content) itself
 * @tparam representation_type defines the type the information (content) is represented for the next processing-level 
 */
template<typename content_type, typename representation_type>
class Content {
public: 
    /**
     * @brief Construct a new Content object from an instance from type content_type (content itself)
     * 
     * @param content information (content itself) of type content_type 
     */
    Content(content_type content) : content(content) {}    

    /**
     * @brief Construct a new Content object from it's representation from type representation_type
     * 
     * @param representation representation of the information (content) from type representation_type
     */
    Content(representation_type representation) : representation(representation) {}    
    
    /**
     * @brief Get the address of the informations representation of type representation-type
     * 
     * @return representation_type* pointer to the informations representation
     */
    representation_type* get_representation() {
        return &representation;
    }                               

    /**
     * @brief Get the address of the informations content of type content_type
     * 
     * @return content_type* 
     */
    content_type* get_content() {                                          
        return &content;
    }

    /**
     * @brief Representation and Content of the object are existent (not default)
     * 
     * @return true Representation and Content are not default 
     * @return false Representation or Content do not exist (only defaultvalues)
     */
    bool isValid(){
        return ((content!=content_type()) & (representation!=representation_type()));
    };

protected:
    /// @brief Content of an information (view on information (content) closer to Service-Layer)             
    content_type content;       

    /// @brief Representation of an information (view on information (content) closer to physical layer)         
    representation_type representation;    

    // Convert the representation-attribute to the content-attribute, 
    // the function has to be called in the constructor of the derived class 

    /**
     * @brief 
     * Defines the mapping from Representation to Content.
     * Convert the class-attribute "representation" to the class-attribute "content".
     * (has to be defined in derived class)
     * (has to be called in the constructor of the derived class)
     * 
     */
    virtual void rep_to_content() = 0;

    /**
     * @brief 
     * Defines the mapping from Content to Representation.
     * Convert the class-attribute "content"to the class-attribute "representation" .
     * (has to be defined in derived class)
     * (has to be called in the constructor of the derived class)
     * 
     */
    virtual void content_to_rep() = 0;  
};

#endif // CONTENT_H