#include <CppLinuxSerial/SerialPort.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

using namespace mn::CppLinuxSerial;

const int BAUD_RATE = 9600;
const char* ANALOG_PORT = "A0";
const char* SERIAL_PORT = "/dev/ttyUSB0";
SerialPort serialPort(SERIAL_PORT, BaudRate::B_9600, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);

int main() {
    try {
        serialPort.Open();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    while (true) {
        std::string pH_Value;
        serialPort.Read(pH_Value);
        float Voltage = std::stoi(pH_Value) * (5.0 / 1023.0);
        serialPort.Write(std::to_string(Voltage) + "\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    serialPort.Close();
    return 0;
}
