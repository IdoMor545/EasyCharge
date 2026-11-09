#ifndef _LOAD_FILES_H_
#define _LOAD_FILES_H_
#include <stdio.h>
#include "Station.h"
#include "CarsTree.h"

void LoadFileStation(const char* filename, BSTStation* StationManager);
void LoadFilePorts(const char* filename, BSTStation* StationManager, BSTCar* CarManager);
void LoadFileBinaryCars(const char* filename, BSTCar* CarManager, BSTStation* StationManager);
void LoadFileQueueCars(const char* filename, BSTStation* StationManager, BSTCar* CarManager);
void loadfile(BSTStation* StationManager, BSTCar* CarManager, char* StationFileName, char* PortFileName, char* CarsFileName, char* QueueCarsFileName);
void writeStationFile(Station* root, FILE* file);
void writePortsFile(Station* root, FILE* file);
void writeCarsFile(Station* stationroot, tCar* carRoot, FILE* file);
void writeLineOfCarsFile(Station* station, FILE* file);
void updateFiles(BSTStation* stationmanager, BSTCar* carmanager, const char* stationFile, const char* portlistFile,
	const char* carstreeFile, const char* carqueueFile);

#endif //!_LOAD_FILES_H_

