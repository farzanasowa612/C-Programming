#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"
#include "models.h"
#include "md5.h"
#include "utils.h"

/* Prototypes for functions private to this file (not in admin.h,
   so no other .c file can call these directly — only adminMenu()
   dispatches to them). */
static void addDoctorMenu(void);
static void addPatientMenu(void);
static void managePaymentsMenu(void);
static void disburseSalaryMenu(void);
static void generateReportMenu(void);

/* ==================== ADMIN DASHBOARD ==================== */
void adminMenu(void) {
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
            while (getchar() != '\n');
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

static void addDoctorMenu(void) {
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

static void addPatientMenu(void) {
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

static void managePaymentsMenu(void) {
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

static void disburseSalaryMenu(void) {
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

static void generateReportMenu(void) {
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
