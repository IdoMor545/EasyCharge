#define _CRT_SECURE_NO_WARNINGS
#include "Station.h"
#include "CarsTree.h"
#include "Errors.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define PlateSize 10
#define STATIONNAME 256
#define MAX_TOP_CUSTOMERS 5
Date CurrentTime()
{
	Date current;
	time_t present = time(NULL);
	struct tm* localTime = localtime(&present);
	current.Year = localTime->tm_year + 1900;
	current.Month = localTime->tm_mon + 1;
	current.Day = localTime->tm_mday;
	current.Hour = localTime->tm_hour - 1;
	current.Min = localTime->tm_min;
	return current;
}
/*
 * This function returns the current system time.
 * it uses the system clock to get the current year, month, day, hour, and minute.
 * the returned Date can be used to track when a car starts or ends charging.
 */

Car* CreateCar(const char* nLicense, portType portType, double totalPayed, int inqueue)
{
	Car* car = (Car*)malloc(sizeof(Car));
	if (!car) return NULL;
	strcpy(car->nLicense, nLicense);
	car->portType = portType;
	car->totalPayed = totalPayed;
	car->inqueue = inqueue;
	car->pPort = NULL;
	return car;
}
/*
 * This function creates a new Car object with the provided  license number, port type, total payed, and queue status.
 * it allocates memory dynamically for the car, copies the license number, and initializes the other fields.
 * if memory allocation fails, the function returns NULL.
 */


tCar* insertBinaryCarTree(tCar* Managerroot, Car* car)
{
	if (car == NULL)return Managerroot;
	if (Managerroot == NULL)
	{
		tCar* newNode = (tCar*)malloc(sizeof(tCar));
		if (newNode == NULL) {
			displayError(ERROR_MEMORY_ALLOCATION_FAILED);
			return NULL;
		}
		newNode->p2car = car;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}
	int compare = strcmp(car->nLicense, Managerroot->p2car->nLicense);
	if (compare < 0)
		Managerroot->left = insertBinaryCarTree(Managerroot->left, car);
	else if (compare > 0)
		Managerroot->right = insertBinaryCarTree(Managerroot->right, car);
	return Managerroot;
}
/* 
* this is a funcation that add car to the carbinary tree recursively by license
* plates when the bigger goes to the right side and the smaller goes to the left side */


Car* searchCar(tCar* Managerroot, const char* nLicense)
{
	if (!Managerroot) return NULL;

	int cmp = strncmp(nLicense, Managerroot->p2car->nLicense, 8);
	if (cmp == 0) return Managerroot->p2car;
	else if (cmp < 0)
		return searchCar(Managerroot->left, nLicense);
	else
		return searchCar(Managerroot->right, nLicense);
}
/*
 * This function searches for a car in the binary tree by its license number.
 * if a matching car is found, it returns a pointer to the Car.
 * if not found, it returns NULL.
 */

void printCar(Car* car)
{
	printf("License Number: %s\n", car->nLicense);
	printf("Port Type: %d\n", car->portType);
	printf("Total Payed: %.2f\n", car->totalPayed);
	printf("In Queue: %d\n", car->inqueue);
	printf("----------------------\n");
}
/*
 * This function prints the details of one Car by:
 * license number, port type, total amount paid, and whether the car is in queue.
 */

void printCarTree(tCar* root)
{
	if (root == NULL) return;

	printCarTree(root->left);
	printCar(root->p2car);
	printCarTree(root->right);
}
/*
 * This function prints all cars in the binary tree in order
 * For each car, it calls the printCar function.
 * If the tree is empty, it returns and doesn't print anything.
 */

void dispCarsAtSt(BSTStation* stationManager)
{
	if (stationManager == NULL || stationManager->root == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	int choice = 0;
	char station_name[256] = { 0 };
	int station_id;
	Station* station = NULL;
	printf("please enter the option you  would like:\n1:search station by name\n2:search station by id\n");
	scanf("%d", &choice);
	while (getchar() != '\n');
	if (choice == 1) {
		printf("please enter  a vaild station name:\n");
		fgets(station_name, sizeof station_name, stdin);
		station_name[strcspn(station_name, "\n")] = '\0';
		station = SearchStationByName(stationManager->root, station_name);
		if (!station) {
			printf("Station not found.\n");
			return;
		}
	}
	else if (choice == 2)
	{
		printf("please enter  a vaild station id:\n");
		if (scanf("%d", &station_id) != 1)
		{
			flushStdin();
			displayError(ERROR_INVALID_INPUT);
			return;
		}
		while (getchar() != '\n');
		station = SearchStationById(stationManager->root, station_id);

	}
	else {
		printf("Invalid choice\n");
		return;
	}
	printf("\n=== Station Details ===\n");
	StationDetails(station);

	printf("\n=== Cars Charging at Station ===\n");
	printChargingCars(station->portsList);

	printf("\n=== Cars in Queue at Station ===\n");
	printQueuedCars(station->carQueue);

}
/*
 * This function allows the user to view all the cars at a specific station.
 * the user chooses whether to search the station by name or ID.
 * if the station is found, it prints the station details,
 * the list of charging cars, and the list of cars waiting in the queue.
 * if the station is not found or input is invalid, an appropriate message is shown.
 */


void checkCarStatus(BSTStation* stationManager, BSTCar* carManager)
{
	if (stationManager == NULL || carManager == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	if (stationManager->root == NULL || carManager->root == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	char license[PLATESIZE];
	char confirm;
	char confirmLine[10];
	do
	{
		printf("Enter car license 8 digits\n");
		if (!safeInput(license, sizeof(license)))
			return;

		if (!isValidLicense(license))
		{
			printf("License must be 8 digits Try again.\n");
			confirm = 'n';
		}
		else {
			printf("You entered '%s'. Is this correct? y - yes n - no:\n", license);
			if (!safeInput(confirmLine, sizeof(confirmLine)))
				return;
			confirm = confirmLine[0];
		}
	} while (confirm != 'y' && confirm != 'Y');

	printf("you entered %s\n", license);
	Car* car = searchCar(carManager->root, license);
	if (!car) {
		printf("%s not found in system.\n", license);
		return;
	}

	if (car->pPort != NULL)
	{
		Port* p = car->pPort;
		Station* station = findStationByPort(stationManager->root, p);
		if (station) {
			printf("Car charging at station %s (ID %d), port %d.\n",
				station->name, station->id, p->num);
			Date date = p->tin;
			int minutes = calcChargingMinutesFromDate(date);
			printf("Charging time: %d minutes.\n", minutes);
		}
		else {
			printf("Car is charging, but station not found.\n");
		}
		return;
	}

	if (car->inqueue == 1)
	{
		Station* station = findStationOfQueuedCar(stationManager->root, car);
		if (station) {
			int pos = getQueuePosition(station->carQueue, car);
			printf("Car in queue at station %s (ID %d), position %d.\n",
				station->name, station->id, pos);
			printf("Requested charge type: ");
			if (car->portType == FAST) printf("Fast\n\n");
			else if (car->portType == MID) printf("Medium\n\n");
			else if (car->portType == SLOW) printf("Slow\n\n");
			else printf("Unknown\n");
		}
		else {
			printf("Car marked as in queue but station not found.\n");
		}
		return;
	}

	printf("Car is not in queue or charging.\n");
}
/*
 * This function checks the current status of a specific car using its license number.
 * it searches for the car in the system and checks:
 *  if the car is charging, it prints the station and port it's charging at and the duration.
 *  if the car is waiting in queue, it prints the station and the car's position in the queue.
 *  if the car is not in the system or not charging or waiting, a specific message is shown.
 */



Station* findStationByPort(Station* currentStation, Port* targetPort) {
	if (currentStation == NULL || targetPort == NULL)
		return NULL;


	Port* currentPort = currentStation->portsList;
	while (currentPort != NULL) {
		if (currentPort == targetPort) {
			return currentStation;
		}
		currentPort = currentPort->next;
	}

	Station* foundInLeft = findStationByPort(currentStation->left, targetPort);
	if (foundInLeft != NULL)
		return foundInLeft;


	Station* foundInRight = findStationByPort(currentStation->right, targetPort);
	return foundInRight;
}
/*
 * This function searches recursively through all stations to find which one owns a given port.
 * it compares the target port pointer to each port in the station’s port list.
 * if the port is found, it returns a pointer to the owning station.
 * if not, it returns NULL.
 */


Station* findStationOfQueuedCar(Station* currentStation, Car* car) {
	if (currentStation == NULL || car == NULL) return NULL;

	carNode* currentNode = currentStation->carQueue->front;
	while (currentNode != NULL) {
		if (currentNode->p2car == car) {
			return currentStation;
		}
		currentNode = currentNode->next;
	}


	Station* foundLeft = findStationOfQueuedCar(currentStation->left, car);
	if (foundLeft != NULL) return foundLeft;


	return findStationOfQueuedCar(currentStation->right, car);
}
/*
 * This function searches all stations to find which one has the given car in its queue.
 * it checks each station’s queue from the front.
 * if it finds the car, it returns a pointer to that station.
 * if not, it returns NULL.
 */


int getQueuePosition(qCar* queue, Car* car) {
	if (queue == NULL || car == NULL || queue->front == NULL) return 0;

	carNode* currentNode = queue->front;
	int position = 1;

	while (currentNode != NULL) {
		if (currentNode->p2car == car)
			return position;
		currentNode = currentNode->next;
		position++;
	}
	return 0;

}
/*
 * This function returns the position  of a car in a station's queue.
 * it goes through the queue from the front and counts positions until it finds the car.
 * if the car is not in the queue, it returns 0.
 */

void stopCharge(BSTStation* stationManager, BSTCar* carManager)
{
	if (stationManager == NULL || carManager == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	if (stationManager->root == NULL || carManager->root == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	char license[PLATESIZE];
	char confirm;
	char confirmLine[10];
	do
	{
		printf("Enter car license 8 digits\n");
		if (!safeInput(license, sizeof(license)))
			return;

		if (!isValidLicense(license))
		{
			printf("License must be 8 digits Try again.\n");
			confirm = 'n';
		}
		else {
			printf("You entered '%s'. Is this correct? y - yes n - no:\n", license);
			if (!safeInput(confirmLine, sizeof(confirmLine)))
				return;
			confirm = confirmLine[0];
		}
	} while (confirm != 'y' && confirm != 'Y');
	
	Car* car = searchCar(carManager->root, license);
	if (car == NULL)
	{
		printf("car with license %s not found!\n", license);
		return;
	}
	if (car->pPort == NULL)
	{
		printf("car with license %s not connected to any Port!\n", license);
		return;
	}

	time_t endTime = time(NULL);
	struct tm start_tm;
	start_tm.tm_year = car->pPort->tin.Year - 1900;
	start_tm.tm_mon = car->pPort->tin.Month - 1;
	start_tm.tm_mday = car->pPort->tin.Day;
	start_tm.tm_hour = car->pPort->tin.Hour;
	start_tm.tm_min = car->pPort->tin.Min;
	start_tm.tm_sec = 0;
	start_tm.tm_isdst = -1;
	time_t StartTime = mktime(&start_tm);
	double ChargingTimeInSec = difftime(endTime, StartTime);
	double ChargingTimeInMin = ChargingTimeInSec / 60;


	Port* PortCar = car->pPort;
	Station* station = findStationByPort(stationManager->root, PortCar);
	car->inqueue = 0;
	car->pPort = NULL;
	car->totalPayed += ChargingTimeInMin * 1.2;
	PortCar->status = 2;
	PortCar->p2car = NULL;
	PortCar->tin = (Date){ 0 };
	printf("car %s no longer plug in and has to pay %.2lf\n", license, (ChargingTimeInMin * 1.2));
	PortCar->p2car = dequeueCarMatchingPortType(station->carQueue, PortCar->portType);
	if (PortCar->p2car == NULL)
	{
		printf("no matching car has found in the Queue\n\n");
		return;
	}
	else
	{
		station->nCars--;
		PortCar->tin = CurrentTime();
		PortCar->p2car->pPort = PortCar;
		printf("car %s plugged in to Port %d in station %s (%d)\n\n", PortCar->p2car->nLicense, PortCar->num, station->name, station->id);
		return;
	}
}
/*
 * This function is used to disconnect a car from its charging port.
 * it asks for the license plate, verifies the car is charging,
 * calculates the total charging time, updates the total payed,
 * and frees the port.
 */


int calcChargingMinutesFromDate(Date tin)
{
	time_t endTime = time(NULL);

	struct tm start_tm = { 0 };
	start_tm.tm_year = tin.Year - 1900;
	start_tm.tm_mon = tin.Month - 1;
	start_tm.tm_mday = tin.Day;
	start_tm.tm_hour = tin.Hour;
	start_tm.tm_min = tin.Min;
	start_tm.tm_sec = 0;

	time_t startTime = mktime(&start_tm);
	if (startTime == (time_t)(-1)) {
		printf("eror occured converting time.\n");
		return -1;
	}

	double diffSeconds = difftime(endTime, startTime);
	int diffMinutes = (int)(diffSeconds / 60.0);
	return diffMinutes;
}
/*
 * This function calculates how many minutes have passed since a given Date.
 * it is used to know how long a car has been charging.
 * if there is a problem converting the date, it returns -1.
 */


int isValidLicense(const char* str)
{
	int length = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] < '0' || str[i] > '9') return FALSE;
		length++;
	}
	return (length == 8);
}
/*
* This func is getting the license and checking if the input is 
* in the correct length and has only numbers return True or False
*/

void flushStdin()
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}
/*
* This func clearing the buffer
*/




int safeInput(char* buffer, int size) {
	int firstTry = 1;
	while (1) {
		if (fgets(buffer, size, stdin) == NULL)
			return 0;

		if (strchr(buffer, '\n') == NULL)
			flushStdin();

		buffer[strcspn(buffer, "\n")] = '\0';

		if (strlen(buffer) == 0) {
			if (!firstTry)
				printf("(Empty input, try again)\n");
			firstTry = 0;
			continue;
		}
		return 1;
	}
}
/*
* This func make sure the string you are putting is in the right size 
*/


void chargeCar(BSTStation* stationManager, BSTCar* carManager)
{
	if (stationManager == NULL || carManager == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	if (stationManager->root == NULL || carManager->root == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	char license[PlateSize];
	char confirmLine[10];
	char confirm;

	do
	{
		printf("Enter car license 8 digits\n");
		if (!safeInput(license, sizeof(license)))
			return;

		if (!isValidLicense(license))
		{
			printf("License must be 8 digits Try again.\n");
			confirm = 'n';
		}
		else {
			printf("You entered '%s'. Is this correct? y - yes n - no:\n", license);
			if (!safeInput(confirmLine, sizeof(confirmLine)))
				return;
			confirm = confirmLine[0];
		}
	} while (confirm != 'y' && confirm != 'Y');

	int searchType = -1;
	char input[20];
	Station* station = NULL;

	while (searchType != 0 && searchType != 1) {
		printf("Choose station search method:\n0 - by ID\n1 - by NAME\n");
		if (!safeInput(input, sizeof(input)))
			return;
		searchType = atoi(input);
		if (searchType != 0 && searchType != 1)
			printf("Invalid choice. Please enter 0 or 1.\n");
	}

	if (searchType == 0) {
		printf("Enter station ID:\n");
		if (!safeInput(input, sizeof(input))) return;
		int id = atoi(input);
		station = searchStation(stationManager->root, &id, 0);
	}
	else {
		printf("Enter station name:\n");
		char name[STATIONNAME];
		if (!safeInput(name, sizeof(name))) return;
		station = searchStation(stationManager->root, name, 1);
	}

	if (station == NULL) {
		printf("Station not found. Returning to menu.\n");
		return;
	}

	Car* car = searchCar(carManager->root, license);
	if (car == NULL) {
		printf("Car not found. Adding new car...\n");
		int type = 0;

		do {
			printf("Enter charging type:\n1 - FAST\n2 - MID\n3 - SLOW\n");
			if (!safeInput(input, sizeof(input)))
				return;
			type = atoi(input);

			if (type < 1 || type > 3) {
				printf("Invalid type. Please choose 1, 2, or 3.\n");
				confirm = 'n';
			}
			else {
				printf("You selected type %d. Is this correct? y - yes n - no\n", type);
				if (!safeInput(confirmLine, sizeof(confirmLine)))
					return;
				confirm = confirmLine[0];
			}
		} while (confirm != 'y' && confirm != 'Y');

		portType carType = (type == 1) ? FAST : (type == 2) ? MID : SLOW;

		car = CreateCar(license, carType, 0, 0);
		if (!car) {
			printf("Failed to create car.\n");
			return;
		}
		insertBinaryCarTree(carManager->root, car);
	}

	findMatchingPortToCar(car, station);
}
/*
 * This function connects a car to a station for charging.
 * The user chooses the station by ID or name.
 * if the car is not found in the system, the function creates it based on user input.
 * then it attempts to find a matching available port.
 * if no port is available, the car is added to the station’s queue.
 */


void insertIntoTopCustomers(const Car* newCar, TopCustomerEntry* topCustomersArr, int* count)
{
	if (*count < MAX_TOP_CUSTOMERS) {
		topCustomersArr[*count].totalPayment = newCar->totalPayed;
		strcpy(topCustomersArr[*count].licenseNumber, newCar->nLicense);
		(*count)++;
	}

	else if (newCar->totalPayed > topCustomersArr[MAX_TOP_CUSTOMERS - 1].totalPayment) {

		topCustomersArr[MAX_TOP_CUSTOMERS - 1].totalPayment = newCar->totalPayed;
		strcpy(topCustomersArr[MAX_TOP_CUSTOMERS - 1].licenseNumber, newCar->nLicense);
	}

	else {
		return;
	}


	for (int i = 0; i < *count - 1; i++) {
		for (int j = i + 1; j < *count; j++) {
			if (topCustomersArr[i].totalPayment < topCustomersArr[j].totalPayment) {
				TopCustomerEntry temp = topCustomersArr[i];
				topCustomersArr[i] = topCustomersArr[j];
				topCustomersArr[j] = temp;
			}
		}
	}
}
/*
 * This helper function updates the top customers array with a new car if it qualifies.
 * if there's space in the array , it adds the car directly.
 * otherwise, it only adds the car if its total payment is higher than the lowest in the top 5.
 * then, it sorts the array  based on total payments.
 */

void collectTopCars(tCar* root, TopCustomerEntry* topCustomersArr, int* count)
{

	if (root == NULL) {
		return;
	}

	if (root->p2car != NULL) {
		insertIntoTopCustomers(root->p2car, topCustomersArr, count);
	}

	collectTopCars(root->left, topCustomersArr, count);
	collectTopCars(root->right, topCustomersArr, count);
}
/*
 * This is a recursive function it goes through the car binary tree and
 * sends each car to the insertIntoTopCustomers function to build the top 5 customers list.
 */

void dispTopCustomers(BSTCar* manager)
{

	TopCustomerEntry topCustomers[MAX_TOP_CUSTOMERS];
	int currentTopCustomersCount = 0;

	for (int i = 0; i < MAX_TOP_CUSTOMERS; i++) {
		topCustomers[i].totalPayment = 0.0;
		strcpy(topCustomers[i].licenseNumber, "");
	}

	if (manager != NULL && manager->root != NULL)
	{
		collectTopCars(manager->root, topCustomers, &currentTopCustomersCount);
	}


	printf("\n--- Top 5 Customers with Highest Expenses ---\n");
	if (currentTopCustomersCount == 0) {
		printf("No customers with payments registered in the system.\n");
	}
	else {
		for (int i = 0; i < currentTopCustomersCount; i++) {
			printf("Customer %d:\n", i + 1);
			printf("  License Number: %s\n", topCustomers[i].licenseNumber);
			printf("  Total Payment: %.2f nis\n", topCustomers[i].totalPayment);
			printf("--------------------------------------\n");
		}
	}

}
/*
 * This function displays the top 5 cars with the highest total payments in the system.
 * it builds the list by scanning the car tree, collecting and sorting the top paying customers.
 * if there are no cars with payments, a message is printed.
 */

tCar* findMinTCarNode(tCar* node)
{
	tCar* current = node;
	while (current && current->left != NULL) {
		current = current->left;
	}
	return current;
}
/*
 * Finds the node with the minimum value in a binary search tree.
 * This is a helper function for deleting nodes from a tree
 */

tCar* deleteCarTNode(tCar* root, const char* license)
{
	if (root == NULL) {
		return root;
	}

	int cmp = strncmp(license, root->p2car->nLicense, 8);

	if (cmp < 0) {
		root->left = deleteCarTNode(root->left, license);
	}
	else if (cmp > 0) {
		root->right = deleteCarTNode(root->right, license);
	}
	else {


		if (root->left == NULL) {
			tCar* temp = root->right;
			free(root->p2car);
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			tCar* temp = root->left;
			free(root->p2car);
			free(root);
			return temp;
		}


		tCar* temp = findMinTCarNode(root->right);


		strncpy(root->p2car->nLicense, temp->p2car->nLicense, PLATESIZE);
		root->p2car->nLicense[PLATESIZE - 1] = '\0';

		root->p2car->portType = temp->p2car->portType;
		root->p2car->totalPayed = temp->p2car->totalPayed;
		root->p2car->pPort = temp->p2car->pPort;
		root->p2car->inqueue = temp->p2car->inqueue;



		root->right = deleteCarTNode(root->right, temp->p2car->nLicense);
	}
	return root;

}
/*
 * This function removes a car from the car binary tree by its license number.
 * it handles all deletion cases: leaf, one child, or two children.
 * if the car has two children, it finds the smallest car
 * and uses it to replace the deleted one.
 * it also frees the memory of the car and the tree node.
 */

void remCustomer(BSTCar* manager)

{
	if (manager == NULL || manager->root == NULL) {
		displayError(ERROR_CAR_NOT_FOUND);
		return;
	}
	while (getchar() != '\n');
	char license[PLATESIZE];
	printf("\n--- Remove Customer ---\n");
	printf("Enter car license 8 digits number to remove: ");
	fgets(license, PLATESIZE, stdin);
	if (strchr(license, '\n') == NULL)
	{
		flushStdin();
	}
	license[strcspn(license, "\n")] = 0;

	if (!isValidLicense(license))
	{
		printf("License must contain 8 numbers!\n");
		return;
	}
	Car* carToRemove = searchCar(manager->root, license);
	if (carToRemove == NULL) {
		displayError(ERROR_CAR_NOT_FOUND);
		return;
	}

	if (carToRemove->pPort != NULL)
	{
		printf("Error: Car %s is currently charging  Cannot be removed.\n",carToRemove->nLicense);
		return;
	}
	if (carToRemove->inqueue == 1) {
		printf("Error: Car %s is currently in a queue. Cannot be remove.\n",
			carToRemove->nLicense);
		return;
	}

	manager->root = deleteCarTNode(manager->root, license);

	printf("Car %s successfully removed from the system.\n", license);
}
/*
 * This function removes a car from the system using its license number.
 *  first checks if the car exists and makes sure the car is not currently charging or in a queue.
 * if all checks pass, it deletes the car from the binary tree.
 * Otherwise, it prints an appropriate error message.
 */