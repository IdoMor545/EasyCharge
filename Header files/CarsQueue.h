#ifndef _CARS_TREE_H_
#define _CARS_TREE_H_
#include "CarsTree.h"
typedef struct carNode carNode;
typedef struct carNode
{
	Car* p2car;
	carNode* next;
}carNode;

typedef struct qCar
{
	carNode* front;
	carNode* rear;
}qCar;
int enqueueCarToStationQueue(qCar* QueueCar, Car* car);
Car* dequeueCarMatchingPortType(qCar* queue, portType type);
void printChargingCars(Port* portsList);
void printQueuedCars(qCar* carQueue);
void destroyCarQueue(qCar* queue);
#endif //! CARS_TREE_H_
