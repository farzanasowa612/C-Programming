#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

void clearScreen() {
    system(CLEAR_SCREEN);
}

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

/* ==================== STRUCTURES ==================== */

typedef struct {
    char id[50];
    char name[100];
    char degree[100];          
    char medical_college[150];  
    char specialty[100];
    char schedule[50];          // Slot label like "10am-12pm", "3pm-5pm", "7pm-9pm"
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

/* ==================== PROTOTYPES ==================== */

void adminMenu();
void doctorMenu(char* doc_id);
void patientMenu(char* pat_id);

void addDoctorMenu();
void addPatientMenu();
void managePaymentsMenu();
void disburseSalaryMenu();
void generateReportMenu();

int navPrompt(int *current_choice, int max_options);

/* ==================== TIME CALCULATOR HELPER ==================== */
// Converts slot range string (e.g. "10am-12pm", "3pm-5pm", "7pm-9pm") or generic time into starting hour/minute,
// then adds (index * 5) minutes for sequential patient appointments.
void calculateAppointmentTime(const char* slot_range, int patient_seq_index, char* output_time) {
    int start_hour = 10;
    int start_min = 0;

    if (strstr(slot_range, "10am") != NULL) {
        start_hour = 10;
        start_min = 0;
    } else if (strstr(slot_range, "3pm") != NULL) {
        start_hour = 15;
        start_min = 0;
    } else if (strstr(slot_range, "7pm") != NULL) {
        start_hour = 19;
        start_min = 0;
    } else if (sscanf(slot_range, "%d:%d", &start_hour, &start_min) != 2) {
        start_hour = 10;
        start_min = 0;
    }

    // Add 5 minutes for each prior patient in the same slot/doctor queue
    int total_minutes = (start_hour * 60) + start_min + (patient_seq_index * 5);
    int final_hour = (total_minutes / 60) % 24;
    int final_min = total_minutes % 60;

    sprintf(output_time, "%02d:%02d", final_hour, final_min);
}

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
            while(getchar() != '\n');
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

/* ==================== NAVIGATION CONTROLLER ==================== */
int navPrompt(int *current_choice, int max_options) {
    printf("\n-------------------------------------------------\n");
    printf("Navigation: [1] Next Option  [2] Previous Option  [3] Home / Main Menu\n");
    printf("Enter choice: ");
    int nav_action;
    if (scanf("%d", &nav_action) != 1) {
        while(getchar() != '\n');
        return 3;
    }
    getchar();

    if (nav_action == 1) {
        (*current_choice)++;
        if (*current_choice > max_options) {
            *current_choice = 1; // Loop back to first option
        }
        return 1;
    } else if (nav_action == 2) {
        (*current_choice)--;
        if (*current_choice < 1) {
            *current_choice = max_options; // Loop back to last option
        }
        return 2;
    } else {
        return 3; // Home / Main Menu
    }
}

/* ==================== ADMIN DASHBOARD ==================== */
void adminMenu() {
    int choice = 1;
    while (1) {
        clearScreen();
        printf("\n--- ADMIN PANEL ---\n");
        printf("1. Add Doctor\n");
        printf("2. Add Patient\n");
        printf("3. Receive Patient Payment\n");
        printf("4. Distribute Doctor Salary\n");
        printf("5. Generate System Report\n");
        printf("6. Logout\n");
        printf("Enter choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        getchar();

        if (choice == 6) break;
        if (choice < 1 || choice > 5) continue;

        while (1) {
            clearScreen();
            if (choice == 1) {
                addDoctorMenu();
            } else if (choice == 2) {
                addPatientMenu();
            } else if (choice == 3) {
                managePaymentsMenu();
            } else if (choice == 4) {
                disburseSalaryMenu();
            } else if (choice == 5) {
                generateReportMenu();
            }

            int action = navPrompt(&choice, 5);
            if (action == 3) break; // Home option returns to admin choice loop
        }
    }
}

void addDoctorMenu() {
    doctor_count++;
    doctors = (Doctor*)realloc(doctors, doctor_count * sizeof(Doctor));
    int index = doctor_count - 1;
    char raw_password[100];

    printf("\n--- ADD DOCTOR ---\n");
    printf("Enter Doctor ID: ");
    fgets(doctors[index].id, sizeof(doctors[index].id), stdin);
    doctors[index].id[strcspn(doctors[index].id, "\n")] = 0;

    printf("Set Password: ");
    fgets(raw_password, sizeof(raw_password), stdin);
    raw_password[strcspn(raw_password, "\n")] = 0;
    generateMD5(raw_password, doctors[index].password_hash);

    printf("Enter Doctor Name: ");
    fgets(doctors[index].name, sizeof(doctors[index].name), stdin);
    doctors[index].name[strcspn(doctors[index].name, "\n")] = 0;

    printf("Enter Degrees: ");
    fgets(doctors[index].degree, sizeof(doctors[index].degree), stdin);
    doctors[index].degree[strcspn(doctors[index].degree, "\n")] = 0;

    printf("Enter Medical College: ");
    fgets(doctors[index].medical_college, sizeof(doctors[index].medical_college), stdin);
    doctors[index].medical_college[strcspn(doctors[index].medical_college, "\n")] = 0;

    printf("Enter Specialty: ");
    fgets(doctors[index].specialty, sizeof(doctors[index].specialty), stdin);
    doctors[index].specialty[strcspn(doctors[index].specialty, "\n")] = 0;

    int slot_choice;
    printf("\nSelect Doctor Timing Slot:\n");
    printf("1. 10am-12pm\n");
    printf("2. 3pm-5pm\n");
    printf("3. 7pm-9pm\n");
    printf("Enter option (1-3): ");
    if (scanf("%d", &slot_choice) != 1) {
        slot_choice = 1;
    }
    getchar();

    if (slot_choice == 2) {
        strcpy(doctors[index].schedule, "3pm-5pm");
    } else if (slot_choice == 3) {
        strcpy(doctors[index].schedule, "7pm-9pm");
    } else {
        strcpy(doctors[index].schedule, "10am-12pm");
    }

    doctors[index].earnings = 0.0;
    printf("\nDoctor profile added successfully!");
}

void addPatientMenu() {
    if (doctor_count == 0) {
        printf("\nNo doctors available! Add a doctor first.");
        return;
    }

    patient_count++;
    patients = (Patient*)realloc(patients, patient_count * sizeof(Patient));
    int index = patient_count - 1;
    char raw_password[100];

    printf("\n--- ADD PATIENT ---\n");
    printf("Enter Patient ID: ");
    fgets(patients[index].id, sizeof(patients[index].id), stdin);
    patients[index].id[strcspn(patients[index].id, "\n")] = 0;

    for (int i = 0; i < patient_count - 1; i++) {
        if (strcmp(patients[i].id, patients[index].id) == 0) {
            printf("Patient ID already exists! Aborting.\n");
            patient_count--;
            return;
        }
    }

    strcpy(patients[index].diagnosis, "Pending");
    strcpy(patients[index].treatment_info, "Pending");
    strcpy(patients[index].prescription, "None");
    patients[index].is_cancelled = 0;
    strcpy(patients[index].cancellation_reason, "None");
    patients[index].total_bill = 0.0;
    patients[index].paid_amount = 0.0;
    patients[index].doctor_rating = 0;
    patients[index].system_rating = 0;

    printf("Set Password: ");
    fgets(raw_password, sizeof(raw_password), stdin);
    raw_password[strcspn(raw_password, "\n")] = 0;
    generateMD5(raw_password, patients[index].password_hash);

    printf("Enter Full Name: ");
    fgets(patients[index].name, sizeof(patients[index].name), stdin);
    patients[index].name[strcspn(patients[index].name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &patients[index].age);
    getchar();

    printf("Enter Symptoms: ");
    fgets(patients[index].symptom, sizeof(patients[index].symptom), stdin);
    patients[index].symptom[strcspn(patients[index].symptom, "\n")] = 0;

    char target_date[50];
    printf("Enter Appointment Date (YYYY-MM-DD): ");
    fgets(target_date, sizeof(target_date), stdin);
    target_date[strcspn(target_date, "\n")] = 0;
    strcpy(patients[index].appointment_date, target_date);

    printf("\nAvailable Doctors:\n");
    for (int i = 0; i < doctor_count; i++) {
        printf("- %s (Slot Window: %s)\n", doctors[i].name, doctors[i].schedule);
    }
    char target_doc[100];
    printf("Enter Doctor Name to assign: ");
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
        printf("Doctor not found! Defaulting assignment.\n");
        strcpy(patients[index].assigned_doctor, "Unassigned");
        strcpy(patients[index].schedule_time, "N/A");
    } else {
        strcpy(patients[index].assigned_doctor, doctors[doc_idx].name);

        // Count existing active patients assigned to this doctor on the same date for 5-minute interval scheduling
        int existing_count = 0;
        for (int i = 0; i < patient_count - 1; i++) {
            if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0 &&
                strcmp(patients[i].appointment_date, target_date) == 0 &&
                patients[i].is_cancelled == 0) {
                existing_count++;
            }
        }

        char calculated_slot_time[20];
        calculateAppointmentTime(doctors[doc_idx].schedule, existing_count, calculated_slot_time);
        strcpy(patients[index].schedule_time, calculated_slot_time);

        printf("Assigned slot sequence #%d set to: %s (Slot Window: %s)\n", 
               existing_count + 1, calculated_slot_time, doctors[doc_idx].schedule);
    }

    printf("Enter OT Details (or 'None'): ");
    fgets(patients[index].ot_schedule, sizeof(patients[index].ot_schedule), stdin);
    patients[index].ot_schedule[strcspn(patients[index].ot_schedule, "\n")] = 0;

    printf("Enter Initial Bill ($): ");
    scanf("%lf", &patients[index].total_bill);
    getchar();

    printf("\nPatient registered successfully!");
}

void managePaymentsMenu() {
    char id[50];
    printf("\n--- PATIENT PAYMENT COLLECTION ---\n");
    printf("Enter Patient ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int idx = -1;
    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].id, id) == 0) { idx = i; break; }
    }

    if (idx != -1) {
        double due = patients[idx].total_bill - patients[idx].paid_amount;
        printf("Patient: %s | Total: $%.2f | Due: $%.2f\n", patients[idx].name, patients[idx].total_bill, due);
        if (due <= 0) {
            printf("No dues remaining.");
            return;
        }
        double pay;
        printf("Enter payment amount ($): ");
        scanf("%lf", &pay);
        getchar();
        if (pay > 0) {
            patients[idx].paid_amount += pay;
            printf("Payment recorded successfully! New Due: $%.2f.", patients[idx].total_bill - patients[idx].paid_amount);
        }
    } else {
        printf("Patient not found.");
    }
}

void disburseSalaryMenu() {
    char id[50];
    printf("\n--- DOCTOR SALARY DISBURSAL ---\n");
    printf("Enter Doctor ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int idx = -1;
    for (int i = 0; i < doctor_count; i++) {
        if (strcmp(doctors[i].id, id) == 0) { idx = i; break; }
    }

    if (idx != -1) {
        double amt;
        printf("Doctor: %s | Accumulated Earnings: $%.2f\n", doctors[idx].name, doctors[idx].earnings);
        printf("Enter salary/disbursal amount ($): ");
        scanf("%lf", &amt);
        getchar();
        if (amt > 0) {
            doctors[idx].earnings += amt;
            printf("Disbursed $%.2f successfully!", amt);
        }
    } else {
        printf("Doctor not found.");
    }
}

void generateReportMenu() {
    printf("=========================================================================\n");
    printf("                  HEALWISH SYSTEM COMPREHENSIVE REPORT                   \n");
    printf("=========================================================================\n");
    printf("Total Doctors: %d | Total Patients: %d\n", doctor_count, patient_count);
    
    printf("\n--- DOCTORS ---\n");
    for (int i = 0; i < doctor_count; i++) {
        printf("ID: %s | Name: %s | Base Slot: %s | Earnings: $%.2f\n", 
               doctors[i].id, doctors[i].name, doctors[i].schedule, doctors[i].earnings);
    }

    printf("\n--- PATIENTS ---\n");
    for (int i = 0; i < patient_count; i++) {
        printf("ID: %s | Name: %s | Doctor: %s | Date: %s | Appt Time: %s | Due: $%.2f\n",
               patients[i].id, patients[i].name, patients[i].assigned_doctor, 
               patients[i].appointment_date, patients[i].schedule_time, 
               patients[i].total_bill - patients[i].paid_amount);
    }
    printf("=========================================================================\n");
}

/* ==================== DOCTOR DASHBOARD ==================== */
void doctorMenu(char* doc_id) {
    int choice = 1;
    int doc_idx = -1;
    for (int i = 0; i < doctor_count; i++) {
        if (strcmp(doctors[i].id, doc_id) == 0) { doc_idx = i; break; }
    }

    while (1) {
        clearScreen();
        printf("\n--- DOCTOR PORTAL (Dr. %s) ---\n", doctors[doc_idx].name);
        printf("1. View Assigned Patients & Conduct Examination\n");
        printf("2. View Schedule, OT & Cancel Appointment\n");
        printf("3. See Own Salary / Earnings\n");
        printf("4. Logout\n");
        printf("Enter choice (1-4): ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        getchar();

        if (choice == 4) break;
        if (choice < 1 || choice > 3) continue;

        while (1) {
            clearScreen();
            if (choice == 1) {
                printf("--- PATIENT QUEUE & EXAMINATION ---\n");
                for (int i = 0; i < patient_count; i++) {
                    if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
                        printf("[%s] Name: %s | Date: %s | Time: %s | Symptoms: %s | Status: %s\n",
                               patients[i].id, patients[i].name, patients[i].appointment_date, 
                               patients[i].schedule_time, patients[i].symptom, 
                               patients[i].is_cancelled ? "CANCELLED" : "ACTIVE");
                    }
                }
                char target_pid[50];
                printf("\nEnter Patient ID to examine (or type 'back' to skip): ");
                fgets(target_pid, sizeof(target_pid), stdin);
                target_pid[strcspn(target_pid, "\n")] = 0;

                if (strcmp(target_pid, "back") != 0) {
                    int p_idx = -1;
                    for (int i = 0; i < patient_count; i++) {
                        if (strcmp(patients[i].id, target_pid) == 0 && strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
                            p_idx = i; break;
                        }
                    }

                    if (p_idx != -1 && !patients[p_idx].is_cancelled) {
                        printf("Diagnosis: ");
                        fgets(patients[p_idx].diagnosis, sizeof(patients[p_idx].diagnosis), stdin);
                        patients[p_idx].diagnosis[strcspn(patients[p_idx].diagnosis, "\n")] = 0;

                        printf("Treatment Info: ");
                        fgets(patients[p_idx].treatment_info, sizeof(patients[p_idx].treatment_info), stdin);
                        patients[p_idx].treatment_info[strcspn(patients[p_idx].treatment_info, "\n")] = 0;

                        printf("Prescription: ");
                        fgets(patients[p_idx].prescription, sizeof(patients[p_idx].prescription), stdin);
                        patients[p_idx].prescription[strcspn(patients[p_idx].prescription, "\n")] = 0;

                        printf("Add Extra Fee ($): ");
                        double add_fee;
                        scanf("%lf", &add_fee);
                        getchar();
                        patients[p_idx].total_bill += add_fee;

                        printf("Examination details updated successfully!\n");
                    } else {
                        printf("Patient not found or appointment is cancelled.\n");
                    }
                }
            } else if (choice == 2) {
                printf("--- SCHEDULE, OT & CANCELLATIONS ---\n");
                for (int i = 0; i < patient_count; i++) {
                    if (strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
                        printf("Patient: %s | Date: %s | Time: %s | OT: %s | Status: %s\n",
                               patients[i].name, patients[i].appointment_date, patients[i].schedule_time,
                               patients[i].ot_schedule, patients[i].is_cancelled ? "CANCELLED" : "ACTIVE");
                    }
                }
                char target_pid[50];
                printf("\nEnter Patient ID to cancel appointment (or press Enter to skip): ");
                fgets(target_pid, sizeof(target_pid), stdin);
                target_pid[strcspn(target_pid, "\n")] = 0;

                if (strlen(target_pid) > 0) {
                    int p_idx = -1;
                    for (int i = 0; i < patient_count; i++) {
                        if (strcmp(patients[i].id, target_pid) == 0 && strcmp(patients[i].assigned_doctor, doctors[doc_idx].name) == 0) {
                            p_idx = i; break;
                        }
                    }
                    if (p_idx != -1) {
                        printf("Cancellation Reason: ");
                        fgets(patients[p_idx].cancellation_reason, sizeof(patients[p_idx].cancellation_reason), stdin);
                        patients[p_idx].cancellation_reason[strcspn(patients[p_idx].cancellation_reason, "\n")] = 0;
                        patients[p_idx].is_cancelled = 1;
                        printf("Appointment successfully cancelled.\n");
                    } else {
                        printf("Invalid Patient ID.\n");
                    }
                }
            } else if (choice == 3) {
                printf("--- OWN SALARY & EARNINGS ---\n");
                printf("Doctor Name: %s\n", doctors[doc_idx].name);
                printf("Total Disbursed Earnings: $%.2f\n", doctors[doc_idx].earnings);
            }

            int action = navPrompt(&choice, 3);
            if (action == 3) break;
        }
    }
}

/* ==================== PATIENT DASHBOARD ==================== */
void patientMenu(char* pat_id) {
    int choice = 1;
    int idx = -1;
    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].id, pat_id) == 0) { idx = i; break; }
    }

    clearScreen();
    printf("=================================================================\n");
    printf("                      NOTIFICATION & ALERTS                      \n");
    printf("=================================================================\n");
    if (patients[idx].is_cancelled) {
        printf("[ALERT] Your appointment with Dr. %s has been CANCELLED.\n", patients[idx].assigned_doctor);
        printf("Reason: %s\n", patients[idx].cancellation_reason);
    } else {
        printf("[NOTICE] Your appointment with Dr. %s is CONFIRMED.\n", patients[idx].assigned_doctor);
        printf("Appointment Time: %s on %s\n", patients[idx].schedule_time, patients[idx].appointment_date);
    }
    printf("=================================================================\n");
    printf("Press Enter to proceed to Patient Portal.");
    getchar();

    while (1) {
        clearScreen();
        printf("\n--- PATIENT PORTAL (%s) ---\n", patients[idx].name);
        printf("1. View Profile Info, Treatment History, Prescription & Dues\n");
        printf("2. View Schedule Date, Slot Time & Cancellation Status\n");
        printf("3. Rate System & Doctor\n");
        printf("4. Logout\n");
        printf("Enter choice (1-4): ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        getchar();

        if (choice == 4) break;
        if (choice < 1 || choice > 3) continue;

        while (1) {
            clearScreen();
            if (choice == 1) {
                printf("--- PROFILE, MEDICAL HISTORY & DUES ---\n");
                printf("Patient ID     : %s\n", patients[idx].id);
                printf("Full Name      : %s (Age: %d)\n", patients[idx].name, patients[idx].age);
                printf("Symptoms       : %s\n", patients[idx].symptom);
                printf("Diagnosis      : %s\n", patients[idx].diagnosis);
                printf("Treatment Info : %s\n", patients[idx].treatment_info);
                printf("Prescription   : %s\n", patients[idx].prescription);
                printf("Total Bill     : $%.2f\n", patients[idx].total_bill);
                printf("Paid Amount    : $%.2f\n", patients[idx].paid_amount);
                printf("Outstanding Due: $%.2f\n", patients[idx].total_bill - patients[idx].paid_amount);
            } else if (choice == 2) {
                printf("--- SCHEDULE & CANCELLATION DETAILS ---\n");
                printf("Assigned Doctor : %s\n", patients[idx].assigned_doctor);
                printf("Appointment Date: %s\n", patients[idx].appointment_date);
                printf("Appointment Time: %s\n", patients[idx].schedule_time);
                printf("OT Details      : %s\n", patients[idx].ot_schedule);
                if (patients[idx].is_cancelled) {
                    printf("Status          : CANCELLED\n");
                    printf("Cancellation Msg: %s\n", patients[idx].cancellation_reason);
                } else {
                    printf("Status          : ACTIVE / CONFIRMED\n");
                }
            } else if (choice == 3) {
                printf("--- RATE SYSTEM & DOCTOR ---\n");
                int dr_rate, sys_rate;
                printf("Rate Doctor %s (1-5): ", patients[idx].assigned_doctor);
                scanf("%d", &dr_rate);
                if (dr_rate < 1) dr_rate = 1; if (dr_rate > 5) dr_rate = 5;
                patients[idx].doctor_rating = dr_rate;

                printf("Rate Healwish System overall (1-5): ");
                scanf("%d", &sys_rate);
                if (sys_rate < 1) sys_rate = 1; if (sys_rate > 5) sys_rate = 5;
                patients[idx].system_rating = sys_rate;

                printf("Thank you for your rating! Feedback recorded.\n");
            }

            int action = navPrompt(&choice, 3);
            if (action == 3) break;
        }
    }
}