#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "load_files.h"
#include "Menu.h"
int main()
{
	BSTStation StationManager;
	StationManager.root = NULL;
	BSTCar CarManager;
	CarManager.root = NULL;
	loadfile(&StationManager, &CarManager, "Stations.txt", "Ports.txt", "Cars.txt", "LineOfCars.txt");
	ClearScreen();
	mainMenu(&StationManager, &CarManager);
	return 0;
}