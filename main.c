#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buffer 4097

struct list
{
    char owner_name[buffer];
    char owner_surname[buffer];
    char phone_number[buffer];
    char animals_name[buffer];
    char animals_species[buffer];
    char animals_age[buffer];
    char animals_gender[buffer];
    char animals_weight[buffer];
    char animals_description[buffer];
    struct list *next;
};

struct appointment
{
    char owner_name[buffer];
    char owner_surname[buffer];
    char animals_name[buffer];
    char day[buffer];
    char month[buffer];
    char year[buffer];
    char reason[buffer];
    char prescription[buffer];
    struct appointment *next;
};

struct appointment *loading_data_about_appointments()
{
    FILE *plik;
    plik = fopen("dane_wizyty.txt", "r");
    if(plik)
    {
        printf("Dane o wizytach zostaly wczytane poprawnie\n");
    }
    else
    {
        printf("Dane o wizytach nie zostaly wczytane poprawnie\n");
    }
    struct appointment *first = NULL;
    struct appointment *element = NULL;
    while(!feof(plik))
    {
        if(first == NULL)
        {
            first = malloc(sizeof(struct appointment));

            fscanf(plik, "%s", first->owner_name);

            fscanf(plik, "%s", first->owner_surname);

            fscanf(plik, "%s", first->animals_name);

            fscanf(plik, "%s", first->day);

            fscanf(plik, "%s", first->month);

            fscanf(plik, "%s ", first->year);

            fgets(first->reason, buffer, plik);
            first->reason[strcspn(first->reason, "\n")] = 0;

            memset(first->prescription, '\0', buffer);
            element = first;
        }
        else
        {
            element->next = malloc(sizeof(struct appointment));

            fscanf(plik, "%s", (element->next)->owner_name);

            fscanf(plik, "%s", (element->next)->owner_surname);

            fscanf(plik, "%s", (element->next)->animals_name);

            fscanf(plik, "%s", (element->next)->day);

            fscanf(plik, "%s", (element->next)->month);

            fscanf(plik, "%s ", (element->next)->year);

            fgets((element->next)->reason, buffer, plik);
            (element->next)->reason[strcspn((element->next)->reason, "\n")] = 0;

            memset((element->next)->prescription, '\0', buffer);
            (element->next)->next = NULL;
            element = element->next;
        }
    }
    fclose(plik);
    return first;
};

void show_list_about_appointments(struct appointment *first)
{
    struct appointment *tmp = first;
    while(tmp != NULL)
    {
        printf("-----------------------------------------\n");
        printf("%s\n", tmp->owner_name);
        printf("%s\n", tmp->owner_surname);
        printf("%s\n", tmp->animals_name);
        printf("%s\n", tmp->day);
        printf("%s\n", tmp->month);
        printf("%s\n", tmp->year);
        printf("%s\n", tmp->reason);
        tmp = tmp->next;
    }
    printf("-----------------------------------------\n");

}

struct appointment *fulfillment_of_recommendations(struct appointment *first)
{
    int spr = 0;
    char *name = malloc(buffer*sizeof(char));
    char *surname = malloc(buffer*sizeof(char));
    char *animals_name= malloc(buffer*sizeof(char));

    printf("Podaj prosze nastepujace dane: \n");
    printf("\n");
    getchar();
    printf("Imie wlasciciela zwierzaka: ");
    gets(name);
    printf("Nazwisko wlasciciela zwierzaka: ");
    gets(surname);
    printf("Imie zwierzaka: ");
    gets(animals_name);

    struct appointment *tmp = first;
    while(tmp != NULL)
    {

        if(strcmp(tmp->owner_name, name) == 0 && strcmp(tmp->owner_surname, surname) == 0 && strcmp(tmp->animals_name, animals_name) == 0)
        {
            spr = 1;
            printf("Wprowadz zalecenia: ");
            gets(tmp->prescription);
        }
        tmp = tmp->next;
    }
    if(spr == 0)
    {
        printf("Nie znaleziono takiej osoby w bazie danych\n");
    }
    free(name);
    free(surname);
    free(animals_name);
    return first;
};

void informations_about_visits_which_took_place(struct appointment *first)
{
    FILE *plik;
    plik = fopen("dane_wizyty_odbyte.txt", "a");
    if(plik)
    {
        printf("Dane o odbytych wizytach zostaly wczytane poprawnie\n");
    }
    else
    {
        printf("Dane o odbytych wizytach nie zostaly wczytane poprawnie\n");
    }
    struct appointment *tmp = first;
    while(tmp != NULL)
    {
        if(tmp->prescription[0] != '\0')
        {
            fprintf(plik, "%s\n", tmp->owner_name);
            fprintf(plik, "%s\n", tmp->owner_surname);
            fprintf(plik, "%s\n", tmp->animals_name);
            fprintf(plik, "%s\n", tmp->day);
            fprintf(plik, "%s\n", tmp->month);
            fprintf(plik, "%s\n", tmp->year);
            fprintf(plik, "%s\n", tmp->prescription);
        }
        tmp = tmp->next;
    }
    fclose(plik);
}


struct appointment *deleting_info_about_appointment_which_happend(struct appointment *first)
{
    if(first == NULL)
    {
        return NULL;
    }
    struct appointment *tmp = NULL; //??
    struct appointment *tmp2 = NULL;
    if((first->prescription[0]) != '\0')
    {
        tmp = first->next;
        free(first);
        first = tmp;
    }

    if(first == NULL)
    {
        return NULL;
    }

    tmp = first;
    while((tmp->next) != NULL)
    {
        while((tmp->next) != NULL && (tmp->next)->prescription[0] == '\0')
        {
            tmp = tmp->next;
        }
        if((tmp->next) != NULL)
        {
            tmp2 = tmp->next;
            tmp->next = tmp2->next;
            free(tmp2);
        }
    }
    return first;
};

void overwriting_of_file(struct appointment *first)
{
    FILE *plik;
    plik = fopen("dane_wizyty.txt", "w+");
    struct appointment *tmp = first;
    while(tmp != NULL)
    {
        fprintf(plik, "%s\n", tmp->owner_name);
        fprintf(plik, "%s\n", tmp->owner_surname);
        fprintf(plik, "%s\n", tmp->animals_name);
        fprintf(plik, "%s\n", tmp->day);
        fprintf(plik, "%s\n", tmp->month);
        fprintf(plik, "%s\n", tmp->year);
        fprintf(plik, "%s", tmp->reason);
        if(tmp->next != NULL)
        {
          fprintf(plik, "\n");
        }
        tmp=tmp->next;
    }
    fclose(plik);
}

void overwriting_of_file2(struct list *first)
{
    FILE *plik;
    plik = fopen("dane_pacjenci.txt", "w+");
    struct list *tmp = first;
    while(tmp != NULL)
    {
        fputs(tmp->owner_name, plik);
        fprintf(plik, " ");

        fputs(tmp->owner_surname, plik);
        fprintf(plik, " ");

        fputs(tmp->phone_number, plik);
        fprintf(plik, " ");

        fputs(tmp->animals_name, plik);
        fprintf(plik, " ");

        fputs(tmp->animals_species, plik);
        fprintf(plik, " ");

        fputs(tmp->animals_age, plik);
        fprintf(plik, " ");

        fputs(tmp->animals_gender, plik);
        fprintf(plik, " ");

        fputs(tmp->animals_weight, plik);
        fprintf(plik, " \n");

        fputs(tmp->animals_description, plik);
        if(tmp->next != NULL)
        {
            fprintf(plik, "\n");
        }
        tmp=tmp->next;
    }
    fclose(plik);
}


struct appointment *making_up_an_appointment(struct appointment *first)
{
    FILE *plik;
    plik = fopen("dane_wizyty.txt", "a");
    struct appointment *el = malloc(sizeof(struct appointment));
    printf("Podaj nastepujace informacje: \n");

    int size;
    fseek(plik, 0, SEEK_END);
    size=ftell(plik);
    if(size != 0)
    {
        fprintf(plik, "\n");
    }

    getchar();
    printf("Podaj imie wlasciciela zwierzaka: ");
    gets(el->owner_name);
    fputs(el->owner_name, plik);
    fprintf(plik, "\n");

    printf("Podaj nazwisko wlasciciela zwierzaka: ");
    gets(el->owner_surname);
    fputs(el->owner_surname, plik);
    fprintf(plik, "\n");

    printf("Podaj imie zwierzaka: ");
    gets(el->animals_name);
    fputs(el->animals_name, plik);
    fprintf(plik, "\n");

    printf("Podaj dzien interesujacej Cie daty: ");
    gets(el->day);
    fputs(el->day, plik);
    fprintf(plik, "\n");

    printf("Podaj miesiac interesujacej Cie daty: ");
    gets(el->month);
    fputs(el->month, plik);
    fprintf(plik, "\n");

    printf("Podaj rok interesujacej Cie daty: ");
    gets(el->year);
    fputs(el->year, plik);
    fprintf(plik, "\n");

    printf("Podaj powod wizyty: ");
    gets(el->reason);
    fputs(el->reason, plik);

    el->next = NULL;
    if(first == NULL)
    {
        return el;
        printf("Proces przebiegl pomyslnie\n");
    }
    struct appointment *tmp = first;
    while((tmp->next) != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = el;
    printf("Proces przebiegl pomyslnie\n");
    fclose(plik);
    return first;
};

struct list *resignation_from_the_services_of_the_clinic(struct list *first)
{
    struct list *el = malloc(sizeof(struct list));
    printf("Podaj prosze nastepujace dane: \n");

    getchar();
    printf("Podaj swoje imie: ");
    gets(el->owner_name);

    printf("Podaj swoje nazwisko: ");
    gets(el->owner_surname);

    struct list *tmp = first;
    struct list *tmp2 = NULL;
    if(first == NULL)
    {
        return NULL;
    }
    if(strcmp(first->owner_name, el->owner_name) == 0 && strcmp(first->owner_surname, el->owner_surname) == 0)
    {
        tmp = first->next;
        free(first);
        first = tmp;
    }
    if(first == NULL)
    {
        return NULL;
    }
    tmp = first;
    while((tmp->next) != NULL)
    {
        while((tmp->next) != NULL && strcmp((tmp->next)->owner_name, el->owner_name) != 0 && (first->owner_surname, el->owner_surname) != 0)
        {
            tmp = tmp->next;
        }
        if((tmp->next) != NULL)
        {
            tmp2 = tmp->next;
            tmp->next = tmp2->next;
            free(tmp2);
        }
    }
    return first;
};



int are_same1(struct appointment *first, struct appointment *second)
{
    int ret = 1;
    if(0 != strcmp(first->owner_name, second->owner_name))
    {
        ret = 0;
        return ret;
    }
    if(0 != strcmp(first->owner_surname, second->owner_surname))
    {
        ret = 0;
        return ret;
    }
    if(0 != strcmp(first->animals_name, second->animals_name))
    {
        ret = 0;
        return ret;
    }
    if(0 != strcmp(first->day, second->day))
    {
        ret = 0;
        return ret;
    }
    if(0 != strcmp(first->month, second->month))
    {
        ret = 0;
        return ret;
    }
    if(0 != strcmp(first->year, second->year))
    {
        ret = 0;
        return ret;
    }
    if(0 != strcmp(first->reason, second->reason))
    {
        ret = 0;
        return ret;
    }
    return ret;
}

int contains_value1(struct appointment *container, struct appointment *val)
{
    int ret = 0;
    struct appointment *pos = container;
    while(pos != NULL)
    {
        if(are_same1(pos, val))
        {
            ret = 1;
            break;
        }
        pos = pos->next;
    }
    return ret;
}

void copy1(struct appointment *dest, struct appointment *source)
{
    strcpy(dest->owner_name, source->owner_name);
    strcpy(dest->owner_surname, source->owner_surname);
    strcpy(dest->animals_name, source->animals_name);
    strcpy(dest->day, source->day);
    strcpy(dest->month, source->month);
    strcpy(dest->year, source->year);
    strcpy(dest->reason, source->reason);
    strcpy(dest->prescription, source->prescription);
    dest->next = NULL;
}

int how_many_elements_in_list1(struct appointment *first)
{
    int i = 0;
    struct appointment *tmp = first;
    while(tmp != NULL)
    {
        ++i;
        tmp = tmp->next;
    }
    return i;
}

struct appointment *sort_by_date(struct appointment *dest, struct appointment *source)
{
    struct appointment *first = source;
    struct appointment *pos_source = source->next;
    struct appointment *pos_dest = dest;
    struct appointment *pos_next = NULL;
    int count = how_many_elements_in_list1(source);
    while(how_many_elements_in_list1(pos_dest) < count)
    {
        pos_source = source;
        while(contains_value1(pos_dest, pos_source))
        {
            pos_source = pos_source->next;
        }
        first = pos_source;
        pos_source = source;
        while(pos_source != NULL)
        {
            if(atoi(pos_source->year) > atoi(first->year) && contains_value1(pos_dest, pos_source) == 0)
            {
                first = pos_source;
            }
            else if(atoi(pos_source->year) == atoi(first->year) && contains_value1(pos_dest, pos_source) == 0)
            {
                if(atoi(pos_source->month) > atoi(first->month))
                {
                    first = pos_source;
                }
                else if(atoi(pos_source->month) == atoi(first->month))
                {
                    if(atoi(pos_source->day) > atoi(first->day))
                    {
                        first = pos_source;
                    }
                }
            }
            pos_source = pos_source->next;
        }
        if(how_many_elements_in_list1(pos_dest) == 0)
        {
            pos_dest = malloc(sizeof(struct appointment));
            copy1(pos_dest, first);
            pos_next = pos_dest;
        }
        else
        {
            pos_next->next = malloc(sizeof(struct appointment));
            copy1(pos_next->next, first);
            pos_next = pos_next->next;
        }
    }
    return pos_dest;
};

void freelist(struct list *first)
{
    struct list *tmp;
    while(first != NULL)
    {
        tmp = first;
        first = first->next;
        free(tmp);
    }
}

void freelist2(struct appointment *first)
{
    struct appointment *tmp;
    while(first != NULL)
    {
        tmp = first;
        first = first->next;
        free(tmp);
    }
}

struct list *loading_data_about_patients()
{
    FILE *plik;
    plik = fopen("dane_pacjenci.txt", "r");
    if(plik)
    {
        printf("Dane o pacjentach i ich wlascicielach zostaly wczytane poprawnie\n");
    }
    else
    {
        printf("Dane o pacjentach i ich wlascicielach nie zostaly wczytane poprawnie\n");
    }
    struct list *first = NULL;
    struct list *element = NULL;
    while(!feof(plik))
    {
        if(first == NULL)
        {
            first = malloc(sizeof(struct list));
            fscanf(plik, "%s", first->owner_name);
            fscanf(plik, "%s", first->owner_surname);
            fscanf(plik, "%s", first->phone_number);
            fscanf(plik, "%s", first->animals_name);
            fscanf(plik, "%s", first->animals_species);
            fscanf(plik, "%s", first->animals_age);
            fscanf(plik, "%s", first->animals_gender);
            fscanf(plik, "%s ", first->animals_weight);
            fgets(first->animals_description, buffer, plik);
            first->animals_description[strcspn(first->animals_description, "\n")] = 0;
            element = first;
        }
        else
        {
            element->next = malloc(sizeof(struct list));
            fscanf(plik, "%s", (element->next)->owner_name);
            fscanf(plik, "%s", (element->next)->owner_surname);
            fscanf(plik, "%s", (element->next)->phone_number);
            fscanf(plik, "%s", (element->next)->animals_name);
            fscanf(plik, "%s", (element->next)->animals_species);
            fscanf(plik, "%s", (element->next)->animals_age);
            fscanf(plik, "%s", (element->next)->animals_gender);
            fscanf(plik, "%s ", (element->next)->animals_weight);
            fgets((element->next)->animals_description, buffer, plik);
            (element->next)->animals_description[strcspn((element->next)->animals_description, "\n")] = 0;
            (element->next)->next = NULL;
            element = element->next;
        }
    }
    fclose(plik);
    return first;
};

struct list *registration_of_a_new_pet(struct list *first)
{
    FILE *plik;
    plik = fopen("dane_pacjenci.txt", "a");
    if(plik)
    {
        printf("Dane o pacjentach i ich wlascicielach zostaly wczytane poprawnie\n");
    }
    else
    {
        printf("Dane o pacjentach i ich wlascicielach nie zostaly wczytane poprawnie\n");
    }
    struct list *element = malloc(sizeof(struct list));
    printf("Podaj prosze nastepujace dane: \n");

    int size;
    fseek(plik, 0, SEEK_END);
    size=ftell(plik);
    if(size != 0)
    {
        fprintf(plik, "\n");
    }

    getchar();
    printf("Imie wlasciciela: ");
    gets(element->owner_name);
    fputs(element->owner_name, plik);
    fprintf(plik, " ");

    printf("Nazwisko wlasciciela: ");
    gets(element->owner_surname);
    fputs(element->owner_surname, plik);
    fprintf(plik, " ");

    printf("Telefon kontaktowy: ");
    gets(element->phone_number);
    fputs(element->phone_number, plik);
    fprintf(plik, " ");

    printf("Imie zwierzaka: ");
    gets(element->animals_name);
    fputs(element->animals_name, plik);
    fprintf(plik, " ");

    printf("Gatunek zwierzaka: ");
    gets(element->animals_species);
    fputs(element->animals_species, plik);
    fprintf(plik, " ");

    printf("Wiek zwierzaka: ");
    gets(element->animals_age);
    fputs(element->animals_age, plik);
    fprintf(plik, " ");

    printf("Plec zwierzaka: ");
    gets(element->animals_gender);
    fputs(element->animals_gender, plik);
    fprintf(plik, " ");

    printf("Mase ciala zwierzaka: ");
    gets(element->animals_weight);
    fputs(element->animals_weight, plik);
    fprintf(plik, "\n");

    printf("Opis zwierzaka: ");
    gets(element->animals_description);
    fputs(element->animals_description, plik);

    element->next = NULL;
    if(first == NULL)
    {
        printf("Rejestracja przebiegla pomyslenie!\n");
        return element;
    }
    struct list *tmp = first;
    while((tmp->next) != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = element;
    printf("Rejestracja przebiegla pomyslenie!\n");
    fclose(plik);
    return first;
};

void show_list_about_patients(struct list *first)
{
    struct list *tmp = first;
    while(tmp != NULL)
    {
        printf("-----------------------------------------\n");
        printf("%s\n", tmp->owner_name);
        printf("%s\n", tmp->owner_surname);
        printf("%s\n", tmp->phone_number);
        printf("%s\n", tmp->animals_name);
        printf("%s\n", tmp->animals_species);
        printf("%s\n", tmp->animals_age);
        printf("%s\n", tmp->animals_gender);
        printf("%s\n", tmp->animals_weight);
        printf("%s\n", tmp->animals_description);
        tmp = tmp->next;
    }
    printf("-----------------------------------------\n");
}

void searching_by_surname(struct list *first)
{
    char *array = malloc(buffer*sizeof(char));
    int i;
    int spr;
    struct list *tmp = first;
    printf("Podaj prosze nazwisko zarejestrowane w naszej bazie danych: ");
    getchar();
    gets(array);
    while(tmp != NULL)
    {
        spr = 1;
        for(i = 0; i < strlen(array); ++i)
        {
            if(tmp->owner_surname[i] != array[i])
            {
                spr = 0;
                break;
            }
        }
        if(spr == 1)
        {
            printf("\n-----------------------------------------\n");
            printf("%s\n", tmp->owner_name);
            printf("%s\n", tmp->owner_surname);
            printf("%s\n", tmp->phone_number);
            printf("%s\n", tmp->animals_name);
            printf("%s\n", tmp->animals_species);
            printf("%s\n", tmp->animals_age);
            printf("%s\n", tmp->animals_gender);
            printf("%s\n", tmp->animals_weight);
            printf("%s", tmp->animals_description);
        }
        tmp = tmp->next;
    }
    printf("\n-----------------------------------------\n");
    free(array);
};

int how_many_elements_in_list(struct list *first)
{
    int i = 0;
    struct list *tmp = first;
    while(tmp != NULL)
    {
        ++i;
        tmp = tmp->next;
    }
    return i;
}

int are_same(struct list *first, struct list *second)
{
    int ret = 1;
    if(0 != strcmp(first->owner_name, second->owner_name))
    {
        ret = 0;
    }
    if(0 != strcmp(first->owner_surname, second->owner_surname))
    {
        ret = 0;
    }
    if(0 != strcmp(first->phone_number, second->phone_number))
    {
        ret = 0;
    }
    if(0 != strcmp(first->animals_name, second->animals_name))
    {
        ret = 0;
    }
    if(0 != strcmp(first->animals_species, second->animals_species))
    {
        ret = 0;
    }
    if(0 != strcmp(first->animals_age, second->animals_age))
    {
        ret = 0;
    }
    if(0 != strcmp(first->animals_gender, second->animals_gender))
    {
        ret = 0;
    }
    if(0 != strcmp(first->animals_weight, second->animals_weight))
    {
        ret = 0;
    }
    if(0 != strcmp(first->animals_description, second->animals_description))
    {
        ret = 0;
    }
    return ret;
}

int contains_value(struct list *container, struct list *val)
{
    int ret = 0;
    struct list *pos = container;
    while(pos != NULL)
    {
        if(are_same(pos, val))
        {
            ret = 1;
            break;
        }
        pos = pos->next;
    }
    return ret;
}

void copy(struct list *dest, struct list *source)
{
    strcpy(dest->owner_name, source->owner_name);
    strcpy(dest->owner_surname, source->owner_surname);
    strcpy(dest->phone_number, source->phone_number);
    strcpy(dest->animals_name, source->animals_name);
    strcpy(dest->animals_species, source->animals_species);
    strcpy(dest->animals_age, source->animals_age);
    strcpy(dest->animals_gender, source->animals_gender);
    strcpy(dest->animals_weight, source->animals_weight);
    strcpy(dest->animals_description, source->animals_description);
    dest->next = NULL;
}

void visit_for_now()
{
    FILE *plik;
    plik = fopen("dane_wizyty_odbyte.txt", "a");
    if(plik)
    {
        printf("Plik dane_wizyty_odbyte zostal otwarty poprawnie\n");
    }
    else
    {
        printf("Plik dane_wizyty_odbyty nie zostal otwarty poprawnie\n");
    }

    char *tmp = malloc(buffer*sizeof(char));
    printf("Podaj prosze nastepujace dane:\n");

    int size;
    fseek(plik, 0, SEEK_END);
    size=ftell(plik);
    if(size != 0)
    {
        fprintf(plik, "\n");
    }
    getchar();
    printf("Podaj swoje imie: ");
    gets(tmp);
    fputs(tmp, plik);
    fprintf(plik, "\n");
    memset(tmp, '\0', buffer);

    printf("Podaj swoje nazwisko: ");
    gets(tmp);
    fputs(tmp, plik);
    fprintf(plik, "\n");
    memset(tmp, '\0', buffer);

    printf("Podaj imie zwierzaka: ");
    gets(tmp);
    fputs(tmp, plik);
    fprintf(plik, "\n");
    memset(tmp, '\0', buffer);

    printf("Podaj, ktory mamy dzisiaj dzien: ");
    gets(tmp);
    fputs(tmp, plik);
    fprintf(plik, "\n");
    memset(tmp, '\0', buffer);

    printf("Podaj obecny miesiac: ");
    gets(tmp);
    fputs(tmp, plik);
    fprintf(plik, "\n");
    memset(tmp, '\0', buffer);

    printf("Podaj obecny rok: ");
    gets(tmp);
    fputs(tmp, plik);
    fprintf(plik, "\n");
    memset(tmp, '\0', buffer);

    printf("Przypisz recepte: ");
    gets(tmp);
    fputs(tmp, plik);
    memset(tmp, '\0', buffer);
    fclose(plik);
    free(tmp);
}

int data_patients_randomization()
{
    FILE *plik1;
    FILE *plik2;
    plik1 = fopen("random_patients_data.txt", "r");
    plik2 = fopen("dane_pacjenci.txt", "a");
    if(plik1)
    {
        printf("Plik random_data_patients zostal otwarty poprawnie\n");
    }
    else
    {
        printf("Plik random_data_patients nie zostal otwarty poprawnie\n");
    }
    if(plik2)
    {
        printf("Plik dane_pacjenci zostal otwarty poprawnie\n");
    }
    else
    {
        printf("Plik dane_pacjenci nie zostal otwarty poprawnie\n");
    }
    int wybor;
    printf("Podaj prosze ile danych osob chcesz wygenerowac(max:39): ");
    scanf("%d", &wybor);
    int i = 1;
    char *tmp = malloc(buffer*sizeof(char));

    int size;
    fseek(plik2, 0, SEEK_END);
    size=ftell(plik2);
    if(size != 0)
    {
        fprintf(plik2, "\n");
    }

    while(i <= wybor)
    {
        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s ", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s ", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s ", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s ", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s ", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s ", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s ", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s ", tmp);
        fprintf(plik2, "%s\n", tmp);
        memset(tmp, '\0', buffer);

        fgets(tmp, buffer, plik1);
        tmp[strcspn(tmp, "\n")] = 0;
        fputs(tmp, plik2);
        memset(tmp, '\0', buffer);

        if((i+1) <= wybor)
        {
            fprintf(plik2, "\n");
        }

        ++i;

    }
    printf("Proces przebiegl pomyslnie!\n");
    free(tmp);
    fclose(plik1);
    fclose(plik2);
}

void data_appointments_randomization()
{
    FILE *plik1;
    FILE *plik2;
    plik1 = fopen("random_appointments_data.txt", "r");
    plik2 = fopen("dane_wizyty.txt", "a");
    if(plik1)
    {
        printf("Plik random_appointments_data zostal otwarty poprawnie\n");
    }
    else
    {
        printf("Plik random_appointments_data nie zostal otwarty poprawnie\n");
    }
    if(plik1)
    {
        printf("Plik dane_wizyty zostal otwarty poprawnie\n");
    }
    else
    {
        printf("Plik dane_wizyty nie zostal otwarty poprawnie\n");
    }
    int wybor;
    int i = 1;
    printf("Podaj prosze ile danych osob chcesz wygenerowac(max:8): ");
    scanf("%d", &wybor);
    char *tmp = malloc(buffer*sizeof(char));

    int size;
    fseek(plik2, 0, SEEK_END);
    size=ftell(plik2);
    if(size != 0)
    {
        fprintf(plik2, "\n");
    }

    while(i <= wybor)
    {
        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s\n", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s\n", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s\n", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s\n", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s", tmp);
        fprintf(plik2, "%s\n", tmp);
        memset(tmp, '\0', buffer);

        fscanf(plik1, "%s ", tmp);
        fprintf(plik2, "%s\n", tmp);
        memset(tmp, '\0', buffer);

        fgets(tmp, buffer, plik1);
        tmp[strcspn(tmp, "\n")] = 0;
        fputs(tmp, plik2);
        memset(tmp, '\0', buffer);

        if((i+1) <= wybor)
        {
            fprintf(plik2, "\n");
        }

        ++i;
    }
    free(tmp);
    fclose(plik1);
    fclose(plik2);
}


struct list *alphabetical_sort_by_surname(struct list *dest, struct list *source)
{
    struct list *first = source;
    struct list *pos_source = source->next;
    struct list *pos_dest = dest;
    struct list *pos_next = NULL;
    int count = how_many_elements_in_list(source);
    while(how_many_elements_in_list(pos_dest) < count)
    {
        pos_source = source;
        while(contains_value(pos_dest, pos_source))
        {
            pos_source = pos_source->next;
        }
        first = pos_source;
        pos_source = source;
        while(pos_source != NULL)
        {
            if(strcmp(first->owner_surname, pos_source->owner_surname) > 0 && contains_value(pos_dest, pos_source) == 0)
            {
                first = pos_source;
            }
            pos_source = pos_source->next;
        }

        if(how_many_elements_in_list(pos_dest) == 0)
        {
            pos_dest = malloc(sizeof(struct list));
            copy(pos_dest, first);
            pos_next = pos_dest;
        }
        else
        {
            pos_next->next = malloc(sizeof(struct list));
            copy(pos_next->next, first);
            pos_next=pos_next->next;
        }
    }
    return pos_dest;
}

void control_panel()
{
    printf("\033[0;0H\033[2J");
    int opcja2 = 25;
    int opcja = 25;
    int how_many;
    printf("Wybierz 1, aby wyswietlic liste pacjentow\n");
    printf("Wybierz 2, aby dodac nowego pacjenta\n");
    printf("Wybierz 3, aby wyszukac pacjenta po nazwisku wlasciciela\n");
    printf("Wybierz 4, aby wyswietlic liste pacjentow posortowana alfabetycznie wg. nazwisk\n");
    printf("Wybierz 5, aby wyswietlic liste zaplanowanych wizyt\n");
    printf("Wybierz 6, aby przypisac recepte dla zwierzaka\n");
    printf("Wybierz 7, aby umowic sie na wizyte\n");
    printf("Wybierz 8, jesli chcesz wypisac sie z kliniki\n");
    printf("Wybierz 9, jesli chcesz posortowac liste danych o wizytach wedlug daty(Od najblizszej do najodleglejszej)\n");
    printf("Wybierz 10, jesli chcesz wygenerowac dane o pacjentach kliniki\n");
    printf("Wybierz 11, jesli chcesz wygenerowac dane na temat wizyt\n");
    printf("Wybierz 12, jesli chcesz odbyc teraz wizyte\n");
    printf("\nOpcja ktora Cie interesuje: ");
    scanf("%d", &opcja);
    printf("\033[0;0H\033[2J");
    if(opcja == 1)
    {
        struct list *information_list = NULL;
        information_list = loading_data_about_patients();
        show_list_about_patients(information_list);
        freelist(information_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 2)
    {
        struct list *information_list = NULL;
        information_list = loading_data_about_patients();
        information_list = registration_of_a_new_pet(information_list);
        show_list_about_patients(information_list);
        freelist(information_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 3)
    {
        struct list *information_list = NULL;
        information_list = loading_data_about_patients();
        searching_by_surname(information_list);
        freelist(information_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 4)
    {
        struct list *information_list = NULL;
        information_list = loading_data_about_patients();
        struct list *information_sorted_list = NULL;
        information_sorted_list = alphabetical_sort_by_surname(information_sorted_list, information_list);
        show_list_about_patients(information_sorted_list);
        freelist(information_list);
        freelist(information_sorted_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 5)
    {
        struct appointment *appointment_list = NULL;
        appointment_list = loading_data_about_appointments();
        show_list_about_appointments(appointment_list);
        freelist2(appointment_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 6)
    {
        struct appointment *appointment_list = NULL;
        appointment_list = loading_data_about_appointments();
        appointment_list = fulfillment_of_recommendations(appointment_list);
        informations_about_visits_which_took_place(appointment_list);
        appointment_list = deleting_info_about_appointment_which_happend(appointment_list);
        overwriting_of_file(appointment_list);
        show_list_about_appointments(appointment_list);
        freelist2(appointment_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 7)
    {
        struct appointment *appointment_list = NULL;
        appointment_list = loading_data_about_appointments();
        appointment_list = making_up_an_appointment(appointment_list);
        show_list_about_appointments(appointment_list);
        freelist2(appointment_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 8)
    {
        struct list *information_list = NULL;
        information_list = loading_data_about_patients();
        information_list = resignation_from_the_services_of_the_clinic(information_list);
        overwriting_of_file2(information_list);
        show_list_about_patients(information_list);
        freelist(information_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 9)
    {
        struct appointment *appointment_list = NULL;
        appointment_list = loading_data_about_appointments();
        struct appointment *appointments_sorted_list = NULL;
        appointments_sorted_list = sort_by_date(appointments_sorted_list, appointment_list);
        show_list_about_appointments(appointments_sorted_list);
        freelist2(appointment_list);
        freelist2(appointments_sorted_list);
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 10)
    {
        data_patients_randomization();
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 11)
    {
        data_appointments_randomization();
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
    else if(opcja == 12)
    {
        visit_for_now();
        while(opcja2 != 1 && opcja2 != 2)
        {
            printf("Wprowadz 1, jesli chcesz kontynuowac lub 2, jesli chcesz zakonczyc\n");
            printf("Opcja: ");
            scanf("%d", &opcja2);
        }
        if(opcja2 == 1)
        {
            control_panel();
        }
    }
}

int main()
{
    control_panel();
    printf("\033[0;0H\033[2J");
    return 0;
}
