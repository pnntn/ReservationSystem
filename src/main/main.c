#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define DIM 20
#define MAX_RESERVATIONS 100

// Definition of the structure for reservations
struct reservation {
    char name[DIM];
    char surname[DIM];
    int code;
    struct reservation *next;
};

typedef struct reservation Reservation;

// Function declarations
int authenticate();
void insert_reservation(Reservation **head);
void display_reservations(Reservation *head);
void delete_reservation(Reservation **head);
void save_reservations(Reservation *head);
bool is_code_unique(Reservation *head, int code); // Check uniqueness of reservation codes

int main() {
    Reservation *head = NULL;
    int authenticated = authenticate();

    if (!authenticated) {
        printf("Authentication failed. Exiting program.\n");
        return 1;
    }

    printf("\nHello! This program simulates an airline reservation system with 100 available seats.\n");

    while (true) {
        printf("-----------------------------------\n");
        printf("Choose one of the following options:\n");
        printf("[1] Insert new reservation\n");
        printf("[2] Delete reservation\n");
        printf("[3] Display reservations\n");
        printf("[4] Save and overwrite reservations to file\n");
        printf("[0] Exit\n");
        printf("-----------------------------------\n");

        printf("Enter your choice: ");
        int choice;
        // Check if input is an integer
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a valid option.\n");
            // Clear input buffer
            while (getchar() != '\n');
            continue; // Restart the loop
        }

        switch (choice) {
            case 1:
                insert_reservation(&head);
                break;
            case 2:
                delete_reservation(&head);
                break;
            case 3:
                display_reservations(head);
                break;
            case 4:
                save_reservations(head);
                break;
            case 0:
                printf("\nGoodbye!\n");
                // Free memory before exiting
                while (head != NULL) {
                    Reservation *temp = head;
                    head = head->next;
                    free(temp);
                }
                return 0;
            default:
                printf("\nInvalid choice! Please enter a valid option.\n");
                break;
        }
    }

    return 0;
}

// Function for user authentication
int authenticate() {
    int password;
    printf("Enter password to start the program: ");
    scanf("%d", &password);
    // Clear input buffer
    while (getchar() != '\n');

    if (password == 1234567890) {
        return 1; // Authentication successful
    } else {
        return 0; // Authentication failed
    }
}

// Function for inserting a new reservation
void insert_reservation(Reservation **head) {
    // Allocate memory for the new reservation
    Reservation *new_reservation = (Reservation*) malloc(sizeof(Reservation));
    if (new_reservation == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new_reservation->next = NULL; // Initialize next pointer to NULL

    // Input passenger details
    printf("\nEnter passenger details\n");
    printf("Name: ");
    scanf("%19s", new_reservation->name); // Limited input to avoid buffer overflow
    printf("Surname: ");
    scanf("%19s", new_reservation->surname); // Limited input to avoid buffer overflow

    // Generate a unique random code for the reservation
    do {
        new_reservation->code = rand() % 1000 + 1;
    } while (!is_code_unique(*head, new_reservation->code));

    printf("Reservation code assigned: %d\n", new_reservation->code);
    printf("Data registered successfully!\n");

    // Insert the new reservation into the linked list
    if (*head == NULL) {
        *head = new_reservation;
    } else {
        Reservation *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_reservation;
    }
}

// Function to check the uniqueness of reservation codes
bool is_code_unique(Reservation *head, int code) {
    Reservation *current = head;
    while (current != NULL) {
        if (current->code == code) {
            return false; // Code is not unique
        }
        current = current->next;
    }
    return true; // Code is unique
}

// Function to display reservations
void display_reservations(Reservation *head) {
    if (head == NULL) {
        printf("\nNo reservations found!\n");
    } else {
        Reservation *current = head;
        while (current != NULL) {
            printf("\nName: %s\n", current->name);
            printf("Surname: %s\n", current->surname);
            printf("Reservation code: %d\n", current->code);
            current = current->next;
        }
    }
}

// Function to delete a reservation
void delete_reservation(Reservation **head) {
    if (*head == NULL) {
        printf("\nNo reservations found!\n");
        return;
    }

    int code;
    printf("\nEnter reservation code to delete: ");
    scanf("%d", &code);
    // Clear input buffer
    while (getchar() != '\n');

    Reservation *current = *head;
    Reservation *prev = NULL;

    while (current != NULL && current->code != code) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("\nReservation code not found!\n");
        return;
    }

    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    printf("Reservation deleted successfully!\n");
}

// Function to save reservations to file
void save_reservations(Reservation *head) {
    if (head == NULL) {
        printf("\nNo reservations found!\n");
        return;
    }

    FILE *file = fopen("reservations.txt", "w"); // Used "w" to overwrite existing file
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    Reservation *current = head;
    while (current != NULL) {
        fprintf(file, "Name: %s\nSurname: %s\nReservation code: %d\n\n", current->name, current->surname, current->code);
        current = current->next;
    }

    fclose(file);
    printf("\nFile saved successfully!\n");
}
