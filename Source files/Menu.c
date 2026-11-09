#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Station.h"
#include "load_files.h"
#include "menu.h"
#include "Errors.h"

#define UPDATE_ALL_FILES updateFiles(stationManager, carManager, "Stations.txt", "Ports.txt", "cars.txt", "LineOfCars.txt")
#define PAUSE pauseAfterAction()
void ClearScreen()
{
#if defined (_WIN32)
	system("cls");

#elif  defined (__linux__) || defined (__unix__)  || defined (__APPLE__)
	system("clear");

#else

#endif 

}


void printMenu()
{
	printf("\n1. locate nearest station by distance\n");
	printf("2. car charging\n");
	printf("3. check car status\n");
	printf("4. stop charging\n");
	printf("5. Display all stations\n");
	printf("6. Display cars at station\n");
	printf("7. Report of stations statistics\n");
	printf("8. Display top customers\n");
	printf("9. add new port\n");
	printf("10. release charging ports\n");
	printf("11. Remove out of order ports\n");
	printf("12. remove customer\n");
	printf("13. close station\n");
	printf("0. exit\n ");
}



void exitSys(BSTStation* stationManager, BSTCar* carManager)
{
	if (stationManager == NULL || carManager == NULL)
	{
		printf("station Manager or car Manager doesnt exists!\n");
		return;
	}
	destroyCarTree(carManager->root);
	destroyStationTree(stationManager->root);
}
/*
  this function performs a clean shutdown of the entire system. It checks if the
  station and car managers are valid, then calls the tree destruction functions
  to recursively free all dynamically allocated memory for stations and cars.
  This is the final operation before the program exits to prevent memory leaks.
 */


void destroyCarTree(tCar* carRoot)
{
	if (carRoot == NULL) return;

	destroyCarTree(carRoot->left);
	destroyCarTree(carRoot->right);

	carRoot->p2car->inqueue = 0;
	strcpy(carRoot->p2car->nLicense,"\0");
	carRoot->p2car->pPort = NULL;
	carRoot->p2car->totalPayed = 0.0;
	free(carRoot->p2car);
	free(carRoot);
}
/*
  Frees up all the memory used by the car tree.
  This function walks through the entire car tree and frees all the nodes,
  including the car struct each node points to. This is used to
  avoid memory leaks when we're done with the program.
 */


void destroyStationTree(Station* stationRoot)
{
	if (stationRoot == NULL) return;

	destroyStationTree(stationRoot->left);
	destroyStationTree(stationRoot->right);

	if (stationRoot->portsList != NULL) destroyPortList(stationRoot->portsList);
	if (stationRoot->carQueue != NULL) destroyCarQueue(stationRoot->carQueue);
	stationRoot->Coord.x = 0;
	stationRoot->Coord.y = 0;
	stationRoot->id = 0;
	stationRoot->nCars = 0;
	free(stationRoot->name);
	free(stationRoot);
}
/*
  Cleans up and frees all memory from the station tree.
  This function goes through the station tree and deletes every station node.
  It also takes care of station's name string,
  the list of charging ports, and the car queue, making sure everything is
  properly freed before the program closes.
 */


void pauseAfterAction()
{
	printf("\nPress ENTER once or twice to return to the menu...");
	fflush(stdout);
	getchar();
	getchar();
}
/*
* This is func to pause after a action in the menu 
* to prevent from the menu to appear right after the action
*/


void mainMenu(BSTStation* stationManager, BSTCar* carManager)
{
	printf("\nWelcome to EasyCharge!\n\n");
	printf("this is our menu pick whatever you like:\n");
	int choice = -1;
	do
	{
		printMenu();
		printf("Enter your choice: ");
		if (scanf("%d", &choice) != 1)
		{
			ClearScreen();
			printf("Invalid input! Please enter a number between 0 and 13\n\n");
			while (getchar() != '\n'); 
			continue;
		}

		if (choice < 0 || choice > 13)
		{
			ClearScreen();
			printf("Invalid choice! Please select a number from the menu\n\n");
			continue;
		}
			ClearScreen();
			switch (choice)
			{
			case 1:
				locNearSt(stationManager);
				PAUSE;
				break;
			case 2:
				chargeCar(stationManager, carManager);
				UPDATE_ALL_FILES;
				PAUSE;
				break;
			case 3:
				checkCarStatus(stationManager, carManager);
				PAUSE;
				break;
			case 4:
				stopCharge(stationManager, carManager);
				UPDATE_ALL_FILES;
				PAUSE;
				break;
			case 5:
				dispAllSt(stationManager);
				PAUSE;
				break;
			case 6:
				dispCarsAtSt(stationManager);
				PAUSE;
				break;
			case 7:
				reportStStat(stationManager);
				PAUSE;
				break;
			case 8:
				dispTopCustomers(carManager);
				PAUSE;
				break;
			case 9:
				addNewPort(stationManager);
				UPDATE_ALL_FILES;
				PAUSE;
				break;
			case 10:
				releasePorts(stationManager);
				UPDATE_ALL_FILES;
				PAUSE;
				break;
			case 11:
				remOutOrderPort(stationManager);
				UPDATE_ALL_FILES;
				PAUSE;
				break;
			case 12:
				remCustomer(carManager);
				UPDATE_ALL_FILES;
				PAUSE;
				break;
			case 13:
				closeSt(stationManager);
				UPDATE_ALL_FILES;
				PAUSE;
				break;

			case 0:
				UPDATE_ALL_FILES;
				exitSys(stationManager, carManager);
				stationManager = NULL;
				carManager = NULL;
				break;
			default:
				displayError(ERROR_INVALID_INPUT);
				break;
			}
			ClearScreen();
	} while (choice != 0);
	printf("Thank you for choosing Easy Charge\nHope to see you soon!\n\n");
}

