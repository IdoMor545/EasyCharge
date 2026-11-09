#include "Station.h"
#include "CarsTree.h"
#include "Errors.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int enqueueCarToStationQueue(qCar* QueueCar, Car* car)
{
    if (car == NULL) return FALSE;
    if (QueueCar == NULL) return FALSE;
    carNode* node = (carNode*)malloc(sizeof(carNode));
    if (node == NULL) return FALSE;

    node->p2car = car;
    node->next = NULL;
    car->inqueue = 1;

    if (QueueCar->rear == NULL)
    {
        QueueCar->front = node;
        QueueCar->rear = node;
        return TRUE;
    }
    else
    {
        QueueCar->rear->next = node;
        QueueCar->rear = node;
        return TRUE;
    }
}
/*
 * This function adds a car to the end of a station's queue.
 * If the queue is empty, the car becomes  front and rear of the queue.
 * It sets the car's 'inqueue'  to 1 to indicate that it is waiting in line.
 * If memory allocation fails or the inputs are invalid , the function returns FALSE.
 * Otherwise, it returns TRUE.
 */

Car* dequeueCarMatchingPortType(qCar* queue, portType type)
{
    if (!queue || !queue->front)
        return NULL;

    carNode* current = queue->front;
    carNode* prev = NULL;

    while (current)
    {
        if (current->p2car && current->p2car->portType == type)
        {
            Car* car = current->p2car;
            if (prev == NULL)
            {
                queue->front = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            if (current == queue->rear)
            {
                queue->rear = prev;
            }
            free(current);
            return car;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}
/*
 * this function removes and returns the first car in the queue that matches a specific port type.
 * it goes through the queue from the front, looking for a car with the specific port type (FAST, MID, SLOW).
 * if a matching car is found, it is removed from the queue and returned.
 * if no matching car is found, or the queue is empty, the function returns NULL.
 */



void printChargingCars(Port* portsList)
{
    if (portsList == NULL) {
        displayError(ERROR_CAR_NOT_FOUND);
        return;
    }
    printf("Cars currently charging:\n");
    Port* current = portsList;
    while (current != NULL) {
        if (current->p2car != NULL) {
            printf("License: %s | Port: %d | Type: ", current->p2car->nLicense, current->num);
            if (current->portType == FAST) printf("FAST");
            else if (current->portType == MID) printf("MID");
            else if (current->portType == SLOW) printf("SLOW");
            else printf("Unknown");
            int minutes = calcChargingMinutesFromDate(current->tin);
            printf(" | Charging for: %d minutes\n", minutes);
        }
        current = current->next;
    }
}
/*
 * this function prints all cars that are currently charging at the station's ports.
 * for each port that has a car connected, it prints the car's license number,
 * the port number, the port type (FAST, MID, or SLOW), and how many minutes the car has been charging.
 */



void printQueuedCars(qCar* carQueue) {
    if (carQueue == NULL || carQueue->front == NULL) {
        displayError(ERROR_QUEUE_EMPTY);
        return;
    }
    printf("Cars in queue:\n");
    carNode* current = carQueue->front;
    while (current != NULL) {
        printf("License: %s | Requested charge type: ", current->p2car->nLicense);
        if (current->p2car->portType == FAST) printf("FAST\n");
        else if (current->p2car->portType == MID) printf("MID\n");
        else if (current->p2car->portType == SLOW) printf("SLOW\n");
        else printf("Unknown\n");
        current = current->next;
    }
}
/*
 * this function prints all cars currently waiting in the station's queue.
 * for each car in the queue, it shows the license number and the requested port type FAST, MID, or SLOW.
 * if the queue is empty or invalid, an error message is printed.
 */

void destroyCarQueue(qCar* queue)
{
    if (queue == NULL) {
        return;
    }

    carNode* current = queue->front;
    while (current != NULL) {
        carNode* nextNode = current->next;
        free(current);
        current = nextNode;
    }
    free(queue);
}
/*
 * Frees the memory of the station's car queue.
 * goes through all car nodes and frees them, then frees the queue itself.
 */