#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Define constants
#define MAX_PATIENTS 100
#define MAX_TRIES 3
#define PASSWORD "1234"

// Structure to store patient information
typedef struct {
    char name[50];
    int age;
    char gender[10];
    int id;
} Patient;

// Structure to store reservation slots
typedef struct {
    int id;
    char slot[20];
} Reservation;

// Global variables
Patient patients[MAX_PATIENTS];
Reservation reservations[5];
int patient_count = 0;

// Function declarations
void admin_mode();
void user_mode();
bool verify_password();
void add_patient();
void edit_patient();
void reserve_slot();
void cancel_reservation();
void view_patient_record();
void view_reservations();
int find_patient_index(int id);
int find_reservation_index(int id);

// Main function
int main() {
    int mode;

    // Initialize available slots
    strcpy(reservations[0].slot, "2pm to 2:30pm");
    strcpy(reservations[1].slot, "2:30pm to 3pm");
    strcpy(reservations[2].slot, "3pm to 3:30pm");
    strcpy(reservations[3].slot, "4pm to 4:30pm");
    strcpy(reservations[4].slot, "4:30pm to 5pm");

    while (1) {
        printf("\nWelcome to the Clinic Management System\n");
        printf("Choose mode:\n1. Admin Mode\n2. User Mode\n3. Exit\n");
        scanf("%d", &mode);

        switch (mode) {
            case 1:
                admin_mode();
                break;
            case 2:
                user_mode();
                break;
            case 3:
                printf("Exiting system...\n");
                return 0;
            default:
                printf("Invalid choice! Please select again.\n");
        }
    }

    return 0;
}

// Admin mode functions
void admin_mode() {
    if (!verify_password()) {
        printf("Access denied! Too many incorrect attempts.\n");
        return;
    }

    int choice;
    do {
        printf("\nAdmin Mode:\n");
        printf("1. Add New Patient Record\n");
        printf("2. Edit Patient Record\n");
        printf("3. Reserve a Slot with the Doctor\n");
        printf("4. Cancel Reservation\n");
        printf("5. Logout\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_patient();
                break;
            case 2:
                edit_patient();
                break;
            case 3:
                reserve_slot();
                break;
            case 4:
                cancel_reservation();
                break;
            case 5:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice! Please select again.\n");
        }
    } while (choice != 5);
}

bool verify_password() {
    char input[20];
    int attempts = 0;

    while (attempts < MAX_TRIES) {
        printf("Enter password: ");
        scanf("%s", input);

        if (strcmp(input, PASSWORD) == 0) {
            return true;
        } else {
            printf("Incorrect password!\n");
            attempts++;
        }
    }

    return false;
}

void add_patient() {
    if (patient_count >= MAX_PATIENTS) {
        printf("Patient limit reached! Cannot add more patients.\n");
        return;
    }

    int id;
    printf("Enter Patient ID: ");
    scanf("%d", &id);

    if (find_patient_index(id) != -1) {
        printf("Patient ID already exists! Entry rejected.\n");
        return;
    }

    Patient new_patient;
    new_patient.id = id;
    printf("Enter Patient Name: ");
    scanf("%s", new_patient.name);
    printf("Enter Patient Age: ");
    scanf("%d", &new_patient.age);
    printf("Enter Patient Gender: ");
    scanf("%s", new_patient.gender);

    patients[patient_count++] = new_patient;
    printf("Patient record added successfully.\n");
}

void edit_patient() {
    int id;
    printf("Enter Patient ID: ");
    scanf("%d", &id);

    int index = find_patient_index(id);
    if (index == -1) {
        printf("Incorrect ID! Patient not found.\n");
        return;
    }

    printf("Editing Patient ID %d:\n", id);
    printf("Enter New Patient Name: ");
    scanf("%s", patients[index].name);
    printf("Enter New Patient Age: ");
    scanf("%d", &patients[index].age);
    printf("Enter New Patient Gender: ");
    scanf("%s", patients[index].gender);

    printf("Patient record updated successfully.\n");
}

void reserve_slot() {
    printf("Available Slots:\n");
    for (int i = 0; i < 5; i++) {
        if (reservations[i].id == 0) {
            printf("%d. %s\n", i + 1, reservations[i].slot);
        }
    }

    int id, slot_choice;
    printf("Enter Patient ID: ");
    scanf("%d", &id);

    if (find_patient_index(id) == -1) {
        printf("Incorrect ID! Patient not found.\n");
        return;
    }

    printf("Enter Slot Number: ");
    scanf("%d", &slot_choice);

    if (slot_choice < 1 || slot_choice > 5 || reservations[slot_choice - 1].id != 0) {
        printf("Invalid or already booked slot!\n");
        return;
    }

    reservations[slot_choice - 1].id = id;
    printf("Reservation successful for slot %s.\n", reservations[slot_choice - 1].slot);
}

void cancel_reservation() {
    int id;
    printf("Enter Patient ID: ");
    scanf("%d", &id);

    int res_index = find_reservation_index(id);
    if (res_index == -1) {
        printf("Reservation not found!\n");
        return;
    }

    reservations[res_index].id = 0;
    printf("Reservation cancelled.\n");
}

// User mode functions
void user_mode() {
    int choice;
    do {
        printf("\nUser Mode:\n");
        printf("1. View Patient Record\n");
        printf("2. View Today's Reservations\n");
        printf("3. Exit User Mode\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_patient_record();
                break;
            case 2:
                view_reservations();
                break;
            case 3:
                printf("Exiting User Mode...\n");
                break;
            default:
                printf("Invalid choice! Please select again.\n");
        }
    } while (choice != 3);
}

void view_patient_record() {
    int id;
    printf("Enter Patient ID: ");
    scanf("%d", &id);

    int index = find_patient_index(id);
    if (index == -1) {
        printf("Incorrect ID! Patient not found.\n");
        return;
    }

    printf("Patient ID: %d\n", patients[index].id);
    printf("Name: %s\n", patients[index].name);
    printf("Age: %d\n", patients[index].age);
    printf("Gender: %s\n", patients[index].gender);
}

void view_reservations() {
    printf("Today's Reservations:\n");
    for (int i = 0; i < 5; i++) {
        if (reservations[i].id != 0) {
            printf("%s - Patient ID: %d\n", reservations[i].slot, reservations[i].id);
        }
    }
}

// Utility functions
int find_patient_index(int id) {
    for (int i = 0; i < patient_count; i++) {
        if (patients[i].id == id) {
            return i;
        }
    }
    return -1;
}

int find_reservation_index(int id) {
    for (int i = 0; i < 5; i++) {
        if (reservations[i].id == id) {
            return i;
        }
    }
    return -1;
}
