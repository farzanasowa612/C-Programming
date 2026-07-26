#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Secure Password Hashes (Pre-computed MD5)
// admin123  -> 0192023a7bbd73250516f069df18b500
// doctor123 -> 3f2dc2a51a89c92694b8e3a241ed497b
// patient123-> 360155b95a8947b6a18836ec4ffc0a5e

const char* ADMIN_HASH = "0192023a7bbd73250516f069df18b500";
const char* DOCTOR_HASH = "3f2dc2a51a89c92694b8e3a241ed497b";
const char* PATIENT_HASH = "360155b95a8947b6a18836ec4ffc0a5e";

// Structures
typedef struct {
    char id[50];
    char name[100];
    char degree[100];          
    char medical_college[150];  
    char specialty[100];
    double earnings;            
    char schedules[20][100];    // Doctor's available time slots
    int schedule_count;
} Doctor;

typedef struct {
    char id[50];
    char name[100];
    int age;
    char symptom[200];
    char assigned_doctor[100];
    char appointment_datetime[100]; // Specific schedule assigned to patient
    char ot_schedule[100];            
    int is_cancelled;                 
    char cancellation_reason[200];     
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
int getIntInput();
void computeMD5(const char* initial_msg, char* outputBuffer);

// Admin Functions
void addPatientRecord();
void addDoctorInformation();
void viewDoctorSchedulesAdmin();
void assignDoctorToPatient();
void processPatientPaymentAdmin();
void processDoctorSalaryAdmin();
void generateReport();

// Doctor Functions
void viewDoctorPayments(int doc_idx);
void makeDoctorSchedule(int doc_idx);
void viewUpcomingOT(int doc_idx);
void cancelDoctorSchedule(int doc_idx);

// Patient Functions
void viewDuesAndMakePayment(int pat_idx);
void rateDoctorAndSystem(int idx);
void viewCancellationStatus(int pat_idx);

// Helper for secure integer input
int getIntInput() {
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        return atoi(buffer);
    }
    return 0;
}

// ==================== NATIVE MD5 IMPLEMENTATION ====================
#define LEFT_ROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void computeMD5(const char *initial_msg, char *outputBuffer) {
    size_t initial_len = strlen(initial_msg);
    size_t padded_len = initial_len + 1;

    // Calculate padding length so it ends at 56 bytes mod 64
    while (padded_len % 64 != 56) {
        padded_len++;
    }

    size_t total_len = padded_len + 8;
    uint8_t *msg = (uint8_t *)calloc(total_len, 1);
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 0x80; // Append '1' bit

    // Append length in bits (64-bit, little-endian)
    uint64_t bits_len = (uint64_t)initial_len * 8;
    for (int i = 0; i < 8; i++) {
        msg[padded_len + i] = (uint8_t)(bits_len >> (i * 8));
    }

    // Initialize variables
    uint32_t h0 = 0x67452301, h1 = 0xefcdab89, h2 = 0x98badcfe, h3 = 0x10325476;

    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
    uint32_t r[] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
    };

    // Process blocks
    for (size_t offset = 0; offset < total_len; offset += 64) {
        uint32_t w[16];
        for (int i = 0; i < 16; i++) { // Endian-safe conversion
            w[i] = (uint32_t)msg[offset + i * 4] | ((uint32_t)msg[offset + i * 4 + 1] << 8) |
                   ((uint32_t)msg[offset + i * 4 + 2] << 16) | ((uint32_t)msg[offset + i * 4 + 3] << 24);
        }

        uint32_t a = h0, b = h1, c = h2, d = h3;

        for (uint32_t i = 0; i < 64; i++) {
            uint32_t f, g;
            if (i < 16) { f = (b & c) | ((~b) & d); g = i; } 
            else if (i < 32) { f = (d & b) | ((~d) & c); g = (5 * i + 1) % 16; } 
            else if (i < 48) { f = b ^ c ^ d; g = (3 * i + 5) % 16; } 
            else { f = c ^ (b | (~d)); g = (7 * i) % 16; }

            uint32_t temp = d;
            d = c;
            c = b;
            b = b + LEFT_ROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
        }
        h0 += a; h1 += b; h2 += c; h3 += d;
    }
    free(msg);

    // Format output to string safely
    sprintf(outputBuffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            (uint8_t)(h0), (uint8_t)(h0 >> 8), (uint8_t)(h0 >> 16), (uint8_t)(h0 >> 24),
            (uint8_t)(h1), (uint8_t)(h1 >> 8), (uint8_t)(h1 >> 16), (uint8_t)(h1 >> 24),
            (uint8_t)(h2), (uint8_t)(h2 >> 8), (uint8_t)(h2 >> 16), (uint8_t)(h2 >> 24),
            (uint8_t)(h3), (uint8_t)(h3 >> 8), (uint8_t)(h3 >> 16), (uint8_t)(h3 >> 24));
}
// ===================================================================

int main() {
    int choice;
    char username[100], password[100], hashed_password[33];

    while (1) {
        printf("\n===================================\n");
        printf("     WELCOME TO HEALWISH SYSTEM     \n");
        printf("===================================\n");
        printf("1. Admin Login\n");
        printf("2. Doctor Login\n");
        printf("3. Patient Login\n");
        printf("4. Exit System\n");
        printf("Enter your choice: ");
        choice = getIntInput();

        if (choice == 4) {
            printf("\nExiting system. Freeing memory...\n");
            free(patients);
            free(doctors);
            break;
        }

        if (choice < 1 || choice > 4) {
            printf("Invalid selection!\n");
            continue;
        }

        printf("Enter Username / Access ID: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0; 

        printf("Enter Password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        computeMD5(password, hashed_password);

        switch (choice) {
            case 1: // ADMIN LOGIN
                if (strcmp(username, "admin") == 0 && strcmp(hashed_password, ADMIN_HASH) == 0) {
                    printf("\nLogin Successful as Admin!\n");
                    adminMenu();
                } else {
                    printf("\nInvalid Admin Credentials!\n");
                }
                break;

            case 2: // DOCTOR LOGIN
                if (strcmp(hashed_password, DOCTOR_HASH) == 0) {
                    int found = -1;
                    for (int i = 0; i < doctor_count; i++) {
                        if (strcmp(doctors[i].id, username) == 0) {
                            found = i;
                            break;
                        }
                    }
                    if (found != -1) {
                        printf("\nLogin Successful! Welcome, %s.\n", doctors[found].name);
                        doctorMenu(doctors[found].id);
                    } else {
                        printf("\nDoctor ID not found! Admin must create profile first.\n");
                    }
                } else {
                    printf("\nIncorrect Password!\n");
                }
                break;

            case 3: // PATIENT LOGIN
                if (strcmp(hashed_password, PATIENT_HASH) == 0) {
                    int found = -1;
                    for (int i = 0; i < patient_count; i++) {
                        if (strcmp(patients[i].id, username) == 0) {
                            found = i;
                            break;
                        }
                    }
                    if (found != -1) {
                        printf("\nLogin Successful! Welcome, %s.\n", patients[found].name);
                        patientMenu(patients[found].id);
                    } else {
                        printf("\nPatient ID not found! Admin must admit patient first.\n");
                    }
                } else {
                    printf("\nIncorrect Password!\n");
                }
                break;
        }
    }
    return 0;
}

/* ==================== ADMIN DASHBOARD ==================== */
void adminMenu() {
    int choice;
    while (1) {
        printf("\n--- ADMIN PANEL ---\n");
        printf("1. Add Patient Record (Admission Only)\n");
        printf("2. Add Doctor Information\n");
        printf("3. View Doctor's Schedule\n");
        printf("4. Assign Doctor & Schedule to a Patient\n");
        printf("5. Receive Patient Payment\n");
        printf("6. Disburse Doctor Payment/Salary\n");
        printf("7. Generate System Report\n");
        printf("8. Logout\n"); 
        printf("Enter choice: ");
        choice = getIntInput();

        if (choice == 8) {
            printf("\nLogging out of Admin Panel...\n");
            break;
        }

        switch (choice) {
            case 1: addPatientRecord(); break;
            case 2: addDoctorInformation(); break;
            case 3: viewDoctorSchedulesAdmin(); break;
            case 4: assignDoctorToPatient(); break;
            case 5: processPatientPaymentAdmin(); break;
            case 6: processDoctorSalaryAdmin(); break;
            case 7: generateReport(); break;
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
    strcpy(patients[index].assigned_doctor, "None Assigned Yet");
    strcpy(patients[index].appointment_datetime, "None");
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
    patients[index].age = getIntInput();

    printf("Enter Symptoms / Problems: ");
    fgets(patients[index].symptom, sizeof(patients[index].symptom), stdin);
    patients[index].symptom[strcspn(patients[index].symptom, "\n")] = 0;

    printf("Enter OT Details (e.g., 'Thu 3pm - OR 2' or enter 'None'): ");
    fgets(patients[index].ot_schedule, sizeof(patients[index].ot_schedule), stdin);
    patients[index].ot_schedule[strcspn(patients[index].ot_schedule, "\n")] = 0;

    printf("Enter Initial Service Charge / Bill ($): ");
    char bill_buffer[50];
    fgets(bill_buffer, sizeof(bill_buffer), stdin);
    patients[index].total_bill = atof(bill_buffer);

    printf("Patient Record Saved Successfully! (Please assign a doctor from the Admin Menu)\n");
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
    doctors[index].schedule_count = 0;

    printf("Doctor Profile Saved!\n");
}

void viewDoctorSchedulesAdmin() {
    if (doctor_count == 0) {
        printf("No doctors available in the system.\n");
        return;
    }
    printf("\n--- DOCTOR SCHEDULES ---\n");
    for (int i = 0; i < doctor_count; i++) {
        printf("\nDoctor: %s (Specialty: %s)\n", doctors[i].name, doctors[i].specialty);
        if (doctors[i].schedule_count == 0) {
            printf("  -> No available slots created by the doctor yet.\n");
        } else {
            for (int j = 0; j < doctors[i].schedule_count; j++) {
                printf("  Slot %d: %s\n", j + 1, doctors[i].schedules[j]);
            }
        }
    }
}

void assignDoctorToPatient() {
    char pat_id[50], doc_id[50];
    
    printf("Enter Patient ID to assign doctor to: ");
    fgets(pat_id, sizeof(pat_id), stdin);
    pat_id[strcspn(pat_id, "\n")] = 0;

    int p_idx = -1;
    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].id, pat_id) == 0) {
            p_idx = i;
            break;
        }
    }

    if (p_idx == -1) {
        printf("Patient ID not found!\n");
        return;
    }

    printf("\nAvailable Doctors:\n");
    for (int i = 0; i < doctor_count; i++) {
        printf("ID: %-15s | Name: %-20s | Specialty: %s\n", doctors[i].id, doctors[i].name, doctors[i].specialty);
    }

    printf("\nEnter Doctor ID to assign: ");
    fgets(doc_id, sizeof(doc_id), stdin);
    doc_id[strcspn(doc_id, "\n")] = 0;

    int d_idx = -1;
    for (int i = 0; i < doctor_count; i++) {
        if (strcmp(doctors[i].id, doc_id) == 0) {
            d_idx = i;
            break;
        }
    }

    if (d_idx == -1) {
        printf("Doctor ID not found!\n");
        return;
    }

    if (doctors[d_idx].schedule_count == 0) {
        printf("This doctor has not opened any available schedules yet. Cannot assign.\n");
        return;
    }

    printf("\nAvailable Time Slots for %s:\n", doctors[d_idx].name);
    for (int j = 0; j < doctors[d_idx].schedule_count; j++) {
        printf("[%d] %s\n", j + 1, doctors[d_idx].schedules[j]);
    }

    printf("Select Slot Number to assign: ");
    int slot_choice = getIntInput();

    if (slot_choice < 1 || slot_choice > doctors[d_idx].schedule_count) {
        printf("Invalid slot selection.\n");
        return;
    }

    // Assign to patient
    strcpy(patients[p_idx].assigned_doctor, doctors[d_idx].name);
    strcpy(patients[p_idx].appointment_datetime, doctors[d_idx].schedules[slot_choice - 1]);
    
    // Remove the booked slot from the doctor's array
    for (int j = slot_choice - 1; j < doctors[d_idx].schedule_count - 1; j++) {
        strcpy(doctors[d_idx].schedules[j], doctors[d_idx].schedules[j + 1]);
    }
    doctors[d_idx].schedule_count--;

    printf("Successfully assigned Patient %s to %s for slot: %s\n", 
           patients[p_idx].name, doctors[d_idx].name, patients[p_idx].appointment_datetime);
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

        char pay_buffer[50];
        printf("Enter Payment Amount Received ($): ");
        fgets(pay_buffer, sizeof(pay_buffer), stdin);
        double pay = atof(pay_buffer);

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
        char amt_buffer[50];
        printf("Doctor: %s | Current Accumulated Earnings: $%.2f\n", doctors[idx].name, doctors[idx].earnings);
        printf("Enter Salary / Payment Amount to Disburse ($): ");
        fgets(amt_buffer, sizeof(amt_buffer), stdin);
        double amount = atof(amt_buffer);

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
        printf("4. Make / Add Available Schedule Slots\n");
        printf("5. View Upcoming OT Schedule\n");
        printf("6. Cancel Assigned Appointment / Schedule\n");
        printf("7. Logout\n"); 
        printf("Enter choice: ");
        choice = getIntInput();

        if (choice == 7) {
            printf("\nLogging out of Doctor Portal...\n");
            break;
        }

        switch (choice) {
            case 1: {
                int matches = 0;
                printf("\n--- Patients Under Treatment of Dr. %s ---\n", doctors[doc_idx].name);
                printf("+--------------------+----------------------+----------------------+----------------------+----------------------+\n");
                printf("|%-20s|%-22s|%-22s|%-22s|%-22s|\n", " Patient ID", " Name", " Scheduled Time", " Appt Status", " OT Details");
                printf("+--------------------+----------------------+----------------------+----------------------+\n");

                for (int i = 0; i < patient_count; i++) {
                    if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
                        char status[30];
                        if (patients[i].is_cancelled) {
                            strcpy(status, "CANCELLED");
                        } else {
                            strcpy(status, "ACTIVE");
                        }

                        printf("| %-19s| %-21s| %-21s| %-21s| %-21s|\n", 
                               patients[i].id, patients[i].name, patients[i].appointment_datetime, status, patients[i].ot_schedule);
                        matches++;
                    }
                }
                printf("+--------------------+----------------------+----------------------+----------------------+----------------------+\n");
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
                    char bill_buffer[50];
                    fgets(bill_buffer, sizeof(bill_buffer), stdin);
                    patients[p_idx].total_bill += atof(bill_buffer);

                    printf("Clinical updates saved successfully.\n");
                } else {
                    printf("Access Denied or Patient ID not found in your assigned queue.\n");
                }
                break;

            case 3: viewDoctorPayments(doc_idx); break;
            case 4: makeDoctorSchedule(doc_idx); break;
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

void makeDoctorSchedule(int doc_idx) {
    if (doctors[doc_idx].schedule_count >= 20) {
        printf("You have reached the maximum allowed available slots (20).\n");
        return;
    }

    printf("\n--- CURRENT UNBOOKED SLOTS ---\n");
    if (doctors[doc_idx].schedule_count == 0) {
        printf("You have no available slots open.\n");
    } else {
        for (int i = 0; i < doctors[doc_idx].schedule_count; i++) {
            printf("[%d] %s\n", i + 1, doctors[doc_idx].schedules[i]);
        }
    }

    printf("\nEnter new available time slot (e.g., 'Monday 10:00 AM - 12:00 PM'): ");
    char new_slot[100];
    fgets(new_slot, sizeof(new_slot), stdin);
    new_slot[strcspn(new_slot, "\n")] = 0;

    int idx = doctors[doc_idx].schedule_count;
    strcpy(doctors[doc_idx].schedules[idx], new_slot);
    doctors[doc_idx].schedule_count++;

    printf("Schedule slot added successfully! Admin can now assign patients to this slot.\n");
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
    printf("\n--- CANCEL ASSIGNED APPOINTMENT ---\n");
    printf("Enter Patient ID whose appointment you want to cancel: ");
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
            printf("This appointment is already marked as CANCELLED.\n");
            return;
        }

        printf("Enter reason for cancellation (e.g., Doctor unavailable / Emergency): ");
        fgets(patients[p_idx].cancellation_reason, sizeof(patients[p_idx].cancellation_reason), stdin);
        patients[p_idx].cancellation_reason[strcspn(patients[p_idx].cancellation_reason, "\n")] = 0;

        patients[p_idx].is_cancelled = 1;

        printf("\nAppointment for Patient %s successfully CANCELLED!\n", patients[p_idx].name);
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
        printf("2. View Assigned Doctor & Schedule\n");
        printf("3. View Schedule Cancellation Status\n");
        printf("4. View Treatment History & Prescription\n");
        printf("5. View Dues & Make Online Payment\n");
        printf("6. Rate Doctor & System\n");
        printf("7. Logout\n"); 
        printf("Enter choice: ");
        choice = getIntInput();

        if (choice == 7) {
            printf("\nLogging out of Patient Portal...\n");
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
    int pay_choice = getIntInput();

    if (pay_choice == 1) {
        printf("Enter Payment Amount ($): ");
        char amt_buffer[50];
        fgets(amt_buffer, sizeof(amt_buffer), stdin);
        double amt = atof(amt_buffer);

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
    doc_rating = getIntInput();
    if (doc_rating < 1) doc_rating = 1;
    if (doc_rating > 5) doc_rating = 5;
    patients[idx].doctor_rating = doc_rating;

    printf("Rate the Healwish System overall from 1 to 5 Stars: ");
    sys_rating = getIntInput();
    if (sys_rating < 1) sys_rating = 1;
    if (sys_rating > 5) sys_rating = 5;
    patients[idx].system_rating = sys_rating;

    printf("\nThank you for your feedback! Your ratings have been saved.\n");
}