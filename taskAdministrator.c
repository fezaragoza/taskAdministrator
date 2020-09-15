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

#define MAX_TASK 1000
#define RAND_TASKS 100
#define MAX_TASK_DURATION 200
#define NUM_MIN_TASKS 2

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
    uint8_t      priority;
    uint16_t     duration;
    uint32_t     time_queued;
    task_state_t state;
} task_t;

typedef struct
{
    task_t       tasks[NUM_MIN_TASKS];
    menu_state_t menu;
    uint16_t     task_count;
} admin_t;

void generateRandomTask(task_t *current_tasks, uint8_t num_tasks);

int main(void)
{   
    time_t t;
    srand((unsigned int) time(&t));

    admin_t admin;
    memset(&admin, 0, sizeof(admin));

    // printf("%d ", sizeof(admin.tasks)/sizeof(task_t)); Reports total current array elements
    generateRandomTask(admin.tasks, 1);

    return 0;
}


void generateRandomTask(task_t *current_tasks, uint8_t num_tasks)
{
    // Compute current size of task_list
    printf("%d ", sizeof(*current_tasks)); // Reports size in bytes of just one task_t, as the pointer points to first element in the list
    printf("%d ", sizeof(*current_tasks)/sizeof(task_t)); // Reports just 1 byte, as they both are the same size

}