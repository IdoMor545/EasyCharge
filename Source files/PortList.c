#define _CRT_SECURE_NO_WARNINGS
#include "PortList.h"
#include "Station.h"
#include "Errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STATIONNAME 256
#define OUT_OF_ORDER 3

Port* createAndAddPort(Port* PortsList, int PortNum, portType type, unsigned int status, Car* p2car, Date tin)
{
    Port* port = (Port*)malloc(sizeof(Port));
    if (!port) return NULL;

    port->num = PortNum;
    port->portType = type;
    port->status = status;
    port->p2car = p2car;
    if (p2car != NULL)
        p2car->pPort = port;
    port->next = NULL;
    port->tin = tin;

    if (PortsList == NULL)
    {
        PortsList = port;
    }
    else
    {
        Port* temp = PortsList;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = port;
    }
    return PortsList;
}
/*
 * Creates a new Port and adds it to the end of the station’s port list.
 * The function allocates memory, sets the port number, type, status, it thete is a connected car, and connection time.
 * Returns the updated head of the port list.
 */



Port* searchPortByNumber(Port* portsList, int PortNumber)
{
    if (portsList == NULL) return NULL;

    Port* curr = portsList;
    while (curr != NULL)
    {
        if (curr->num == PortNumber)
            return curr;
        curr = curr->next;
    }
    return NULL;
}
/*
 * Searches the station's port list for a port with a specific number.
 * if found, returns a pointer to the port.
 * if not found, returns NULL.
 */



void findMatchingPortToCar(Car* car, Station* station)
{
    if (car == NULL || station == NULL)
    {
        displayError(ERROR_INVALID_INPUT);
        return;
    }
    if (car->pPort != NULL)
    {
        printf("your car is already charging!\n");
        return;
    }

    Port* temp = station->portsList;
    while (temp != NULL)
    {
        if (temp->status == 2 && temp->portType == car->portType)
        {
            assignCar2Port(car, temp);
            return;
        }

        temp = temp->next;
    }

    printf("system didnt find free port for your car you will be in the Queue station\n");
    enqueueCarToStationQueue(station->carQueue, car);
    return;
}
/*
 * Tries to find an available port in a station that matches the car’s required port type.
 * if a matching port is found, the car is connected to it using assignCar2Port.
 * if not, the car is added to the station’s queue.
 * if the car is already charging or input is invalid, shows an error.
 */



int assignCar2Port(Car* car, Port* port)
{
    if (car == NULL || port == NULL)
    {
        displayError(ERROR_INVALID_INPUT);
        return FALSE;
    }
    port->p2car = car;
    car->pPort = port;
    port->status = 1;
    port->tin = CurrentTime();
    printf("your car %s succesfully connected to port %d\n", car->nLicense, port->num);
    return TRUE;
}
/*
 * Connects a car to a given port.
 * updates both the port and the car.
 * sets the port status to occupied and stores the current time as the start time.
 * returns TRUE on success, or FALSE on invalid input.
 */


void addNewPort(BSTStation* stationManager)
{
    if (stationManager == NULL || stationManager->root == NULL)
    {
        displayError(ERROR_INVALID_INPUT);
        return;
    }
       
    int choice;
    printf("you need to enter a choice of seacrhing a station for adding a port\n0-ID\n1-NAME\n");
    if (scanf("%d", &choice) != 1)
    {
        flushStdin();
        displayError(ERROR_INVALID_INPUT);
        return;
    }
    Station* station;
    switch (choice)
    {
    case 0:
        printf("enter ID of the station\n");
        int id;
        if (scanf("%d", &id) != 1)
        {
            flushStdin();
            displayError(ERROR_INVALID_INPUT);
            return;
        }
        station = searchStation(stationManager->root, &id, choice);
        if (station == NULL)
        {
            flushStdin();
            displayError(ERROR_STATION_NOT_FOUND);
            return;
        }
        break;
    case 1:
        printf("enter Name of the station\n");
        char StationName[STATIONNAME];
        scanf(" %[^\n]", StationName);
        station = searchStation(stationManager->root, StationName, choice);
        if (station == NULL)
        {
            displayError(ERROR_STATION_NOT_FOUND);
            return;
        }
        break;
    default:
        displayError(ERROR_INVALID_INPUT);
        return;
    }
    portType portType;
    printf("enter the PortType you want fo your port\n1-SLOW\n2-MID\n3-FAST\n");
    if (scanf("%d", &choice) != 1)
    {
        flushStdin();
        displayError(ERROR_INVALID_INPUT);
        return;
    }
    switch (choice)
    {
    case 1:
        portType = SLOW;
        break;
    case 2:
        portType = MID;
        break;
    case 3:
        portType = FAST;
        break;
    default:
        printf("your choice for portype invalid!\n");
        return;
        break;
    }
    int numport = 1;
    Port* temp = station->portsList;
    while (temp != NULL)
    {
        numport++;
        temp = temp->next;
    }
    Date date = { 0 };
    station->portsList = createAndAddPort(station->portsList, numport, portType, 2, NULL, date);
    if (station->portsList == NULL)
    {
        displayError(ERROR_MEMORY_ALLOCATION_FAILED);
        return;
    }
    Port* newport = station->portsList;
    while (newport->next != NULL)
    {
        newport = newport->next;
    }
    station->nPorts++;
    printf("your new port created in %s station (%d) || number new port %d\n", station->name, station->id, newport->num);;
    Car* car = dequeueCarMatchingPortType(station->carQueue, portType);
    if (car == NULL)
    {
        displayError(ERROR_CAR_NOT_FOUND);
        return;
    }
    else
    {
        assignCar2Port(car, newport);
    }
    return;
}
/*
 * Adds a new port to a specific station based on user input.
 * the user chooses the station by ID or name and selects the desired port type.
 * the new port is given the next available port number and is initialized as free.
 * after creating the port, the function tries to connect a matching car from the station’s queue.
 * if no match is found, the port remains free.
 */



void releasePorts(BSTStation* stationManager)
{
    if (stationManager == NULL || stationManager->root == NULL)
    {
        displayError(ERROR_INVALID_INPUT);
        return;
    }
    printf("enter station ID please\n");
    int id;
    if (scanf("%d", &id) != 1)
    {
        flushStdin();
        displayError(ERROR_INVALID_INPUT);
        return;
    }
    Station* station = searchStation(stationManager->root, &id, 0);
    if (station == NULL)
    {
        flushStdin();
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }
    Port* tempPort = station->portsList;
    int pluggout = 0;
    while (tempPort != NULL)
    {
        if (tempPort->p2car != NULL)
        {
            int diffMin = calcChargingMinutesFromDate(tempPort->tin);
            if (diffMin > 600)
            {
                pluggout++;
                tempPort->status = 2;
                tempPort->p2car->totalPayed += (double)diffMin * 1.2;
                tempPort->p2car->pPort = NULL;
                tempPort->p2car = NULL;
                Car* car = dequeueCarMatchingPortType(station->carQueue, tempPort->portType);
                if (car != NULL)
                {
                    tempPort->p2car = car;
                    car->pPort = tempPort;
                    tempPort->tin = CurrentTime();
                    printf("new car - %s is now connected to Port %d\n", car->nLicense, tempPort->num);
                }
            }
        }
        tempPort = tempPort->next;
    }
    printf("%d car were pluggout because of 10 Hours charging limit\n", pluggout);
    return;
}
/*
 * Releases all cars that have been charging for more than 10 hours at a given station.
 * For each  port:
 * - Updates the cars payment.
 * - disconnects the car.
 * - frees the port.
 * - tries to connect the next matching car from the queue.
 * At the end, prints how many cars were unplugged.
 */




Port* removePortByNumber(Port* portsList, int portNum, int* success)
{
    *success = 0;

    if (portsList == NULL) {
        printf("the Port list is empty.\n");
        return NULL;
    }

    Port* current = portsList;
    Port* prev = NULL;

    while (current != NULL) {
        if (current->num == portNum) {
            if (current->status != OUT_OF_ORDER) {
                displayError(ERROR_INVALID_INPUT);
                printf("the Port %d is not out of order so it cannot be removed.\n", portNum);
                return portsList;
            }
            if (current->p2car != NULL) {
                printf("Port %d is still connected to a car. Disconnect the car first.\n", portNum);
                return portsList;
            }

            if (prev == NULL) {
                portsList = current->next;
            }
            else {
                prev->next = current->next;
            }

            free(current);
            *success = 1;
            return portsList;
        }

        prev = current;
        current = current->next;
    }

    displayError(ERROR_PORT_NOT_FOUND);
    return portsList;
}
/*
 * Removes a specific port from the port list if:
 * The port is out of order and  No car is connected to it.
 * Returns the updated list and sets *success = 1 if the port was removed successfully.
 * if removal is not allowed, it prints the reason and returns the list unchanged.
 */



int printOutOfOrderPorts(Port* portsList)
{
    if (portsList == NULL) {
        printf("there are No ports in this station.\n");
        return FALSE;
    }
    Port* current = portsList;
    int foundAny = 0;
    printf("  out of order ports: \n");
    while (current != NULL) {
        if (current->status == OUT_OF_ORDER)
        {
            printf("  - Port Number: %d | Type: ", current->num);
            if (current->portType == SLOW) printf("SLOW");
            else if (current->portType == MID) printf("MID");
            else if (current->portType == FAST) printf("FAST");
            else printf("UNKNOWN");
            printf("\n");
            foundAny = 1;
        }
        current = current->next;
    }
    if (!foundAny)
    {
        printf("No out of order ports found in this station.\n");
        return FALSE;
    }
    return TRUE;
}
/*
 * Goes through the port list and prints all ports that are marked as OUT_OF_ORDER.
 * returns TRUE if any such ports were found, otherwise returns FALSE.
 * if the list is empty, prints a specific message.
 */


void remOutOrderPort(BSTStation* manager)
{
    if (manager == NULL || manager->root == NULL) {
        printf("No stations available in the system.\n");
        return;
    }

    int stationID;
    printf("\n--- Remove Out of Order Charging Port ---\n");
    printf("Enter station ID: ");
    scanf("%d", &stationID);


    Station* station = SearchStationById(manager->root, stationID);
    if (station == NULL)
    {
        flushStdin();
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }


    printf("\n out-of-order ports at station: %s (ID: %d) \n", station->name, station->id);
    int bool = printOutOfOrderPorts(station->portsList);
    if (bool == FALSE)
        return;

    int portToRemove;
    printf("Enter the number of the out-of-order port to remove: ");
    scanf("%d", &portToRemove);

    int success;
    station->portsList = removePortByNumber(station->portsList, portToRemove, &success);

    if (success) {

        station->nPorts--;
        printf("Port %d successfully removed from station %s.\n", portToRemove, station->name);
    }
    else {

        printf("Operation to remove port %d from station %s failed\n", portToRemove, station->name);
    }
}
/*
 * Allows the user to remove an OUT_OF_ORDER port from a selected station.
 * First, the user selects the station by ID.
 * Then, all out-of-order ports are shown at the station he chose.
 * The user chooses which port to remove, and the function removes it from the list
 * if  it is allowed and updates the port count, and at the end prints confirmation.
 */

void destroyPortList(Port* headPort)
{
    Port* current = headPort;
    while (current != NULL)
    {
        Port* nextPort = current->next;
        free(current);
        current = nextPort;
    }
}
/*
 * Frees the memory of the linked list of ports.
 *  used when deleting a station or shutting down the system.
 */