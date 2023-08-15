#include <stdio.h>
#include <stdint.h>

uint32_t encode_altitude(float height) {
    // Calculate the data value
    int32_t data_value = height / 0.125;

    // Ensure the value is within the valid range
    data_value = data_value & 0x7FFFF;

    // Set the SSM to "Normal Operations"
    uint8_t ssm = 0b11;

    // Set the SDI to "all systems"
    uint8_t sdi = 0b00;

    // Set the label to 0261 for altitude
    uint8_t label = 0261;

    // Calculate the parity bit
    uint32_t parity = 1; // start with 1 for odd parity
    for (int i = 0; i < 32; i++) {
        if ((data_value >> i) & 1) {
            parity = 1 - parity;
        }
    }

    // Combine the fields into a 32-bit message
    uint32_t message = (parity << 31) | (ssm << 29) | (data_value << 10) | (sdi << 8) | label;

    return message;
}

void transmit_data(uint32_t message) {
    // This function simulates the transmission of data.
    // It can include error simulation, BPRZ encoding, etc.

    // Print the bytes in hexadecimal format with spaces between them
    printf("%02x %02x %02x %02x\n", (message >> 24) & 0xFF, (message >> 16) & 0xFF, (message >> 8) & 0xFF, message & 0xFF);
}

int main() {
    uint32_t encoded_message;

    encoded_message = encode_altitude(135.7);
    transmit_data(encoded_message); // Expected result: e0 10 94 b1

    encoded_message = encode_altitude(1.0);
    transmit_data(encoded_message); // Expected result: 60 00 20 b1

    encoded_message = encode_altitude(-0.5);
    transmit_data(encoded_message); // Expected result: 7f ff a0 b1

    encoded_message = encode_altitude(0.0);
    transmit_data(encoded_message); // Expected result: e0 00 00 b1

    encoded_message = encode_altitude(75.25);
    transmit_data(encoded_message); // Expected result: 60 09 68 b1

    return 0;
}
