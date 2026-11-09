#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Station.h"
#include "PortList.h"
#include "CarsTree.h"
#include "CarsQueue.h"
#include "Errors.h"
#define SIZE 256
#define TypeSize 5
#define PlateSize 10

void LoadFileStation(const char* filename, BSTStation* StationManager)
{
	FILE* pfile = fopen(filename, "r");
	if (pfile == NULL)
	{
		displayError(ERROR_FILE_OPEN_FAILED);
		return;
	}
	char buffer[SIZE];

	fgets(buffer, SIZE, pfile); // this line is too no read the first line of the text
	while (fgets(buffer, SIZE, pfile) != NULL)
	{
		int id;
		char StationName[SIZE];
		int NumPorts;
		Coord coord;
		if (sscanf(buffer, "%d,%99[^,],%d,%lf,%lf", &id, StationName, &NumPorts, &coord.x, &coord.y) == 5)
		{
			Station* station = CreateStation(id, StationName, coord, NumPorts, 0);
			if (station != NULL)
			{
				printf("station %s number %d succefully uploaded!\n", station->name, station->id);
			}
			StationManager->root = insertBinaryStationTree(StationManager->root, station);
			if (StationManager->root == NULL)
				displayError(ERROR_MEMORY_ALLOCATION_FAILED);
		}
		else
			printf("failed in making a station\n");
	}
	fclose(pfile);
	return;
}
/*
 * Loads station data from a text file and inserts the stations into the binary search tree.
 * for each line in the file , the function extracts the station's ID, name,
 * number of ports, and coordinates, then creates a Station object and inserts it into the tree.
 * If file opening fails or parsing fails, shows an error message.
 */



void LoadFilePorts(const char* filename, BSTStation* StationManager, BSTCar* CarManager)
{
	FILE* pfile = fopen(filename, "r");
	if (pfile == NULL)
	{
		displayError(ERROR_FILE_OPEN_FAILED);
		return;
	}
	char buffer[SIZE];
	fgets(buffer, SIZE, pfile);
	while (fgets(buffer, SIZE, pfile) != NULL)
	{
		int StationId;
		int PortNumber;
		char StrType[TypeSize];
		int Status;
		Date tin;
		char CarLicense[PlateSize];
		int matched = sscanf(buffer, "%d,%d,%4[^,],%d,%d,%d,%d,%d,%d,%9[^,]", &StationId, &PortNumber, StrType, &Status, &tin.Year, &tin.Month, &tin.Day, &tin.Hour, &tin.Min, &CarLicense);
		if (matched != 10)
		{
			displayError(ERROR_INVALID_INPUT);
			return;
		}
		portType Type;

		if (strcmp(StrType, "FAST") == 0) Type = FAST;
		else if (strcmp(StrType, "MID") == 0) Type = MID;
		else if (strcmp(StrType, "SLOW") == 0) Type = SLOW;
		else
		{
			displayError(ERROR_INVALID_INPUT);
			return;
		}
		Station* stationPort = SearchStationById(StationManager->root, StationId);
		if (stationPort == NULL)
		{
			displayError(ERROR_STATION_NOT_FOUND);
			continue;
		}
		Car* CarPort = searchCar(CarManager->root, CarLicense);
		if (CarPort == NULL)
		{
			printf("Port %d in station %d has conected and has no car!\n", PortNumber, StationId);
		}
		else
			printf("Port %d conected to station %d and has car with num %s\n", PortNumber, StationId, CarLicense);
		stationPort->portsList = createAndAddPort(stationPort->portsList, PortNumber, Type, Status, CarPort, tin);
		if (stationPort->portsList == NULL)
		{
			printf("Port %d could not be added to the station %d\n", PortNumber, StationId);
		}
	}
	fclose(pfile);
	return;
}
/*
 * Loads port data from a text file and adds the ports to their right stations.
 * each line includes station ID, port number, port type, status, start time, and license of a connected car.
 * the function connects the port to the correct station and links the car if it exists.
 * if port type is invalid or the station is not found, displays an error.
 */



void LoadFileBinaryCars(const char* filename, BSTCar* CarManager, BSTStation* StationManager)
{
	FILE* pfile = fopen(filename, "r");
	if (pfile == NULL)
	{
		displayError(ERROR_FILE_OPEN_FAILED);
		return;
	}char buffer[SIZE];
	fgets(buffer, SIZE, pfile);
	while (fgets(buffer, SIZE, pfile) != NULL)
	{
		char LicensePlate[PlateSize];
		char StrType[TypeSize];
		double TotalPayed;
		int StationId;
		int PortNumber;
		int InQueue;
		int matched = sscanf(buffer, "%9[^,],%4[^,],%lf,%d,%d,%d", LicensePlate, StrType, &TotalPayed, &StationId, &PortNumber, &InQueue);
		if (matched != 6)
		{
			displayError(ERROR_INVALID_INPUT);
			return;
		}
		portType Type;

		if (strcmp(StrType, "FAST") == 0) Type = FAST;
		else if (strcmp(StrType, "MID") == 0) Type = MID;
		else if (strcmp(StrType, "SLOW") == 0) Type = SLOW;
		else
		{
			displayError(ERROR_INVALID_INPUT);
			return;
		}
		Car* car = CreateCar(LicensePlate, Type, TotalPayed, InQueue);
		Station* station = SearchStationById(StationManager->root, StationId);
		if (station != NULL)
		{
			Port* carport = searchPortByNumber(station->portsList, PortNumber);
			car->pPort = carport;
		}
		CarManager->root = insertBinaryCarTree(CarManager->root, car);
	}
	printCarTree(CarManager->root);
	fclose(pfile);
	return;
}
/*
 * Loads car data from a text file and adds the cars to the binary search tree.
 * each line contains license number, port type, total payed, station ID, port number, and queue status.
 * if the car is connected to a port, the function finds the station and port and links them to the car.
 * at the end the car  inserted into the tree.
 */

void LoadFileQueueCars(const char* filename, BSTStation* StationManager, BSTCar* CarManager)
{
	FILE* pfile = fopen(filename, "r");
	if (pfile == NULL)
	{
		displayError(ERROR_FILE_OPEN_FAILED);
		return;
	}
	char buffer[SIZE];
	fgets(buffer, SIZE, pfile);
	while (fgets(buffer, SIZE, pfile) != NULL)
	{
		char CarLicense[PlateSize];
		int StationId;
		int matched = sscanf(buffer, "%9[^,],%d", CarLicense, &StationId);
		if (matched != 2)
		{
			displayError(ERROR_INVALID_INPUT);
			return;
		}
		Car* car = searchCar(CarManager->root, CarLicense);
		Station* station = SearchStationById(StationManager->root, StationId);
		if (enqueueCarToStationQueue(station->carQueue, car) == FALSE)
		{
			printf("error in car %s joing the Queue!\n", car->nLicense);
		}
		station->nCars++;
		printf("the car %s is in line of station %d\n", station->carQueue->rear->p2car->nLicense, station->id);
	}
	fclose(pfile);
	return;
}

/*
 * Loads queue data from a text file and adds each car to the correct station's queue.
 * each line includes a license number and a station ID.
 * the function searches for the car and the station, then adds the car to the queue.
 * if the operation fails, an error message is printed.
 */

void loadfile(BSTStation* StationManager, BSTCar* CarManager, char* StationFileName, char* PortFileName, char* CarsFileName, char* QueueCarsFileName)
{
	LoadFileStation(StationFileName, StationManager);
	LoadFileBinaryCars(CarsFileName, CarManager, StationManager);
	LoadFilePorts(PortFileName, StationManager, CarManager);
	LoadFileQueueCars(QueueCarsFileName, StationManager, CarManager);
	return;
}
/*
 * Calls all the  loading functions (stations, cars, ports, and queues).
 * each step prints status messages and errors if they occur.
 */



void writeStationFile(Station* root, FILE* file)
{
	if (root == NULL) return;

	writeStationFile(root->left, file);

	fprintf(file, "%d,%s,%d,%.2lf,%.2lf\n", root->id, root->name, root->nPorts, root->Coord.x, root->Coord.y);

	writeStationFile(root->right, file);
}
/*
 * Writes all station data into a text file in in-order
 *for each station, it saves its ID, name, number of ports, and coordinates.
 * it isused when saving the current state of the system.
 */


void writePortsFile(Station* root, FILE* file)
{
	if (root == NULL) return;
	writePortsFile(root->left, file);

	Port* port = root->portsList;
	while (port != NULL)
	{
		char portType[10];
		switch (port->portType)
		{
		case SLOW:
			strcpy(portType, "SLOW");
			break;
		case MID:
			strcpy(portType, "MID");
			break;
		case FAST:
			strcpy(portType, "FAST");
			break;
		default:
			strcpy(portType, "UNKNOWN");
		}
		char carLicense[PLATESIZE];
		if (port->p2car == NULL)
		{
			strcpy(carLicense, "-1");
			fprintf(file, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n", root->id, port->num, portType, port->status, 0, 0, 0, 0, 0, carLicense);
		}
		else
		{
			strcpy(carLicense, port->p2car->nLicense);
			fprintf(file, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n", root->id, port->num, portType, port->status, port->tin.Year, port->tin.Month, port->tin.Day, port->tin.Hour, port->tin.Min, carLicense);
		}
		port = port->next;
	}

	writePortsFile(root->right, file);
}
/*
 * Writes all ports of all stations into a text file.
 * for each port, it saves station ID, port number, type, status, start time, and connected car if there any.
 * The function handles all port types and writes -1 if no car is connected.
 */

void writeCarsFile(Station* stationroot, tCar* carRoot, FILE* file)
{
	if (stationroot == NULL || carRoot == NULL)	return;


	writeCarsFile(stationroot, carRoot->left, file);

	Car* car = carRoot->p2car;
	char carType[10];
	switch (car->portType)
	{
	case SLOW:
		strcpy(carType, "SLOW");
		break;
	case MID:
		strcpy(carType, "MID");
		break;
	case FAST:
		strcpy(carType, "FAST");
		break;
	default:
		strcpy(carType, "UNKNOWN");
	}

	if (carRoot->p2car->pPort == NULL)
	{
		fprintf(file, "%s,%s,%.2f,%d,%d,%d\n", car->nLicense, carType, car->totalPayed, 0, 0, car->inqueue);
	}
	else
	{
		Station* station = findStationByPort(stationroot, car->pPort);
		fprintf(file, "%s,%s,%.2f,%d,%d,%d\n", car->nLicense, carType, car->totalPayed, station->id, car->pPort->num, car->inqueue);
	}

	writeCarsFile(stationroot, carRoot->right, file);
}
/*
 * Writes all car data into a text file using in-order .
 * for each car, it saves license, port type, total payed, station ID, port number, and whether it's in queue.
 * if the car is not connected to a port, the default values are used.
 */

void writeLineOfCarsFile(Station* station, FILE* file)
{
	if (station == NULL) return;
	writeLineOfCarsFile(station->left, file);

	carNode* temp = station->carQueue->front;
	while (temp != NULL)
	{
		fprintf(file, "%s,%d\n", temp->p2car->nLicense, station->id);
		temp = temp->next;
	}

	writeLineOfCarsFile(station->right, file);
}
/*
 * Writes queue data for all stations into a text file.
 * for each car in a station's queue, it writes the license number and the station ID.
 */

void updateFiles(BSTStation* stationmanager, BSTCar* carmanager, const char* stationFile, const char* portsFile,
	const char* carstreeFile, const char* carqueueFile)
{
	if (stationmanager == NULL || carmanager == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	if (stationmanager->root == NULL || carmanager->root == NULL)
	{
		displayError(ERROR_INVALID_INPUT);
		return;
	}
	FILE* fileStation = fopen(stationFile, "w");
	if (fileStation == NULL)
	{
		printf("couldnt open station file!\n");
		return;
	}
	FILE* filePorts = fopen(portsFile, "w");
	if (filePorts == NULL)
	{
		printf("couldnt open ports file!\n");
		fclose(fileStation);
		return;
	}

	FILE* fileCars = fopen(carstreeFile, "w");
	if (fileCars == NULL)
	{
		printf("couldnt open cars file!\n");
		fclose(fileStation);
		fclose(filePorts);
		return;
	}
	FILE* fileLineOfCars = fopen(carqueueFile, "w");
	if (fileLineOfCars == NULL)
	{
		printf("couldnt open line of cars file!\n");
		fclose(fileStation);
		fclose(filePorts);
		fclose(fileCars);
		return;
	}
	fprintf(fileStation, "ID,StationName,NumOfPorts,CoordX,CoordY\n");
	writeStationFile(stationmanager->root, fileStation);
	fprintf(filePorts, "StationID,PortNumber,PortsType,Status,Year,Month,Day,Hour,Min,CarLicense\n");
	writePortsFile(stationmanager->root, filePorts);
	fprintf(fileCars, "License,PortType,TotalPayed,StationID,PortNumber,InQueue\n");
	writeCarsFile(stationmanager->root, carmanager->root, fileCars);
	fprintf(fileLineOfCars, "License,StationId\n");
	writeLineOfCarsFile(stationmanager->root, fileLineOfCars);

	fclose(fileStation);
	fclose(fileCars);
	fclose(filePorts);
	fclose(fileLineOfCars);
}
/*
 * Saves the entire system state into four text files:
 * - Stations file
 * - Ports file
 * - Cars file
 * - Queue file
 * it opens each file for writing, calls the relevant write function, and handles file errors.
 * it is used to update and save the data  before exiting the program.
 */