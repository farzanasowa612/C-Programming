#include <stdio.h>
#include <string.h>
#include "doctor.h"
#include "models.h"
#include "utils.h"

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
            while (getchar() != '\n');
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
