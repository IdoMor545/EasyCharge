#ifndef _PORT_LIST_DATA_
#define _PORT_LIST_DATA_
typedef struct Station Station;
typedef struct BSTStation BSTStation;
typedef enum { FAST, MID, SLOW } portType;
typedef struct Car Car;
typedef struct qCar qCar;

typedef struct Date
{
	unsigned int Year;
	unsigned int Month;
	unsigned int Day;
	unsigned int Hour;
	unsigned int Min;
}Date;

typedef struct Port
{
	int num;
	portType portType;
	unsigned int status;
	Car* p2car;
	Date tin;
	struct Port* next;
}Port;

typedef struct PortManager
{
	Port* head;
}PortManager;

int assignCar2Port(Car* car, Port* port);
Port* createAndAddPort(Port* PortsList, int PortNum, portType type, unsigned int status, Car* p2car, Date tin);
Port* searchPortByNumber(Port* portsList, int PortNumber);
void findMatchingPortToCar(Car* car, Station* station);
void addNewPort(BSTStation* stationManager);
void releasePorts(BSTStation* stationManager);
Port* removePortByNumber(Port* portsList, int portNum, int* success);
int printOutOfOrderPorts(Port* portsList);
void remOutOrderPort(BSTStation* manager);
void destroyPortList(Port* headPort);
#endif // _PORT_LIST_DATA
