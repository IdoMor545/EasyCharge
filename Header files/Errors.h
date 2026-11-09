#ifndef _ERRORS_H_
#define _ERRORS_H_


typedef enum {
    ERROR_SUCCESS = 0,
    ERROR_STATION_NOT_FOUND,
    ERROR_CAR_NOT_FOUND,
    ERROR_PORT_NOT_FOUND,
    ERROR_INVALID_INPUT,
    ERROR_FILE_OPEN_FAILED,
    ERROR_STATION_NOT_EMPTY,
    ERROR_MEMORY_ALLOCATION_FAILED,
    ERROR_QUEUE_EMPTY
} ErrorCode;


void displayError(ErrorCode code);

#endif // _ERRORS_H_
