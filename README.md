# Conference Room Booking System

![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

A lightweight, command-line interface (CLI) application written in C for managing and reserving conference rooms. It features admin-authenticated room management, robust time-conflict detection, and an advanced cancellation system that supports dynamic partial cancellations.

## 📌 Features

* **Admin Authentication:** Password-protected commands for adding and removing rooms to prevent unauthorized modifications.
* **Room Management:** Store and query room specifications, including building name, room ID, AC availability, board type, and seating capacity.
* **Conflict Detection:** Prevents double-booking by checking start and end times (24-hour format) against existing reservations.
* **Smart Cancellations:** Supports both full and partial reservation cancellations. The system dynamically adjusts existing timeslots or splits them into new slots depending on the overlap.
* **In-Memory Storage:** Utilizes C structs and global arrays for fast, stateless execution during runtime.

## 🚀 Compilation & Execution

Ensure you have a C compiler (like GCC) installed on your system.

**1. Compile the code:**
```bash
gcc main.c -o booking_system
```

**2. Run the executable:**
```bash
./booking_system
```

## 💻 Usage & Commands
Once the program is running, you interact with it using specific terminal commands.

⚠️ **Admin Password:** Actions that modify the room database (`add`, `remove`) require the admin password.

- **Password:** `Group10sCEDProject`

### Available Commands
---------------------------------------------------
| Command	| Syntax	| Description |
| Add Room	| `add <building> <roomId> <AC/Non-AC> <Smartboard/Blackboard> <seats>`	| Registers a new room. Requires admin password.|	
| Remove Room	| `remove <building> <roomId>`	| Deletes a room from the system. Requires admin password. |
| Reserve Room	| `reserve <building> <roomId> <start_time> <end_time>`	| Books a room. Times must be integers in 24-hour format (e.g., 14 16).	|
| Cancel	| `cancel <building> <roomId> <start_time> <end_time>`	| Cancels a booking. Supports partial time overlaps.			 |
| View Rooms |	`rooms` |	Displays a list of all currently available rooms and their specs.			 |
| View Slots |	`timeslots` |	Displays all active reservations across all rooms.		 |
| Exit	| `exit` |	Terminates the application.	 |

### Example Workflows

```
ENTER YOUR COMMAND : add AcademicBlock A101 AC Smartboard 60
Enter password : Group10sCEDProject
Room added successfully into the memory.

ENTER YOUR COMMAND : reserve AcademicBlock A101 10 14
Room reserved successfully.

ENTER YOUR COMMAND : timeslots
Reserved timeslots are :
BUILDING : AcademicBlock, ROOM ID : A101, START TIME : 10, END TIME : 14

ENTER YOUR COMMAND : cancel AcademicBlock A101 11 12
Your reservation has been partially cancelled. We've adjusted the existing reservation and created a new one.
```

## 🏗️ Data Structures

The system relies on two primary structs mapped to global arrays (capped at 100 rooms and 500 timeslots for memory safety):

- `Room`: Stores metadata (`building`, `roomId`, `acType`, `boardType`, `seatingCapacity`).

- `Timeslot`: Stores active reservations (`building`, `roomId`, `startTime`, `endTime`).

## 📄 License
This project is licensed under the terms of the LICENSE file included in this repository.
