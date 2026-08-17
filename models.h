#ifndef MODELS_H
#define MODELS_H

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

/* These arrays live in main.c (that's where they're actually created).
   "extern" here just tells every other .c file: "this variable exists
   somewhere else, the linker will find it — don't create a new one." */
extern Patient* patients;
extern Doctor* doctors;
extern int patient_count;
extern int doctor_count;

#endif
