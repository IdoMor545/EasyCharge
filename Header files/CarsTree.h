#ifndef _CAR_TREE_H_
#define _CAR_TREE_H_
#include "PortList.h"
#define PLATESIZE 10
typedef struct tCar tCar;

typedef struct Car
{
	char nLicense[PLATESIZE];
	portType portType;
	double totalPayed;
	Port* pPort;
	int inqueue;
}Car;
typedef struct tCar
{
	Car* p2car;
	tCar* right;
	tCar* left;
}tCar;

typedef struct BSTCar
{
	tCar* root;
}BSTCar;

typedef struct TopCustomerEntry {
	char licenseNumber[PLATESIZE];
	double totalPayment;
} TopCustomerEntry;

int safeInput(char* buffer, int size);
int isValidLicense(const char* str);
void flushStdin();
Date CurrentTime();
// funcation for checking
void printCar(Car* car);
void printCarTree(tCar* root);
void dispCarsAtSt(BSTStation* stationManager);

Car* CreateCar(const char* nLicense, portType portType, double totalPayed, int inqueue);
tCar* insertBinaryCarTree(tCar* Managerroot, Car* car);
Car* searchCar(tCar* Managerroot, const char* nLicense);

// checking 
int getQueuePosition(qCar* queue, Car* car);
Station* findStationOfQueuedCar(Station* currentStation, Car* car);
Station* findStationByPort(Station* currentStation, Port* targetPort);
void checkCarStatus(BSTStation* stationManager, BSTCar* carManager);
int calcChargingMinutesFromDate(Date tin);
//func 4
void stopCharge(BSTStation* stationManager, BSTCar* carManager);
// func 2
void chargeCar(BSTStation* stationManager, BSTCar* carManager);

void dispTopCustomers(BSTCar* manager);
void insertIntoTopCustomers(const Car* newCar, TopCustomerEntry* topCustomersArr, int* count);
void collectTopCars(tCar* root, TopCustomerEntry* topCustomersArr, int* count);
tCar* findMinTCarNode(tCar* node);
tCar* deleteCarTNode(tCar* root, const char* license);
void remCustomer(BSTCar* manager);
#endif // !_CAR_TREE_H_