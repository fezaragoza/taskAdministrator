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

#define MAX_TASKS 1000u
#define RAND_TASKS 3u
#define MAX_TASK_DURATION 200u // [m/s]
#define MAX_TASK_PRIORITY 5u

typedef enum
{
    ESPERA = 0,
    EJECUTANDO, // EN COLA
    PAUSA,
    KILL
} task_state_t;

typedef enum
{
    IDLE = 0,
    EJECUTAR,
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

void init(admin_t*);
void generateInitRandomTask(task_t*);
void bubbleSort(task_t*, uint16_t);
void swapTask(task_t*, task_t*);
void menu(admin_t*);
void printInfo(admin_t *);
void grid(uint16_t, uint16_t, uint16_t,\
            uint16_t, uint16_t);
void ejecutar(admin_t*);

int main(void)
{   
    time_t t;
    srand((unsigned int) time(&t));

    admin_t admin;
    memset(&admin, 0, sizeof(admin));

    // printf("%d ", sizeof(admin.tasks)/sizeof(task_t)); Reports total current array elements
    // uint16_t current_tasks_size = sizeof(admin.tasks)/sizeof(task_t);
    // printf("%d\n", current_tasks_size);
    
    init(&admin);
    // while(admin.menu != SALIR)
    // {
    //     menu(&admin);

    //     // printInfo(&admin);
    // }

    printInfo(&admin);

    return 0;
}

void init(admin_t *admin)
{
    printf("Bienvenido Administrador.\n");
    printf("Generando %d tareas iniciales...\n", RAND_TASKS);
    generateInitRandomTask(admin->tasks);
    admin->task_count = sizeof(admin->tasks)/sizeof(task_t);
    printf("Sorteando tareas...\n");
    bubbleSort(admin->tasks, admin->task_count);
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

void bubbleSort(task_t *current_tasks, uint16_t tasks_size)
{
    uint16_t n = tasks_size;
    for (size_t i = 0; i < tasks_size; i++)
    {
        for (size_t j = 0; j < n-1; j++)
        {
            if ((current_tasks + j)->priority >  (current_tasks + j + 1)->priority)
            {
                swapTask((current_tasks + j), (current_tasks + j + 1));
            }
            else if ((current_tasks + j)->priority == (current_tasks + j + 1)->priority)
            {
                if ((current_tasks + j)->duration > (current_tasks + j + 1)->duration)
                {
                    swapTask((current_tasks + j), (current_tasks + j + 1));
                }
                else if ((current_tasks + j)->duration == (current_tasks + j + 1)->duration)
                {
                    if ((current_tasks + j)->time_queued < (current_tasks + j + 1)->time_queued)
                    {
                        swapTask((current_tasks + j), (current_tasks + j + 1));
                    }
                } 
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

void menu(admin_t *admin)
{
    menu_state_t seleccion;
    uint8_t menu_sel;
    _Bool repetir;

    printMenu();
    scanf("i", &menu_sel);

    if ((menu_sel < 0) || (menu_sel > 6)){
        repetir = true;
    }
    else
    {
        seleccion = (menu_state_t)(menu_sel);
        switch (seleccion)
        {
        case EJECUTAR:
            // ejecutar(admin);
            break;
        case AGREGAR:
            /* code */
            break;
        case ELIMINAR:
            /* code */
            break;
        case PAUSAR:
            /* code */
            break;
        case IMPRIMIR:
            /* code */
            break;
        case SALIR:
            /* code */
            break;
        default:
            break;
        }
    }
    
    
    if (repetir)
        menu(admin);

}

void printMenu(void)
{
    printf("ADMINISTRADOR DE TAREAS.\n");
    printf("\n");
    printf("¿Qué deseas hacer?\n");
    printf("1. Ejecutar Tarea.\n");
    printf("2. Agregar Tarea.\n");
    printf("3. Eliminar Tarea.\n");
    printf("4. Pausar Tarea.\n");
    printf("5. Imprimir Tarea.\n");
    printf("6. Salir del Menu.\n");
    printf("\n");
}

void printInfo(admin_t *admin)
{
    uint16_t count_espera;
    uint16_t count_ejecucion;
    uint16_t count_eliminada;
    uint16_t count_pausada;

    for (size_t i = 0; i < admin->task_count; i++)
    {
        switch (admin->tasks[i].state)
        {
        case ESPERA:
            ++count_espera;
            break;
        case EJECUTANDO:
            ++count_ejecucion;
            break;
        case PAUSA:
            ++count_pausada;
            break;
        case KILL:
            ++count_eliminada;
            break;
        default:
            break;
        }
    }
    grid(admin->task_count, count_espera, count_ejecucion,\
            count_pausada, count_eliminada);
}

void grid(uint16_t total, uint16_t espera, uint16_t ejecutando,\
            uint16_t pausa, uint16_t kill)
{
    printf(" _______________________________________________________");
    printf("_______________________________________________________\n");
    printf("|\t\t\t|\t\t\t|\t\t\t|\t\t|\t\t\t|\n");
    printf("|\tTOTALES\t\t|\tESPERA\t\t|\tEJECUCION\t|\tKILL\t|\tPAUSADA\t\t|\n");
    printf("|\t\t\t|\t\t\t|\t\t\t|\t\t|\t\t\t|\n");
    printf(" --------------------------------------------------------");
    printf("--------------------------------------------------------\n");
    printf("|\t    %d    \t|\t   %d    \t|\t   %d     \t|\t  %d  \t|\t   %d     \t|\n", total, espera, ejecutando, kill, pausa);
    printf(" --------------------------------------------------------");
    printf("--------------------------------------------------------\n");
}

// void ejecutar(admin_t *admin)
// {
//     printf("Cuantas tareas deseas ejecutar? \n")
// }