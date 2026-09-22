#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

/* ===== Константы ===== */
#define INVENTORY_SIZE 10
#define MAX_HOURS 24
#define START_DAY 1
#define START_HOUR 8

/* ID предметов */
#define ITEM_EMPTY      0
#define ITEM_WOOD       1
#define ITEM_STONE      2
#define ITEM_SEEDS      3
#define ITEM_IRON       4
#define ITEM_GOLD       5
#define ITEM_WATER      6
#define ITEM_FOOD       7
#define ITEM_TOOL       8
#define ITEM_ARTIFACT   9

/* ===== Глобальные переменные ===== */
int current_day = START_DAY;
int current_hour = START_HOUR;

int inventory[INVENTORY_SIZE] = {
    ITEM_WOOD, ITEM_STONE, ITEM_SEEDS, ITEM_EMPTY, ITEM_IRON,
    ITEM_WOOD, ITEM_EMPTY, ITEM_GOLD, ITEM_EMPTY, ITEM_FOOD
};

/* ===== Прототипы функций ===== */
void show_menu(void);
int read_number(const char *text);
void show_time(void);
void work_time(void);
void show_inventory(void);
void put_item(void);
void drop_item(void);
void remove_duplicates(void);
const char* get_item_name(int id);

/* ===== Главная функция ===== */
int main(void) {

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice;

    do {
        show_menu();
        choice = read_number("Your choice: ");

        switch (choice) {
            case 0:
                printf("Exit. Bye!\n");
                break;
            case 1:
                show_time();
                break;
            case 2:
                work_time();
                break;
            case 3:
                show_inventory();
                break;
            case 4:
                put_item();
                break;
            case 5:
                drop_item();
                break;
            case 6:
                remove_duplicates();
                break;
            default:
                printf("Wrong choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}

/* ===== Меню ===== */
void show_menu(void) {
    printf("\n===== HAPPY FARMER =====\n");
    printf("[0] Exit\n");
    printf("[1] Show time\n");
    printf("[2] Work (skip time)\n");
    printf("[3] Show inventory\n");
    printf("[4] Put item\n");
    printf("[5] Drop item\n");
    printf("[6] Remove duplicates (variant 9)\n");
    printf("===========================\n");
}

/* ===== Ввод числа с защитой от дурака ===== */
int read_number(const char *text) {
    int number;
    char ch;

    while (1) {
        printf("%s", text);

        if (scanf("%d", &number) == 1) {
            while ((ch = getchar()) != '\n' && ch != EOF);
            return number;
        } else {
            printf("Error! Enter a number.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }
}

/* ===== 1. Показать время ===== */
void show_time(void) {
    printf("Current time: Day %d, %02d:00\n", current_day, current_hour);
}

/* ===== 2. Поработать ===== */
void work_time(void) {
    int hours = read_number("How many hours to work? ");

    if (hours < 0) {
        printf("Error: hours cannot be negative.\n");
        return;
    }

    current_hour = current_hour + hours;

    while (current_hour >= MAX_HOURS) {
        current_hour = current_hour - MAX_HOURS;
        current_day = current_day + 1;
    }

    printf("Passed %d h. Now: Day %d, %02d:00\n",
           hours, current_day, current_hour);
}

/* ===== 3. Показать инвентарь ===== */
void show_inventory(void) {
    printf("\n--- Inventory ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Slot %d: [%d] (%s)\n",
               i, inventory[i], get_item_name(inventory[i]));
    }
}

/* ===== 4. Положить предмет ===== */
void put_item(void) {
    int index = read_number("Slot index (0-9): ");

    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Error: index must be 0-9.\n");
        return;
    }

    int id = read_number("Item ID (0-9): ");

    if (id < 0 || id > 9) {
        printf("Error: ID must be 0-9.\n");
        return;
    }

    inventory[index] = id;
    printf("Item [%d] (%s) placed in slot %d.\n",
           id, get_item_name(id), index);
}

/* ===== 5. Выбросить предмет ===== */
void drop_item(void) {
    int index = read_number("Slot index to clear (0-9): ");

    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Error: index must be 0-9.\n");
        return;
    }

    if (inventory[index] == ITEM_EMPTY) {
        printf("Slot %d is already empty.\n", index);
    } else {
        printf("Item [%d] (%s) dropped from slot %d.\n",
               inventory[index], get_item_name(inventory[index]), index);
        inventory[index] = ITEM_EMPTY;
    }
}

/* ===== 6. Вариант 9: Устранение дубликатов ===== */
void remove_duplicates(void) {
    printf("\n--- Before ---\n");
    show_inventory();

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == ITEM_EMPTY) {
            continue;
        }

        for (int j = i + 1; j < INVENTORY_SIZE; j++) {
            if (inventory[j] == inventory[i]) {
                inventory[j] = ITEM_EMPTY;
            }
        }
    }

    printf("\n--- After ---\n");
    show_inventory();
}

/* ===== Имя предмета по ID ===== */
const char* get_item_name(int id) {
    switch (id) {
        case ITEM_EMPTY:    return "Empty";
        case ITEM_WOOD:     return "Wood";
        case ITEM_STONE:    return "Stone";
        case ITEM_SEEDS:    return "Seeds";
        case ITEM_IRON:     return "Iron";
        case ITEM_GOLD:     return "Gold";
        case ITEM_WATER:    return "Water";
        case ITEM_FOOD:     return "Food";
        case ITEM_TOOL:     return "Tool";
        case ITEM_ARTIFACT: return "Artifact";
        default:            return "Unknown";
    }
}