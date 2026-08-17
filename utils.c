#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

void clearScreen(void) {
    system(CLEAR_SCREEN);
}

/* ==================== NAVIGATION CONTROLLER ==================== */
int navPrompt(int *current_choice, int max_options) {
    printf("\n-------------------------------------------------\n");
    printf("Navigation: [1] Next Option  [2] Previous Option  [3] Home / Main Menu\n");
    printf("Enter choice: ");
    int nav_action;
    if (scanf("%d", &nav_action) != 1) {
        while (getchar() != '\n');
        return 3;
    }
    getchar();

    if (nav_action == 1) {
        (*current_choice)++;
        if (*current_choice > max_options) {
            *current_choice = 1;
        }
        return 1;
    } else if (nav_action == 2) {
        (*current_choice)--;
        if (*current_choice < 1) {
            *current_choice = max_options;
        }
        return 2;
    } else {
        return 3;
    }
}

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

    int total_minutes = (start_hour * 60) + start_min + (patient_seq_index * 5);
    int final_hour = (total_minutes / 60) % 24;
    int final_min = total_minutes % 60;

    sprintf(output_time, "%02d:%02d", final_hour, final_min);
}
