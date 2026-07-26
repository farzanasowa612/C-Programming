#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* ==================== MD5 IMPLEMENTATION ====================
 * Self-contained MD5 (RFC 1321) implementation, embedded directly so the
 * project compiles with a plain `gcc file.c -o output` (no -lcrypto/OpenSSL
 * needed). Used to hash passwords before storing/comparing them.
 * ============================================================== */

typedef struct {
    uint64_t size;
    uint32_t buffer[4];
    uint8_t  input[64];
    uint8_t  digest[16];
} MD5Context;

#define MD5_A 0x67452301
#define MD5_B 0xefcdab89
#define MD5_C 0x98badcfe
#define MD5_D 0x10325476

static const uint32_t md5_S[] = {
    7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
    5, 9,14,20, 5, 9,14,20, 5, 9,14,20, 5, 9,14,20,
    4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
    6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
};

static const uint32_t md5_K[] = {
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
    0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
    0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
    0x6b901122,0xfd987193,0xa679438e,0x49b40821,
    0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
    0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
    0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
    0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
    0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
    0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
    0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
    0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
    0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
    0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
    0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
    0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
};

static const uint8_t md5_PADDING[64] = { 0x80 /* rest zero-initialized */ };

#define MD5_F(X,Y,Z) ((X & Y) | (~X & Z))
#define MD5_G(X,Y,Z) ((X & Z) | (Y & ~Z))
#define MD5_H(X,Y,Z) (X ^ Y ^ Z)
#define MD5_I(X,Y,Z) (Y ^ (X | ~Z))

static uint32_t md5_rotateLeft(uint32_t x, uint32_t n) {
    return (x << n) | (x >> (32 - n));
}

static void md5Step(uint32_t *buffer, uint32_t *input) {
    uint32_t AA = buffer[0], BB = buffer[1], CC = buffer[2], DD = buffer[3];
    uint32_t E; unsigned int j;

    for (unsigned int i = 0; i < 64; ++i) {
        switch (i / 16) {
            case 0: E = MD5_F(BB, CC, DD); j = i; break;
            case 1: E = MD5_G(BB, CC, DD); j = ((i * 5) + 1) % 16; break;
            case 2: E = MD5_H(BB, CC, DD); j = ((i * 3) + 5) % 16; break;
            default: E = MD5_I(BB, CC, DD); j = (i * 7) % 16; break;
        }
        uint32_t temp = DD;
        DD = CC;
        CC = BB;
        BB = BB + md5_rotateLeft(AA + E + md5_K[i] + input[j], md5_S[i]);
        AA = temp;
    }

    buffer[0] += AA; buffer[1] += BB; buffer[2] += CC; buffer[3] += DD;
}

static void md5Init(MD5Context *ctx) {
    ctx->size = 0;
    ctx->buffer[0] = MD5_A;
    ctx->buffer[1] = MD5_B;
    ctx->buffer[2] = MD5_C;
    ctx->buffer[3] = MD5_D;
}

static void md5Update(MD5Context *ctx, const uint8_t *input_buffer, size_t input_len) {
    uint32_t input[16];
    unsigned int offset = ctx->size % 64;
    ctx->size += (uint64_t)input_len;

    for (unsigned int i = 0; i < input_len; ++i) {
        ctx->input[offset++] = input_buffer[i];

        if (offset % 64 == 0) {
            for (unsigned int j = 0; j < 16; ++j) {
                input[j] = (uint32_t)(ctx->input[(j*4)+3]) << 24 |
                           (uint32_t)(ctx->input[(j*4)+2]) << 16 |
                           (uint32_t)(ctx->input[(j*4)+1]) << 8  |
                           (uint32_t)(ctx->input[(j*4)]);
            }
            md5Step(ctx->buffer, input);
            offset = 0;
        }
    }
}

static void md5Finalize(MD5Context *ctx) {
    uint32_t input[16];
    unsigned int offset = ctx->size % 64;
    unsigned int padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

    md5Update(ctx, md5_PADDING, padding_length);
    ctx->size -= (uint64_t)padding_length;

    for (unsigned int j = 0; j < 14; ++j) {
        input[j] = (uint32_t)(ctx->input[(j*4)+3]) << 24 |
                   (uint32_t)(ctx->input[(j*4)+2]) << 16 |
                   (uint32_t)(ctx->input[(j*4)+1]) << 8  |
                   (uint32_t)(ctx->input[(j*4)]);
    }
    input[14] = (uint32_t)(ctx->size * 8);
    input[15] = (uint32_t)((ctx->size * 8) >> 32);

    md5Step(ctx->buffer, input);

    for (unsigned int i = 0; i < 4; ++i) {
        ctx->digest[(i*4)+0] = (uint8_t)((ctx->buffer[i] & 0x000000FF));
        ctx->digest[(i*4)+1] = (uint8_t)((ctx->buffer[i] & 0x0000FF00) >> 8);
        ctx->digest[(i*4)+2] = (uint8_t)((ctx->buffer[i] & 0x00FF0000) >> 16);
        ctx->digest[(i*4)+3] = (uint8_t)((ctx->buffer[i] & 0xFF000000) >> 24);
    }
}

// Hashes a null-terminated string and writes a 32-char lowercase hex digest
// (+ null terminator) into 'output_hex', which must be at least 33 bytes.
void md5_hash(const char *input, char *output_hex) {
    MD5Context ctx;
    md5Init(&ctx);
    md5Update(&ctx, (const uint8_t *)input, strlen(input));
    md5Finalize(&ctx);

    for (int i = 0; i < 16; i++) {
        sprintf(output_hex + (i * 2), "%02x", ctx.digest[i]);
    }
    output_hex[32] = '\0';
}
/* ================== END MD5 IMPLEMENTATION =================== */

// Hardcoded Admin credentials (password stored as MD5 hash, not plain text)
#define ADMIN_USERNAME      "admin"
#define ADMIN_PASSWORD_HASH "0192023a7bbd73250516f069df18b500" // MD5 of "admin123"

// Structures
typedef struct {
    char id[50];
    char name[100];
    char degree[100];
    char medical_college[150];
    char specialty[100];
    double earnings;            // Accumulated total payment received
    char password_hash[33];     // MD5 hash of the doctor's own login password
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
    char password_hash[33];     // MD5 hash of the patient's own login password
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
void addDoctorInformation();
void deleteDoctorInformation();
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

        // Hash whatever the user typed so it can be compared against
        // the MD5 hashes stored for admin/doctors/patients.
        char entered_hash[33];
        md5_hash(password, entered_hash);

        switch (choice) {
            case 1: // ADMIN LOGIN
                if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(entered_hash, ADMIN_PASSWORD_HASH) == 0) {
                    printf("\nLogin Successful as Admin! Wishing you a better life.\n");
                    adminMenu();
                } else {
                    printf("\nInvalid Admin Credentials!\n");
                }
                break;

            case 2: { // DOCTOR LOGIN
                int found = -1;
                for (int i = 0; i < doctor_count; i++) {
                    if (strcmp(doctors[i].id, username) == 0) {
                        found = i;
                        break;
                    }
                }
                if (found == -1) {
                    printf("\nDoctor ID not found! Create via Admin Panel first.\n");
                } else if (strcmp(entered_hash, doctors[found].password_hash) == 0) {
                    printf("\nLogin Successful! Welcome, %s. Wishing you a better life.\n", doctors[found].name);
                    doctorMenu(doctors[found].id);
                } else {
                    printf("\nIncorrect Password!\n");
                }
                break;
            }

            case 3: { // PATIENT LOGIN
                int found = -1;
                for (int i = 0; i < patient_count; i++) {
                    if (strcmp(patients[i].id, username) == 0) {
                        found = i;
                        break;
                    }
                }
                if (found == -1) {
                    printf("\nPatient ID not found! Admit via Admin Panel first.\n");
                } else if (strcmp(entered_hash, patients[found].password_hash) == 0) {
                    printf("\nLogin Successful! Welcome, %s. Wishing you a better life.\n", patients[found].name);
                    patientMenu(patients[found].id);
                } else {
                    printf("\nIncorrect Password!\n");
                }
                break;
            }

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
        printf("3. Add Doctor Information\n");
        printf("4. Delete Doctor Information\n");
        printf("5. Receive Patient Payment\n");
        printf("6. Disburse Doctor Payment/Salary\n");
        printf("7. Generate System Report\n");
        printf("8. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid Option.\n");
            getchar();
            continue;
        }
        getchar();

        if (choice == 8) {
            printf("\nLogging out of Admin Panel... Wishing you a better life.\n");
            break;
        }

        switch (choice) {
            case 1: addPatientRecord(); break;
            case 2: deletePatientRecord(); break;
            case 3: addDoctorInformation(); break;
            case 4: deleteDoctorInformation(); break;
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

    char raw_password[100];
    printf("Set Login Password for this Patient: ");
    fgets(raw_password, sizeof(raw_password), stdin);
    raw_password[strcspn(raw_password, "\n")] = 0;
    md5_hash(raw_password, patients[index].password_hash);

    printf("Patient Record, Doctor Assignment & OT Details Saved Successfully!\n");
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

    char raw_password[100];
    printf("Set Login Password for this Doctor: ");
    fgets(raw_password, sizeof(raw_password), stdin);
    raw_password[strcspn(raw_password, "\n")] = 0;
    md5_hash(raw_password, doctors[index].password_hash);

    printf("Doctor Profile Saved!\n");
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
                        printf("Cannot examine this patient � the appointment has been cancelled.\n");
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