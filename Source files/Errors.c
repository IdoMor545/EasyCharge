#include "Errors.h"
#include <stdio.h>

void displayError(ErrorCode code) {
    switch (code) {
    case ERROR_SUCCESS:
        printf("Success: Operation completed successfully.\n");
        break;
    case ERROR_STATION_NOT_FOUND:
        printf("Error: Station not found.\n");
        break;
    case ERROR_CAR_NOT_FOUND:
        printf("Error: Car not found. Please check the license plate number.\n");
        break;
    case ERROR_PORT_NOT_FOUND:
        printf("Error: Port not found. Please check the port number.\n");
        break;
    case ERROR_INVALID_INPUT:
        printf("Error: Invalid input. Please check the provided data.\n");
        break;
    case ERROR_FILE_OPEN_FAILED:
        printf("Error: Failed to open the file. Please check the permissions.\n");
        break;
    case ERROR_STATION_NOT_EMPTY:
        printf("Error: Station is not empty. cant perform this operation.\n");
        break;
    case ERROR_MEMORY_ALLOCATION_FAILED:
        printf("Error: Memory allocation failed. The system may be out of memory.\n");
        break;
    case ERROR_QUEUE_EMPTY:
        printf("Error: The queue is empty. No cars to dequeue.\n");
        break;
    default:
        printf(" unknown error occurred.\n");
        break;
    }
}
/**
 * Prints a specific error message based on the provided error code.
 * This function gets an error code and, using a switch statement, prints
 * a clear message to the screen. It's a single place to handle and show
 * different kinds of system errors.
 */