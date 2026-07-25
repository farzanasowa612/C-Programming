#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structures
typedef struct {
    char id[50];
    char name[100];
    char degree[100];          
    char medical_college[150];  
    char specialty[100];
    double earnings;            // Accumulated total payment received
} Doctor;

typedef struct {
    char id[50];
    char name[100];
    int age;
    char symptom[200];
    char assigned_doctor[100];
    char appointment_datetime[100];
    char ot_schedule[100];            // OT Details assigned by Admin
    int is_cancelled;                 // 0 = Active, 1 = Cancelled by Doctor
    char cancellation_reason[200];     // Reason recorded when cancelled
    char diagnosis[200];
    char treatment_info[200];
    char prescription[300];
    double total_bill;
    double paid_amount;
    int doctor_rating;
    int system_rating;
} Patient;

// Dynamic Pointer Arrays
Patient* patients = NULL;
Doctor* doctors = NULL;
int patient_count = 0;
int doctor_count = 0;

// Function Prototypes
void adminMenu();
void doctorMenu(char* doc_id);
void patientMenu(char* pat_id);

// Admin Functions
void addPatientRecord();
void deletePatientRecord();
void searchPatientRecord();
void addDoctorInformation();
void deleteDoctorInformation();
void searchDoctorRecord();
void processPatientPaymentAdmin();
void processDoctorSalaryAdmin();
void generateReport();

// Doctor Functions
void viewDoctorPayments(int doc_idx);
void viewDoctorAssignedSchedule(int doc_idx);
void viewUpcomingOT(int doc_idx);
void cancelDoctorSchedule(int doc_idx);

// Patient Functions
void viewDuesAndMakePayment(int pat_idx);
void rateDoctorAndSystem(int idx);
void viewCancellationStatus(int pat_idx);

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
            case 1: // ADMIN LOGIN
                if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
                    printf("\nLogin Successful as Admin! Wishing you a better life.\n");
                    adminMenu();
                } else {
                    printf("\nInvalid Admin Credentials!\n");
                }
                break;

            case 2: // DOCTOR LOGIN
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

            case 3: // PATIENT LOGIN
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
        printf("1. Add Patient Record (Includes Doctor Assignment & OT Details)\n");
        printf("2. Delete Patient Record\n");
        printf("3. Search Patient Record\n");
        printf("4. Add Doctor Information\n");
        printf("5. Delete Doctor Information\n");
        printf("6. Search Doctor Record\n");
        printf("7. Receive Patient Payment\n");
        printf("8. Disburse Doctor Payment/Salary\n");
        printf("9. Generate System Report\n");
        printf("10. Logout\n"); 
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid Option.\n");
            getchar();
            continue;
        }
        getchar();

        if (choice == 10) {
            printf("\nLogging out of Admin Panel... Wishing you a better life.\n");
            break;
        }

        switch (choice) {
            case 1: addPatientRecord(); break;
            case 2: deletePatientRecord(); break;
            case 3: searchPatientRecord(); break;
            case 4: addDoctorInformation(); break;
            case 5: deleteDoctorInformation(); break;
            case 6: searchDoctorRecord(); break;
            case 7: processPatientPaymentAdmin(); break;
            case 8: processDoctorSalaryAdmin(); break;
            case 9: generateReport(); break;
            default: printf("Invalid option.\n");
        }
    }
}

void addPatientRecord() {
    char id[50];
    printf("Enter Patient ID to Add: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].id, id) == 0) {
            printf("Patient ID already exists! Creation aborted.\n");
            return;
        }
    }

    patient_count++;
    patients = (Patient*)realloc(patients, patient_count * sizeof(Patient));
    int index = patient_count - 1;

    strcpy(patients[index].id, id);
    strcpy(patients[index].diagnosis, "Pending");
    strcpy(patients[index].treatment_info, "Pending");
    strcpy(patients[index].prescription, "None");
    patients[index].is_cancelled = 0;
    strcpy(patients[index].cancellation_reason, "None");
    patients[index].total_bill = 0.0;
    patients[index].paid_amount = 0.0;
    patients[index].doctor_rating = 0;
    patients[index].system_rating = 0;

    printf("Enter Full Name: ");
    fgets(patients[index].name, sizeof(patients[index].name), stdin);
    patients[index].name[strcspn(patients[index].name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &patients[index].age);
    getchar();

    printf("Enter Symptoms / Problems: ");
    fgets(patients[index].symptom, sizeof(patients[index].symptom), stdin);
    patients[index].symptom[strcspn(patients[index].symptom, "\n")] = 0;

    printf("Enter Assigned Doctor Name (e.g., Dr. Smith): ");
    fgets(patients[index].assigned_doctor, sizeof(patients[index].assigned_doctor), stdin);
    patients[index].assigned_doctor[strcspn(patients[index].assigned_doctor, "\n")] = 0;

    printf("Enter Appointment Date & Time (e.g., Mon 10:00 AM): ");
    fgets(patients[index].appointment_datetime, sizeof(patients[index].appointment_datetime), stdin);
    patients[index].appointment_datetime[strcspn(patients[index].appointment_datetime, "\n")] = 0;

    printf("Enter OT Details (e.g., 'Thu 3pm - OR 2' or enter 'None'): ");
    fgets(patients[index].ot_schedule, sizeof(patients[index].ot_schedule), stdin);
    patients[index].ot_schedule[strcspn(patients[index].ot_schedule, "\n")] = 0;

    printf("Enter Initial Service Charge / Bill ($): ");
    scanf("%lf", &patients[index].total_bill);
    getchar();

    printf("Patient Record, Doctor Assignment & OT Details Saved Successfully!\n");
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
    printf("\n+--------------------+----------------------+-----+----------------------+----------------------+----------------------+----------------------+\n");
    printf("|%-20s|%-22s|%-5s|%-22s|%-22s|%-22s|%-22s|\n", " Patient ID", " Name", " Age", " Assigned Doctor", " Appointment Time", " Status", " OT Details");
    printf("+--------------------+----------------------+-----+----------------------+----------------------+----------------------+----------------------+\n");

    for (int i = 0; i < patient_count; i++) {
        if (strstr(patients[i].id, query) || strstr(patients[i].name, query)) {
            char status[30];
            if (patients[i].is_cancelled) {
                strcpy(status, "CANCELLED");
            } else {
                strcpy(status, "ACTIVE");
            }

            printf("| %-19s| %-21s| %-4d| %-21s| %-21s| %-21s| %-21s|\n", 
                   patients[i].id, patients[i].name, patients[i].age, patients[i].assigned_doctor, patients[i].appointment_datetime, status, patients[i].ot_schedule);
            found = 1;
        }
    }
    printf("+--------------------+----------------------+-----+----------------------+----------------------+----------------------+----------------------+\n");
    
    if (!found) printf("No patient records matched.\n");
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

    printf("Enter Degrees: ");
    fgets(doctors[index].degree, sizeof(doctors[index].degree), stdin);
    doctors[index].degree[strcspn(doctors[index].degree, "\n")] = 0;

    printf("Enter Medical College Name: ");
    fgets(doctors[index].medical_college, sizeof(doctors[index].medical_college), stdin);
    doctors[index].medical_college[strcspn(doctors[index].medical_college, "\n")] = 0;

    printf("Enter Specialty: ");
    fgets(doctors[index].specialty, sizeof(doctors[index].specialty), stdin);
    doctors[index].specialty[strcspn(doctors[index].specialty, "\n")] = 0;

    doctors[index].earnings = 0.0;

    printf("Doctor Profile Saved!\n");
}

void deleteDoctorInformation() {
    char id[50];
    printf("Enter Doctor ID to Delete: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int found = -1;
    for (int i = 0; i < doctor_count; i++) {
        if (strcmp(doctors[i].id, id) == 0) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        for (int i = found; i < doctor_count - 1; i++) {
            doctors[i] = doctors[i + 1];
        }
        doctor_count--;
        if (doctor_count > 0) {
            doctors = (Doctor*)realloc(doctors, doctor_count * sizeof(Doctor));
        } else {
            free(doctors);
            doctors = NULL;
        }
        printf("Doctor record deleted successfully.\n");
    } else {
        printf("Doctor Record not found.\n");
    }
}

void searchDoctorRecord() {
    char query[100];
    printf("Enter Doctor ID or Name to search: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = 0;

    int found = 0;
    printf("\n+--------------------+----------------------+----------------------+----------------------+\n");
    printf("|%-20s|%-22s|%-22s|%-22s|\n", " Doctor ID", " Name", " Specialty", " College");
    printf("+--------------------+----------------------+----------------------+----------------------+\n");

    for (int i = 0; i < doctor_count; i++) {
        if (strstr(doctors[i].id, query) || strstr(doctors[i].name, query)) {
            printf("| %-19s| %-21s| %-21s| %-21s|\n", 
                   doctors[i].id, doctors[i].name, doctors[i].specialty, doctors[i].medical_college);
            found = 1;
        }
    }
    printf("+--------------------+----------------------+----------------------+----------------------+\n");

    if (!found) printf("No doctor records matched.\n");
}

void processPatientPaymentAdmin() {
    char id[50];
    printf("Enter Patient ID for payment collection: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int idx = -1;
    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].id, id) == 0) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        double due = patients[idx].total_bill - patients[idx].paid_amount;
        printf("Patient: %s | Total Bill: $%.2f | Already Paid: $%.2f | Due Amount: $%.2f\n",
               patients[idx].name, patients[idx].total_bill, patients[idx].paid_amount, due);

        if (due <= 0) {
            printf("No remaining dues for this patient.\n");
            return;
        }

        double pay;
        printf("Enter Payment Amount Received ($): ");
        scanf("%lf", &pay);
        getchar();

        if (pay > 0) {
            patients[idx].paid_amount += pay;
            printf("Payment successfully registered. Remaining Due: $%.2f\n", 
                   patients[idx].total_bill - patients[idx].paid_amount);
        } else {
            printf("Invalid Payment Amount.\n");
        }
    } else {
        printf("Patient ID not found.\n");
    }
}

void processDoctorSalaryAdmin() {
    char id[50];
    printf("Enter Doctor ID for Salary/Payment Disbursal: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int idx = -1;
    for (int i = 0; i < doctor_count; i++) {
        if (strcmp(doctors[i].id, id) == 0) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        double amount;
        printf("Doctor: %s | Current Accumulated Earnings: $%.2f\n", doctors[idx].name, doctors[idx].earnings);
        printf("Enter Salary / Payment Amount to Disburse ($): ");
        scanf("%lf", &amount);
        getchar();

        if (amount > 0) {
            doctors[idx].earnings += amount;
            printf("Payment of $%.2f successfully disbursed to Dr. %s!\n", amount, doctors[idx].name);
        } else {
            printf("Invalid Payment Amount.\n");
        }
    } else {
        printf("Doctor ID not found.\n");
    }
}

void generateReport() {
    printf("\n===================================================================================\n");
    printf("                       HEALWISH SYSTEM COMPREHENSIVE REPORT                        \n");
    printf("===================================================================================\n");
    printf("Total Active Patients in Database:  %d\n", patient_count);
    printf("Total Registered Doctors in Database: %d\n", doctor_count);
    
    printf("\n--- REGISTERED DOCTORS LIST ---\n");
    if (doctor_count > 0) {
        printf("+--------------------+----------------------+----------------------+----------------------+\n");
        printf("|%-20s|%-22s|%-22s|%-22s|\n", " Doctor ID", " Name", " Specialty", " Total Paid ($)");
        printf("+--------------------+----------------------+----------------------+----------------------+\n");
        for (int i = 0; i < doctor_count; i++) {
            char earn_str[30];
            snprintf(earn_str, sizeof(earn_str), "%.2f", doctors[i].earnings);
            printf("| %-19s| %-21s| %-21s| %-21s|\n", 
                   doctors[i].id, doctors[i].name, doctors[i].specialty, earn_str);
        }
        printf("+--------------------+----------------------+----------------------+----------------------+\n");
    } else {
        printf("No doctors currently in the database.\n");
    }

    printf("\n--- PATIENTS LIST ---\n");
    if (patient_count > 0) {
        printf("+--------------------+----------------------+-----+----------------------+----------------------+----------------------+\n");
        printf("|%-20s|%-22s|%-5s|%-22s|%-22s|%-22s|\n", " Patient ID", " Name", " Age", " Assigned Doctor", " Status", " OT Schedule");
        printf("+--------------------+----------------------+-----+----------------------+----------------------+----------------------+----------------------+\n");
        for (int i = 0; i < patient_count; i++) {
            char status[30];
            if (patients[i].is_cancelled) {
                strcpy(status, "CANCELLED");
            } else {
                strcpy(status, "ACTIVE");
            }

            printf("| %-19s| %-21s| %-4d| %-21s| %-21s| %-21s|\n", 
                   patients[i].id, patients[i].name, patients[i].age, patients[i].assigned_doctor, status, patients[i].ot_schedule);
        }
        printf("+--------------------+----------------------+-----+----------------------+----------------------+----------------------+----------------------+\n");
    } else {
        printf("No patients currently in the database.\n");
    }
    printf("===================================================================================\n");
}

/* ==================== DOCTOR DASHBOARD ==================== */
void doctorMenu(char* doc_id) {
    int choice;
    char target_pid[50];
    int doc_idx = -1;

    for(int i = 0; i < doctor_count; i++) {
        if(strcmp(doctors[i].id, doc_id) == 0) {
            doc_idx = i;
            break;
        }
    }

    while (1) {
        printf("\n--- DOCTOR PORTAL (Logged in as: %s) ---\n", doctors[doc_idx].name);
        printf("1. View Assigned Patient Queue\n");
        printf("2. Conduct Examination (Diagnosis/Treatment/Prescription)\n");
        printf("3. View Own Payments & Earnings\n");
        printf("4. View Assigned Schedule (Set by Admin)\n");
        printf("5. View Upcoming OT Schedule\n");
        printf("6. Cancel My Assigned Appointment / Schedule\n");
        printf("7. Logout\n"); 
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 7) {
            printf("\nLogging out of Doctor Portal... Wishing you a better life.\n");
            break;
        }

        switch (choice) {
            case 1: {
                int matches = 0;
                printf("\n--- Patients Under Treatment of Dr. %s ---\n", doctors[doc_idx].name);
                printf("+--------------------+----------------------+----------------------+----------------------+----------------------+\n");
                printf("|%-20s|%-22s|%-22s|%-22s|%-22s|\n", " Patient ID", " Name", " Symptom", " Appt Status", " OT Details");
                printf("+--------------------+----------------------+----------------------+----------------------+----------------------+\n");

                for (int i = 0; i < patient_count; i++) {
                    if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
                        char status[30];
                        if (patients[i].is_cancelled) {
                            strcpy(status, "CANCELLED");
                        } else {
                            strcpy(status, "ACTIVE");
                        }

                        printf("| %-19s| %-21s| %-21s| %-21s| %-21s|\n", 
                               patients[i].id, patients[i].name, patients[i].symptom, status, patients[i].ot_schedule);
                        matches++;
                    }
                }
                printf("+--------------------+----------------------+----------------------+----------------------+----------------------+----------------------+\n");
                if(matches == 0) printf("No patients currently assigned to your queue.\n");
                break;
            }
            case 2:
                printf("Enter Patient ID to examine: ");
                fgets(target_pid, sizeof(target_pid), stdin);
                target_pid[strcspn(target_pid, "\n")] = 0;

                int p_idx = -1;
                for (int i = 0; i < patient_count; i++) {
                    if (strcmp(patients[i].id, target_pid) == 0 && strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
                        p_idx = i;
                        break;
                    }
                }

                if (p_idx != -1) {
                    if (patients[p_idx].is_cancelled) {
                        printf("Cannot examine this patient — the appointment has been cancelled.\n");
                        break;
                    }

                    printf("Record Diagnosis: ");
                    fgets(patients[p_idx].diagnosis, sizeof(patients[p_idx].diagnosis), stdin);
                    patients[p_idx].diagnosis[strcspn(patients[p_idx].diagnosis, "\n")] = 0;

                    printf("Record Treatment Info: ");
                    fgets(patients[p_idx].treatment_info, sizeof(patients[p_idx].treatment_info), stdin);
                    patients[p_idx].treatment_info[strcspn(patients[p_idx].treatment_info, "\n")] = 0;

                    printf("Add Prescription: ");
                    fgets(patients[p_idx].prescription, sizeof(patients[p_idx].prescription), stdin);
                    patients[p_idx].prescription[strcspn(patients[p_idx].prescription, "\n")] = 0;

                    printf("Enter Additional Medical Fee/Bill to add ($): ");
                    double additional_bill;
                    scanf("%lf", &additional_bill);
                    getchar();
                    patients[p_idx].total_bill += additional_bill;

                    printf("Clinical updates saved successfully.\n");
                } else {
                    printf("Access Denied or Patient ID not found in your assigned queue.\n");
                }
                break;

            case 3: viewDoctorPayments(doc_idx); break;
            case 4: viewDoctorAssignedSchedule(doc_idx); break;
            case 5: viewUpcomingOT(doc_idx); break;
            case 6: cancelDoctorSchedule(doc_idx); break;
            default: printf("Invalid option.\n");
        }
    }
}

void viewDoctorPayments(int doc_idx) {
    printf("\n+----------------------------------------------------+\n");
    printf("|                DOCTOR FINANCIAL STATUS             |\n");
    printf("+----------------------------------------------------+\n");
    printf("| Doctor Name        : %-29s |\n", doctors[doc_idx].name);
    printf("| Total Disbursed Pay: $%-28.2f |\n", doctors[doc_idx].earnings);
    printf("+----------------------------------------------------+\n");
}

void viewDoctorAssignedSchedule(int doc_idx) {
    int matches = 0;
    printf("\n--- ASSIGNED APPOINTMENTS & SCHEDULE (ADMIN SET) ---\n");
    printf("+--------------------+----------------------+----------------------+----------------------+\n");
    printf("|%-20s|%-22s|%-22s|%-22s|\n", " Patient ID", " Patient Name", " Scheduled Time", " Status");
    printf("+--------------------+----------------------+----------------------+----------------------+\n");

    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
            char status[30];
            if (patients[i].is_cancelled) {
                strcpy(status, "CANCELLED");
            } else {
                strcpy(status, "ACTIVE");
            }

            printf("| %-19s| %-21s| %-21s| %-21s|\n", 
                   patients[i].id, patients[i].name, patients[i].appointment_datetime, status);
            matches++;
        }
    }
    printf("+--------------------+----------------------+----------------------+----------------------+\n");
    if (matches == 0) {
        printf("No appointments or schedule assigned by Admin yet.\n");
    }
}

void viewUpcomingOT(int doc_idx) {
    int matches = 0;
    printf("\n--- YOUR UPCOMING OT SCHEDULE (FROM ASSIGNED PATIENTS) ---\n");
    printf("+--------------------+----------------------+----------------------+----------------------+\n");
    printf("|%-20s|%-22s|%-22s|%-22s|\n", " Patient ID", " Patient Name", " OT Schedule Details", " Appt Status");
    printf("+--------------------+----------------------+----------------------+----------------------+\n");

    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0 && strcmp(patients[i].ot_schedule, "None") != 0 && strcmp(patients[i].ot_schedule, "") != 0) {
            char status[30];
            if (patients[i].is_cancelled) {
                strcpy(status, "CANCELLED");
            } else {
                strcpy(status, "ACTIVE");
            }

            printf("| %-19s| %-21s| %-21s| %-21s|\n", 
                   patients[i].id, patients[i].name, patients[i].ot_schedule, status);
            matches++;
        }
    }
    printf("+--------------------+----------------------+----------------------+----------------------+\n");
    if (matches == 0) {
        printf("No active OT operations currently scheduled for your patients.\n");
    }
}

void cancelDoctorSchedule(int doc_idx) {
    char target_pid[50];
    printf("\n--- CANCEL APPOINTMENT / SCHEDULE ---\n");
    printf("Enter Patient ID whose appointment you want to cancel: ");
    fgets(target_pid, sizeof(target_pid), stdin);
    target_pid[strcspn(target_pid, "\n")] = 0;

    int p_idx = -1;
    for (int i = 0; i < patient_count; i++) {
        // Author verification: Ensures only the assigned doctor can cancel their own schedule
        if (strcmp(patients[i].id, target_pid) == 0 && strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
            p_idx = i;
            break;
        }
    }

    if (p_idx != -1) {
        if (patients[p_idx].is_cancelled) {
            printf("This appointment is already marked as CANCELLED.\n");
            return;
        }

        printf("Enter reason for cancellation (e.g., Doctor unavailable / Emergency): ");
        fgets(patients[p_idx].cancellation_reason, sizeof(patients[p_idx].cancellation_reason), stdin);
        patients[p_idx].cancellation_reason[strcspn(patients[p_idx].cancellation_reason, "\n")] = 0;

        patients[p_idx].is_cancelled = 1;

        printf("\nAppointment for Patient %s (ID: %s) successfully CANCELLED!\n", patients[p_idx].name, patients[p_idx].id);
        printf("Notice updated across Admin and Patient portals.\n");
    } else {
        printf("Error: Patient ID not found or you are not authorized to cancel this appointment.\n");
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
        printf("2. View Appointment & Assigned Doctor Details\n");
        printf("3. View Schedule Cancellation Status\n");
        printf("4. View Treatment History & Prescription\n");
        printf("5. View Dues & Make Online Payment\n");
        printf("6. Rate Doctor & System\n");
        printf("7. Logout\n"); 
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 7) {
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
                for(int i = 0; i < doctor_count; i++) {
                    if(strcmp(doctors[i].name, patients[idx].assigned_doctor) == 0) {
                        doc_idx = i;
                        break;
                    }
                }

                printf("\n+----------------------------------------------------+\n");
                printf("|            APPOINTMENT & DOCTOR DETAILS            |\n");
                printf("+----------------------------------------------------+\n");
                printf("| Schedule Time  : %-33s |\n", patients[idx].appointment_datetime);
                printf("| OT Schedule    : %-33s |\n", patients[idx].ot_schedule);
                if (doc_idx != -1) {
                    printf("| Doctor Name    : %-33s |\n", doctors[doc_idx].name);
                    printf("| Degree(s)      : %-33s |\n", doctors[doc_idx].degree);
                    printf("| Specialty      : %-33s |\n", doctors[doc_idx].specialty);
                } else {
                    printf("| Assigned Doctor: %-33s |\n", patients[idx].assigned_doctor);
                }
                printf("+----------------------------------------------------+\n");
                break;
            }

            case 3:
                viewCancellationStatus(idx);
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

            case 5:
                viewDuesAndMakePayment(idx);
                break;

            case 6:
                rateDoctorAndSystem(idx);
                break;

            default:
                printf("Invalid option.\n");
        }
    }
}

void viewCancellationStatus(int pat_idx) {
    printf("\n+----------------------------------------------------+\n");
    printf("|             APPOINTMENT CANCELLATION STATUS        |\n");
    printf("+----------------------------------------------------+\n");
    if (patients[pat_idx].is_cancelled) {
        printf("| STATUS: CANCELLED BY DOCTOR                        |\n");
        printf("| Assigned Doctor : %-32s |\n", patients[pat_idx].assigned_doctor);
        printf("| Reason          : %-32s |\n", patients[pat_idx].cancellation_reason);
        printf("| Message: Your appointment has been cancelled by    |\n");
        printf("| your assigned doctor. Please contact the Admin.    |\n");
    } else {
        printf("| STATUS: ACTIVE                                     |\n");
        printf("| Your appointment is confirmed and active.          |\n");
        printf("| Scheduled Time: %-34s |\n", patients[pat_idx].appointment_datetime);
    }
    printf("+----------------------------------------------------+\n");
}

void viewDuesAndMakePayment(int pat_idx) {
    double due = patients[pat_idx].total_bill - patients[pat_idx].paid_amount;
    printf("\n+----------------------------------------------------+\n");
    printf("|                PATIENT BILLING STATUS              |\n");
    printf("+----------------------------------------------------+\n");
    printf("| Total Hospital Bill : $%-27.2f |\n", patients[pat_idx].total_bill);
    printf("| Amount Paid         : $%-27.2f |\n", patients[pat_idx].paid_amount);
    printf("| Net Dues Outstanding: $%-27.2f |\n", due);
    printf("+----------------------------------------------------+\n");

    if (due <= 0) {
        printf("You have no pending dues.\n");
        return;
    }

    printf("\nWould you like to make a payment now? (1 = Yes, 0 = No): ");
    int pay_choice;
    scanf("%d", &pay_choice);
    getchar();

    if (pay_choice == 1) {
        double amt;
        printf("Enter Payment Amount ($): ");
        scanf("%lf", &amt);
        getchar();

        if (amt > 0 && amt <= due) {
            patients[pat_idx].paid_amount += amt;
            printf("Payment of $%.2f received successfully! Remaining Dues: $%.2f\n", 
                   amt, patients[pat_idx].total_bill - patients[pat_idx].paid_amount);
        } else if (amt > due) {
            printf("Amount exceeds total dues ($%.2f). Please enter an exact or smaller amount.\n", due);
        } else {
            printf("Invalid Payment Amount.\n");
        }
    }
}

void rateDoctorAndSystem(int idx) {
    int doc_rating, sys_rating;

    printf("\n--- FEEDBACK & RATING ---\n");
    printf("Rate your assigned doctor (%s) from 1 to 5 Stars: ", patients[idx].assigned_doctor);
    scanf("%d", &doc_rating);
    if (doc_rating < 1) doc_rating = 1;
    if (doc_rating > 5) doc_rating = 5;
    patients[idx].doctor_rating = doc_rating;

    printf("Rate the Healwish System overall from 1 to 5 Stars: ");
    scanf("%d", &sys_rating);
    if (sys_rating < 1) sys_rating = 1;
    if (sys_rating > 5) sys_rating = 5;
    patients[idx].system_rating = sys_rating;

    printf("\nThank you for your feedback! Your ratings have been saved.\n");
}