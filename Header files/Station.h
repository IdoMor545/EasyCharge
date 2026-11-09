#ifndef _STATION_PROGRAM_
#define _STATION_PROGRAM_
#include "PortList.h"
#include "CarsQueue.h"
#define TRUE 1
#define FALSE 0
typedef struct Coord
{
	double x;
	double y;
}Coord;

typedef struct Station
{
	int id;
	char* name;
	int nPorts;
	Coord Coord;
	Port* portsList;
	int nCars;
	qCar* carQueue;
	Station* right;
	Station* left;
}Station;
typedef struct BSTStation
{
	Station* root;
}BSTStation;


void CheckingStation(Station* station);

Station* CreateStation(int id, const char* name, Coord coord, int nPorts, int nCars);
Station* insertBinaryStationTree(Station* rootManager, Station* newStation);
Station* SearchStationById(Station* Managerroot, int id);
Station* SearchStationByName(Station* rootManager, const char* name);
void locNearSt(BSTStation* Manager);
Station* findNearestStation(Station* root, Coord target, Station** ClosetStation, double* MinimumDist);

typedef Station* (*SearchFunc)(Station* root, void* target);
Station* searchStation(Station* root, void* target, int searchType);
Station* ById(Station* root, void* target);
Station* ByName(Station* root, void* target);
Station* ByCoord(Station* root, void* target);

void printStation(Station* station);
void printStationTree(Station* root);
void printAllStations(BSTStation* manager);
void StationDetails(Station* rootManager);
void printStations(Station* rootManager);
void dispAllSt(BSTStation* Manager);
void reportStStat(BSTStation* stationManager);
Station* findMinStationNode(Station* node);
Station* deleteStationNode(Station* root, int stationId);
void closeSt(BSTStation* manager);
#endif // !_STATION_PROGRAM_
