#include "mockSoftwareSerial.h"

arduinoMocking::SoftwareSerial::SoftwareSerial(int rx, int tx) : rxPin(rx), txPin(tx){};

void arduinoMocking::SoftwareSerial::begin(long baud) {

};

void arduinoMocking::SoftwareSerial::println(const std::string &message) {

};

void arduinoMocking::SoftwareSerial::print(const std::string &message) {

};


void arduinoMocking::SoftwareSerial::write(char data) {
    dataQueue.addElement(data);
}

void arduinoMocking::SoftwareSerial::flush(){
    std::string result;
    while (!dataQueue.empty()) {
        result += *(dataQueue.getElement());
        dataQueue.deleteElement();
    }
    //std::cout << "Flushed data: " << result << std::endl;
}


int arduinoMocking::SoftwareSerial::read() {
    if (dataQueue.empty()) {
        return -1; 
    }
    char data = *(dataQueue.getElement());
    dataQueue.deleteElement();
    return static_cast<int>(data);
}


int arduinoMocking::SoftwareSerial::peek() {
    if (dataQueue.empty()) {
        return -1; 
    }
    return static_cast<int>(*(dataQueue.getElement()));
}


int arduinoMocking::SoftwareSerial::available() {
    return (dataQueue.empty())?0:1;
}