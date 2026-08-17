#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"
#include "md5.h"
#include "utils.h"
#include "admin.h"
#include "doctor.h"
#include "patient.h"

/* This is where the shared global variables are ACTUALLY created.
   models.h only *declares* them (extern) so every .c file can see
   them; here in main.c they're *defined* (memory is really set aside). */
Patient* patients = NULL;
Doctor* doctors = NULL;
int patient_count = 0;
int doctor_count = 0;

/* ==================== MAIN ==================== */
int main() {
    int choice;
    char username[100], password[100], hashed_input[33];
    char admin_hash[] = "0192023a7bbd73250516f069df18b500"; // admin123

    while (1) {
        clearScreen();
        printf("\n===================================\n");
        printf("     WELCOME TO HEALWISH SYSTEM     \n");
        printf("===================================\n");
        printf("1. Admin Login\n");
        printf("2. Doctor Login\n");
        printf("3. Patient Login\n");
        printf("4. Exit System\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        getchar();

        if (choice == 4) {
            clearScreen();
            printf("\nExiting system. Freeing memory...\n");
            free(patients);
            free(doctors);
            break;
        }

        clearScreen();
        printf("Enter Username / Access ID: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        printf("Enter Password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        generateMD5(password, hashed_input);

        if (choice == 1) {
            if (strcmp(username, "admin") == 0 && strcmp(hashed_input, admin_hash) == 0) {
                adminMenu();
            } else {
                printf("\nInvalid Admin Credentials! Press Enter to retry.");
                getchar();
            }
        } else if (choice == 2) {
            int found = -1;
            for (int i = 0; i < doctor_count; i++) {
                if (strcmp(doctors[i].id, username) == 0 && strcmp(doctors[i].password_hash, hashed_input) == 0) {
                    found = i;
                    break;
                }
            }
            if (found != -1) {
                doctorMenu(doctors[found].id);
            } else {
                printf("\nInvalid Doctor ID or Password! Press Enter to retry.");
                getchar();
            }
        } else if (choice == 3) {
            int found = -1;
            for (int i = 0; i < patient_count; i++) {
                if (strcmp(patients[i].id, username) == 0 && strcmp(patients[i].password_hash, hashed_input) == 0) {
                    found = i;
                    break;
                }
            }
            if (found != -1) {
                patientMenu(patients[found].id);
            } else {
                printf("\nInvalid Patient ID or Password! Press Enter to retry.");
                getchar();
            }
        }
    }
    return 0;
}
