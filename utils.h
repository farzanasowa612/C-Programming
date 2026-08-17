#ifndef UTILS_H
#define UTILS_H

void clearScreen(void);
int navPrompt(int *current_choice, int max_options);
void calculateAppointmentTime(const char* slot_range, int patient_seq_index, char* output_time);

#endif
