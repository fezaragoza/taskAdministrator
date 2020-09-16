// taskAdministrator.c -- by Fernando Zaragoza

/*
* INCLUDES
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define MAX_TASKS 1000
#define RAND_TASKS 3
#define MAX_TASK_DURATION 200 // [m/s]
#define MAX_TASK_PRIORITY 5

typedef enum
{
    ESPERA = 0,
    EJECUTANDO, // EN COLA
    PAUSA,
    KILL
} task_state_t;

typedef enum
{
    EJECUTAR = 0,
    AGREGAR,
    ELIMINAR,
    PAUSAR,
    IMPRIMIR,
    SALIR,
} menu_state_t;

typedef struct
{
    char         identif;
    uint16_t     priority;
    uint16_t     duration;
    uint32_t     time_queued;
    task_state_t state;
} task_t;

typedef struct
{
    task_t       tasks[RAND_TASKS];
    menu_state_t menu;
    uint16_t     task_count;
} admin_t;

void generateInitRandomTask(task_t*);

void swapTask(task_t*, task_t*);

int main(void)
{   
    time_t t;
    srand((unsigned int) time(&t));

    admin_t admin;
    memset(&admin, 0, sizeof(admin));

    // printf("%d ", sizeof(admin.tasks)/sizeof(task_t)); Reports total current array elements
    uint16_t current_tasks_size = sizeof(admin.tasks)/sizeof(task_t);
    printf("%d\n", current_tasks_size);
    generateInitRandomTask(admin.tasks);
    bubbleSort(admin.tasks, current_tasks_size);

    return 0;
}


void generateInitRandomTask(task_t *current_tasks)
{
    // Compute current size of task_list
    // printf("%d ", sizeof(*current_tasks)); // Reports size in bytes of just one task_t, as the pointer points to first element in the list
    // printf("%d ", sizeof(*current_tasks)/sizeof(task_t)); // Reports just 1 byte, as they both are the same size. *current_tasks points to the first element.

    for (size_t i = 0; i < RAND_TASKS; i++)
    {
        // (current_tasks + i)->identif = 'AB'
        (current_tasks + i)->priority    = rand() % MAX_TASK_PRIORITY;
        (current_tasks + i)->duration    = rand() % MAX_TASK_DURATION;
        (current_tasks + i)->time_queued = 0;
        (current_tasks + i)->state       = ESPERA;
        printf("%d ",  (current_tasks + i)->priority);
        printf("%d ",  (current_tasks + i)->duration);
        printf("\n");
    }
    
}

// void addPriority(task_t *current_tasks)
// {

// }

void bubbleSort(task_t *current_tasks, uint16_t tasks_size)
{
    uint16_t n = tasks_size;
    for (size_t i = 0; i < tasks_size; i++)
    {
        for (size_t j = 0; j < n-1; j++)
        {
            if ((current_tasks + j)->priority >  (current_tasks + j + 1)->priority)
            {
                // register uint16_t temp = (current_tasks + j + 1)->priority;
                // (current_tasks + j + 1)->priority = (current_tasks + j)->priority;
                // (current_tasks + j)->priority = temp;

                // task_t *temp = malloc(sizeof(*temp));
                // memcpy(temp, (current_tasks + j + 1), sizeof(task_t));
                // memcpy((current_tasks + j + 1), (current_tasks + j), sizeof(task_t));
                // memcpy((current_tasks + j), temp, sizeof(task_t));

                swapTask((current_tasks + j), (current_tasks + j + 1));
            }
            else if ((current_tasks + j)->priority == (current_tasks + j + 1)->priority)
            {
                // if ((current_tasks + j)->duration > (current_tasks + j + 1)->duration)
                // {
                //     task_t *temp = malloc(sizeof(*temp));
                //     memcpy(temp, (current_tasks + j + 1), sizeof(task_t));
                //     memcpy((current_tasks + j + 1), (current_tasks + j), sizeof(task_t));
                //     memcpy((current_tasks + j), temp, sizeof(task_t));
                // }
                // else
                // {
                //     task_t *temp = malloc(sizeof(*temp));
                //     memcpy(temp, (current_tasks + j + 1), sizeof(task_t));
                //     memcpy((current_tasks + j + 1), (current_tasks + j), sizeof(task_t));
                //     memcpy((current_tasks + j), temp, sizeof(task_t));
                // }
                
                
            }
            
        }
        --n;
    }
    for (size_t i = 0; i < tasks_size; i++)
    {
        printf("%d ",  (current_tasks + i)->priority);
        printf("%d ",  (current_tasks + i)->duration);
        printf("\n");
    }
    
    
}

void swapTask(task_t *task_behind, task_t *task_ahead)
{
    register task_t *temp = malloc(sizeof(*temp));
    // Destination ----- Source
    memcpy(temp, task_ahead, sizeof(task_t));
    memcpy(task_ahead, task_behind, sizeof(task_t));
    memcpy(task_behind, temp, sizeof(task_t));
}