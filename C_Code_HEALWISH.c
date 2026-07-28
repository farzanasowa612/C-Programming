#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* ==================== CUSTOM MD5 IMPLEMENTATION ==================== */

typedef struct {
    uint32_t state[4];
    uint32_t count[2];
    unsigned char buffer[64];
} MD5_CTX;

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
    (a) += F((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
    (a) += I((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
  }

void MD5Decode(uint32_t *output, const unsigned char *input, unsigned int len) {
    unsigned int i, j;
    for (i = 0, j = 0; j < len; i++, j += 4)
        output[i] = ((uint32_t)input[j]) | (((uint32_t)input[j+1]) << 8) |
                    (((uint32_t)input[j+2]) << 16) | (((uint32_t)input[j+3]) << 24);
}

void MD5Encode(unsigned char *output, uint32_t *input, unsigned int len) {
    unsigned int i, j;
    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = (unsigned char)(input[i] & 0xff);
        output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
        output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
        output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
    }
}

void MD5Transform(uint32_t state[4], const unsigned char block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    MD5Decode(x, block, 64);

    FF(a, b, c, d, x[ 0], 7, 0xd76aa478); 
    FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); 
    FF(c, d, a, b, x[ 2], 17, 0x242070db); 
    FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); 
    FF(a, b, c, d, x[ 4], 7, 0xf57c0faf); 
    FF(d, a, b, c, x[ 5], 12, 0x4787c62a); 
    FF(c, d, a, b, x[ 6], 17, 0xa8304613); 
    FF(b, c, d, a, x[ 7], 22, 0xfd469501); 
    FF(a, b, c, d, x[ 8], 7, 0x698098d8); 
    FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); 
    FF(c, d, a, b, x[10], 17, 0xffff5bb1); 
    FF(b, c, d, a, x[11], 22, 0x895cd7be); 
    FF(a, b, c, d, x[12], 7, 0x6b901122); 
    FF(d, a, b, c, x[13], 12, 0xfd987193); 
    FF(c, d, a, b, x[14], 17, 0xa679438e); 
    FF(b, c, d, a, x[15], 22, 0x49b40821); 

    GG(a, b, c, d, x[ 1], 5, 0xf61e2562); 
    GG(d, a, b, c, x[ 6], 9, 0xc040b340); 
    GG(c, d, a, b, x[11], 14, 0x265e5a51); 
    GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); 
    GG(a, b, c, d, x[ 5], 5, 0xd62f105d); 
    GG(d, a, b, c, x[10], 9, 0x2441453);  
    GG(c, d, a, b, x[15], 14, 0xd8a1e681); 
    GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); 
    GG(a, b, c, d, x[ 9], 5, 0x21e1cde6); 
    GG(d, a, b, c, x[14], 9, 0xc33707d6); 
    GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); 
    GG(b, c, d, a, x[ 8], 20, 0x455a14ed); 
    GG(a, b, c, d, x[13], 5, 0xa9e3e905); 
    GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8); 
    GG(c, d, a, b, x[ 7], 14, 0x676f02d9); 
    GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); 

    HH(a, b, c, d, x[ 5], 4, 0xfffa3942); 
    HH(d, a, b, c, x[ 8], 11, 0x8771f681); 
    HH(c, d, a, b, x[11], 16, 0x6d9d6122); 
    HH(b, c, d, a, x[14], 23, 0xfde5380c); 
    HH(a, b, c, d, x[ 1], 4, 0xa4beea44); 
    HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); 
    HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); 
    HH(b, c, d, a, x[10], 23, 0xbebfbc70); 
    HH(a, b, c, d, x[13], 4, 0x289b7ec6); 
    HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); 
    HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); 
    HH(b, c, d, a, x[ 6], 23, 0x4881d05); 
    HH(a, b, c, d, x[ 9], 4, 0xd9d4d039); 
    HH(d, a, b, c, x[12], 11, 0xe6db99e5); 
    HH(c, d, a, b, x[15], 16, 0x1fa27cf8); 
    HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); 

    II(a, b, c, d, x[ 0], 6, 0xf4292244); 
    II(d, a, b, c, x[ 7], 10, 0x432aff97); 
    II(c, d, a, b, x[14], 15, 0xab9423a7); 
    II(b, c, d, a, x[ 5], 21, 0xfc93a039); 
    II(a, b, c, d, x[12], 6, 0x655b59c3); 
    II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); 
    II(c, d, a, b, x[10], 15, 0xffeff47d); 
    II(b, c, d, a, x[ 1], 21, 0x85845dd1); 
    II(a, b, c, d, x[ 8], 6, 0x6fa87e4f); 
    II(d, a, b, c, x[15], 10, 0xfe2ce6e0); 
    II(c, d, a, b, x[ 6], 15, 0xa3014314); 
    II(b, c, d, a, x[13], 21, 0x4e0811a1); 
    II(a, b, c, d, x[ 4], 6, 0xf7537e82); 
    II(d, a, b, c, x[11], 10, 0xbd3af235); 
    II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); 
    II(b, c, d, a, x[ 9], 21, 0xeb86d391); 

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

void MD5Init(MD5_CTX *context) {
    context->count[0] = context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}

void MD5Update(MD5_CTX *context, const unsigned char *input, unsigned int inputLen) {
    unsigned int i, index, partLen;
    index = (unsigned int)((context->count[0] >> 3) & 0x3F);
    if ((context->count[0] += ((uint32_t)inputLen << 3)) < ((uint32_t)inputLen << 3))
        context->count[1]++;
    context->count[1] += ((uint32_t)inputLen >> 29);
    partLen = 64 - index;
    if (inputLen >= partLen) {
        memcpy(&context->buffer[index], input, partLen);
        MD5Transform(context->state, context->buffer);
        for (i = partLen; i + 63 < inputLen; i += 64)
            MD5Transform(context->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }
    memcpy(&context->buffer[index], &input[i], inputLen - i);
}

void MD5Final(unsigned char digest[16], MD5_CTX *context) {
    unsigned char bits[8];
    unsigned int index, padLen;
    static unsigned char PADDING[64] = { 0x80 };
    MD5Encode(bits, context->count, 8);
    index = (unsigned int)((context->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5Update(context, PADDING, padLen);
    MD5Update(context, bits, 8);
    MD5Encode(digest, context->state, 16);
}

// Wrapper function to use in the system
void generateMD5(const char *string, char *output_buffer) {
    MD5_CTX context;
    unsigned char digest[16];
    MD5Init(&context);
    MD5Update(&context, (const unsigned char*)string, strlen(string));
    MD5Final(digest, &context);
    
    for(int i = 0; i < 16; i++) {
        sprintf(&output_buffer[i*2], "%02x", (unsigned int)digest[i]);
    }
    output_buffer[32] = '\0';
}

/* ==================== SYSTEM STRUCTURES & VARIABLES ==================== */

typedef struct {
    char id[50];
    char name[100];
    char degree[100];          
    char medical_college[150];  
    char specialty[100];
    char schedule[50];          
    char password_hash[33];     
    double earnings;            
} Doctor;

typedef struct {
    char id[50];
    char name[100];
    int age;
    char symptom[200];
    char assigned_doctor[100];
    char appointment_date[50];
    char schedule_time[50];           
    char password_hash[33];           
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

Patient* patients = NULL;
Doctor* doctors = NULL;
int patient_count = 0;
int doctor_count = 0;

/* ==================== FUNCTION PROTOTYPES ==================== */

void adminMenu();
void doctorMenu(char* doc_id);
void patientMenu(char* pat_id);

void addDoctorInformation();
void addPatientRecord();
void processPatientPaymentAdmin();
void processDoctorSalaryAdmin();
void generateReport();

void viewDoctorPayments(int doc_idx);
void viewDoctorAssignedSchedule(int doc_idx);
void viewUpcomingOT(int doc_idx);
void cancelDoctorSchedule(int doc_idx);

void viewDuesAndMakePayment(int pat_idx);
void rateDoctorAndSystem(int idx);
void viewCancellationStatus(int pat_idx);

/* ==================== MAIN LOGIC ==================== */

int main() {
    int choice;
    char username[100], password[100], hashed_input[33];
    
    // Admin Master Password Hash ("admin123")
    char admin_hash[] = "0192023a7bbd73250516f069df18b500"; 

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
        
        generateMD5(password, hashed_input);

        switch (choice) {
            case 1: // ADMIN LOGIN
                if (strcmp(username, "admin") == 0 && strcmp(hashed_input, admin_hash) == 0) {
                    printf("\nLogin Successful as Admin! Wishing you a better life.\n");
                    adminMenu();
                } else {
                    printf("\nInvalid Admin Credentials!\n");
                }
                break;

            case 2: // DOCTOR LOGIN
                {
                    int found = -1;
                    for (int i = 0; i < doctor_count; i++) {
                        if (strcmp(doctors[i].id, username) == 0 && strcmp(doctors[i].password_hash, hashed_input) == 0) {
                            found = i;
                            break;
                        }
                    }
                    if (found != -1) {
                        printf("\nLogin Successful! Welcome, %s. Wishing you a better life.\n", doctors[found].name);
                        doctorMenu(doctors[found].id);
                    } else {
                        printf("\nInvalid Doctor ID or Password!\n");
                    }
                }
                break;

            case 3: // PATIENT LOGIN
                {
                    int found = -1;
                    for (int i = 0; i < patient_count; i++) {
                        if (strcmp(patients[i].id, username) == 0 && strcmp(patients[i].password_hash, hashed_input) == 0) {
                            found = i;
                            break;
                        }
                    }
                    if (found != -1) {
                        printf("\nLogin Successful! Welcome, %s. Wishing you a better life.\n", patients[found].name);
                        patientMenu(patients[found].id);
                    } else {
                        printf("\nInvalid Patient ID or Password!\n");
                    }
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
        printf("1. Add Doctor Information & Schedule\n");
        printf("2. Add Patient Record (Assign Doctor & OT)\n");
        printf("3. Receive Patient Payment\n");
        printf("4. Disburse Doctor Payment/Salary\n");
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
            case 1: addDoctorInformation(); break;
            case 2: addPatientRecord(); break;
            case 3: processPatientPaymentAdmin(); break;
            case 4: processDoctorSalaryAdmin(); break;
            case 5: generateReport(); break;
            default: printf("Invalid option.\n");
        }
    }
}

void addDoctorInformation() {
    doctor_count++;
    doctors = (Doctor*)realloc(doctors, doctor_count * sizeof(Doctor));
    int index = doctor_count - 1;
    char raw_password[100];

    printf("Enter Doctor ID: ");
    fgets(doctors[index].id, sizeof(doctors[index].id), stdin);
    doctors[index].id[strcspn(doctors[index].id, "\n")] = 0;

    printf("Set Password for Doctor Account: ");
    fgets(raw_password, sizeof(raw_password), stdin);
    raw_password[strcspn(raw_password, "\n")] = 0;
    generateMD5(raw_password, doctors[index].password_hash);

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

    int sched_choice = 0;
    while(sched_choice < 1 || sched_choice > 3) {
        printf("\nSelect Doctor's Daily Schedule:\n");
        printf("1. 10:00 AM - 12:00 PM\n");
        printf("2. 03:00 PM - 05:00 PM\n");
        printf("3. 07:00 PM - 09:00 PM\n");
        printf("Enter Choice (1/2/3): ");
        scanf("%d", &sched_choice);
        getchar();
        
        switch(sched_choice) {
            case 1: strcpy(doctors[index].schedule, "10am - 12pm"); break;
            case 2: strcpy(doctors[index].schedule, "3pm - 5pm"); break;
            case 3: strcpy(doctors[index].schedule, "7pm - 9pm"); break;
            default: printf("Invalid choice, please select 1, 2, or 3.\n");
        }
    }

    doctors[index].earnings = 0.0;

    printf("\nDoctor Profile Secured and Saved!\n");
}

void addPatientRecord() {
    if (doctor_count == 0) {
        printf("No doctors available in the system! Please add a doctor first.\n");
        return;
    }

    char id[50], raw_password[100];
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

    printf("Enter Account Password for this Patient: ");
    fgets(raw_password, sizeof(raw_password), stdin);
    raw_password[strcspn(raw_password, "\n")] = 0;
    generateMD5(raw_password, patients[index].password_hash);

    printf("Enter Full Name: ");
    fgets(patients[index].name, sizeof(patients[index].name), stdin);
    patients[index].name[strcspn(patients[index].name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &patients[index].age);
    getchar();

    printf("Enter Symptoms / Problems: ");
    fgets(patients[index].symptom, sizeof(patients[index].symptom), stdin);
    patients[index].symptom[strcspn(patients[index].symptom, "\n")] = 0;

    int doc_assigned = 0;
    while (!doc_assigned) {
        char target_date[50];
        printf("\nEnter Appointment Date (YYYY-MM-DD): ");
        fgets(target_date, sizeof(target_date), stdin);
        target_date[strcspn(target_date, "\n")] = 0;

        char target_doc[100];
        printf("Enter Doctor Name to assign (e.g., Dr. Smith): ");
        fgets(target_doc, sizeof(target_doc), stdin);
        target_doc[strcspn(target_doc, "\n")] = 0;

        int doc_idx = -1;
        for (int i = 0; i < doctor_count; i++) {
            if (strcmp(doctors[i].name, target_doc) == 0) {
                doc_idx = i;
                break;
            }
        }

        if (doc_idx == -1) {
            printf("Doctor not found in system. Please try again.\n");
            continue;
        }

        int daily_count = 0;
        for (int i = 0; i < patient_count - 1; i++) { 
            if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0 &&
                strcmp(patients[i].appointment_date, target_date) == 0 &&
                patients[i].is_cancelled == 0) {
                daily_count++;
            }
        }

        if (daily_count < 20) {
            strcpy(patients[index].appointment_date, target_date);
            strcpy(patients[index].assigned_doctor, doctors[doc_idx].name);
            strcpy(patients[index].schedule_time, doctors[doc_idx].schedule);
            
            printf("\n>>> SUCCESS: Doctor Assigned! Patient #%d of 20 for %s on %s.\n", 
                   daily_count + 1, doctors[doc_idx].name, target_date);
            doc_assigned = 1;
        } else {
            printf("\n>>> DOCTOR SCHEDULE FULL: Dr. %s already has 20 patients on %s.\n", doctors[doc_idx].name, target_date);
            printf("Please shift to the next day, or select another available doctor from the list below:\n");
            
            printf("+----------------------+----------------------+\n");
            printf("| Available Doctors    | Daily Schedule Slot  |\n");
            printf("+----------------------+----------------------+\n");
            for (int i = 0; i < doctor_count; i++) {
                if (i != doc_idx) {
                    printf("| %-20s | %-20s |\n", doctors[i].name, doctors[i].schedule);
                }
            }
            printf("+----------------------+----------------------+\n");
        }
    }

    printf("Enter OT Details (e.g., 'Thu 3pm - OR 2' or enter 'None'): ");
    fgets(patients[index].ot_schedule, sizeof(patients[index].ot_schedule), stdin);
    patients[index].ot_schedule[strcspn(patients[index].ot_schedule, "\n")] = 0;

    printf("Enter Initial Service Charge / Bill ($): ");
    scanf("%lf", &patients[index].total_bill);
    getchar();

    printf("\nPatient Record Secured and Saved Successfully!\n");
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
        printf("|%-20s|%-22s|%-22s|%-22s|\n", " Doctor ID", " Name", " Daily Schedule", " Total Paid ($)");
        printf("+--------------------+----------------------+----------------------+----------------------+\n");
        for (int i = 0; i < doctor_count; i++) {
            char earn_str[30];
            snprintf(earn_str, sizeof(earn_str), "%.2f", doctors[i].earnings);
            printf("| %-19s| %-21s| %-21s| %-21s|\n", 
                   doctors[i].id, doctors[i].name, doctors[i].schedule, earn_str);
        }
        printf("+--------------------+----------------------+----------------------+----------------------+\n");
    } else {
        printf("No doctors currently in the database.\n");
    }

    printf("\n--- PATIENTS LIST ---\n");
    if (patient_count > 0) {
        printf("+--------------------+----------------------+----------------------+----------------------+\n");
        printf("|%-20s|%-22s|%-22s|%-22s|\n", " Patient ID", " Name", " Assigned Doctor", " Appt Date");
        printf("+--------------------+----------------------+----------------------+----------------------+\n");
        for (int i = 0; i < patient_count; i++) {
            printf("| %-19s| %-21s| %-21s| %-21s|\n", 
                   patients[i].id, patients[i].name, patients[i].assigned_doctor, patients[i].appointment_date);
        }
        printf("+--------------------+----------------------+----------------------+----------------------+\n");
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
        printf("2. Conduct Examination (Diagnosis/Treatment)\n");
        printf("3. View Own Payments & Earnings\n");
        printf("4. View My Schedule & Appointments\n");
        printf("5. View Upcoming OT Schedule\n");
        printf("6. Cancel Assigned Appointment\n");
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
                printf("+--------------------+----------------------+----------------------+----------------------+\n");
                printf("|%-20s|%-22s|%-22s|%-22s|\n", " Patient ID", " Name", " Date", " Appt Status");
                printf("+--------------------+----------------------+----------------------+----------------------+\n");

                for (int i = 0; i < patient_count; i++) {
                    if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
                        char status[30];
                        if (patients[i].is_cancelled) strcpy(status, "CANCELLED");
                        else strcpy(status, "ACTIVE");

                        printf("| %-19s| %-21s| %-21s| %-21s|\n", 
                               patients[i].id, patients[i].name, patients[i].appointment_date, status);
                        matches++;
                    }
                }
                printf("+--------------------+----------------------+----------------------+----------------------+\n");
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
    printf("\n--- YOUR SCHEDULED HOURS: %s ---\n", doctors[doc_idx].schedule);
    printf("+--------------------+----------------------+----------------------+\n");
    printf("|%-20s|%-22s|%-22s|\n", " Date", " Patient Name", " Status");
    printf("+--------------------+----------------------+----------------------+\n");

    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
            char status[30];
            if (patients[i].is_cancelled) strcpy(status, "CANCELLED");
            else strcpy(status, "ACTIVE");

            printf("| %-19s| %-21s| %-21s|\n", 
                   patients[i].appointment_date, patients[i].name, status);
            matches++;
        }
    }
    printf("+--------------------+----------------------+----------------------+\n");
    if (matches == 0) {
        printf("No appointments scheduled for you yet.\n");
    }
}

void viewUpcomingOT(int doc_idx) {
    int matches = 0;
    printf("\n--- YOUR UPCOMING OT SCHEDULE ---\n");
    printf("+--------------------+----------------------+----------------------+\n");
    printf("|%-20s|%-22s|%-22s|\n", " Patient Name", " OT Schedule Details", " Appt Status");
    printf("+--------------------+----------------------+----------------------+\n");

    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0 && strcmp(patients[i].ot_schedule, "None") != 0 && strcmp(patients[i].ot_schedule, "") != 0) {
            char status[30];
            if (patients[i].is_cancelled) strcpy(status, "CANCELLED");
            else strcpy(status, "ACTIVE");

            printf("| %-19s| %-21s| %-21s|\n", 
                   patients[i].name, patients[i].ot_schedule, status);
            matches++;
        }
    }
    printf("+--------------------+----------------------+----------------------+\n");
    if (matches == 0) {
        printf("No active OT operations currently scheduled for your patients.\n");
    }
}

void cancelDoctorSchedule(int doc_idx) {
    char target_pid[50];
    printf("\n--- CANCEL APPOINTMENT ---\n");
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

        printf("Enter reason for cancellation (e.g., Doctor unavailable): ");
        fgets(patients[p_idx].cancellation_reason, sizeof(patients[p_idx].cancellation_reason), stdin);
        patients[p_idx].cancellation_reason[strcspn(patients[p_idx].cancellation_reason, "\n")] = 0;

        patients[p_idx].is_cancelled = 1;

        printf("\nAppointment for Patient %s successfully CANCELLED!\n", patients[p_idx].name);
    } else {
        printf("Error: Patient ID not found or unauthorized to cancel.\n");
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
        printf("2. View Scheduled Date & Doctor Timing\n");
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
                printf("| Date           : %-33s |\n", patients[idx].appointment_date);
                printf("| Doctor's Timing: %-33s |\n", patients[idx].schedule_time);
                printf("| OT Schedule    : %-33s |\n", patients[idx].ot_schedule);
                if (doc_idx != -1) {
                    printf("| Doctor Name    : %-33s |\n", doctors[doc_idx].name);
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
    } else {
        printf("| STATUS: ACTIVE                                     |\n");
        printf("| Your appointment is confirmed and active.          |\n");
        printf("| Scheduled Date: %-34s |\n", patients[pat_idx].appointment_date);
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