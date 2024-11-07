const char* ANALOG_PORT = "A0";
const char* SERIAL_PORT = "/dev/ttyUSB0";
SerialPort serialPort(SERIAL_PORT, BaudRate::B_9600, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);

int main() {
    if (!serialPort.Open()) {
        std::cerr << "Error opening serial port" << std::endl;
        return 1;
    }

    while (true) {
        int pH_Value = analogRead(ANALOG_PORT);
        float Voltage = pH_Value * (5.0 / 1023.0);
        serialPort.Write(std::to_string(Voltage) + "\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    serialPort.Close();
    return 0;
}
