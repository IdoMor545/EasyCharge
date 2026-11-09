#ifndef _My_menu
#define _My_menu
#include "CarsTree.h"
#include "Station.h"
#include "PortList.h"
#include "CarsQueue.h"
void ClearScreen();
void printMenu();
void destroyCarTree(tCar* carRoot);
void destroyStationTree(Station* stationRoot);
void exitSys(BSTStation* stationManager, BSTCar* carManager);
void pauseAfterAction();
void mainMenu(BSTStation* stationManager, BSTCar* carManager);
#endif // !\My_menu
