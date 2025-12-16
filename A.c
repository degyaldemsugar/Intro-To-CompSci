#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

/*
Option A: Student Grade Calculator
Create a program that:
.= Accepts multiple student names and their test scores.
.= Calculates the average score and assigns a letter grade (A–F).
.= Displays the class average and the top-performing student.
Skills used: loops, conditionals, lists/arrays, functions, basic file I/O (optional).
*/

typedef enum
{
    A = 90,
    B = 80,
    C = 70,
    D = 60,
    F = 50, // and anything below obv
} student_grade;

typedef struct student_node
{
    char name[50];
    student_grade grade;
    int score;
    struct student_node *next;
} student_node;

student_grade calculate_grade(int score)
{
    if (score <= 0)
    {
        return F;
    }
    switch (score / 10)
    { // integer division removes the decimal part and rounds towards zero to the nearest int.
    case 10:
    case 9:
        return A;
    case 8:
        return B;
    case 7:
        return C;
    case 6:
        return D;
    default:
        return F;
    }
}

char *get_string_grade(student_grade grade)
{
    switch (grade)
    {
    case A:
        return "A";
    case B:
        return "B";
    case C:
        return "C";
    case D:
        return "D";
    default:
        return "F";
    }
}

void display_top_student(student_node **head)
{
    student_node *curr = *head;

    if (*head == NULL)
    {
        printf("There are no students.\n");
        return;
    }
    student_node *top = curr;
    int max_score = curr->score;

    while (curr != NULL)
    {
        if (max_score < curr->score)
        {
            max_score = curr->score;
            top = curr;
        }
        curr = curr->next;
    }

    printf("The top student is: %s with a score of %i and a grade of %s \n", top->name, top->score, get_string_grade(top->grade));
    return;
}

void display_class_average(student_node **head)
{
    student_node *curr = *head;
    if (*head == NULL)
    {
        printf("There are no students. \n");
        return;
    }

    int total_score = 0;
    int number_of_students = 0;

    while (curr != NULL)
    {
        total_score += curr->score;
        number_of_students++;
        curr = curr->next;
    }

    float class_average = (float)total_score / number_of_students;
    printf("The class average was %.2f and the number of students were %i, with a total score of %i \n", class_average, number_of_students, total_score);
}

void display_student(char name[50], student_node **head)
{
    student_node *curr = *head;
    if (*head == NULL)
    {
        printf("There are no students. \n");
        return;
    }

    while (curr != NULL)
    {
        if (strcmp(curr->name, name) == 0)
        {
            printf("Student Name: %s \n Score: %i \n Grade: %s \n", curr->name, curr->score, get_string_grade(curr->grade));
            return;
        }
        curr = curr->next;
    }
    printf("No student was found with name of '%s' \n", name);
    return;
}

void add_student(char name[50], int score, student_grade grade, student_node **head)
{
    student_node *node = (student_node *)malloc(sizeof(student_node));
    if (node == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(node->name, name);
    node->score = score;
    node->grade = calculate_grade(score);
    node->next = *head;
    *head = node;
    printf(" \nThe student %s was added successfully \n", name);
}

void delete_student(char name[50], student_node **head)
{
    student_node *curr = *head;
    student_node *prev = NULL;

    if (*head == NULL)
    {
        printf("THe list is empty. Cannot delete '%s' \n", name);
        return;
    }

    while (curr != NULL)
    {
        if (strcmp(curr->name, name) == 0)
        {
            if (prev == NULL)
            {
                *head = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            printf("The student '%s' was deleted successfully.\n", name);
            return;
        }

        prev = curr;
        curr = curr->next;
    }

    printf("No student was found with the name '%s' \n", name);
    return;
}

void create_student(student_node **head)
{
    int score = 0; // to help loop later
    char first_name[25] = "";
    char last_name[25] = "";
    char name[50] = "";
    student_grade grade;
    printf("Adding new student...\n");
    printf("Enter Student First Name: ");
    scanf("%24s", first_name);
    printf("Enter Student Last Name: ");
    scanf("%24s", last_name);

    do
    {
        printf("Enter Student Score(0-100): ");
        if (scanf("%i", &score) != 1)
        {
            score = -1; // keep loop going.
            int c;
            printf("Please Enter A Valid Number. \n");
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
        }
        else if (score < 0 || score > 100)
        {
            printf("Score must be between 0 and 100 \n");
        }
    } while (score < 0 || score > 100);
    strcpy(name, first_name);
    strcat(name, " ");
    strcat(name, last_name);
    grade = calculate_grade(score);
    add_student(name, score, grade, head);
    printf("\n");
}

void remove_student(student_node **head)
{
    char first_name[25] = "";
    char last_name[25] = "";
    char name[50] = "";

    printf("Deleting Student... \n");
    printf("Enter Student First Name: ");
    scanf("%24s", first_name);
    printf("Enter Student Last Name: ");
    scanf("%24s", last_name);
    strcpy(name, first_name);
    strcat(name, " ");
    strcat(name, last_name);
    printf(" \nSTUDENT TO DELETE %s \n", name);
    delete_student(name, head);
    printf("\n");
}

void save_students(student_node *head, const char *file_name)
{
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        perror("Error opening file for writing");
        return;
    }

    fprintf(fp, "this_file_is_legit_trust\n");

    student_node *curr = head;
    while (curr != NULL)
    {
        fprintf(fp, "%s, %d \n", curr->name, curr->score);
        curr = curr->next;
    }

    printf("Student data successfully saved to %s\n", file_name);
    fclose(fp);
}

void load_students(student_node **head, const char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("No existing student file found. Starting empty. \n");
        return;
    }

    char line[100];

    if (fgets(line, sizeof(line), fp) == NULL)
    {
        printf("Error: File is empty or could not be read. Aborting load.\n");
        fclose(fp);
        return;
    }

    if (strncmp(line, "this_file_is_legit_trust", 25) != 0)
    {
        printf("Error: File signature mismatch. This is not a valid student data file: %s.\n", file_name);
        fclose(fp);
        return;
    }

    char name[50];
    int score;
    student_grade grade;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%49[^,],%d", name, &score) == 2)
        {
            if (score >= 0 && score <= 100)
            {
                grade = calculate_grade(score);
                add_student(name, score, grade, head);
            }
        }
    }
    printf("Student data successfully loaded from %s\n", file_name);
    fclose(fp);
}

#define NUMBER_OF_STRINGS 50
#define MAX_STRING_LENGTH 100

char current_files[NUMBER_OF_STRINGS][MAX_STRING_LENGTH];

int get_current_files()
{
    DIR *d;
    int i = 0;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (i < NUMBER_OF_STRINGS)
            {
                strncpy(current_files[i], dir->d_name, MAX_STRING_LENGTH);
                i++;
            }
        }
        closedir(d);
        return i;
    }
    else
    {
        perror("Error opening directory");
        return -1;
    }
}

void list_current_files(int file_count)
{
    if (file_count == -1)
    {
        printf("Error trying to list directory because it failed to read it.");
        return;
    }
    printf("Current Directory: \n");
    printf("---------------------------\n");
    for (size_t i = 0; i < file_count; i++)
    {

        printf("%s \n", current_files[i]);
    }
    printf("---------------------------\n");
    printf("End of current Directory.\n");
}

int does_save_file_exist(char file_name[256], int file_count)
{

    if (file_count == -1)
    {
        printf("Error trying to list directory because it failed to read it. \n");
        return -1;
    }
    for (size_t i = 0; i < file_count; i++)
    {
        if ((strcmp(current_files[i], file_name)) == 0)
        {
            printf("The file %s exists in the current dir\n", file_name);
            return 1;
        }
        continue;
    }

    printf("The file %s does not exist in the current dir \n", file_name);
    return 0;
}

void free_memory(student_node **head)
{
    student_node *curr = *head;
    student_node *temp = NULL;
    while (curr != NULL)
    {
        temp = curr->next;
        free(curr);
        curr = temp;
    }
    *head = NULL;
}

void flush_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int main()
{
    student_node *head = NULL;
    int flag = 0;
    int open_save_file_choice = -1;
    int ok = -1;
    int file_count = get_current_files();

    printf("Do you wish to load a save file? 1 for yes 0 for no: \n");
    while (open_save_file_choice != 0 && open_save_file_choice != 1)
    {
        printf("Please enter 0 or 1: ");
        if (scanf("%i", &open_save_file_choice) != 1)
        {
            flush_input_buffer();
            open_save_file_choice = -1;
        }
    }

    if (open_save_file_choice == 1)
    {
        while (ok != 1)
        {
            printf("Your current files are: \n");
            list_current_files(file_count);
            printf("Please type your save file to open: ");
            char filename[256] = "";

            // made to read input once to prevent buffer overflows
            if (scanf("%255s", filename) != 1)
            {
                fprintf(stderr, "Error: Could not read filename from input.\n");
                flush_input_buffer();
                continue;
            }

            int file_exists = does_save_file_exist(filename, file_count);

            if (file_exists == 1)
            {
                load_students(&head, filename);
                ok = 1;
                break;
            }
            else if (file_exists == -1)
            {
                perror("Error checking if file exists aborting file loading... \n");
                ok = 1;
                break;
            }
            else if (file_exists == 0)
            {
                printf("File '%s' not found. Please enter a file that exists.\n", filename);
            }
        }
    }

    printf(" \nWelcome to student program: \n Enter 1 to add a student, \n 2 to delete a student, \n 3 to see top student, \n 4 to see class average  \n 5 to quit the program. \n ");

    while (!flag)
    {

        printf("\n Type here: ");
        int choice;
        if (scanf("%i", &choice) != 1)
        {
            choice = -1;
            flush_input_buffer();
        }

        while (choice < 1 || choice > 5)
        {
            printf("Choice must be in this range: 1-5. \n");
            printf("\n Type here: ");
            if (scanf("%i", &choice) != 1)
            {
                choice = -1;
                flush_input_buffer();
            }
        }

        switch (choice)
        {
        case 1:
            create_student(&head);
            break;
        case 2:
            remove_student(&head);
            break;
        case 3:
            display_top_student(&head);
            break;
        case 4:
            display_class_average(&head);
            break;
        case 5:
            flag = 1;
            break;
        default:
            printf("Invalid Choice.");
            break;
        }
    }

    char file_name[256] = "";
    int save_successful = 0;

    printf("What do you want to save your file as? ");

    while (!save_successful)
    {
        if (scanf("%255s", file_name) != 1)
        {
            fprintf(stderr, "Error: Could not read filename. Retrying.\n");
            flush_input_buffer();
            continue;
        }

        int file_exists = does_save_file_exist(file_name, file_count);

        if (file_exists == 1)
        {
            printf("File '%s' exists. Overwrite? (1 for yes, 0 for no): ", file_name);
            int overwrite_choice = -1;

            while (overwrite_choice != 0 && overwrite_choice != 1)
            {
                if (scanf("%i", &overwrite_choice) == 1)
                {
                    if (overwrite_choice == 1)
                    {
                        printf("Overwriting file...\n");
                        save_students(head, file_name);
                        save_successful = 1;
                    }
                    else if (overwrite_choice == 0)
                    {
                        printf("Please enter a new, unique file name: ");
                        break;
                    }
                }
                else
                {
                    printf("Invalid input. Enter 1 or 0: ");
                    flush_input_buffer();
                    overwrite_choice = -1;
                }
            }
        }
        else if (file_exists == 0)
        {
            printf("Saving file...\n");
            save_students(head, file_name);
            save_successful = 1;
        }
        else
        {
            fprintf(stderr, "Error: Cannot check file existence. Aborting save.\n");
            break;
        }
    }

    printf("Freeing memory...\n");
    free_memory(&head);
    printf("Memory was successfully freed...\n");
    printf("Program can safely close...\n");
    printf("---------------------------\n");
    printf("END OF STUDENT PROGRAM\n");
    exit(EXIT_SUCCESS);
}