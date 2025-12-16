#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/*
    Create a program that:
    Lets a user add, remove, and view tasks.
    Saves tasks to a file and loads them when the program starts (optional for extra
    credit).

    Skills used: lists, file handling, basic menus, functions.
*/

#define task_name_size 128
#define task_desc_size 512
#define task_time_size 30

char *get_current_time()
{
    time_t t = time(NULL);
    struct tm *ptr;
    ptr = localtime(&t);
    return asctime(ptr);
}

typedef enum completion
{
    yes = 1,
    no = 0,
} completion;

typedef struct task_data
{
    char name[task_name_size];
    char desc[task_desc_size];
    char time[task_time_size];
    completion completed;
} task_data;

typedef struct task_node
{
    task_data data;
    struct task_node *next;
} task_node;

void add_task(task_node **head, char *name, char *desc, char *time, completion is_completed)
{
    task_node *node = (task_node *)malloc(sizeof(task_node));
    if (node == NULL)
    {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    strcpy(node->data.name, name);
    strcpy(node->data.desc, desc);
    strcpy(node->data.time, time);
    node->data.completed = is_completed;

    node->next = *head;
    *head = node;
}

void remove_task(task_node **head, char *name)
{
    task_node *curr = *head;
    task_node *prev = NULL;

    if (*head == NULL)
    {
        fprintf(stderr, "Error: The linked list is empty.\n");
        return;
    }

    while (curr != NULL)
    {
        if (strcmp(curr->data.name, name) == 0)
        {
            if (prev == NULL)
            {
                *head = curr->next;
                free(curr);
                printf("Successfully deleted task `%s` ", name);
                return;
            }
            prev->next = curr->next;
            free(curr);
            printf("Successfully deleted task `%s` ", name);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    fprintf(stderr, "Error: Task with name '%s' not found.\n", name);
}

void view_tasks(task_node **head)
{
    task_node *curr = *head;
    size_t i = 1;

    if (*head == NULL)
    {
        fprintf(stderr, "Error: The linked list is empty.\n");
        return;
    }

    // Total width is 4 + 10 + 20 + 45 + 26 + (5 pipes) = 110 characters
    printf("+------+------------+----------------------+-----------------------------------------------+--------------------------+\n");
    printf("| %-4s | %-10s | %-20s | %-45s | %-24s |\n",
           "ID", "Completed?", "Task Name", "Description", "Time Started");
    printf("+------+------------+----------------------+-----------------------------------------------+--------------------------+\n");

    while (curr != NULL)
    {

        const char *completion_status = curr->data.completed == yes ? "Yes" : "No"; // enum to string

        printf("| %-4zu | %-10s | %-20s | %-45s | %-24s |\n",
               i,
               completion_status,
               curr->data.name,
               curr->data.desc,
               curr->data.time);

        curr = curr->next;
        i++;
    }

    printf("+------+------------+----------------------+-----------------------------------------------+--------------------------+\n");
}

void create_task(task_node **head)
{
    char name[task_name_size];
    char desc[task_desc_size];
    char time_str[task_time_size];

    const char *current_time = get_current_time();
    if (current_time == NULL)
    {
        fprintf(stderr, "Error: Failed to retrieve current time.\n");
        return;
    }

    strncpy(time_str, current_time, task_time_size - 1);
    time_str[task_time_size - 1] = '\0';
    size_t len = strlen(time_str);
    if (len > 0 && time_str[len - 1] == '\n')
    {
        time_str[len - 1] = '\0';
    }

    printf("Please enter task name (max %d chars): ", task_name_size - 1);
    if (fgets(name, task_name_size, stdin) == NULL)
    {
        fprintf(stderr, "Error reading task name.\n");
        return;
    }

    len = strlen(name);
    if (len > 0 && name[len - 1] == '\n')
    {
        name[len - 1] = '\0';
    }

    printf("Please enter task description (max %d chars): ", task_desc_size - 1);
    if (fgets(desc, task_desc_size, stdin) == NULL)
    {
        fprintf(stderr, "Error reading task description.\n");
        return;
    }

    len = strlen(desc);
    if (len > 0 && desc[len - 1] == '\n')
    {
        desc[len - 1] = '\0';
    }

    add_task(head, name, desc, time_str, no);
    printf("Task '%s' created successfully.\n", name);
}

void delete_task(task_node **head)
{
    char name[task_name_size];
    size_t len;

    printf("Please enter task name (max %d chars): ", task_name_size - 1);
    if (fgets(name, task_name_size, stdin) == NULL)
    {
        fprintf(stderr, "Error reading task name.\n");
        return;
    }

    len = strlen(name);
    if (len > 0 && name[len - 1] == '\n')
    {
        name[len - 1] = '\0';
    }

    remove_task(head, name);
}

void mark_task_as_complete(task_node **head)
{
    task_node *curr = *head;
    char name[task_name_size];
    size_t len;

    if (*head == NULL)
    {
        fprintf(stderr, "Error: The linked list is empty.\n");
        return;
    }

    printf("Please enter task name (max %d chars): ", task_name_size - 1);
    if (fgets(name, task_name_size, stdin) == NULL)
    {
        fprintf(stderr, "Error reading task name.\n");
        return;
    }

    len = strlen(name);
    if (len > 0 && name[len - 1] == '\n')
    {
        name[len - 1] = '\0';
    }

    while (curr != NULL)
    {
        if (strcmp(curr->data.name, name) == 0)
        {
            curr->data.completed = yes;
            printf("Task '%s' succesfully marked as complete.\n", name);
            return;
        }

        curr = curr->next;
    }
    printf("Could not find task '%s'.\n", name);
}

void flush_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void free_memory(task_node **head)
{
    task_node *curr = *head;
    task_node *temp = NULL;
    while (curr != NULL)
    {
        temp = curr->next;
        free(curr);
        curr = temp;
    }
    *head = NULL;
}

void save_list(task_node *head, const char *file_name)
{
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        perror("Error opening file for writing");
        return;
    }

    fprintf(fp, "this_file_is_legit_trust\n");

    task_node *curr = head;
    while (curr != NULL)
    {
        fprintf(fp, "%s, %s, %s, %d \n", curr->data.name, curr->data.desc, curr->data.time, curr->data.completed);
        curr = curr->next;
    }

    printf("Data successfully saved to %s\n", file_name);
    fclose(fp);
}

void load_file(task_node **head, const char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {

        printf("No existing task file found: %s. Starting empty.\n", file_name); 
        return;
    }

    char line[task_desc_size + 100]; 
    const int SIGNATURE_LEN = 24; 


    if (fgets(line, sizeof(line), fp) == NULL)
    {
        printf("Error: File is empty or could not read signature. Aborting load.\n");
        fclose(fp);
        return;
    }

    if (strncmp(line, "this_file_is_legit_trust", SIGNATURE_LEN) != 0)
    {
        printf("Error: File signature mismatch. This is not a valid task data file: %s.\n", file_name);
        fclose(fp);
        return;
    }

    char name[task_name_size];
    char desc[task_desc_size];
    char time_str[task_time_size];
    int completed_val; 

    while (fgets(line, sizeof(line), fp) != NULL)
    {
       
        int matched = sscanf(line, "%127[^,], %511[^,], %29[^,], %d",
                             name, desc, time_str, &completed_val); 

        if (matched == 4)
        {
            add_task(head, name, desc, time_str, (completion)completed_val); 
        } else {
            fprintf(stderr, "Warning: Skipping malformed line during load.\n");
        }
    }
    
    printf("Data successfully loaded from %s\n", file_name);
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





int main()
{
    task_node *head = NULL;

    int flag = 1;
    puts("Welcome to my to do list!");
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

    flush_input_buffer();

    if (open_save_file_choice == 1)
    {
        while (ok != 1)
        {
            printf("Your current files are: \n");
            list_current_files(file_count);
            printf("Please type your save file to open: ");
            char filename[256] = "";

            // made to read input once to prevent buffer overflows
            if (fgets(filename, sizeof(filename), stdin) == NULL)
            {
                fprintf(stderr, "Error: Could not read filename from input.\n");
                continue; 
            }
            
            // remove newline
            size_t len = strlen(filename);
            if (len > 0 && filename[len - 1] == '\n') {
                filename[len - 1] = '\0';
            }

            if (filename[0] == '\0') {
                 printf("Filename cannot be empty. Please enter a filename.\n");
                 continue;
            }

            int file_exists = does_save_file_exist(filename, file_count);

            if (file_exists == 1)
            {
                load_file(&head, filename);
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



    while (flag)
    {
        int choice; // Use int for user input

        do
        {
            printf("Type (1) to add task, (2) to remove task, (3) to view tasks, (4) to mark task as complete, and (5) to exit: ");

            // scanf returns 1 on success
            if (scanf("%d", &choice) != 1)
            {
                puts("Invalid input. Please enter a number (1-5).");
                flush_input_buffer();
                choice = 0; // force loop to continue
            }
            else if (choice < 1 || choice > 5)
            {

                puts("Choice must be in this range 1-5.");
            }
            else
            {
                flush_input_buffer();
            }

        } while (choice < 1 || choice > 5);

        //flush_input_buffer(); // can cause issues if not cleared before each choice.

        switch (choice)
        {
        case 1:
            create_task(&head);
            break;
        case 2:
            delete_task(&head);
            break;
        case 3:
            view_tasks(&head);
            break;
        case 4:
            mark_task_as_complete(&head);
            break;
        case 5:
            flag = 0;
            break;
        default:
            puts("Invalid Choice.");
            break;
        }
    }




char file_name[256] = "";
    int save_successful = 0;

    printf("\nWhat do you want to save your file as? ");
    

    while (!save_successful)
    {

        if (fgets(file_name, sizeof(file_name), stdin) == NULL)
        {
            fprintf(stderr, "Error: Could not read filename. Aborting save.\n");
            break; 
        }


        size_t len = strlen(file_name);
        if (len > 0 && file_name[len - 1] == '\n') {
            file_name[len - 1] = '\0';
        }
        
    
        if (file_name[0] == '\0') {
            printf("Filename cannot be empty. Please try again.\n");
            continue;
        }

        int file_exists = does_save_file_exist(file_name, file_count);

        if (file_exists == 1)
        {
            printf("File '%s' exists. Overwrite? (1 for yes, 0 for no): ", file_name);
            int overwrite_choice = -1;

            
            do {
                if (scanf("%i", &overwrite_choice) != 1) {
                    printf("Invalid input. Enter 1 or 0: ");
                    flush_input_buffer();
                    overwrite_choice = -1; // continue loop
                } else if (overwrite_choice != 0 && overwrite_choice != 1) {
                    printf("Invalid input. Enter 1 or 0: ");
                } else {
                    flush_input_buffer(); 
                }
            } while (overwrite_choice != 0 && overwrite_choice != 1);


            if (overwrite_choice == 1)
            {
                printf("Overwriting file...\n");
                save_list(head, file_name);
                save_successful = 1;
            }
            else // overwrite_choice == 0
            {
                printf("Please enter a new, unique file name: ");
                
            }
        }
        else if (file_exists == 0)
        {
            printf("Saving file...\n");
            save_list(head, file_name);
            save_successful = 1;
        }
        else
        {
            fprintf(stderr, "Error: Cannot check file existence. Aborting save.\n");
            break;
        }
    }


    free_memory(&head);
    puts("Memory successfully freed...");
    puts("Exiting program");
    exit(EXIT_SUCCESS);
    return 0;
}