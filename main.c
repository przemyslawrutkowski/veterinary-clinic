#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"

#define BUFFER_SIZE 256

typedef struct Owner
{
    int id;
    char firstName[BUFFER_SIZE];
    char lastName[BUFFER_SIZE];
    char phoneNumber[BUFFER_SIZE];
} Owner;

typedef struct Animal
{
    int id;
    struct Owner owner;
    char name[BUFFER_SIZE];
    char species[BUFFER_SIZE];
    char age[BUFFER_SIZE];
    char gender[BUFFER_SIZE];
    char weight[BUFFER_SIZE];
} Animal;

typedef struct Date
{
    char day[3];
    char month[10];
    char year[5];
} Date;

typedef struct Appointment
{
    int id;
    struct Animal animal;
    struct Date date;
    char reason[BUFFER_SIZE];
    char prescription[BUFFER_SIZE];
} Appointment;

typedef struct Node
{
    void *value;
    struct Node *next;
    struct Node *prev;
    size_t dataSize;
} Node;

typedef struct List
{
    struct Node *head;
    struct Node *tail;
    int length;
} List;

typedef enum
{
    APPOINTMENT,
    ANIMAL,
    OWNER
} DataType;

typedef enum
{
    COMPLETED,
    UPCOMING
} AppointmentType;

int compareIds(void *data, int id, DataType type)
{
    int dataId;

    switch (type)
    {
    case APPOINTMENT:
        dataId = ((Appointment *)data)->id;
        break;
    case ANIMAL:
        dataId = ((Animal *)data)->id;
        break;
    case OWNER:
        dataId = ((Owner *)data)->id;
        break;
    }

    return dataId - id;
}

int monthToNumber(char *month)
{
    char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"};
    int numMonths = sizeof(months) / sizeof(months[0]);

    for (int i = 0; i < numMonths; ++i)
    {
        if (strcmp(month, months[i]) == 0)
            return i;
    }
    return -1;
}

int compareDates(Date dateA, Date dateB)
{
    struct tm tmA = {0};
    struct tm tmB = {0};

    tmA.tm_mday = atoi(dateA.day);
    tmA.tm_mon = monthToNumber(dateA.month);
    tmA.tm_year = atoi(dateA.year) - 1900;

    tmB.tm_mday = atoi(dateB.day);
    tmB.tm_mon = monthToNumber(dateB.month);
    tmB.tm_year = atoi(dateB.year) - 1900;

    time_t timeA = mktime(&tmA);
    time_t timeB = mktime(&tmB);

    if (timeA < timeB)
        return -1;
    else if (timeA > timeB)
        return 1;
    else
        return 0;
}

Node *createNode(void *data, size_t dataSize)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation for newNode failed.\n");
        return NULL;
    }
    newNode->value = malloc(dataSize);
    if (newNode->value == NULL)
    {
        printf("Memory allocation for newNode->value failed.\n");
        free(newNode);
        return NULL;
    }

    memcpy(newNode->value, data, dataSize);
    newNode->dataSize = dataSize;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void pushNode(List *list, Node *node)
{
    if (list->length == 0)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    ++list->length;
}

int removeNode(List *list, int id, DataType type)
{
    if (list->length == 0)
    {
        return 0;
    }

    Node *curr = list->head;

    while (curr)
    {
        if (compareIds(curr->value, id, type) == 0)
        {
            if (list->length == 1)
            {
                list->head = NULL;
                list->tail = NULL;
            }
            else
            {
                if (!curr->prev && curr->next)
                {
                    curr->next->prev = NULL;
                    list->head = curr->next;
                    curr->next = NULL;
                }
                else if (curr->prev && !curr->next)
                {
                    curr->prev->next = NULL;
                    list->tail = curr->prev;
                    curr->prev = NULL;
                }
                else
                {
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                    curr->prev = NULL;
                    curr->next = NULL;
                }
            }
            free(curr->value);
            free(curr);
            --list->length;
            return 0;
        }
        curr = curr->next;
    }
    return -1;
}

void *getTail(List *list, DataType type)
{
    if (list->length == 0)
        return NULL;

    switch (type)
    {
    case APPOINTMENT:
        return (Appointment *)list->tail->value;
    case ANIMAL:
        return (Animal *)list->tail->value;
    case OWNER:
        return (Owner *)list->tail->value;
    }
}

int loadAppointments(List *list, AppointmentType type)
{
    FILE *file = fopen("data.json", "r");
    if (!file)
    {
        printf("Data has not been loaded properly.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    if (!data)
    {
        printf("Memory allocation for appointments data failed.\n");
        fclose(file);
        return 1;
    }
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    cJSON *parsedJson = cJSON_Parse(data);
    if (!parsedJson)
    {
        printf("Error parsing JSON\n");
        free(data);
        return 1;
    }

    char *appointmentType;
    if (type == COMPLETED)
    {
        appointmentType = "completedAppointments";
    }
    else
    {
        appointmentType = "upcomingAppointments";
    }

    cJSON *appointmentsArray = cJSON_GetObjectItem(parsedJson, appointmentType);
    if (!cJSON_IsArray(appointmentsArray))
    {
        printf("Error: Appointments collection is not an array.\n");
        cJSON_Delete(parsedJson);
        free(data);
        return 1;
    }

    cJSON *appointmentObj = NULL;
    cJSON_ArrayForEach(appointmentObj, appointmentsArray)
    {
        cJSON *id = cJSON_GetObjectItem(appointmentObj, "id");
        cJSON *animalObj = cJSON_GetObjectItem(appointmentObj, "animal");
        cJSON *ownerObj = cJSON_GetObjectItem(animalObj, "owner");
        cJSON *dateObj = cJSON_GetObjectItem(appointmentObj, "date");
        cJSON *reason = cJSON_GetObjectItem(appointmentObj, "reason");
        cJSON *prescription = cJSON_GetObjectItem(appointmentObj, "prescription");

        Appointment *appointment = malloc(sizeof(Appointment));
        if (!appointment)
        {
            printf("Memory allocation for appointment failed.\n");
            cJSON_Delete(parsedJson);
            free(data);
            return 1;
        }
        appointment->id = id->valueint;

        appointment->animal.id = cJSON_GetObjectItem(animalObj, "id")->valueint;
        appointment->animal.owner.id = cJSON_GetObjectItem(ownerObj, "id")->valueint;
        strncpy(appointment->animal.owner.firstName, cJSON_GetObjectItem(ownerObj, "firstName")->valuestring, BUFFER_SIZE);
        strncpy(appointment->animal.owner.lastName, cJSON_GetObjectItem(ownerObj, "lastName")->valuestring, BUFFER_SIZE);
        strncpy(appointment->animal.owner.phoneNumber, cJSON_GetObjectItem(ownerObj, "phoneNumber")->valuestring, BUFFER_SIZE);
        strncpy(appointment->animal.name, cJSON_GetObjectItem(animalObj, "name")->valuestring, BUFFER_SIZE);
        strncpy(appointment->animal.species, cJSON_GetObjectItem(animalObj, "species")->valuestring, BUFFER_SIZE);
        strncpy(appointment->animal.age, cJSON_GetObjectItem(animalObj, "age")->valuestring, BUFFER_SIZE);
        strncpy(appointment->animal.gender, cJSON_GetObjectItem(animalObj, "gender")->valuestring, BUFFER_SIZE);
        strncpy(appointment->animal.weight, cJSON_GetObjectItem(animalObj, "weight")->valuestring, BUFFER_SIZE);

        strncpy(appointment->date.day, cJSON_GetObjectItem(dateObj, "day")->valuestring, 3);
        strncpy(appointment->date.month, cJSON_GetObjectItem(dateObj, "month")->valuestring, 10);
        strncpy(appointment->date.year, cJSON_GetObjectItem(dateObj, "year")->valuestring, 5);

        strncpy(appointment->reason, reason->valuestring, BUFFER_SIZE);
        strncpy(appointment->prescription, prescription->valuestring, BUFFER_SIZE);

        Node *node = createNode(appointment, sizeof(Appointment));
        if (!node)
        {
            free(appointment);
            cJSON_Delete(parsedJson);
            free(data);
            return 1;
        }
        pushNode(list, node);

        free(appointment);
    }

    cJSON_Delete(parsedJson);
    free(data);
    return 0;
}

int loadAnimals(List *list)
{
    FILE *file = fopen("data.json", "r");
    if (!file)
    {
        printf("Data has not been loaded properly.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    if (!data)
    {
        printf("Memory allocation for animals data failed.\n");
        fclose(file);
        return 1;
    }
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    cJSON *parsedJson = cJSON_Parse(data);
    if (!parsedJson)
    {
        printf("Error parsing JSON\n");
        free(data);
        return 1;
    }

    cJSON *animalsArray = cJSON_GetObjectItem(parsedJson, "animals");
    if (!cJSON_IsArray(animalsArray))
    {
        printf("Error: Animals collection is not an array.\n");
        cJSON_Delete(parsedJson);
        free(data);
        return 1;
    }

    cJSON *animalObj = NULL;
    cJSON_ArrayForEach(animalObj, animalsArray)
    {
        cJSON *id = cJSON_GetObjectItem(animalObj, "id");
        cJSON *ownerObj = cJSON_GetObjectItem(animalObj, "owner");
        cJSON *name = cJSON_GetObjectItem(animalObj, "date");
        cJSON *species = cJSON_GetObjectItem(animalObj, "reason");
        cJSON *age = cJSON_GetObjectItem(animalObj, "prescription");
        cJSON *gender = cJSON_GetObjectItem(animalObj, "prescription");
        cJSON *weight = cJSON_GetObjectItem(animalObj, "prescription");

        Animal *animal = malloc(sizeof(Animal));
        if (!animal)
        {
            printf("Memory allocation for animal failed.\n");
            cJSON_Delete(parsedJson);
            free(data);
            return 1;
        }
        animal->id = id->valueint;

        animal->owner.id = cJSON_GetObjectItem(ownerObj, "id")->valueint;
        strncpy(animal->owner.firstName, cJSON_GetObjectItem(ownerObj, "firstName")->valuestring, BUFFER_SIZE);
        strncpy(animal->owner.lastName, cJSON_GetObjectItem(ownerObj, "lastName")->valuestring, BUFFER_SIZE);
        strncpy(animal->owner.phoneNumber, cJSON_GetObjectItem(ownerObj, "phoneNumber")->valuestring, BUFFER_SIZE);

        strncpy(animal->name, cJSON_GetObjectItem(animalObj, "name")->valuestring, BUFFER_SIZE);
        strncpy(animal->species, cJSON_GetObjectItem(animalObj, "species")->valuestring, BUFFER_SIZE);
        strncpy(animal->age, cJSON_GetObjectItem(animalObj, "age")->valuestring, BUFFER_SIZE);
        strncpy(animal->gender, cJSON_GetObjectItem(animalObj, "gender")->valuestring, BUFFER_SIZE);
        strncpy(animal->weight, cJSON_GetObjectItem(animalObj, "weight")->valuestring, BUFFER_SIZE);

        Node *node = createNode(animal, sizeof(Animal));
        if (!node)
        {
            free(animal);
            cJSON_Delete(parsedJson);
            free(data);
            return 1;
        }
        pushNode(list, node);

        free(animal);
    }

    cJSON_Delete(parsedJson);
    free(data);
    return 0;
}

void freeList(List *list)
{
    Node *current = list->head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

char *readFromStdin()
{
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        size_t length = strlen(buffer);
        buffer[length - 1] = '\0'; // getting rid of \n character

        char *result = malloc((length + 1) * sizeof(char)); // strlen does not count \0, so we have to add 1 to the size
        if (!result)
        {
            printf("Memory allocation for stdin failed.\n");
            return NULL;
        }

        strcpy(result, buffer);
        return result;
    }
    return NULL;
}

int getLatestId(List *list, DataType type)
{
    void *tail = getTail(list, type);
    if (!tail)
        return 1;

    switch (type)
    {
    case APPOINTMENT:
        return ((Appointment *)tail)->id + 1;
    case ANIMAL:
        return ((Animal *)tail)->id + 1;
    case OWNER:
        return ((Owner *)tail)->id + 1;
    }
}

void *getValueById(List *list, int id, DataType type)
{
    Node *el = list->head;
    while (el)
    {
        if (compareIds(el->value, id, ANIMAL) == 0)
        {
            switch (type)
            {
            case APPOINTMENT:
                return (Appointment *)el->value;
            case ANIMAL:
                return (Animal *)el->value;
            case OWNER:
                return (Owner *)el->value;
            }
        }

        el = el->next;
    }
    return NULL;
}

void **convertLinkedListToArray(List *list, DataType type)
{
    int length = list->length;
    void **array = malloc(length * sizeof(void *));
    if (!array)
    {
        printf("Memory allocation for the array of elements of the linked list failed.\n");
        return NULL;
    }
    int idx = 0;

    Node *el = list->head;
    while (el)
    {
        switch (type)
        {
        case APPOINTMENT:
            array[idx] = (Appointment *)el->value;
            break;
        case ANIMAL:
            array[idx] = (Animal *)el->value;
            break;
        case OWNER:
            array[idx] = (Owner *)el->value;
            break;
        }
        ++idx;
        el = el->next;
    }

    return array;
}

void displayAppointment(Appointment *appointment)
{
    printf("-----------------------------------------\n");
    printf("%d\n", appointment->id);
    printf("%s %s\n", appointment->animal.owner.firstName, appointment->animal.owner.lastName);
    printf("%s\n", appointment->animal.name);
    printf("%s/%s/%s\n", appointment->date.day, appointment->date.month, appointment->date.year);
    printf("%s\n", appointment->reason);
    printf("%s\n", appointment->prescription);
    printf("-----------------------------------------\n");
}

void displayAnimal(Animal *animal)
{
    printf("-----------------------------------------\n");
    printf("%d\n", animal->id);
    printf("%s\n", animal->name);
    printf("%s %s\n", animal->owner.firstName, animal->owner.lastName);
    printf("-----------------------------------------\n");
}

void displayAppointments(List *list)
{
    Node *el = list->head;
    while (el)
    {
        Appointment *appointment = (Appointment *)el->value;
        displayAppointment(appointment);
        el = el->next;
    }
}

void displayAnimals(List *list)
{
    Node *el = list->head;
    while (el)
    {
        Animal *animal = (Animal *)el->value;
        displayAnimal(animal);
        el = el->next;
    }
}

void displayAnimalsByLastName(List *list)
{
    printf("Enter the last name:\n");
    char *lastNameStr = readFromStdin();
    if (!lastNameStr)
        return;

    Node *el = list->head;
    while (el)
    {
        Animal *animal = (Animal *)el->value;
        if (strcmp(animal->owner.lastName, lastNameStr) == 0)
        {
            displayAnimal(animal);
        }
        el = el->next;
    }

    free(lastNameStr);
}

void displayAnimalsSortedByLastName(List *list)
{
    Animal **animalsArr = (Animal **)convertLinkedListToArray(list, ANIMAL);
    if (!animalsArr)
        return;

    int length = list->length;

    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < length - i; ++j)
        {
            if (j + 1 <= length - 1 && strcmp(animalsArr[j]->owner.lastName, animalsArr[j + 1]->owner.lastName) > 0)
            {
                Animal *temp = animalsArr[j];
                animalsArr[j] = animalsArr[j + 1];
                animalsArr[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < length; ++i)
    {
        displayAnimal(animalsArr[i]);
    }

    free(animalsArr);
}

void displayAppointmentsSortedByDate(List *list)
{
    Appointment **appointmentsArr = (Appointment **)convertLinkedListToArray(list, APPOINTMENT);
    if (!appointmentsArr)
        return;
    int length = list->length;

    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < length - i; ++j)
        {
            if (j + 1 <= length - 1 && compareDates(appointmentsArr[j]->date, appointmentsArr[j + 1]->date) > 0)
            {
                Appointment *temp = appointmentsArr[j];
                appointmentsArr[j] = appointmentsArr[j + 1];
                appointmentsArr[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < length; ++i)
    {
        displayAppointment(appointmentsArr[i]);
    }

    free(appointmentsArr);
}

void displayAppointmentsByDate(List *list)
{
    printf("Displaying appointments of the day...\n\n");

    printf("Enter the day:\n");
    char *dayStr = readFromStdin();
    if (!dayStr)
        return;

    printf("Enter the month:\n");
    char *monthStr = readFromStdin();
    if (!monthStr)
        return;

    printf("Enter the year:\n");
    char *yearStr = readFromStdin();
    if (!yearStr)
        return;

    Date date;
    strncpy(date.day, dayStr, sizeof(date.day) - 1);
    date.day[sizeof(date.day) - 1] = '\0';

    strncpy(date.month, monthStr, sizeof(date.month) - 1);
    date.month[sizeof(date.month) - 1] = '\0';

    strncpy(date.year, yearStr, sizeof(date.year) - 1);
    date.year[sizeof(date.year) - 1] = '\0';

    Node *el = list->head;
    while (el)
    {
        Appointment *appointment = (Appointment *)el->value;
        if (
            strcmp(appointment->date.day, date.day) == 0 &&
            strcmp(appointment->date.month, date.month) == 0 &&
            strcmp(appointment->date.year, date.year) == 0)
        {
            displayAppointment(appointment);
        }
        el = el->next;
    }
    free(dayStr);
    free(monthStr);
    free(yearStr);
}

void scheduleAppointment(List *appointments, List *animals)
{
    Appointment appointment;
    printf("Scheduling a new appointment...\n");

    printf("Enter the animal ID:\n");
    char *animalIdStr = readFromStdin();
    if (!animalIdStr)
        return;
    int id = atoi(animalIdStr);
    if (id == 0 && animalIdStr[0] != '0') // user entered value that cannot be converted to int
    {
        printf("Invalid animal ID.\n");
        free(animalIdStr);
        return;
    }

    Animal *animal = (Animal *)getValueById(animals, id, ANIMAL);
    if (!animal)
    {
        printf("There is no animal with the given ID.\n");
        return;
    }

    appointment.id = getLatestId(appointments, APPOINTMENT);

    appointment.animal = *animal;

    printf("Enter the day:\n");
    char *dayStr = readFromStdin();
    if (!dayStr)
        return;

    printf("Enter the month:\n");
    char *monthStr = readFromStdin();
    if (!monthStr)
        return;

    printf("Enter the year:\n");
    char *yearStr = readFromStdin();
    if (!yearStr)
        return;

    Date date;
    strncpy(date.day, dayStr, sizeof(date.day) - 1);
    date.day[sizeof(date.day) - 1] = '\0';

    strncpy(date.month, monthStr, sizeof(date.month) - 1);
    date.month[sizeof(date.month) - 1] = '\0';

    strncpy(date.year, yearStr, sizeof(date.year) - 1);
    date.year[sizeof(date.year) - 1] = '\0';
    appointment.date = date;

    free(dayStr);
    free(monthStr);
    free(yearStr);

    printf("Enter the reason:\n");
    char *reasonStr = readFromStdin();
    if (!reasonStr)
        return;
    strncpy(appointment.reason, reasonStr, sizeof(appointment.reason));
    appointment.reason[sizeof(appointment.reason) - 1] = '\0';

    strcpy(appointment.prescription, "");

    Node *node = createNode(&appointment, sizeof(Appointment));
    pushNode(appointments, node);

    printf("Appointment scheduled successfully with ID: %d.\n", appointment.id);

    free(reasonStr);
    free(animalIdStr);
}

void completeAppointment(List *upcomingAppointments, List *completedAppointments, List *animals)
{
    printf("Completing appointment...\n");

    printf("Enter the appointment ID:\n");
    char *appointmentIdStr = readFromStdin();
    if (!appointmentIdStr)
        return;
    int id = atoi(appointmentIdStr);
    if (id == 0 && appointmentIdStr[0] != '0') // user entered value that cannot be converted to int
    {
        printf("Invalid appointment ID.\n");
        free(appointmentIdStr);
        return;
    }

    Appointment *appointment = getValueById(upcomingAppointments, id, APPOINTMENT);

    if (!appointment)
    {
        printf("There is no appointment with the given ID.\n");
        return;
    }

    printf("Enter prescription content:\n");
    char *prescriptionStr = readFromStdin();
    if (!prescriptionStr)
        return;

    strncpy(appointment->prescription, prescriptionStr, sizeof(appointment->prescription));
    appointment->prescription[sizeof(appointment->prescription) - 1] = '\0';

    Appointment *completedAppointment = malloc(sizeof(Appointment));
    if (!completedAppointment)
    {
        printf("Memory allocation for completed appointment failed.\n");
        free(prescriptionStr);
        return;
    }
    memcpy(completedAppointment, appointment, sizeof(Appointment));

    removeNode(upcomingAppointments, id, APPOINTMENT);

    Node *newNode = createNode(completedAppointment, sizeof(Appointment));
    pushNode(completedAppointments, newNode);

    displayAppointment(appointment);
    printf("Appointment with ID: %d successfully completed.\n", appointment->id);

    free(appointmentIdStr);
    free(prescriptionStr);
}

void displayMenu()
{
    printf("1. Display the list of upcoming appointments\n");
    printf("2. Display the list of completed appointments\n");
    printf("3. Display the list of animals\n");
    printf("4. Display the list of upcoming appointments on specific day\n");
    printf("5. Display the list of completed appointments on specific day\n");
    printf("6. Display the list of upcoming appointments(sorted by date)\n");
    printf("7. Display the list of completed appointments(sorted by date)\n");
    printf("8. Display the list of animals(sorted by owner's last name)\n");
    printf("9. Display the list of animals(based on owner's last name)\n");
    printf("10. Schedule an appointment\n");
    printf("11. Complete an appointment\n");
    printf("12. Exit\n");
}

void clearScreen()
{
    system("clear");
}

void execute(List *upcomingAppointments, List *completedAppointments, List *animals, int choice)
{
    switch (choice)
    {
    case 1:
        displayAppointments(upcomingAppointments);
        return;
    case 2:
        displayAppointments(completedAppointments);
        return;
    case 3:
        displayAnimals(animals);
        return;
    case 4:
        displayAppointmentsByDate(upcomingAppointments);
        return;
    case 5:
        displayAppointmentsByDate(completedAppointments);
        return;
    case 6:
        displayAppointmentsSortedByDate(upcomingAppointments);
        return;
    case 7:
        displayAppointmentsSortedByDate(completedAppointments);
        return;
    case 8:
        displayAnimalsSortedByLastName(animals);
        return;
    case 9:
        displayAnimalsByLastName(animals);
        return;
    case 10:
        scheduleAppointment(upcomingAppointments, animals);
        return;
    case 11:
        completeAppointment(upcomingAppointments, completedAppointments, animals);
        return;
    }
}

void pauseForUser()
{
    printf("Press Enter to continue...");
    while (getchar() != '\n')
    {
    }
}

void run(List *upcomingAppointments, List *completedAppointments, List *animals)
{
    char *choiceStr;
    int choiceInt = 9999;
    do
    {
        clearScreen();
        displayMenu();
        printf("Enter your choice: ");
        choiceStr = readFromStdin();
        if (!choiceStr)
            return;
        choiceInt = atoi(choiceStr);
        if (choiceInt == 12)
            break;
        else if (choiceInt >= 1 && choiceInt <= 11)
        {
            execute(upcomingAppointments, completedAppointments, animals, choiceInt);
            pauseForUser();
        }
    } while (1);
}

int main()
{
    List completedAppointments;
    completedAppointments.head = NULL;
    completedAppointments.tail = NULL;
    completedAppointments.length = 0;

    List upcomingAppointments;
    upcomingAppointments.head = NULL;
    upcomingAppointments.tail = NULL;
    upcomingAppointments.length = 0;

    List animals;
    animals.head = NULL;
    animals.tail = NULL;
    animals.length = 0;

    int loadAppointmentsResultA = loadAppointments(&completedAppointments, COMPLETED);
    if (loadAppointmentsResultA == 1)
    {
        return loadAppointmentsResultA;
    }

    int loadAppointmentsResultB = loadAppointments(&upcomingAppointments, UPCOMING);
    if (loadAppointmentsResultB == 1)
    {
        return loadAppointmentsResultB;
    }

    int loadAnimalsResult = loadAnimals(&animals);
    if (loadAnimalsResult == 1)
    {
        return loadAnimalsResult;
    }

    run(&upcomingAppointments, &completedAppointments, &animals);

    freeList(&upcomingAppointments);
    freeList(&completedAppointments);
    freeList(&animals);

    return 0;
}