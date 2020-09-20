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
#include <poll.h>

/*
* DEFINES
*/
#define MAX_TASKS         1000u
#define RAND_TASKS        100
#define MAX_TASK_PRIORITY 100u
#define MAX_TASK_DURATION 6u   // [s]
#define POLL_INPUT_TIME   3000 // [m/s]

#define LOOP_TASKS for (size_t i = 0; i < admin->task_count.actual; i++)
#define GEN_IDENTIF(x, y, z) x##y##z 

/*
* TYPEDEFS
*/
typedef enum
{
    ESPERA = 0,
    EJECUTADA, // EN COLA
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
    VERTAREAS,
} menu_state_t;

typedef struct
{
    unsigned char identif[3];
    uint16_t      priority;
    uint16_t      duration;
    uint32_t      time_queued;
    task_state_t  state;
} task_t;

typedef struct
{
    uint16_t total;
    uint16_t actual;
    uint16_t espera;
    uint16_t ejecutadas;
    uint16_t pausadas;
    uint16_t eliminidas;
} task_count_t;

typedef struct
{
    menu_state_t menu;
    task_count_t task_count;
    task_t*      tasks; // = NULL
} admin_t;


/*
* FUNCTION PROTOTYPE DECLARATION
*/
void init(admin_t*);
void generateInitRandomTask(task_t*);
void bubbleSort(task_t*, uint16_t);
void swapTask(task_t*, task_t*);
void menu(admin_t*);
menu_state_t getMenuSelection(void);
void printMenu(void);
void printInfo(admin_t*);
void grid(uint16_t, uint16_t, uint16_t,\
            uint16_t, uint16_t);
void addTimeQueued(admin_t*, uint8_t);
void ejecutar(admin_t*);
void agregar(admin_t*);
void eliminar(admin_t*);
void pausar(admin_t*);
void imprimir(admin_t*);
void printCurrentTaskInfo(task_t*);
void verTareas(admin_t*);

/*
* MAIN
*/
int main(void)
{   
    time_t begin = time(NULL);
    time_t t;
    srand((unsigned int) time(&t)); // Semilla para generar numeros random

    admin_t* admin;
    admin = (admin_t*)malloc(sizeof(*admin));             // Allocate memory for whole admin.

    admin->tasks = malloc((RAND_TASKS * sizeof(task_t))); // Allocate memeory for initial random tasks.
    memset(admin, 0, sizeof(admin));                      // Set whole admin struct to 0.
    admin->task_count.actual = RAND_TASKS;                // Need to keep track of array size as it's a pointer, init woth malloc.
    
    init(admin);                                          // Fill initial tasks and bubble sort them.
    while(admin->menu != SALIR)
    {
        menu(admin);                                      // Continuos process.
    }

    time_t end = time(NULL);
    double time_spent = (double)(end - begin);
    printf("Tiempo en ejecucion: %f seg.\n", time_spent);
    printf("Total de memoria consumida por las tareas en bytes: %d\n", (admin->task_count.actual * sizeof(task_t)));
    free(admin);
    return 0;
}

/*
* FUNCTION DEFINITIONS
*/
void init(admin_t *admin)
{
    printf("Bienvenido Administrador.\n");
    printf("Generando %d tareas iniciales...\n", RAND_TASKS);
    generateInitRandomTask(admin->tasks);
    printf("Sorteando tareas...\n");
    bubbleSort(admin->tasks, admin->task_count.actual);
}

void generateInitRandomTask(task_t *current_tasks)
{
    const char table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',\
                            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    uint8_t size_table = sizeof(table) / sizeof(char);

    for (size_t i = 0; i < RAND_TASKS; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {    
            (current_tasks + i)->identif[j] = table[rand() % size_table]; // Generate a 3 capital letter identifier randomly.
        }
        (current_tasks + i)->priority    = rand() % MAX_TASK_PRIORITY;
        (current_tasks + i)->duration    = rand() % MAX_TASK_DURATION;
        (current_tasks + i)->time_queued = 0;
        (current_tasks + i)->state       = ESPERA;
        // Fill above the remaining taks fields with data.
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
    printMenu();
    printInfo(admin);

    admin->menu = getMenuSelection(); // Retrieve selection from user.

    switch (admin->menu)              // Switch selection.
    {
    case EJECUTAR:
        ejecutar(admin);
        addTimeQueued(admin, 6);
        system("clear");
        break;
    case AGREGAR:
        agregar(admin);
        addTimeQueued(admin, 5);
        system("clear");
        break;
    case ELIMINAR:
        eliminar(admin);
        addTimeQueued(admin, 5);
        system("clear");
        break;
    case PAUSAR:
        pausar(admin);
        addTimeQueued(admin, 3);
        system("clear");
        break;
    case IMPRIMIR:
        imprimir(admin);
        addTimeQueued(admin, 4);
        system("clear");
        break;
    case SALIR:
        printf("GRACIAS POR USAR EL ADMINISTRADOR, HASTA PRONTO\n");
        break;
    case VERTAREAS:
        verTareas(admin);
        printf("Press ENTER key to continue.\n");
        while(getchar()!='\n');
        getchar();
        addTimeQueued(admin, 4);
        system("clear");
    default:
    case IDLE:
        addTimeQueued(admin, 1);
        sleep(1);
        system("clear");
        break;
    }

}

menu_state_t getMenuSelection(void)
{
    menu_state_t seleccion;
    uint32_t menu_sel = 0;

    static struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    // Give timeout waiting on user input
    if( poll(&mypoll, 1, POLL_INPUT_TIME) )
    {
        scanf("%i", &menu_sel);
    }
    
    // If timeout completes, then menu_sel remains 0, aka IDLE.
    if ((menu_sel == 0))
    {
        seleccion = IDLE;
    }
    else if ((menu_sel < 0) || (menu_sel > 7)){
        printf("Seleccion fuera de rango. Ingrese de nuevo.\n");
        seleccion = IDLE;
    }
    else
    {
        seleccion = (menu_state_t)(menu_sel);
    }
    return seleccion;
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
    printf("7. Ver Tareas.\n");
    printf("\n");
}

void printInfo(admin_t *admin)
{
    uint16_t count_espera = 0;
    uint16_t count_ejecucion = 0;
    uint16_t count_pausada = 0;
    uint16_t count_eliminada = 0;

    // Loop all the tasks and count the states of all of them.
    LOOP_TASKS
    {
        switch (admin->tasks[i].state)
        {
        case ESPERA:
            ++count_espera;
            break;
        case EJECUTADA:
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

    admin->task_count.espera     = count_espera;
    admin->task_count.ejecutadas = count_ejecucion;
    admin->task_count.pausadas   = count_pausada;
    admin->task_count.eliminidas = count_eliminada;

    // Print information in grid
    grid(admin->task_count.actual, admin->task_count.espera, admin->task_count.ejecutadas,\
            admin->task_count.pausadas, admin->task_count.eliminidas);
}

void grid(uint16_t total, uint16_t espera, uint16_t ejecutando,\
            uint16_t pausa, uint16_t kill)
{
    printf(" ___________________________________________________________________");
    printf("____________________________________________________________________\n");
    printf("|\t\t\t|\t\t\t|\t\t\t|\t\t|\t\t\t|\t\t\t|\n");
    printf("|\tTOTALES\t\t|\tESPERA\t\t|\tEJECUTADAS\t|\tKILL\t|\tPAUSADAS\t|\tMEMORIA\t\t|\n");
    printf("|\t\t\t|\t\t\t|\t\t\t|\t\t|\t\t\t|\t\t\t|\n");
    printf(" -------------------------------------------------------------------");
    printf("--------------------------------------------------------------------\n");
    printf("|\t    %d    \t|\t   %d    \t|\t   %d     \t|\t  %d  \t|\t   %d     \t|\t   %d     \t|\n", \
                total, espera, ejecutando, kill, pausa, total*sizeof(task_t));
    printf(" -------------------------------------------------------------------");
    printf("--------------------------------------------------------------------\n");
}

void addTimeQueued(admin_t *admin, uint8_t time)
{
    // Add time to time_queued field of each task.
    LOOP_TASKS
    {
        admin->tasks[i].time_queued += time; // ms
    }
}

void ejecutar(admin_t *admin)
{
    uint32_t tareasEjec;
    system("clear");
    verTareas(admin);
    printf("\n¿Cuantas tareas deseas ejecutar? \n");
    scanf("%i", &tareasEjec);

    // Do the number of tasks the user specified.
    for (size_t i = 0; i < tareasEjec; i++)
    {
        if ((admin->tasks[i].state != PAUSA))
        {
            printf("Ejecutando Tarea: %s\n", admin->tasks[i].identif);
            printf("Duracion: %d seg.\n", admin->tasks[i].duration);
            sleep(admin->tasks[i].duration);
            printf("TAREA FINALIZADA\n");
            admin->tasks[i].state = EJECUTADA;
            admin->tasks[i].priority = 5000; // Setting this to a big value to put them at the back when sorting.

            printf("TAREAS ACTUALIZADAS.\n");
            printInfo(admin);
        }
        else
        {
            // Task is paused, skipping it until unpaused.
            printf("Tarea: %s pausada, saltando a la siguiente.\n", admin->tasks[i].identif);
            tareasEjec++;
        }
    }
    bubbleSort(admin->tasks, admin->task_count.actual);
    sleep(3);
}

void agregar(admin_t *admin)
{
    unsigned char tareaAdd[4];
    uint32_t priority;
    uint32_t duration;
    system("clear");
    verTareas(admin);
    if (admin->task_count.actual < MAX_TASKS)
    {
        admin->task_count.actual++;
        admin->tasks = (task_t*)realloc(admin->tasks, admin->task_count.actual * sizeof(*admin->tasks));
        // Add task data
        printf("\nAgrega la tarea.\n");
        printf("Ingresa el identificador de tres letras mayusculas:\n");
        scanf("%3s", &tareaAdd);
        printf("Ingresa la prioridad, valor entre 0 y 100:\n");
        scanf("%i", &priority);
        printf("Ingresa la duracion, valor en segundos:\n");
        scanf("%i", &duration);
        
        for (size_t i = 0; i < 3; i++)
        {
            (admin->tasks[admin->task_count.actual-1].identif[i] = tareaAdd[i]);
        }
        admin->tasks[admin->task_count.actual-1].priority = priority;
        admin->tasks[admin->task_count.actual-1].duration = duration;
        admin->tasks[admin->task_count.actual-1].time_queued = 0;
        admin->tasks[admin->task_count.actual-1].state = ESPERA;

        printf("TAREAS ACTUALIZADAS\n");
        printInfo(admin);
        bubbleSort(admin->tasks, admin->task_count.actual);
        sleep(3);
    }
    else
    {
        printf("El administrador ha llegado a su limite de 1000 tareas, no se pueden agregar mas.");
    }
    
}

void eliminar(admin_t *admin)
{
    unsigned char tareaElim[4];
    _Bool eliminada = false;
    system("clear");
    verTareas(admin);
    printf("\n¿Cual tarea deseas eliminar? Ingresa el identificador de tres letras mayusculas:\n");
    scanf("%3s", &tareaElim);

    // Loop through all tasks to search for specified task
    LOOP_TASKS
    {
        if ((admin->tasks[i].identif[0] == tareaElim[0]) &&\
                (admin->tasks[i].identif[1] == tareaElim[1]) &&\
                    (admin->tasks[i].identif[2] == tareaElim[2]))
        {
            admin->tasks[i].state = KILL;
            admin->tasks[i].priority = 6000; // Setting this to a big value to put them at the back when sorting.
            eliminada = true;
            printf("Tarea Eliminada: %s\n", admin->tasks[i].identif);
        }
    }

    if (!eliminada)
    {
        puts("Ninguna tarea fue eliminada, no se encontró identificador.");
    }

    printf("TAREAS ACTUALIZADAS\n");
    printInfo(admin);
    bubbleSort(admin->tasks, admin->task_count.actual);
    sleep(3);
}

void pausar(admin_t *admin)
{
    unsigned char tareaPausar[4];
    _Bool pausada = false;
    system("clear");
    verTareas(admin);
    printf("\n¿Cual tarea deseas pausar? Ingresa el identificador de tres letras mayusculas:\n");
    scanf("%3s", &tareaPausar);

    LOOP_TASKS
    {
        if ((admin->tasks[i].identif[0] == tareaPausar[0]) &&\
                (admin->tasks[i].identif[1] == tareaPausar[1]) &&\
                    (admin->tasks[i].identif[2] == tareaPausar[2]))
        {
            // Pause, unpause the task.
            if (admin->tasks[i].state != PAUSA)
            {
                admin->tasks[i].state = PAUSA;
                pausada = true;
                printf("Tarea Pausada: %s\n", admin->tasks[i].identif);
            }
            else
            {
                admin->tasks[i].state = ESPERA;
                pausada = true;
                printf("Tarea Despausada: %s\n", admin->tasks[i].identif);
            }
        }
    }

    if (!pausada)
    {
        puts("Ninguna tarea fue pausada, no se encontró identificador.");
    }

    printf("TAREAS ACTUALIZADAS\n");
    printInfo(admin);
    bubbleSort(admin->tasks, admin->task_count.actual);
    sleep(3);
}

void imprimir(admin_t* admin)
{
    unsigned char tareaImpr[4];
    _Bool impresa = false;
    system("clear");
    verTareas(admin);
    printf("\n¿Cual tarea deseas imprimir? Ingresa el identificador de tres letras mayusculas:\n");
    scanf("%3s", &tareaImpr);

    LOOP_TASKS
    {
        if ((admin->tasks[i].identif[0] == tareaImpr[0]) &&\
                (admin->tasks[i].identif[1] == tareaImpr[1]) &&\
                    (admin->tasks[i].identif[2] == tareaImpr[2]))
        {
            printCurrentTaskInfo((admin->tasks + i));
            impresa = true;
        }
    }

    if (!impresa)
    {
        puts("No se puede imprimir tarea, no se encontró identificador.");
    }

    printf("TAREAS ACTUALIZADAS\n");
    printInfo(admin);
    sleep(4);

}

void printCurrentTaskInfo(task_t* task)
{
    printf("________________________\n");
    printf("|\t\t\t|\n");
    printf("|  TASK: %s\t\t|\n", task->identif);
    printf("|  Prioridad: %d\t\t|\n", task->priority);
    printf("|  Duracion: %d\t\t|\n", task->duration);
    printf("|  T en cola: %d\t\t|\n", task->time_queued);
    char *estado ;
    switch (task->state)
    {
    case ESPERA:
        estado = "ESPERA";
        break;
    case EJECUTADA:
        estado = "EJECUTADA";
        break;
    case PAUSA:
        estado = "PAUSA";
        break;
    case KILL:
        estado = "TERMINADA";
        break;
    default:
        break;
    }
    printf("|  Estado: %s\t|\n", estado);
    printf("| Tamano bytes: %d|\n", sizeof(*task));
    printf("-------------------------\n");
}

void verTareas(admin_t *admin)
{
    // Print all tasks in the list, according to their priority.
    printf("___________________________________________________________");
    printf("___________________________________________________________\n");
    printf("|");
    LOOP_TASKS
    {
        printf("  %d. %s  |", (i + 1), admin->tasks[i].identif);
        if ((i % 10 == 0) && (i != 0))
        {
            printf("\n|");
        }
    }
    printf("\n---------------------------------------------------------");
    printf("----------------------------------------------------------\n");
}