#include <stdio.h>
#include <string.h>
// This structure stores details related to available rooms.
struct Room {
    char building[20];
    char roomId[10];
    char acType[10];        // AC or Non-AC
    char boardType[15];     // Smartboard (or) Blackboard
    int seatingCapacity;    // No. of seats : {10, 20, 30, 40, 50, 60, 120, 240}
};
// This structure stores details related to reserved rooms.
struct Timeslot {
    char building[20];
    char roomId[10];
    int startTime;
    int endTime;
};
struct Room rooms[100]; // Global varible to store details of available rooms
struct Timeslot timeslots[500]; // Global variable to store details of reserved rooms.
int roomCount = 0; // Global variable to store no. of available rooms
int timeslotCount = 0; // Global variable to store no. of reserved timeslots.
const char correctPassword[] = "Group10sCEDProject"; // Password for admin related activities
// Function to check password.
int checkPassword() {
    char inputPassword[30];
    printf("Enter password : ");
    scanf("%s", inputPassword);
    if (strcmp(inputPassword, correctPassword) == 0) {
        return 1; // Password is correct
    } else {
        printf("Incorrect password! Cannot execute the command.\n\n");
        return 0; // Password is incorrect
    }
}
// Function to check if correct string has been entered in AC position.
int isValidAcType(char acType[]) {
    return strcmp(acType, "AC") == 0 || strcmp(acType, "Non-AC") == 0;
}
// Function to check if correct string has been entered in Boardtype position.
int isValidBoardType(char boardType[]) {
    return strcmp(boardType, "Smartboard") == 0 || strcmp(boardType, "Blackboard") == 0;
}
// Function to check if valid start & end times have been entered
int isValidTime(int time) {
    return time >= 0 && time < 24;
}
// Function to add a room (this requires password)
void addRoom(char building[], char roomId[], char acType[], char boardType[], int seatingCapacity) {
    if (checkPassword() == 0) {
        return;
    }
    for (int i = 0; i < roomCount; i++) {
        if (strcmp(rooms[i].building, building) == 0 && strcmp(rooms[i].roomId, roomId) == 0) {
            printf("The entered room already exists in the memory.\n\n");
            return;
        }
    }
    // Copy related data into rooms array from local function variable
    strcpy(rooms[roomCount].building, building);
    strcpy(rooms[roomCount].roomId, roomId);
    strcpy(rooms[roomCount].acType, acType);
    strcpy(rooms[roomCount].boardType, boardType);
    rooms[roomCount].seatingCapacity = seatingCapacity;
    roomCount++;
    printf("Room added successfully into the memory.\n\n");
}
// Function used to remove a room (this requires password)
void removeRoom(char building[], char roomId[]) {
    if (checkPassword() == 0) {
        return;
    }
    int found = 0; // Flag used to check if the room is found
    for (int i = 0; i < roomCount; i++) {
        if (strcmp(rooms[i].building, building) == 0 && strcmp(rooms[i].roomId, roomId) == 0) {
            found = 1;
            for (int j = i; j < roomCount - 1; j++) {
                rooms[j] = rooms[j + 1]; // Shift the remaining rooms one index up.
            }
            roomCount--; // Decrement the room count
            printf("Room removed successfully.\n\n");
            break;
        }
    }
    if (found == 0) {
        printf("Room doesn't exist.\n\n");
    }
}
// Function to reserve a room
void reserveRoom(char building[], char roomId[], int startTime, int endTime) {
    // Check if there are rooms added
    if (roomCount == 0) {
        printf("No rooms available for reservation. Please add a room first.\n\n");
        return;
    }
        // Check if entered start time and end time are valid.
    if (!isValidTime(startTime) || !isValidTime(endTime) || startTime >= endTime) {
        printf("Invalid time slots. Write in 24 hr clock system, and also check your entered start time and end time.\n\n");
        return;
    }
    // Check if the given room to reserve exists; only reserve it if exists.
    int roomExists = 0;
    for (int i = 0; i < roomCount; i++) {
        if (strcmp(rooms[i].building, building) == 0 && strcmp(rooms[i].roomId, roomId) == 0) {
            roomExists = 1;
            break;
        }
    }
    if (roomExists == 0) {
        printf("Room does not exist. Please add the room before reserving it.\n\n");
        return;
    }
    // Check if the room is free in the given timeslot.
    for (int i = 0; i < timeslotCount; i++) {
        if (strcmp(timeslots[i].building, building) == 0 && strcmp(timeslots[i].roomId, roomId) == 0 &&
            ((startTime >= timeslots[i].startTime && startTime < timeslots[i].endTime) ||
             (endTime > timeslots[i].startTime && endTime <= timeslots[i].endTime))) {
            printf("There is a time slot conflict. Room has already been reserved.\n\n");
            return;
        }
    }
    // All conditions are fulfilled till now; hence now reserve the room by copying the contents into timeslotCount array.
    strcpy(timeslots[timeslotCount].building, building);
    strcpy(timeslots[timeslotCount].roomId, roomId);
    timeslots[timeslotCount].startTime = startTime;
    timeslots[timeslotCount].endTime = endTime;
    timeslotCount++;
    printf("Room reserved successfully.\n\n");
}
// Function to cancel a reservation, allowing partial cancellations
void cancelRoom(char building[], char roomId[], int startTime, int endTime) {
    // Check if correct time inputs have been given.
    if (!isValidTime(startTime) || !isValidTime(endTime) || startTime >= endTime) {
        printf("Invalid time slots. Times must be antered in 24 hr clock. Also, check your entered start & end time\n\n");
        return;
    }
    // Search for the reservation to be canceled
    int reservationFound = 0;
    for (int i = 0; i < timeslotCount; i++) {
        if (strcmp(timeslots[i].building, building) == 0 && strcmp(timeslots[i].roomId, roomId) == 0) {
            // Case 1: Exact match of time slot for full cancellation
            if (timeslots[i].startTime == startTime && timeslots[i].endTime == endTime) {
                reservationFound = 1;
                // Shift subsequent reservations down an index
                for (int j = i; j < timeslotCount - 1; j++) {
                    timeslots[j] = timeslots[j + 1];
                }
                timeslotCount--;
                printf("Reservation completely cancelled successfully.\n\n");
                break;
            }
            // Case 2: Partial overlap is there at the start
            else if (startTime <= timeslots[i].startTime && endTime < timeslots[i].endTime && endTime > timeslots[i].startTime) {
                reservationFound = 1;
                timeslots[i].startTime = endTime; // Adjust the start time
                printf("Your reservation has been partially cancelled. Adjusted the start time to %d.\n\n", timeslots[i].startTime);
                break;
            }
            // Case 3: Partial overlap is there at the end
            else if (startTime > timeslots[i].startTime && startTime < timeslots[i].endTime && endTime >= timeslots[i].endTime) {
                reservationFound = 1;
                timeslots[i].endTime = startTime; // Adjust the end time
                printf("Your reservation has been partially cancelled. Adjusted the end time to %d.\n\n", timeslots[i].endTime);
                break;
            }
            // Case 4: Full containment of the cancellation within the reserved slot (split into two)
            else if (startTime > timeslots[i].startTime && endTime < timeslots[i].endTime) {
                reservationFound = 1;
                // Create a new timeslot for the remaining period after the cancellation
                for (int j = timeslotCount; j > i; j--) {
                    timeslots[j] = timeslots[j - 1];  // Shift elements to create space for the new slot
                }
                timeslotCount++;
                // Adjust the current reservation to end at the start of the cancellation period
                timeslots[i].endTime = startTime;
                // Create the new timeslot starting after the cancellation period
                strcpy(timeslots[i + 1].building, building);
                strcpy(timeslots[i + 1].roomId, roomId);
                timeslots[i + 1].startTime = endTime;
                timeslots[i + 1].endTime = timeslots[i].endTime;
                printf("Your reservation has been partially cancelled. We've adjusted the existing reservation and created a new one.\n\n");
                break;
            }
        }
    }
    // If no reservation was found
    if (reservationFound == 0) {
        printf("Reservation not found. Check if the correct time and room details have been entered.\n\n");
    }
}
// Function to display all rooms along with their respective classification.
void displayRooms() {
    if (roomCount == 0) {
        printf("No rooms are available.\n\n");
        return;
    }
    printf("Rooms available are :\n");
    for (int i = 0; i < roomCount; i++) {
        printf("BUILDING : %s, ROOM ID : %s, AC/NON-AC : %s, BOARD TYPE : %s, MAX CAPACIY : %d\n",
               rooms[i].building, rooms[i].roomId, rooms[i].acType,
               rooms[i].boardType, rooms[i].seatingCapacity);
    }
    printf("\n");
}
// Function to display all reserved timeslots
void displayTimeSlots() {
    if (timeslotCount == 0) {
        printf("No timeslots have been reserved.\n\n");
        return;
    }
    printf("Reserved timeslots are :\n");
    for (int i = 0; i < timeslotCount; i++) {
        printf("BUILDING : %s, ROOM ID : %s, START TIME : %d, END TIME : %d\n",
               timeslots[i].building, timeslots[i].roomId,
               timeslots[i].startTime, timeslots[i].endTime);
    }
    printf("\n");
}
// Function used to enter input.
void input() {
    char command[50], building[20], roomId[10], acType[10], boardType[15];
    int startTime, endTime, seatingCapacity;
    while (1) {
        printf("ENTER YOUR COMMAND : ");
        scanf("%s", command);
        if (strcmp(command, "add") == 0) { // Accessing addRoom function
            scanf("%s %s %s %s %d", building, roomId, acType, boardType, &seatingCapacity);
            if (isValidAcType(acType) && isValidBoardType(boardType)) {
                addRoom(building, roomId, acType, boardType, seatingCapacity);
            } else {
                printf("Invalid command. Room cannot be added.\n\n");
            }
        } else if (strcmp(command, "remove") == 0) { // Accessing removeRoom function
            scanf("%s %s", building, roomId);
            removeRoom(building, roomId);
        } else if (strcmp(command, "reserve") == 0) { // Accessing reserveRoom function
            scanf("%s %s %d %d", building, roomId, &startTime, &endTime);
            reserveRoom(building, roomId, startTime, endTime);
        } else if (strcmp(command, "cancel") == 0) { // Accessing cancelRoom function
            scanf("%s %s %d %d", building, roomId, &startTime, &endTime);
            cancelRoom(building, roomId, startTime, endTime);
        } else if (strcmp(command, "rooms") == 0) { // Using displayRoooms function
            displayRooms();
        } else if (strcmp(command, "timeslots") == 0) { // Using displayTimeSlots function.
            displayTimeSlots();
        } else if (strcmp(command, "exit") == 0) { // Exiting the conference booking system.
            printf("THANK YOU! PLEASE VISIT AGAIN.\n\n");
            break;
        } else {
            printf("Invalid command. Please try again.\n\n"); // If some wrong input is given
        }
    }
}
int main() {
    // Printing few user friendly guidelines.
    printf("WELCOME TO OUR CONFERENCE ROOM BOOKING SYTEM !\n");
    printf("Available commands & their respective features have been displayed below for your convenience.\n\n");
    printf("1st command :- add *building's Name* *roomId* *AC/Non-AC* *Smartboard/Blackboard* *No. of seats*\n");
    printf("2nd command :- remove *building's Name* *roomId* *AC/Non-AC* *Smartboard/Blackboard*\n");
    printf("3rd command :- reserve *building's Name* *roomId* *start time* *end time*\n");
    printf("4th command :- cancel *building's Name* *roomId* *start time* *end time*\n");
    printf("5th command :- rooms : To display all available rooms.\n");
    printf("6th command :- timeslots : To display all reserved timeslots.\n");
    printf("7th command :- exit : Exit the application.\n\n");
    input();
    return 0;
}
