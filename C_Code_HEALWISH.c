#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structures
typedef struct {
    char id[50];
    char name[100];
    char degree[100];          // Added Field
    char medical_college[150];  // Added Field
    char specialty[100];
} Doctor;

typedef struct {
    char id[50];
    char name[100];
    int age;
    char symptom[200];
    char assigned_doctor[100];
    char appointment_datetime[100];
    char diagnosis[200];
    char treatment_info[200];
    char prescription[300];
} Patient;

// Dynamic Pointer Arrays (No Limits)
Patient* patients = NULL;
Doctor* doctors = NULL;
int patient_count = 0;
int doctor_count = 0;

// Function Prototypes
void adminMenu();
void doctorMenu(char* doc_id);
void patientMenu(char* pat_id);
void updatePatientRecord();
void deletePatientRecord();
void searchPatientRecord();
void addDoctorInformation();
void generateReport();

int main() {
    int choice;
    char username[100], password[100];

    while (1) {
        printf("\n===================================\n");
        printf("     WELCOME TO HEALWISH SYSTEM     \n");
        printf("===================================\n");
        printf("1. Admin Login\n");
        printf("2. Doctor Login\n");
        printf("3. Patient Login\n");
        printf("4. Exit System\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid Input!\n");
            exit(1);
        }
        getchar(); // Clear buffer

        if (choice == 4) {
            printf("\nExiting system. Wishing you a better life! Freeing memory...\n");
            free(patients);
            free(doctors);
            break;
        }

        printf("Enter Username / Access ID: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0; 

        printf("Enter Password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        switch (choice) {
            case 1: // USE CASE: LOGIN (ADMIN)
                if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
                    printf("\nLogin Successful as Admin! Wishing you a better life.\n");
                    adminMenu();
                } else {
                    printf("\nInvalid Admin Credentials!\n");
                }
                break;

            case 2: // USE CASE: LOGIN (DOCTOR)
                if (strcmp(password, "doctor123") == 0) {
                    int found = -1;
                    for (int i = 0; i < doctor_count; i++) {
                        if (strcmp(doctors[i].id, username) == 0) {
                            found = i;
                            break;
                        }
                    }
                    if (found != -1) {
                        printf("\nLogin Successful! Welcome, %s. Wishing you a better life.\n", doctors[found].name);
                        doctorMenu(doctors[found].id);
                    } else {
                        printf("\nDoctor ID not found! Create via Admin Panel first.\n");
                    }
                } else {
                    printf("\nIncorrect Password!\n");
                }
                break;

            case 3: // USE CASE: LOGIN (PATIENT)
                if (strcmp(password, "patient123") == 0) {
                    int found = -1;
                    for (int i = 0; i < patient_count; i++) {
                        if (strcmp(patients[i].id, username) == 0) {
                            found = i;
                            break;
                        }
                    }
                    if (found != -1) {
                        printf("\nLogin Successful! Welcome, %s. Wishing you a better life.\n", patients[found].name);
                        patientMenu(patients[found].id);
                    } else {
                        printf("\nPatient ID not found! Admit via Admin Panel first.\n");
                    }
                } else {
                    printf("\nIncorrect Password!\n");
                }
                break;

            default:
                printf("Invalid selection!\n");
        }
    }
    return 0;
}

/* ==================== ADMIN DASHBOARD ==================== */
void adminMenu() {
    int choice;
    while (1) {
        printf("\n--- ADMIN PANEL ---\n");
        printf("1. Add / Update Patient Record\n");
        printf("2. Delete Patient Record\n");
        printf("3. Search Patient Record\n");
        printf("4. Add Doctor Information\n");
        printf("5. Generate System Report\n");
        printf("6. Logout\n"); 
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid Option.\n");
            getchar();
            continue;
        }
        getchar();

        if (choice == 6) {
            printf("\nLogging out of Admin Panel... Wishing you a better life.\n");
            break;
        }

        switch (choice) {
            case 1: updatePatientRecord(); break;
            case 2: deletePatientRecord(); break;
            case 3: searchPatientRecord(); break;
            case 4: addDoctorInformation(); break;
            case 5: generateReport(); break;
            default: printf("Invalid option.\n");
        }
    }
}

void updatePatientRecord() {
    char id[50];
    printf("Enter Patient ID to Add/Update: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int index = -1;
    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].id, id) == 0) {
            index = i;
            break;
        }
    }

    if (index > -1) {
        printf("Updating Existing Patient Record...\n");
    } else {
        patient_count++;
        patients = (Patient*)realloc(patients, patient_count * sizeof(Patient));
        index = patient_count - 1;
        strcpy(patients[index].id, id);
        strcpy(patients[index].diagnosis, "Pending");
        strcpy(patients[index].treatment_info, "Pending");
        strcpy(patients[index].prescription, "None");
        printf("Creating New Patient Record...\n");
    }

    printf("Enter Full Name: ");
    fgets(patients[index].name, sizeof(patients[index].name), stdin);
    patients[index].name[strcspn(patients[index].name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &patients[index].age);
    getchar();

    printf("Enter Symptoms / Problems: ");
    fgets(patients[index].symptom, sizeof(patients[index].symptom), stdin);
    patients[index].symptom[strcspn(patients[index].symptom, "\n")] = 0;

    printf("Enter Assigned Doctor Name: ");
    fgets(patients[index].assigned_doctor, sizeof(patients[index].assigned_doctor), stdin);
    patients[index].assigned_doctor[strcspn(patients[index].assigned_doctor, "\n")] = 0;

    printf("Enter Appointment Date & Time: ");
    fgets(patients[index].appointment_datetime, sizeof(patients[index].appointment_datetime), stdin);
    patients[index].appointment_datetime[strcspn(patients[index].appointment_datetime, "\n")] = 0;

    printf("Patient Record Saved Successfully!\n");
}

void deletePatientRecord() {
    char id[50];
    printf("Enter Patient ID to delete: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int found = -1;
    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].id, id) == 0) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        for (int i = found; i < patient_count - 1; i++) {
            patients[i] = patients[i + 1];
        }
        patient_count--;
        if (patient_count > 0) {
            patients = (Patient*)realloc(patients, patient_count * sizeof(Patient));
        } else {
            free(patients);
            patients = NULL;
        }
        printf("Patient record deleted successfully.\n");
    } else {
        printf("Record not found.\n");
    }
}

void searchPatientRecord() {
    char query[100];
    printf("Enter Patient ID or Name to search: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = 0;

    int found = 0;
    
    // Print Table Header
    printf("\n+--------------------+----------------------+-----+----------------------+\n");
    printf("|%-20s|%-22s|%-5s|%-22s|\n", " Patient ID", " Name", " Age", " Assigned Doctor");
    printf("+--------------------+----------------------+-----+----------------------+\n");

    for (int i = 0; i < patient_count; i++) {
        if (strstr(patients[i].id, query) || strstr(patients[i].name, query)) {
            printf("| %-19s| %-21s| %-4d| %-21s|\n", 
                   patients[i].id, patients[i].name, patients[i].age, patients[i].assigned_doctor);
            found = 1;
        }
    }
    printf("+--------------------+----------------------+-----+----------------------+\n");
    
    if (!found) printf("No records matched.\n");
}

void addDoctorInformation() {
    doctor_count++;
    doctors = (Doctor*)realloc(doctors, doctor_count * sizeof(Doctor));
    int index = doctor_count - 1;

    printf("Enter Doctor ID: ");
    fgets(doctors[index].id, sizeof(doctors[index].id), stdin);
    doctors[index].id[strcspn(doctors[index].id, "\n")] = 0;

    printf("Enter Doctor Name: ");
    fgets(doctors[index].name, sizeof(doctors[index].name), stdin);
    doctors[index].name[strcspn(doctors[index].name, "\n")] = 0;

    printf("Enter Degrees (e.g., MBBS, FCPS): ");
    fgets(doctors[index].degree, sizeof(doctors[index].degree), stdin);
    doctors[index].degree[strcspn(doctors[index].degree, "\n")] = 0;

    printf("Enter Medical College Name: ");
    fgets(doctors[index].medical_college, sizeof(doctors[index].medical_college), stdin);
    doctors[index].medical_college[strcspn(doctors[index].medical_college, "\n")] = 0;

    printf("Enter Specialty: ");
    fgets(doctors[index].specialty, sizeof(doctors[index].specialty), stdin);
    doctors[index].specialty[strcspn(doctors[index].specialty, "\n")] = 0;

    printf("Doctor Profile Saved!\n");
}

void generateReport() {
    printf("\n+=============================================+\n");
    printf("|         HEALWISH SYSTEM SYSTEM REPORT       |\n");
    printf("+=============================================+\n");
    printf("| Total Active Patients in Database:  %-7d |\n", patient_count);
    printf("| Total Registered Doctors in Database: %-6d |\n", doctor_count);
    printf("+---------------------------------------------+\n");
}

/* ==================== DOCTOR DASHBOARD ==================== */
void doctorMenu(char* doc_id) {
    int choice;
    char target_pid[50];
    char doc_name[100] = "";

    for(int i = 0; i < doctor_count; i++) {
        if(strcmp(doctors[i].id, doc_id) == 0) {
            strcpy(doc_name, doctors[i].name);
        }
    }

    while (1) {
        printf("\n--- DOCTOR PORTAL ---\n");
        printf("1. View Patient Queue & History\n");
        printf("2. Conduct Examination (Diagnosis/Treatment/Prescription)\n");
        printf("3. Logout\n"); 
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 3) {
            printf("\nLogging out of Doctor Portal... Wishing you a better life.\n");
            break;
        }

        if (choice == 1) { 
            int matches = 0;
            printf("\n+--------------------+----------------------+----------------------+----------------------+\n");
            printf("|%-20s|%-22s|%-22s|%-22s|\n", " Patient ID", " Name", " Symptom", " Diagnosis Status");
            printf("+--------------------+----------------------+----------------------+----------------------+\n");

            for (int i = 0; i < patient_count; i++) {
                if (strcmp(patients[i].assigned_doctor, doc_name) == 0) {
                    printf("| %-19s| %-21s| %-21s| %-21s|\n", 
                           patients[i].id, patients[i].name, patients[i].symptom, patients[i].diagnosis);
                    matches++;
                }
            }
            printf("+--------------------+----------------------+----------------------+----------------------+\n");
            if(matches == 0) printf("No patients assigned to your name yet.\n");
        } 
        else if (choice == 2) {
            printf("Enter Patient ID to examine: ");
            fgets(target_pid, sizeof(target_pid), stdin);
            target_pid[strcspn(target_pid, "\n")] = 0;

            int idx = -1;
            for (int i = 0; i < patient_count; i++) {
                if (strcmp(patients[i].id, target_pid) == 0 && strcmp(patients[i].assigned_doctor, doc_name) == 0) {
                    idx = i;
                    break;
                }
            }

            if (idx != -1) {
                printf("Record Diagnosis: ");
                fgets(patients[idx].diagnosis, sizeof(patients[idx].diagnosis), stdin);
                patients[idx].diagnosis[strcspn(patients[idx].diagnosis, "\n")] = 0;

                printf("Record Treatment Info: ");
                fgets(patients[idx].treatment_info, sizeof(patients[idx].treatment_info), stdin);
                patients[idx].treatment_info[strcspn(patients[idx].treatment_info, "\n")] = 0;

                printf("Add Prescription: ");
                fgets(patients[idx].prescription, sizeof(patients[idx].prescription), stdin);
                patients[idx].prescription[strcspn(patients[idx].prescription, "\n")] = 0;

                printf("Clinical updates saved successfully.\n");
            } else {
                printf("Access Denied or Patient ID not found in your assigned queue.\n");
            }
        }
    }
}

/* ==================== PATIENT DASHBOARD ==================== */
void patientMenu(char* pat_id) {
    int choice;
    int idx = -1;
    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].id, pat_id) == 0) {
            idx = i;
            break;
        }
    }

    while (1) {
        printf("\n--- PATIENT PORTAL ---\n");
        printf("1. View Own Information\n");
        printf("2. View Assigned Doctor Details\n");
        printf("3. View Appointment Information\n");
        printf("4. View Treatment History & Prescription\n");
        printf("5. Logout\n"); 
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 5) {
            printf("\nLogging out of Patient Portal... Wishing you a better life.\n");
            break;
        }

        switch (choice) {
            case 1: 
                printf("\n+----------------------------------------------------+\n");
                printf("|                  MY PROFILE DETAILS                |\n");
                printf("+----------------------------------------------------+\n");
                printf("| Patient ID : %-37s |\n", patients[idx].id);
                printf("| Full Name  : %-37s |\n", patients[idx].name);
                printf("| Age        : %-37d |\n", patients[idx].age);
                printf("+----------------------------------------------------+\n");
                break;
            case 2: {
                int doc_idx = -1;
                // Try searching for the doctor object based on the patient's record value
                for(int i = 0; i < doctor_count; i++) {
                    if(strcmp(doctors[i].name, patients[idx].assigned_doctor) == 0) {
                        doc_idx = i;
                        break;
                    }
                }

                printf("\n+----------------------------------------------------+\n");
                printf("|              ASSIGNED DOCTOR DETAILS               |\n");
                printf("+----------------------------------------------------+\n");
                if (doc_idx != -1) {
                    printf("| Doctor Name    : %-33s |\n", doctors[doc_idx].name);
                    printf("| Degree(s)      : %-33s |\n", doctors[doc_idx].degree);
                    printf("| Medical College: %-33s |\n", doctors[doc_idx].medical_college);
                    printf("| Specialty      : %-33s |\n", doctors[doc_idx].specialty);
                } else {
                    printf("| Doctor Name    : %-33s |\n", patients[idx].assigned_doctor);
                    printf("| Detailed info not added by admin yet.              |\n");
                }
                printf("+----------------------------------------------------+\n");
                break;
            }
            case 3: 
                printf("\n+----------------------------------------------------+\n");
                printf("|              APPOINTMENT INFORMATION               |\n");
                printf("+----------------------------------------------------+\n");
                printf("| Schedule: %-40s |\n", patients[idx].appointment_datetime);
                printf("+----------------------------------------------------+\n");
                break;
            case 4: 
                printf("\n+----------------------------------------------------+\n");
                printf("|             HEALTH SUMMARY & TREATMENT             |\n");
                printf("+----------------------------------------------------+\n");
                printf("| Reported Symptoms: %-31s |\n", patients[idx].symptom);
                printf("| Diagnosis Result : %-31s |\n", patients[idx].diagnosis);
                printf("| Treatment Info   : %-31s |\n", patients[idx].treatment_info);
                printf("| Rx / Prescription: %-31s |\n", patients[idx].prescription);
                printf("+----------------------------------------------------+\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    }
}