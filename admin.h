#ifndef ADMIN_H
#define ADMIN_H

/* main.c only ever needs to call adminMenu() — everything else
   (addDoctorMenu, addPatientMenu, managePaymentsMenu, ...) is an
   internal implementation detail of admin.c. */
void adminMenu(void);

#endif
