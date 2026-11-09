# EasyCharge - Smart EV Charging Station Management System

## Overview
**EasyCharge** is a Command Line Interface (CLI) application developed in C for managing smart Electric Vehicle (EV) charging stations. The system efficiently handles charging ports, vehicle queues, and station data using advanced data structures and dynamic memory management.

## Key Features
* **Interactive CLI Menu:** User-friendly command-line interface for seamless station management.
* **Station Management:** Search, locate nearest, add, or remove stations. Tracks ports and queue statuses.
* **Port Management:** Dynamic allocation, tracking, and management of charging ports (FAST, MID, SLOW). Handles connections and disconnections.
* **Vehicle Queueing System:** Efficient handling of waiting vehicles using FIFO queues.
* **Fast Vehicle Lookup:** Quick search, insertion, and management of cars using Binary Search Trees (BST).
* **Billing System:** Calculates charging time and updates the total amount paid. Displays top customers.
* **Data Persistence:** Persistent state management by loading and saving data from/to local files.
* **Robust Error Handling:** Centralized error checking, validation, and strict memory leak prevention.

## Technical Architecture & Data Structures
This project was designed from first principles, utilizing custom-implemented data structures in C to ensure optimal performance and resource management:
* **Binary Search Tree (`CarsTree.h` / `CarsTree.c`):** Manages the vehicle database for optimized searching, sorting, and removal operations using license plates as keys.
* **Queue (`CarsQueue.h` / `CarsQueue.c`):** A custom FIFO queue that manages the waiting line of vehicles requiring an available charging port.
* **Linked List (`PortList.h` / `PortList.c`):** Dynamically manages the availability and state of charging ports within a station.
* **Binary Search Tree (`Station.h` / `Station.c`):** Manages the entire network of stations allowing fast lookup by ID or name.
* **File I/O (`load_files.h`):** Handles safe parsing, saving, and exporting of the entire system state (stations, ports, cars, and queues).
* **Error Handling (`Errors.h` / `Errors.c`):** Centralized error codes (e.g., `ERROR_STATION_NOT_FOUND`, `ERROR_MEMORY_ALLOCATION_FAILED`) and safe-exit routines.
* **Menu System (`Menu.h` / `Menu.c`):** Modular user interface.
* **Main Entry Point (`main.c`):** Initializes data structures and loads files on startup.

## Getting Started

### Prerequisites
* GCC Compiler (or any standard C compiler)
* Windows (WSL/MinGW), Linux, or macOS environment

**Option 2: Using GCC (Linux / WSL / macOS)**
Open your terminal, navigate to the project directory containing all the files, and run the following command to compile:
```bash
gcc -Wall -Wextra -std=c99 *.c -o EasyCharge
Then, execute the application: ./EasyCharge '''

Author:
Ido Mor
B.Sc. Computer Engineering Student
