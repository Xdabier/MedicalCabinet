#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *patientFile = "patients.txt";
const char *medsFile = "meds.txt";
const char *appointmentsFile = "appointments.txt";

const char *patientFileTemp = "patientsTemp.txt";
const char *medsFileTemp = "medsTemp.txt";
const char *appointmentsFileTemp = "appointmentsTemp.txt";

struct date {
    int year;
    int month;
    int day;
};

struct patient {
    char name[50];
    char lastName[50];
    char address[150];
    int number;
    char civilStat[20];
    struct date birthDate;
};

struct appointment {
    int number;
    int state;
    struct date dateApp;
};


struct medicament {
    char name[50];
    int qtt;
    int id;
};

void managementPats(int newInfoOrNot);

void managementMeds(int info);

void managementApps(int info);

struct patient getPAtientByID(int number);


//management appointment

//the sous-menus
//sous menu pour patients

struct date getDate() {
    struct date dt;
    printf("Enter date (year month day ---- separated by a space) == \n");
    scanf("%i", &dt.year);
    scanf("%i", &dt.month);
    scanf("%i", &dt.day);
    return dt;
}

int changeStat() {
    int state;
    do {
        printf("Enter 1 for Active or 0 for Non-active = \n");
        scanf("%i", &state);
    } while(state > 1 || state < 0);

    return state;
}

void cancelApp() {
    FILE *filePointer;
    FILE *filePointerTemp;
    struct appointment appointment1, appointment2;

    int number;
    printf("Write the patient number who is related for the appointment to cancel (Non-active / Active) = \n");
    scanf("%i", &number);

    if (memberExistence(number) == 0) {
        printf("This patient does not exist! \n ________________________ \n"
               "please enter a number of a patient that exists to edit the related appointment!! :)) \n");
        printf("______________________________________ \n");
        printf("______________________________________ \n");
        printf("click on any NUMBER to get back to main-menu! \n");
        scanf("%i");
        managementApps(0);
    }


    filePointer = fopen(appointmentsFile, "r");
    filePointerTemp = fopen(appointmentsFileTemp, "a+");

    // write in the temporary file all users except the selected one
    do {
        fscanf(filePointer, "%i %i %i %i %i \n", &appointment1.number, &appointment1.state, &appointment1.dateApp.year, &appointment1.dateApp.month,
               &appointment1.dateApp.day);

        if (appointment1.number != number)
            fprintf(filePointerTemp, "%i %i %i %i %i \n", appointment1.number, appointment1.state, appointment1.dateApp.year, appointment1.dateApp.month,
                    appointment1.dateApp.day);
    } while (!feof(filePointer));

    //opening the editing menu
    do {
        fscanf(filePointer, "%i %i %i %i %i \n", &appointment1.number, &appointment1.state, &appointment1.dateApp.year, &appointment1.dateApp.month,
               &appointment1.dateApp.day);

        if (appointment1.number == number) {
            struct patient pat;
            char *state;

            if (appointment1.state == 0) state = "Non-active";
            else state = "Active";

            pat = getPAtientByID(appointment1.number);
            printf("The appointment you want to edit is ==>> \n");
            printf("Name = %s \n", pat.name);
            printf("Lastname = %s \n", pat.lastName);
            printf("Appointment state = %s, \nAppointment date= %i / %i / %i \n", state,
                   appointment1.dateApp.year, appointment1.dateApp.month, appointment1.dateApp.day);


            printf("press 1 to cancel and get back to main menu // \n");
            printf("press 2 to edit the appointment state (Active / Non-active)// \n");

            int x;
            scanf("%i", &x);
            if (x == 2) {
                appointment2 = appointment1;
                appointment2.state = changeStat();
                break;
            } else {
                managementApps(0);
            }
        }
    } while (!feof(filePointer));

    fclose(filePointer);
    fclose(filePointerTemp);

    //in case of edition and save
    filePointer = fopen(appointmentsFile, "w+");
    filePointerTemp = fopen(appointmentsFileTemp, "r");

    do {
        fscanf(filePointerTemp, "%i %i %i %i %i \n", &appointment1.number, &appointment1.state, &appointment1.dateApp.year, &appointment1.dateApp.month,
               &appointment1.dateApp.day);
        fprintf(filePointer, "%i %i %i %i %i \n", appointment1.number, appointment1.state, appointment1.dateApp.year, appointment1.dateApp.month,
                appointment1.dateApp.day);
    } while (!feof(filePointerTemp));

    fprintf(filePointer, "%i %i %i %i %i \n", appointment2.number, appointment2.state, appointment2.dateApp.year, appointment2.dateApp.month,
            appointment2.dateApp.day);

    fclose(filePointerTemp);
    fclose(filePointer);

    int ret = remove(appointmentsFileTemp);

    if (ret == 0) {
        managementApps(3);
    } else {
        printf("error = %i", ret);
    }
}

void showApps() {
    FILE *filePointer;

    filePointer = fopen(appointmentsFile, "r");

    int i = 0;

    do {
        i++;
        struct appointment appointment1;
        char *state;
        fscanf(filePointer, "%i %i %i %i %i \n", &appointment1.number, &appointment1.state, &appointment1.dateApp.year, &appointment1.dateApp.month,
               &appointment1.dateApp.day);

        if (appointment1.state == 0) state = "Non-active";
        else state = "Active";

        printf("%i -- ) \n", i);
        struct patient pat;
        pat = getPAtientByID(appointment1.number);
        printf("nom = %s \n", pat.name);
        printf("prenom = %s \n", pat.lastName);

        printf("Appointment status = %s, \nAppointment date = %i / %i / %i \n", state, appointment1.dateApp.year, appointment1.dateApp.month,
               appointment1.dateApp.day);
    } while (!feof(filePointer));

    fclose(filePointer);
    printf("______________________________________ \n");
    printf("click on any NUMBER to get back to main-menu! \n");
    scanf("%i");
    managementApps(0);
}

void addApp(int again) {
    FILE *filePointer;
    struct appointment appointment1;
    int numberPatient;

    filePointer = fopen(appointmentsFile, "a+");

    if (again == 1) {
        printf("The number of the patient you have entered does not correspond to any patient. \n"
               "_____________ \n"
               "Please enter an number that corresponds to a patient who exists in the database or CREATE to NEW patient!!  \n _____________ \n THANKS!! \n");
        printf("Choose 0 to get back to the previous Menu! \n ____________________________________ \n");
    }
    printf("Enter the patient number (Choose 0 to get back to the previous Menu) = \n");
    scanf("%i", &numberPatient);

    if (numberPatient == 0) managementApps(0);

    if (memberExistence(numberPatient) == 1) {
        appointment1.number = numberPatient;
        printf("Enter the appointment Stat (1 for Active or 0 for Non-active) = \n");
        scanf("%i", &appointment1.state);
        printf("Enter the appointment date (year month day ---- separated by a space) = \n");
        scanf("%i", &appointment1.dateApp.year);
        scanf("%i", &appointment1.dateApp.month);
        scanf("%i", &appointment1.dateApp.day);

        fprintf(filePointer, "%i %i %i %i %i \n", appointment1.number, appointment1.state, appointment1.dateApp.year, appointment1.dateApp.month,
                appointment1.dateApp.day);
        fclose(filePointer);
        managementApps(1);
    } else addApp(1);
}

void editApp() {
    FILE *filePointer;
    FILE *filePointerTemp;
    struct appointment appointment1, appointment2;

    int number;
    printf("Enter the patient number qui correspond au appointment a modifier = \n");
    scanf("%i", &number);

    if (memberExistence(number) == 0) {
        printf("This patient does not exist! \n ________________________ \n"
               "please enter a number of a patient that exists to edit to edit the related appointment!! :)) \n");
        printf("______________________________________ \n");
        printf("______________________________________ \n");
        printf("click on any NUMBER to get back to main-menu! \n");
        scanf("%i");
        managementApps(0);
    }


    filePointer = fopen(appointmentsFile, "r");
    filePointerTemp = fopen(appointmentsFileTemp, "a+");

    // write in the temporary file all users except the selected one
    do {
        fscanf(filePointer, "%i %i %i %i %i \n", &appointment1.number, &appointment1.state, &appointment1.dateApp.year, &appointment1.dateApp.month,
               &appointment1.dateApp.day);

        if (appointment1.number != number)
            fprintf(filePointerTemp, "%i %i %i %i %i \n", appointment1.number, appointment1.state, appointment1.dateApp.year, appointment1.dateApp.month,
                    appointment1.dateApp.day);
    } while (!feof(filePointer));

    //opening the editing menu
    do {
        fscanf(filePointer, "%i %i %i %i %i \n", &appointment1.number, &appointment1.state, &appointment1.dateApp.year, &appointment1.dateApp.month,
               &appointment1.dateApp.day);

        if (appointment1.number == number) {
            struct patient pat;
            char *state;

            if (appointment1.state == 0) state = "Non-active";
            else state = "Active";

            pat = getPAtientByID(appointment1.number);
            printf("The appointment you want to edit is  ==>> \n");
            printf("Nom = %s \n", pat.name);
            printf("Lastname = %s \n", pat.lastName);
            printf("Appointment status = %s, \nAppointment date = %i / %i / %i \n", state,
                   appointment1.dateApp.year, appointment1.dateApp.month, appointment1.dateApp.day);


            printf("press 1 to cancel and get back to main menu // \n ");
            printf("press 2 to edit the appointment date// \n ");

            int x;
            scanf("%i", &x);
            if (x == 2) {
                appointment2 = appointment1;
                appointment2.dateApp = getDate();
                break;
            } else {
                managementApps(0);
            }
        }
    } while (!feof(filePointer));

    fclose(filePointer);
    fclose(filePointerTemp);

    //in case of edition and save
    filePointer = fopen(appointmentsFile, "w+");
    filePointerTemp = fopen(appointmentsFileTemp, "r");

    do {
        fscanf(filePointerTemp, "%i %i %i %i %i \n", &appointment1.number, &appointment1.state, &appointment1.dateApp.year, &appointment1.dateApp.month,
               &appointment1.dateApp.day);
        fprintf(filePointer, "%i %i %i %i %i \n", appointment1.number, appointment1.state, appointment1.dateApp.year, appointment1.dateApp.month,
                appointment1.dateApp.day);
    } while (!feof(filePointerTemp));

    fprintf(filePointer, "%i %i %i %i %i \n", appointment2.number, appointment2.state, appointment2.dateApp.year, appointment2.dateApp.month,
            appointment2.dateApp.day);

    fclose(filePointerTemp);
    fclose(filePointer);

    int ret = remove(appointmentsFileTemp);

    if (ret == 0) {
        managementApps(3);
    } else {
        printf("error = %i", ret);
    }

}


//management Medss
void showMeds() {
    FILE *filePointer;

    filePointer = fopen(medsFile, "r");

    int i = 0;

    do {
        i++;
        struct medicament medicament1;
        fscanf(filePointer, "%s %i %i \n", &medicament1.name, &medicament1.qtt, &medicament1.id);
        printf("%i -- ) \n name = %s, \n quantity = %i, \n ID = %i \n",
               i, medicament1.name, medicament1.qtt, medicament1.id);
    } while (!feof(filePointer));

    fclose(filePointer);
    printf("______________________________________ \n");
    printf("click on any NUMBER to get back to main-menu! \n");
    scanf("%i");
    managementMeds(0);
}

void addMed() {
    FILE *filePointer;
    struct medicament medicament1;

    filePointer = fopen(medsFile, "a+");

    printf("Enter medicament name = \n");
    scanf("%s", &medicament1.name);
    printf("Enter medicament quantity = \n");
    scanf("%i", &medicament1.qtt);
    printf("Enter medicament ID = \n");
    scanf("%i", &medicament1.id);

    fprintf(filePointer, "%s %i %i \n", medicament1.name, medicament1.qtt, medicament1.id);
    fclose(filePointer);
    managementMeds(1);
}

void supprimMed() {
    FILE *filePointer;
    FILE *filePointerTemp;
    struct medicament medicament1;


    filePointer = fopen(medsFile, "r");
    filePointerTemp = fopen(medsFileTemp, "a+");

    int number;
    printf("Enter the patient number to delete = \n");
    scanf("%i", &number);


    do {
        fscanf(filePointer, "%s %i %i \n", &medicament1.name, &medicament1.qtt, &medicament1.id);

        if (medicament1.id != number)
            fprintf(filePointerTemp, "%s %i %i \n", medicament1.name, medicament1.qtt, medicament1.id);
    } while (!feof(filePointer));

    fclose(filePointer);
    fclose(filePointerTemp);

    filePointer = fopen(medsFile, "w+");
    filePointerTemp = fopen(medsFileTemp, "r");

    do {
        fscanf(filePointerTemp, "%s %i %i \n", &medicament1.name, &medicament1.qtt, &medicament1.id);
        fprintf(filePointer, "%s %i %i \n", medicament1.name, medicament1.qtt, medicament1.id);
    } while (!feof(filePointerTemp));

    fclose(filePointerTemp);
    fclose(filePointer);

    int ret = remove(medsFileTemp);

    if (ret == 0) {
        managementMeds(2);
    } else {
        printf("error = %i", ret);
    }

}

void searchMed() {
    FILE *filePointer;

    filePointer = fopen(medsFile, "r");

    int i = 0;
    int foad = 0;
    int number;
    printf("Enter the medicament ID you want to look for = \n");
    scanf("%i", &number);

    do {
        i++;
        struct medicament medicament1;
        fscanf(filePointer, "%s %i %i \n", &medicament1.name, &medicament1.qtt, &medicament1.id);
        if (medicament1.id == number) {
            printf("%i -- ) \n name = %s, \n quantity = %i, \n id = %i \n",
                   i, medicament1.name, medicament1.qtt, medicament1.id);
            foad = 1;
        }
    } while (!feof(filePointer));

    if (foad == 0)
        printf("The number of the medicament you have entered does not correspond to any medicament. \n"
               "______________ \n"
               "please try again with an other number! \n");

    fclose(filePointer);
    printf("______________________________________ \n");
    printf("______________________________________ \n");
    printf("click on any NUMBER to get back to main-menu! \n");
    scanf("%i");
    managementMeds(0);
}

//management Patientsss

struct patient getPAtientByID(int number) {
    FILE *filePointer;

    filePointer = fopen(patientFile, "r");

    do {
        struct patient patient1;
        fscanf(filePointer, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);
        if (patient1.number == number) {
            return patient1;
        }
    } while (!feof(filePointer));

    fclose(filePointer);
}

int memberExistence(int number) {
    FILE *filePointer;

    filePointer = fopen(patientFile, "r");

    int foad = 0;

    do {
        struct patient patient1;
        fscanf(filePointer, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);
        if (patient1.number == number) {
            foad = 1;
            fclose(filePointer);
            return foad;
        }
    } while (!feof(filePointer));

    fclose(filePointer);

    return foad;
}

void showPats() {
    FILE *filePointer;

    filePointer = fopen(patientFile, "r");

    int i = 0;

    do {
        i++;
        struct patient patient1;
        fscanf(filePointer, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);
        printf("%i -- ) \n name = %s, \n last name = %s, \n address = %s, \n state civil = %s, \n number = %i, \n birthday = %i / %i / %i \n",
               i, patient1.name, patient1.lastName, patient1.address, patient1.civilStat, patient1.number,
               patient1.birthDate.year, patient1.birthDate.month, patient1.birthDate.day);
    } while (!feof(filePointer));

    fclose(filePointer);
    printf("______________________________________ \n");
    printf("click on any NUMBER to get back to main-menu! \n");
    scanf("%i");
    managementPats(0);

}

struct patient getNewPatsInfo() {
    struct patient patient1;

    printf("Patient name = \n");
    scanf("%s", &patient1.name);
    printf("Patient Last-name = \n");
    scanf("%s", &patient1.lastName);
    printf("Patient address = \n");
    scanf("%s", &patient1.address);
    printf("Patient civil-state = \n");
    scanf("%s", &patient1.civilStat);
    printf("Patient number = \n");
    scanf("%i", &patient1.number);
    printf("Patient birthday (year month day --- separated by a space) = \n");
    scanf("%i", &patient1.birthDate.year);
    scanf("%i", &patient1.birthDate.month);
    scanf("%i", &patient1.birthDate.day);

    return patient1;
}

void addPats() {
    FILE *filePointer;
    struct patient patient1;

    filePointer = fopen(patientFile, "a+");

    patient1 = getNewPatsInfo();

    fprintf(filePointer, "%s %s %s %s %i %i %i %i \n", patient1.name, patient1.lastName, patient1.address,
            patient1.civilStat, patient1.number, patient1.birthDate.year, patient1.birthDate.month,
            patient1.birthDate.day);
    fclose(filePointer);
    managementPats(1);
}

void supprimPats() {
    FILE *filePointer;
    FILE *filePointerTemp;
    struct patient patient1;


    filePointer = fopen(patientFile, "r");
    filePointerTemp = fopen(patientFileTemp, "a+");

    int number;
    printf("Enter the patient number to delete = \n");
    scanf("%i", &number);


    do {
        fscanf(filePointer, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);

        if (patient1.number != number)
            fprintf(filePointerTemp, "%s %s %s %s %i %i %i %i \n", patient1.name, patient1.lastName, patient1.address,
                    patient1.civilStat, patient1.number, patient1.birthDate.year, patient1.birthDate.month,
                    patient1.birthDate.day);
    } while (!feof(filePointer));

    fclose(filePointer);
    fclose(filePointerTemp);

    filePointer = fopen(patientFile, "w+");
    filePointerTemp = fopen(patientFileTemp, "r");

    do {
        fscanf(filePointerTemp, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);
        fprintf(filePointer, "%s %s %s %s %i %i %i %i \n", patient1.name, patient1.lastName, patient1.address,
                patient1.civilStat, patient1.number, patient1.birthDate.year, patient1.birthDate.month,
                patient1.birthDate.day);
    } while (!feof(filePointerTemp));

    fclose(filePointerTemp);
    fclose(filePointer);

    int ret = remove(patientFileTemp);

    if (ret == 0) {
        managementPats(2);
    } else {
        printf("error = %i", ret);
    }

}

void searchPats() {
    FILE *filePointer;

    filePointer = fopen(patientFile, "r");

    int i = 0;
    int foad = 0;
    int number;
    printf("Enter the patient number to look for = \n");
    scanf("%i", &number);

    do {
        i++;
        struct patient patient1;
        fscanf(filePointer, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);
        if (patient1.number == number) {
            printf("%i -- ) \n name = %s, \n last name = %s, \n address = %s, \n state civil = %s, \n number = %i, \n birthday = %i / %i / %i \n",
                   i, patient1.name, patient1.lastName, patient1.address, patient1.civilStat, patient1.number,
                   patient1.birthDate.year, patient1.birthDate.month, patient1.birthDate.day);
            foad = 1;
        }
    } while (!feof(filePointer));

    if (foad == 0)
        printf("The number of the patient you have entered does not correspond to any patient.! \n"
               "______________ \n"
               "please try again with an other number! \n");

    fclose(filePointer);
    printf("______________________________________ \n");
    printf("______________________________________ \n");
    printf("click on any NUMBER to get back to main-menu! \n");
    scanf("%i");
    managementPats(0);
}

void editPats() {
    FILE *filePointer;
    FILE *filePointerTemp;
    struct patient patient1, patient2;

    int number;
    printf("Enter the patient number to edit = \n");
    scanf("%i", &number);

    if (memberExistence(number) == 0) {
        printf("This patient does not exist! \n"
               "________________________ \n"
               "please enter a number of a patient that exists to edit!! :)) \n");
        printf("______________________________________ \n");
        printf("______________________________________ \n");
        printf("click on any NUMBER to get back to main-menu! \n");
        scanf("%i");
        managementPats(0);
    }


    filePointer = fopen(patientFile, "r");
    filePointerTemp = fopen(patientFileTemp, "a+");

    // write in the temporary file all users except the selected one
    do {
        fscanf(filePointer, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);

        if (patient1.number != number)
            fprintf(filePointerTemp, "%s %s %s %s %i %i %i %i \n", patient1.name, patient1.lastName, patient1.address,
                    patient1.civilStat, patient1.number, patient1.birthDate.year, patient1.birthDate.month,
                    patient1.birthDate.day);
    } while (!feof(filePointer));

    //opening the editing menu
    do {
        fscanf(filePointer, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);

        if (patient1.number == number) {
            printf("The patient you want to edit is  ==>> \n"
                   "1 ) name = %s, \n"
                   "2 ) last name = %s, \n"
                   "3 ) address = %s, \n"
                   "4 )state civil = %s, \n"
                   "5 ) number = %i, \n"
                   "6 )birthday = %i / %i / %i \n",
                   patient1.name, patient1.lastName, patient1.address, patient1.civilStat, patient1.number,
                   patient1.birthDate.year, patient1.birthDate.month, patient1.birthDate.day);

            printf("press 1 to cancel and get back to main menu // \n ");
            printf("press 2 to edit // \n ");

            int x;
            scanf("%i", &x);
            if (x == 2) {
                patient2 = getNewPatsInfo();
            } else {
                managementPats(0);
            }
        }
    } while (!feof(filePointer));

    fclose(filePointer);
    fclose(filePointerTemp);

    //in case of edition and save
    filePointer = fopen(patientFile, "w+");
    filePointerTemp = fopen(patientFileTemp, "r");

    do {
        fscanf(filePointerTemp, "%s %s %s %s %i %i %i %i \n", &patient1.name, &patient1.lastName, &patient1.address,
               &patient1.civilStat, &patient1.number, &patient1.birthDate.year, &patient1.birthDate.month,
               &patient1.birthDate.day);
        fprintf(filePointer, "%s %s %s %s %i %i %i %i \n", patient1.name, patient1.lastName, patient1.address,
                patient1.civilStat, patient1.number, patient1.birthDate.year, patient1.birthDate.month,
                patient1.birthDate.day);
    } while (!feof(filePointerTemp));

    fprintf(filePointer, "%s %s %s %s %i %i %i %i \n", patient2.name, patient2.lastName, patient2.address,
            patient2.civilStat, patient2.number, patient2.birthDate.year, patient2.birthDate.month,
            patient2.birthDate.day);

    fclose(filePointerTemp);
    fclose(filePointer);

    int ret = remove(patientFileTemp);

    if (ret == 0) {
        managementPats(3);
    } else {
        printf("error = %i", ret);
    }

}

void principalMenu() {
    int choice;
    system("clear");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("************MANAGEMENT OF A MEDICAL CABINET*************\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("*******press 1 to get to: Patient management.***********\n");
    printf("*******press 2 to get to: Drug Management.**************\n");
    printf("*******press 3 to get to: Appointments Management.******\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("***********PRESS 0 TO EXIT!*****************************\n");
    printf("********************************************************\n");
    printf("********************************************************\n");

    scanf("%i", &choice);

    if ((choice < 0) || (choice > 3)) {
        printf("Wrong choice! please choose again. \n");
        principalMenu();
    } else {
        if (choice == 1) {
            managementPats(0);
        } else if (choice == 2) managementMeds(0);
        else if (choice == 3) managementApps(0);
        else if (choice == 0) exit(0);
    }
}

//the sous-menus
//sous menu pour patients
void managementPats(int newInfoOrNot) {
    int choice;
    system("clear");
    if (newInfoOrNot == 1)
    printf("************PATIENT ADDED SUCCESSFULLY!!******************\n");
    if (newInfoOrNot == 2)
    printf("************PATIENT DELETED SUCCESSFULLY!!****************\n");
    if (newInfoOrNot == 3)
    printf("************PATIENT EDITED SUCCESSFULLY!!*****************\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("************MANAGEMENT OF A MEDICAL CABINET***************\n");
    printf("********************PATIENT MANAGEMENT********************\n");
    printf("**********************************************************\n");
    printf("*******press 1 to get to: Show the patients.**************\n");
    printf("*******press 2 to get to: Add a patient.******************\n");
    printf("*******press 3 to get to: Delete a patient.***************\n");
    printf("*******press 4 to get to: Search a patient.***************\n");
    printf("*******press 5 to get to: Edit a patient.*****************\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("*********press 6 to get back to Principal Menu************\n");
    printf("**********************************************************\n");
    printf("*********************************************************\n");

    scanf("%i", &choice);

    if ((choice < 1) || (choice > 6)) {
        printf("Wrong choice! please choose again. \n");
        managementPats(0);
    } else {
        if (choice == 1) showPats();
        else if (choice == 2) addPats();
        else if (choice == 3) supprimPats();
        else if (choice == 4) searchPats();
        else if (choice == 5) editPats();
        else if (choice == 6) principalMenu();
    }
}

//sous menu pour meds

void managementMeds(int info) {
    int choice;
    system("clear");
    if (info == 1)
        printf("**********MEDICAMENT CREATED!!**************************\n");
    if (info == 2)
        printf("**********MEDICAMENT DELETED!!**************************\n");
    printf("********************************************************\n");
    printf("************MANAGEMENT OF A MEDICAL CABINET****************\n");
    printf("****************GESTION MEDICAMENTS*********************\n");
    printf("********************************************************\n");
    printf("*******press 1 to get to: Show the Medicaments.*****\n");
    printf("*******press 2 to get to: Add a Medicament.********\n");
    printf("*******press 3 to get to: Delete a Medicament.*******\n");
    printf("*******press 4 to get to: Search a Medicament.*****\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("*********press 5 to get back to Principal Menu*********\n");
    printf("********************************************************\n");
    printf("********************************************************\n");

    scanf("%i", &choice);

    if ((choice < 1) || (choice > 5)) {
        printf("Wrong choice! please choose again. \n");
        managementMeds(0);
    } else {
        if (choice == 1) showMeds();
        else if (choice == 2) addMed();
        else if (choice == 3) supprimMed();
        else if (choice == 4) searchMed();
        else if (choice == 5) principalMenu();
    }
}

//sous menu pour appointments

void managementApps(int info) {
    int choice;
    system("clear");
    if (info == 1)
        printf("********APPOINTMENT CREATED!! ************************************\n");
    if (info == 3)
        printf("********APPOINTMENT DATE EDITED*******************************\n");
    if (info == 2)
        printf("************APPOINTMENT STATUS EDITED***************************\n");
    printf("********************************************************\n");
    printf("************MANAGEMENT OF A MEDICAL CABINET****************\n");
    printf("*******************APPOINTMENTS MANAGEMENT*************************\n");
    printf("********************************************************\n");
    printf("*******press 1 to get to: Show the appointments.************\n");
    printf("*******press 2 to get to: Add a appointment.***************\n");
    printf("*******press 3 to get to: Edit a appointment.**************\n");
    printf("*******press 4 to get to: Annulee a appointment.**************\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("*********press 5 to get back to Principal Menu*********\n");
    printf("********************************************************\n");
    printf("********************************************************\n");

    scanf("%i", &choice);

    if ((choice < 1) || (choice > 5)) {
        printf("Wrong choice! please choose again. \n");
        managementApps(0);
    } else {
        if (choice == 1) showApps();
        else if (choice == 2) addApp(0);
        else if (choice == 3) editApp();
        else if (choice == 4) cancelApp();
        else if (choice == 5) principalMenu();
    }
}

//Menu pricipale


void main() {

    principalMenu();

}
