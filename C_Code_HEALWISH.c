#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "healthcare_data.txt"

// Data Structures based on the SRS Specification
typedef struct {
    char id[20];
    char name[50];
    char age[5];
    char disease[50];
    char doctor[50];
    char diagnosis[100];
    char treatment[100];
    char prescription[100];
} Patient;

typedef struct {
    char id[20];
    char name[50];
    char specialty[50];
} Doctor;

// Function Prototypes
void mainMenu();
void adminMenu();
void doctorMenu();
void patientMenu();

void addPatient();
void viewPatients();
void searchPatient();
void deletePatient();

void addDoctor();
void viewDoctors();

void doctorConsultation();
void viewPatientHistory();

// Helper Functions for File Handling
void savePatientToFile(Patient p);
void saveDoctorToFile(Doctor d);
int findPatientByID(const char* id, Patient* foundPatient);
void updatePatientRecord(Patient updatedPatient);

int main() {
    mainMenu();
    return 0;
}

// --- MENUS & INTERFACES ---

void mainMenu() {
    int choice;
    while(1) {
        printf("\n=============================================\n");
        printf("      HEALTHCARE MANAGEMENT SYSTEM (C)       \n");
        printf("=============================================\n");
        printf("1. Admin Portal\n");
        printf("2. Doctor Portal\n");
        printf("3. Patient Portal\n");
        printf("4. Exit System\n");
        printf("---------------------------------------------\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // Clear buffer

        switch(choice) {
            case 1: adminMenu(); break;
            case 2: doctorMenu(); break;
            case 3: patientMenu(); break;
            case 4:
                printf("\nExiting system securely. Goodbye!\n");
                exit(0);
            default: printf("\nInvalid choice! Try again.\n");
        }
    }
}

void adminMenu() {
    char username[20], password[20];
    printf("\n--- Admin Authentication ---\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    // Basic hardcoded security check as per specifications
    if (strcmp(username, "admin") != 0 || strcmp(password, "admin123") != 0) {
        printf("\nAccess Denied! Invalid credentials.\n");
        return;
    }

    int choice;
    while(1) {
        printf("\n=============================================\n");
        printf("                ADMIN DASHBOARD              \n");
        printf("=============================================\n");
        printf("1. Add New Patient Record\n");
        printf("2. View All Patient Records\n");
        printf("3. Search Patient by ID\n");
        printf("4. Delete Patient Record\n");
        printf("5. Add New Doctor Profile\n");
        printf("6. View Active Doctor List\n");
        printf("7. Return to Main Menu (Logout)\n");
        printf("---------------------------------------------\n");
        printf("Enter operational choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: searchPatient(); break;
            case 4: deletePatient(); break;
            case 5: addDoctor(); break;
            case 6: viewDoctors(); break;
            case 7: return;
            default: printf("\nInvalid assignment option.\n");
        }
    }
}

void doctorMenu() {
    char docName[50];
    printf("\n--- Doctor Access Portal ---\n");
    printf("Enter Your Last Name (e.g., 'Dr.Smith'): ");
    scanf("%s", docName);
    getchar();

    int choice;
    while(1) {
        printf("\n=============================================\n");
        printf("         MEDICAL CONSOLE: %s         \n", docName);
        printf("=============================================\n");
        printf("1. Evaluate Patient (Add Diagnosis / Prescription)\n");
        printf("2. View a Patient's Complete Medical History\n");
        printf("3. Return to Main Menu (Logout)\n");
        printf("---------------------------------------------\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: doctorConsultation(); break;
            case 2: viewPatientHistory(); break;
            case 3: return;
            default: printf("\nInvalid choice.\n");
        }
    }
}

void patientMenu() {
    char patID[20];
    Patient p;
    printf("\n--- Patient Medical Portal ---\n");
    printf("Enter Your Patient ID: ");
    scanf("%s", patID);
    getchar();

    if (findPatientByID(patID, &p)) {
        printf("\n=============================================\n");
        printf("         YOUR PERSONAL MEDICAL FILE          \n");
        printf("=============================================\n");
        printf("Patient ID Reference:  %s\n", p.id);
        printf("Full Name:             %s\n", p.name);
        printf("Age:                   %s\n", p.age);
        printf("Primary Complaint:     %s\n", p.disease);
        printf("Assigned Consultant:   %s\n", p.doctor);
        printf("---------------------------------------------\n");
        printf("Clinical Diagnosis:    %s\n", p.diagnosis);
        printf("Treatment Plan:        %s\n", p.treatment);
        printf("Active Prescriptions:  %s\n", p.prescription);
        printf("=============================================\n");
    } else {
        printf("\nNo record found tracking that Patient ID.\n");
    }
    printf("\nPress Enter to return to main menu...");
    getchar();
}

// --- CORE ENGINE & FILE LOGIC ---

void addPatient() {
    Patient p;
    printf("\n--- Create Patient Record ---\n");
    printf("Enter Patient ID: ");
    scanf("%s", p.id);
    getchar();
    printf("Enter Full Name: ");
    fgets(p.name, sizeof(p.name), stdin); p.name[strcspn(p.name, "\n")] = 0;
    printf("Enter Age: ");
    scanf("%s", p.age);
    getchar();
    printf("Enter Primary Disease/Symptom: ");
    fgets(p.disease, sizeof(p.disease), stdin); p.disease[strcspn(p.disease, "\n")] = 0;
    printf("Assign Doctor Name: ");
    fgets(p.doctor, sizeof(p.doctor), stdin); p.doctor[strcspn(p.doctor, "\n")] = 0;

    // Initialize empty treatment fields
    strcpy(p.diagnosis, "Awaiting Evaluation");
    strcpy(p.treatment, "Under Review");
    strcpy(p.prescription, "None");

    savePatientToFile(p);
    printf("\nPatient record saved successfully data stream.\n");
}

void viewPatients() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) {
        printf("\nNo patient records data file found.\n");
        return;
    }

    char line[500];
    printf("\n%-10s %-20s %-5s %-20s %-20s\n", "ID", "Name", "Age", "Disease", "Assigned Doctor");
    printf("--------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "PATIENT:", 8) == 0) {
            Patient p;
            sscanf(line, "PATIENT:%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
                   p.id, p.name, p.age, p.disease, p.doctor, p.diagnosis, p.treatment, p.prescription);
            printf("%-10s %-20s %-5s %-20s %-20s\n", p.id, p.name, p.age, p.disease, p.doctor);
        }
    }
    fclose(file);
}

void searchPatient() {
    char id[20];
    Patient p;
    printf("\nEnter Patient ID to search: ");
    scanf("%s", id);
    getchar();

    if (findPatientByID(id, &p)) {
        printf("\nPatient Found:\n");
        printf("ID: %s | Name: %s | Age: %s | Disease: %s | Doctor: %s\n", p.id, p.name, p.age, p.disease, p.doctor);
    } else {
        printf("\nPatient matching ID '%s' was not located.\n", id);
    }
}

void deletePatient() {
    char id[20];
    printf("\nEnter Patient ID to remove: ");
    scanf("%s", id);
    getchar();

    FILE *file = fopen(DATA_FILE, "r");
    if (!file) return;

    FILE *tempFile = fopen("temp.txt", "w");
    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "PATIENT:", 8) == 0) {
            Patient p;
            sscanf(line, "PATIENT:%[^|]", p.id);
            if (strcmp(p.id, id) == 0) {
                found = 1;
                continue; // Skip writing to copy file (Delete)
            }
        }
        fprintf(tempFile, "%s", line);
    }

    fclose(file);
    fclose(tempFile);

    remove(DATA_FILE);
    rename("temp.txt", DATA_FILE);

    if (found) printf("\nPatient record removed safely.\n");
    else printf("\nRecord ID not found.\n");
}

void addDoctor() {
    Doctor d;
    printf("\n--- Register Doctor Profile ---\n");
    printf("Enter Doctor ID: ");
    scanf("%s", d.id);
    getchar();
    printf("Enter Doctor Name (Prefix with Dr.): ");
    fgets(d.name, sizeof(d.name), stdin); d.name[strcspn(d.name, "\n")] = 0;
    printf("Enter Area of Specialty: ");
    fgets(d.specialty, sizeof(d.specialty), stdin); d.specialty[strcspn(d.specialty, "\n")] = 0;

    saveDoctorToFile(d);
    printf("\nDoctor profile appended to database.\n");
}

void viewDoctors() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) {
        printf("\nNo active healthcare providers registry discovered.\n");
        return;
    }

    char line[500];
    printf("\n%-10s %-20s %-20s\n", "Doc ID", "Name", "Specialty");
    printf("----------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "DOCTOR:", 7) == 0) {
            Doctor d;
            sscanf(line, "DOCTOR:%[^|]|%[^|]|%[^\n]", d.id, d.name, d.specialty);
            printf("%-10s %-20s %-20s\n", d.id, d.name, d.specialty);
        }
    }
    fclose(file);
}

void doctorConsultation() {
    char id[20];
    Patient p;
    printf("\nEnter Patient ID to consult: ");
    scanf("%s", id);
    getchar();

    if (findPatientByID(id, &p)) {
        printf("\nCurrent Chart Conditions: %s\n", p.disease);
        printf("Enter Clinical Diagnosis Updates: ");
        fgets(p.diagnosis, sizeof(p.diagnosis), stdin); p.diagnosis[strcspn(p.diagnosis, "\n")] = 0;
        printf("Enter Treatment Regimen Instructions: ");
        fgets(p.treatment, sizeof(p.treatment), stdin); p.treatment[strcspn(p.treatment, "\n")] = 0;
        printf("Issue Prescription Orders: ");
        fgets(p.prescription, sizeof(p.prescription), stdin); p.prescription[strcspn(p.prescription, "\n")] = 0;

        updatePatientRecord(p);
        printf("\nPatient treatment updates committed.\n");
    } else {
        printf("\nTarget patient not found.\n");
    }
}

void viewPatientHistory() {
    char id[20];
    Patient p;
    printf("\nEnter Patient ID: ");
    scanf("%s", id);
    getchar();

    if (findPatientByID(id, &p)) {
        printf("\n--- Historical Log: %s ---\n", p.name);
        printf("Diagnosis: %s\n", p.diagnosis);
        printf("Treatment History: %s\n", p.treatment);
        printf("Prescriptions Provided: %s\n", p.prescription);
    } else {
        printf("\nRecord missing.\n");
    }
}

// --- FILE STREAM HELPERS ---

void savePatientToFile(Patient p) {
    FILE *file = fopen(DATA_FILE, "a");
    if (file) {
        fprintf(file, "PATIENT:%s|%s|%s|%s|%s|%s|%s|%s\n",
                p.id, p.name, p.age, p.disease, p.doctor, p.diagnosis, p.treatment, p.prescription);
        fclose(file);
    }
}

void saveDoctorToFile(Doctor d) {
    FILE *file = fopen(DATA_FILE, "a");
    if (file) {
        fprintf(file, "DOCTOR:%s|%s|%s\n", d.id, d.name, d.specialty);
        fclose(file);
    }
}

int findPatientByID(const char* id, Patient* foundPatient) {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) return 0;

    char line[500];
    int success = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "PATIENT:", 8) == 0) {
            Patient p;
            sscanf(line, "PATIENT:%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
                   p.id, p.name, p.age, p.disease, p.doctor, p.diagnosis, p.treatment, p.prescription);
            if (strcmp(p.id, id) == 0) {
                *foundPatient = p;
                success = 1;
                break;
            }
        }
    }
    fclose(file);
    return success;
}

void updatePatientRecord(Patient updatedPatient) {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) return;

    FILE *tempFile = fopen("temp.txt", "w");
    char line[500];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "PATIENT:", 8) == 0) {
            Patient p;
            sscanf(line, "PATIENT:%[^|]", p.id);
            if (strcmp(p.id, updatedPatient.id) == 0) {
                fprintf(tempFile, "PATIENT:%s|%s|%s|%s|%s|%s|%s|%s\n",
                        updatedPatient.id, updatedPatient.name, updatedPatient.age,
                        updatedPatient.disease, updatedPatient.doctor, updatedPatient.diagnosis,
                        updatedPatient.treatment, updatedPatient.prescription);
                continue;
            }
        }
        fprintf(tempFile, "%s", line);
    }
    fclose(file);
    fclose(tempFile);

    remove(DATA_FILE);
    rename("temp.txt", DATA_FILE);
}