#include "pHcalibration.cpp"
#include <CppLinuxSerial/SerialPort.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "ports.hpp"
using namespace mn::CppLinuxSerial;
using namespace std::chrono;

// Timer instance
auto lastMeasurement = steady_clock::now();
const milliseconds MEASUREMENT_INTERVAL(500); // 500ms interval

// pH sensor calibration and measurement variables
float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10], temp;
float ph_act;

// Serial port configuration
bool isSerialConnected = false;

void setup() 
{
    try {
        serialPort.Open();
        isSerialConnected = true;
        std::cout << "Serial port connected successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: Could not open serial port: " << e.what() << std::endl;
        isSerialConnected = false;
    }
}

// Function to measure pH value
void measure_pH() 
{
    // Collect samples
    for(int i = 0; i < 10; i++) 
    { 
        serialPort.Write("A0\n");  // Request reading from A0
        std::string reading;
        serialPort.Read(reading);  // Read response
        buffer_arr[i] = std::stoi(reading);  // Convert string to integer
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    
    // Sort samples (bubble sort)
    for(int i = 0; i < 9; i++)
    {
        for(int j = i + 1; j < 10; j++)
        {
            if(buffer_arr[i] > buffer_arr[j])
            {
                temp = buffer_arr[i];
                buffer_arr[i] = buffer_arr[j];
                buffer_arr[j] = temp;
            }
        }
    }
    
    // Calculate average of middle values
    avgval = 0;
    for(int i = 2; i < 8; i++)
        avgval += buffer_arr[i];
    
    // Convert to pH value
    float volt = (float)avgval * 5.0 / 1024 / 6; 
    ph_act = -5.70 * volt + calibration_value;

    // Update serial output to use CppLinuxSerial
    if (isSerialConnected) {
        std::string phString = "pH Val: " + std::to_string(ph_act) + "\n";
        try {
            serialPort.Write(phString);
        } catch (const std::exception& e) {
            std::cerr << "Error writing to serial port: " << e.what() << std::endl;
            isSerialConnected = false;
        }
    }
}

// Add cleanup function
void cleanup() 
{
    if (isSerialConnected) {
        try {
            serialPort.Close();
        } catch (const std::exception& e) {
            std::cerr << "Error closing serial port: " << e.what() << std::endl;
        }
    }
}

void loop() 
{
    if (!isSerialConnected) {
        std::cerr << "Error: Serial port not connected" << std::endl;
        return;
    }

    auto now = steady_clock::now();
    if (now - lastMeasurement >= MEASUREMENT_INTERVAL) {
        measure_pH();
        lastMeasurement = now;
    }
}

