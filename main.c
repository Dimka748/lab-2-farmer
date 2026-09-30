#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

/* ===== Константы ===== */
#define INVENTORY_SIZE 10
#define MAX_HOURS 24
#define START_DAY 1
#define START_HOUR 8
#define MAX_FARMER_NAME 32

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
char farmer_name[MAX_FARMER_NAME] = ""; 

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
void read_string(const char *prompt, char *buf, int size); 

/* ===== Ввод строки с защитой ===== */
void read_string(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';
    }
}
/* ===== Главная функция ===== */
int main(void) {

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice;

    printf("Введите имя фермера: ");
read_string("", farmer_name, MAX_FARMER_NAME);

/* Если имя пустое — ставим запасное */
if (strlen(farmer_name) == 0) {
    strcpy(farmer_name, "Безымянный");
}

    do {
        show_menu();
        choice = read_number("Ваш выбор: ");

        switch (choice) {
            case 0:
                printf("Выход. Пока!\n");
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
                printf("Неверный пункт. Попробуйте снова.\n");
        }
    } while (choice != 0);

    return 0;
}

/* ===== Меню ===== */
void show_menu(void) {
    printf("\n===== ВЕСЁЛЫЙ ФЕРМЕР: %s =====\n", farmer_name);
    printf("[0] Выход\n");
    printf("[1] Посмотреть на часы\n");
    printf("[2] Поработать (промотать время)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет\n");
    printf("[5] Выбросить предмет\n");
    printf("[6] Устранение дубликатов (вариант 9)\n");
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
            printf("Ошибка! Введите число.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }
}

/* ===== 1. Показать время ===== */
void show_time(void) {
    printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
}

/* ===== 2. Поработать ===== */
void work_time(void) {
    int hours = read_number("Сколько часов работать? ");

    if (hours < 0) {
        printf("Ошибка: часы не могут быть отрицательными.\n");
        return;
    }

    current_hour = current_hour + hours;

    while (current_hour >= MAX_HOURS) {
        current_hour = current_hour - MAX_HOURS;
        current_day = current_day + 1;
    }

    printf("Прошло %d ч. Теперь: День %d, %02d:00\n",
           hours, current_day, current_hour);
}

/* ===== 3. Показать инвентарь ===== */
void show_inventory(void) {
    printf("\n--- Инвентарь ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] (%s)\n",
               i, inventory[i], get_item_name(inventory[i]));
    }
}

/* ===== 4. Положить предмет ===== */
void put_item(void) {
    int index = read_number("Индекс слота (0-9): ");

    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Ошибка: индекс должен быть от 0 до 9.\n");
        return;
    }

    int id = read_number("ID предмета (0-9): ");

    if (id < 0 || id > 9) {
        printf("Ошибка: ID должен быть от 0 до 9.\n");
        return;
    }

    inventory[index] = id;
    printf("Предмет [%d] (%s) положен в слот %d.\n",
           id, get_item_name(id), index);
}

/* ===== 5. Выбросить предмет ===== */
void drop_item(void) {
    int index = read_number("Индекс слота для очистки (0-9): ");

    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Ошибка: индекс должен быть от 0 до 9.\n");
        return;
    }

    if (inventory[index] == ITEM_EMPTY) {
        printf("Слот %d уже пуст.\n", index);
    } else {
        printf("Предмет [%d] (%s) выброшен из слота %d.\n",
               inventory[index], get_item_name(inventory[index]), index);
        inventory[index] = ITEM_EMPTY;
    }
}

/* ===== 6. Вариант 9: Устранение дубликатов ===== */
void remove_duplicates(void) {
    printf("\n--- До обработки ---\n");
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

    printf("\n--- После обработки ---\n");
    show_inventory();
}

/* ===== Имя предмета по ID ===== */
const char* get_item_name(int id) {
    switch (id) {
        case ITEM_EMPTY:    return "Пусто";
        case ITEM_WOOD:     return "Дерево";
        case ITEM_STONE:    return "Камень";
        case ITEM_SEEDS:    return "Семена";
        case ITEM_IRON:     return "Железо";
        case ITEM_GOLD:     return "Золото";
        case ITEM_WATER:    return "Вода";
        case ITEM_FOOD:     return "Еда";
        case ITEM_TOOL:     return "Инструмент";
        case ITEM_ARTIFACT: return "Артефакт";
        default:            return "Неизвестно";
    }
}