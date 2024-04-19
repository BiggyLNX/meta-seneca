#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Define I2C address of the TSC2007-Q1 device
#define TSC2007_ADDRESS 0x48

// Define screen dimensions
#define SCREEN_WIDTH 1000  // Width of the screen
#define SCREEN_HEIGHT 1000 // Height of the screen

// Function to read X, Y, or Z coordinates from the TSC2007-Q1
void readCoordinates(int file, uint8_t command, int *value) {
    uint8_t data[2];

    // Write command byte to specify the operation
    if (write(file, &command, 1) != 1) {
        perror("Write failed");
        exit(1);
    }

    // Read 2 bytes of data
    if (read(file, data, 2) != 2) {
        perror("Read failed");
        exit(1);
    }

    // Combine the bytes to form the 12-bit value
    *value = (data[0] << 8) | data[1];
}

// Function to map touch coordinates to Cartesian plane
void mapCoordinates(int x, int y, int *mapped_x, int *mapped_y) {
    // Map X coordinate
    *mapped_x = x * 1000 / 4095;

    // Map Y coordinate (invert Y axis to match Cartesian plane)
    *mapped_y = (4095 - y) * 1000 / 4095;

    // Clamp mapped coordinates within screen bounds
    if (*mapped_x < 0) {
        *mapped_x = 0;
    } else if (*mapped_x > 1000) {
        *mapped_x = 1000;
    }

    if (*mapped_y < 0) {
        *mapped_y = 0;
    } else if (*mapped_y > 1000) {
        *mapped_y = 1000;
    }
}

int main() {
    char *device = "/dev/i2c-5";
    int file;
    int x, y;

    // Open the I2C bus
    if ((file = open(device, O_RDWR)) < 0) {
        perror("Failed to open the bus.");
        exit(1);
    }

    // Set the I2C slave address
    if (ioctl(file, I2C_SLAVE, TSC2007_ADDRESS) < 0) {
        perror("Failed to acquire bus access and/or talk to slave.");
        exit(1);
    }

    while (1) {
        // Read X coordinate
        readCoordinates(file, 0b11001000, &x); // Command byte for reading X coordinate

        // Read Y coordinate
        readCoordinates(file, 0b11001001, &y); // Command byte for reading Y coordinate

        // Map touch coordinates to Cartesian plane
        int mapped_x, mapped_y;
        mapCoordinates(x, y, &mapped_x, &mapped_y);

        // Print the mapped coordinates
        printf("Mapped Touch position: X=%d, Y=%d\n", mapped_x, mapped_y);

        // Add your code here to update the display based on the X and Y coordinates
        // For example, you can draw something on a graphical display or trigger actions based on touch position

        usleep(100000); // Sleep for 100ms before reading again
    }

    // Close the I2C bus (this will never execute in the loop)
    close(file);

    return 0;
}
