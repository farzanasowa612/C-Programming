#include <stdio.h>
#include <string.h>
#include "patient.h"
#include "models.h"
#include "utils.h"

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
            while (getchar() != '\n');
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
