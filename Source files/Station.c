#define _CRT_SECURE_NO_WARNINGS
#include "Station.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Errors.h"
#define LARGENUM 100000
#define TypeSize 5
#define OUTOFORDER 3
#define STATIONNAME 256

Station* CreateStation(int id, const char* name, Coord coord, int nPorts, int nCars)
{
    Station* station = (Station*)malloc(sizeof(Station));
    if (!station) return NULL;

    station->name = _strdup(name);
    if (!station->name) {
        free(station);
        return NULL;
    }

    station->id = id;
    station->Coord = coord;
    station->nPorts = nPorts;
    station->portsList = NULL;
    station->nCars = nCars;

    station->carQueue = (qCar*)malloc(sizeof(qCar));
    if (station->carQueue == NULL) {
        free(station->name);
        free(station);
        return NULL;
    }
    station->carQueue->front = NULL;
    station->carQueue->rear = NULL;
    station->right = NULL;
    station->left = NULL;

    return station;
}
/*
 * Creates a new Station  with the given ID, name, coordinates, number of ports, and cars.
 * allocates memory for the station, initializes its fields, and creates an empty queue for cars.
 * returns a pointer to the station, or NULL if memory allocation fails.
 */



Station* insertBinaryStationTree(Station* rootManager, Station* newStation)
{
    if (rootManager == NULL) return newStation;

    if (newStation->id < rootManager->id)
        rootManager->left = insertBinaryStationTree(rootManager->left, newStation);
    else if (newStation->id > rootManager->id)
        rootManager->right = insertBinaryStationTree(rootManager->right, newStation);
    return rootManager;
}
/*
 * Inserts a station into the station binary search tree, sorted by station ID,
 * and recursively finds the correct position and returns the updated tree root.
 */



Station* SearchStationById(Station* Managerroot, int id)
{
    if (Managerroot == NULL) return NULL;
    if (id == Managerroot->id) return Managerroot;
    if (id < Managerroot->id) return SearchStationById(Managerroot->left, id);
    else return SearchStationById(Managerroot->right, id);
}
/*
 * Searches the station tree for a station with a specific ID.
 * returns a pointer to the station if found, or NULL if not.
 */


Station* SearchStationByName(Station* rootManager, const char* name)
{
    if (rootManager == NULL) return NULL;

    int compare = strcmp(name, rootManager->name);

    if (compare == 0)
        return rootManager;

    Station* leftFound = SearchStationByName(rootManager->left, name);
    if (leftFound != NULL) return leftFound;
    else
        return SearchStationByName(rootManager->right, name);
}
/*
 * Searches the station tree for a station with a specific name.
 * Returns the first matching station found using in-order .
 */


void locNearSt(BSTStation* Manager)
{
    if (Manager == NULL)
    {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }
    Coord coord;
    printf("put your coordinates x,y\n");
    if (scanf("%lf,%lf", &coord.x, &coord.y) != 2) {
        printf("Invalid input! Please enter coordinates in the format x,y\n");
        flushStdin();
        return;
    }
    Station* nearestStation = NULL;
    double Distance = LARGENUM;
    nearestStation = findNearestStation(Manager->root, coord, &nearestStation, &Distance);

    if (nearestStation == NULL) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    printf("\nNearest Station Info:\n");
    printf("Name: %s\n", nearestStation->name);
    printf("ID: %d\n", nearestStation->id);
    printf("Distance: %.3f\n", Distance);
    Port* temp1 = nearestStation->portsList;
    int InOrderPorts = 0;
    int slowports = 0, midports = 0, fastports = 0;
    while (temp1 != NULL)
    {
        if (temp1->status != 3)
        {
            InOrderPorts++;
            switch (temp1->portType)
            {
            case(SLOW):
                slowports++;
                break;
            case(MID):
                midports++;
                break;
            case(FAST):
                fastports++;
                break;
            }
        }
        temp1 = temp1->next;
    }
    printf("%d Ports InOrder in the station\n", InOrderPorts);
    printf("there are %d FastPorts, %d MidPorts, %d SlowPorts\n", fastports, midports, slowports);
    printf("Cars in Queue: %d\n\n", nearestStation->nCars);
}
/**
Locates and displays information about the nearest charging station.
this function ask the user to enter their coordinates (x, y),
it calculates the distance, and if a station is found, it prints
a  report including  name, ID, distance, and a summary
of its available charging ports and queued cars.*/



Station* findNearestStation(Station* root, Coord target, Station** ClosetStation, double* MinimumDis)
{
    if (root == NULL)
        return NULL;

    double distance = sqrt(pow(target.x - root->Coord.x, 2) + pow(target.y - root->Coord.y, 2));
    if (distance < *MinimumDis)
    {
        *MinimumDis = distance;
        *ClosetStation = root;
    }
    findNearestStation(root->left, target, ClosetStation, MinimumDis);
    findNearestStation(root->right, target, ClosetStation, MinimumDis);

    return *ClosetStation;
}
/*
 * Recursive helping function that compares the distance of each station to the given target coordinates.
 * it updates the pointer to the closest station found and the minimum distance so far.
 */


void CheckingStation(Station* station)
{
    if (station == NULL)
    {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }
    printf("station id: %d\n", station->id);
    printf("name %s\n", station->name);
    printf("coord %.2f,%.2f\n", station->Coord.x, station->Coord.y);
    printf("number of cars %d\n", station->nCars);
    printf("number of ports %d\n\n", station->nPorts);
}
/*
 * this function Prints  information about a single station, including  ID, name, coordinates,
 * number of charging cars, and number of ports.
 */

void printStation(Station* station)
{
    if (!station) return;

    printf("Station ID: %d\n", station->id);
    printf("Name: %s\n", station->name);
    printf("Coordinates: (%.2f, %.2f)\n", station->Coord.x, station->Coord.y);
    printf("Number of Ports: %d\n", station->nPorts);
    printf("Number of Cars: %d\n", station->nCars);
    printf("------------------------\n");
}
/*
 * Prints station information , including ID, name, coordinates,
 * number of ports, and number of cars.
 */

void printStationTree(Station* root)
{
    if (root == NULL) return;

    printStationTree(root->left);
    printStation(root);
    printStationTree(root->right);
}
//Recursively prints all stations in the tree using in-order method


void printAllStations(BSTStation* manager) {
    if (!manager || manager->root == NULL) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }
    printStationTree(manager->root);

}
/*
 *a function to print all stations in the system.
 * if the station tree is empty, an error is shown.
 */



void StationDetails(Station* rootManager)
{
    if (rootManager == NULL)
    {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }
    printf("-----------------------\n");
    printf("Station ID: %d\n", rootManager->id);
    printf("Station name: %s\n", rootManager->name);
    Port* temp = rootManager->portsList;
    int NumOfCars = 0;
    printf("all %d ports:\n", rootManager->nPorts);
    while (temp != NULL)
    {
        printf(" port %d ", temp->num);
        switch (temp->status)
        {
        case 1:
            printf("is occupied ");
            break;
        case 2:
            printf("is free ");
            break;
        case 3:
            printf("is out of order ");
            break;
        }

        switch (temp->portType)
        {
        case SLOW:
            printf("and SLOW Type\n");
            break;
        case MID:
            printf("and MID Type\n");
            break;
        case FAST:
            printf("and FAST Type\n");
            break;
        }
        if (temp->p2car != NULL)
            NumOfCars++;
        temp = temp->next;
    }
    printf("%d cars is charging in the station \n", NumOfCars);
    printf("%d cars wating in line in the station \n", rootManager->nCars);
    printf("-----------------------\n\n");
}
/*
 * Prints a full report of a station: ID, name, list of ports (number, status, type),
 * number of cars charging, and number of cars waiting in the queue.
 * if the station is NULL, an error is shown.
 */

void printStations(Station* rootManager)
{
    if (rootManager == NULL) return;
    printStations(rootManager->left);
    StationDetails(rootManager);
    printStations(rootManager->right);
}
/*
 *the function Recursively prints detailed information of all stations in the system,
 * including port and queue status, using StationDetails function
 */

void dispAllSt(BSTStation* Manager)
{
    if (!Manager || Manager->root == NULL)
    {
        printf("NO data availble in this station!\n");
        return;
    }
    printf("====== ALL STATIONS DATA ======\n\n");
    printStations(Manager->root);
}
/*
 * Displays information of all stations in the system.
 * if there are no stations, prints a specific message.
 */



Station* ById(Station* root, void* target)
{
    return SearchStationById(root, *((int*)target));
}

Station* ByName(Station* root, void* target) {
    return SearchStationByName(root, (char*)target);
}

Station* ByCoord(Station* root, void* target)
{
    Coord coord = *((Coord*)target);
    Station* closest = NULL;
    double Minimum = LARGENUM;
    return findNearestStation(root, coord, &closest, &Minimum);
}

static SearchFunc searchFunctions[] = {
    ById,
    ByName,
    ByCoord
};
/**
 * These are helper functions used for generic station search.
 * - ById: searches station by ID.
 * - ByName: searches station by name.
 * - ByCoord: finds the closest station to given coordinates.
 */

Station* searchStation(Station* root, void* target, int searchType)
{
    int numTypes = sizeof(searchFunctions) / sizeof(searchFunctions[0]);
    if (searchType < 0 || searchType >= numTypes)
    {
        printf("Search %d Type  not matching\n", searchType);
        return NULL;
    }

    return searchFunctions[searchType](root, target);
}
/**
 * a  station search function.
 * based on the searchType value (0 = ID, 1 = Name, 2 = Coord), it calls the appropriate function.
 * if the searchType is invalid, prints an error.
 */

void reportStStat(BSTStation* stationManager)
{
    if (stationManager == NULL || stationManager->root == NULL)
    {
        displayError(ERROR_INVALID_INPUT);
        return;
    }
 
    printf("enter\n0 - search station By ID\n1 - search station By Name\n");
    int SearchType;
    if (scanf("%d", &SearchType) != 1)
    {
        flushStdin();
        displayError(ERROR_INVALID_INPUT);
        return;
    }
    Station* station;
    switch (SearchType)
    {
    case 0:
        printf("enter ID of the station\n");
        int id;
        if (scanf("%d", &id))
        {
            flushStdin();
            displayError(ERROR_INVALID_INPUT);
            return;
        }
        station = searchStation(stationManager->root, &id, SearchType);
        if (station == NULL)
        {
            displayError(ERROR_STATION_NOT_FOUND);
            return;
        }
        break;
    case 1:
        printf("enter Name of the station\n");
        char StationName[STATIONNAME];
        scanf(" %[^\n]", StationName);
        station = searchStation(stationManager->root, StationName, SearchType);
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
    double occupiedPorts = 0.0;
    double OutOfOrderPorts = 0.0;
    Port* temp = station->portsList;
    while (temp != NULL)
    {
        if (temp->status == 1)
        {
            occupiedPorts++;
        }
        if (temp->status == 3)
            OutOfOrderPorts++;

        temp = temp->next;
    }
    if (station->nPorts == 0)
    {
        printf("there is no ports in the station!\n");
        return;
    }
    double efficiency = occupiedPorts / station->nPorts * 100;
    double BrokePorts = OutOfOrderPorts / station->nPorts * 100;
    printf("%.2lf%% efficiency in station %s\n", efficiency, station->name);
    printf("%.2lf%% of out of order ports in station %s\n", BrokePorts, station->name);
    if (station->nCars == 0)
    {
        printf("There is no traffic at %s station\n", station->name);
        return;
    }
    double BusyLevel = occupiedPorts / station->nCars;
    if (BusyLevel > 1)
    {
        printf("There is no car load at %s station\n\n", station->name);
        return;
    }
    if (BusyLevel == 1)
    {
        printf("There is a balance traffic at %s station\n\n", station->name);
        return;
    }
    if (BusyLevel < 1 && BusyLevel > 0.2)
    {
        printf("There is heavy traffic at %s station\n\n", station->name);
        return;
    }
    if (BusyLevel > 0.2)
    {
        printf("There is very heavy traffic at %s station\n\n", station->name);
        return;
    }
}
/*
 * This function first ask the user to select a search method (by ID or name)
 * and then searches for the station. If the station is found, it calculates
 * and prints key performance indicators, including the percentage of occupied
 * ports, the percentage of out-of-order ports, and a traffic level assessment
 * based on the ratio of occupied ports to queued cars.
 */



Station* findMinStationNode(Station* node)
{
    Station* current = node;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}
/*
 * Finds the node with the minimum ID in a binary search tree of stations.
 * this function is an helper used  for node deletion.
 * it recursively traverses the left child of a given node until it finds
 * the leftmost node, which represents the station with the smallest ID
 * in that subtree.
 */


Station* deleteStationNode(Station* root, int stationId)
{

    if (root == NULL) {
        return root;
    }

    if (stationId < root->id) {
        root->left = deleteStationNode(root->left, stationId);
    }
    else if (stationId > root->id) {
        root->right = deleteStationNode(root->right, stationId);
    }
    else {


        if (root->left == NULL) {
            Station* temp = root->right;
            if (root->portsList) {
                destroyPortList(root->portsList);
            }
            if (root->carQueue) {
                destroyCarQueue(root->carQueue);
            }
            if (root->name) {
                free(root->name);
            }
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Station* temp = root->left;
            if (root->portsList) {
                destroyPortList(root->portsList);
            }
            if (root->carQueue) {
                destroyCarQueue(root->carQueue);
            }
            if (root->name) {
                free(root->name);
            }
            free(root);
            return temp;
        }


        Station* replacement_node = findMinStationNode(root->right);


        if (root->portsList) {
            destroyPortList(root->portsList);
        }
        if (root->carQueue) {
            destroyCarQueue(root->carQueue);
        }
        if (root->name) {
            free(root->name);
        }


        root->id = replacement_node->id;
        root->name = _strdup(replacement_node->name);
        root->Coord = replacement_node->Coord;
        root->nPorts = replacement_node->nPorts;
        root->nCars = replacement_node->nCars;
        root->portsList = replacement_node->portsList;
        root->carQueue = replacement_node->carQueue;


        root->right = NULL;
        root->left = NULL;


        root->right = deleteStationNode(replacement_node, replacement_node->id);
    }
    return root;
}
/*
* Deletes a station from the tree by its ID.
* handles all cases (leaf, one child, two children).
* if the station has ports or cars, their memory is released properly.
* if two children exist, replaces the station with the smallest one in the right subtree.
*/



void closeSt(BSTStation* manager)
{
    if (manager == NULL || manager->root == NULL) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    char inputBuffer[100];
    int searchChoice;
    Station* targetStation = NULL;

    printf("\n--- Close Station Operation ---\n");
    printf("How would you like to locate the station to close?\n");
    printf("1. By ID\n");
    printf("2. By Name\n");
    printf("Enter your selection: ");

    if (scanf("%d", &searchChoice) != 1 || (searchChoice != 1 && searchChoice != 2))
    {
        displayError(ERROR_INVALID_INPUT);
        flushStdin();
        return;
    }
    while (getchar() != '\n');

    printf("Please enter the search term: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    size_t len = strlen(inputBuffer);
    if (len > 0 && inputBuffer[len - 1] == '\n') {
        inputBuffer[len - 1] = '\0';
    }

    if (searchChoice == 1)
    {
        int stationID_val = atoi(inputBuffer);
        targetStation = SearchStationById(manager->root, stationID_val);
    }
    else {
        targetStation = SearchStationByName(manager->root, inputBuffer);
    }

    if (targetStation == NULL) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    if (targetStation->nCars > 0)
    {
        displayError(ERROR_STATION_NOT_EMPTY);
        printf(" in Station %s (ID: %d)  There are %d active cars.\n",
            targetStation->name, targetStation->id, targetStation->nCars);
        return;
    }

    char name_to_print[STATIONNAME];
    strcpy(name_to_print, targetStation->name);
    int id_to_print = targetStation->id;

    manager->root = deleteStationNode(manager->root, id_to_print);

    printf("Station '%s' (ID: %d) has been successfully closed and removed from the system.\n", name_to_print, id_to_print);
}
/*
 * Deletes a station from the system after a user confirmation.
 * This function first ask the user to identify the station to be closed,
 * either by its ID or name. It validates that the station exists and that it
 * does not have any active cars. if all conditions are met, it removes the
 * station node from the binary tree, frees its allocated memory, and confirms
 * the successful operation to the user.
 */