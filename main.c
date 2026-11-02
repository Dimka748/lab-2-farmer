#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

/* ===== Константы ===== */
#define INVENTORY_SIZE 10
#define MAX_HOURS 24
#define START_DAY 1
#define START_HOUR 8
#define MAX_FARMER_NAME 32
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32

#define MAP_MIN_SIZE 5
#define MAP_MAX_SIZE 30
#define TREE_CHANCE 20
#define STONE_CHANCE 10

/* ===== Глобальные переменные ===== */
int current_day = START_DAY;
int current_hour = START_HOUR;
char farmer_name[MAX_FARMER_NAME] = ""; 
char item_names[MAX_ITEMS][MAX_NAME_LEN]; 

int inventory[INVENTORY_SIZE] = {
    1, 2, 3, 0, 4,
    1, 0, 5, 0, 7
};

/* ===== Стандартные названия ===== */
void set_default_items(void) {
    const char *def[MAX_ITEMS] = {
        "Пусто", "Дерево", "Камень", "Семена_пшеницы", "Железо",
        "Золото", "Вода", "Еда", "Инструмент", "Артефакт"
    };
    for (int i = 0; i < MAX_ITEMS; i++) {
        strncpy(item_names[i], def[i], MAX_NAME_LEN - 1);
        item_names[i][MAX_NAME_LEN - 1] = '\0';
    }
}

/* ===== Загрузка items.txt ===== */
void load_items(void) {
    FILE *f = fopen("items.txt", "r");
    if (f == NULL) {
        printf("Предупреждение: items.txt не найден. Используем стандартные названия.\n");
        set_default_items();
        return;
    }

    char line[MAX_NAME_LEN];
    int id = 0;
    while (id < MAX_ITEMS && fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;
        strncpy(item_names[id], line, MAX_NAME_LEN - 1);
        item_names[id][MAX_NAME_LEN - 1] = '\0';
        id++;
    }
    fclose(f);

    for (; id < MAX_ITEMS; id++) {
        snprintf(item_names[id], MAX_NAME_LEN, "Предмет_%d", id);
    }
}

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
void load_items(void);
void set_default_items(void);
void find_item_by_name(void);
void write_diary(void);
void process_schedule(void);

// Лаба 4
void start_adventure(void);
char** create_map(int height, int width);
void free_map(char** map, int height);
void generate_map(char** map, int height, int width);
void draw_map(char** map, int height, int width, int hero_x, int hero_y, int shout_turns);
void game_loop(char** map, int height, int width, int start_x, int start_y);

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
    srand(time(NULL));

    printf("Введите имя фермера: ");
    read_string("", farmer_name, MAX_FARMER_NAME);

    if (strlen(farmer_name) == 0) {
        strcpy(farmer_name, "Безымянный");
    }

    load_items();

    int choice;
    do {
        show_menu();
        choice = read_number("Ваш выбор: ");
        switch (choice) {
            case 0: printf("Выход. Пока, %s!\n", farmer_name); break;
            case 1: show_time(); break;
            case 2: work_time(); break;
            case 3: show_inventory(); break;
            case 4: put_item(); break;
            case 5: drop_item(); break;
            case 6: remove_duplicates(); break;
            case 7: find_item_by_name(); break;
            case 8: write_diary(); break;
            case 9: process_schedule(); break;
            case 10: start_adventure(); break;
            default: printf("Неверный пункт.\n");
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
    printf("[6] Устранение дубликатов\n");
    printf("[7] Поиск предмета по названию\n");
    printf("[8] Записать в дневник фермера\n");
    printf("[9] Расписание жителей деревни\n");
    printf("[10] Отправиться в приключение\n");
    printf("===========================\n");
}

/* ===== Ввод числа ===== */
int read_number(const char *text) {
    char buffer[32];
    while (1) {
        printf("%s", text);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            int value;
            if (sscanf(buffer, "%d", &value) == 1) {
                return value;
            } else {
                printf("Ошибка! Введите число.\n");
            }
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
    current_hour += hours;
    while (current_hour >= MAX_HOURS) {
        current_hour -= MAX_HOURS;
        current_day += 1;
    }
    printf("Прошло %d ч. Теперь: День %d, %02d:00\n", hours, current_day, current_hour);
}

/* ===== 3. Показать инвентарь ===== */
void show_inventory(void) {
    printf("\n--- Инвентарь ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i]));
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
    printf("Предмет [%d] (%s) положен в слот %d.\n", id, get_item_name(id), index);
}

/* ===== 5. Выбросить предмет ===== */
void drop_item(void) {
    int index = read_number("Индекс слота для очистки (0-9): ");
    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Ошибка: индекс должен быть от 0 до 9.\n");
        return;
    }
    if (inventory[index] == 0) {
        printf("Слот %d уже пуст.\n", index);
    } else {
        printf("Предмет [%d] (%s) выброшен из слота %d.\n", inventory[index], get_item_name(inventory[index]), index);
        inventory[index] = 0;
    }
}

/* ===== 6. Устранение дубликатов ===== */
void remove_duplicates(void) {
    printf("\n--- До обработки ---\n");
    show_inventory();
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == 0) continue;
        for (int j = i + 1; j < INVENTORY_SIZE; j++) {
            if (inventory[j] == inventory[i]) inventory[j] = 0;
        }
    }
    printf("\n--- После обработки ---\n");
    show_inventory();
}

/* ===== 7. Поиск предмета по названию ===== */
void find_item_by_name(void) {
    char query[MAX_NAME_LEN];
    read_string("Введите название предмета: ", query, MAX_NAME_LEN);

    int found_id = -1;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(item_names[i], query) == 0) {
            found_id = i;
            break;
        }
    }

    if (found_id == -1) {
        printf("Предмет '%s' не найден в каталоге.\n", query);
        return;
    }

    printf("ID предмета '%s' = %d. Ищем в рюкзаке...\n", query, found_id);
    int count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == found_id) {
            printf("  Найдено в слоте %d\n", i);
            count++;
        }
    }
    if (count == 0) printf("В рюкзаке такого предмета нет.\n");
    else printf("Всего найдено: %d\n", count);
}

/* ===== 8. Дневник фермера ===== */
void write_diary(void) {
    FILE *f = fopen("diary.txt", "a");
    if (f == NULL) {
        printf("Ошибка: не удалось открыть diary.txt.\n");
        return;
    }
    fprintf(f, "=== День %d, %02d:00 ===\n", current_day, current_hour);
    fprintf(f, "Фермер: %s\n", farmer_name);
    fprintf(f, "Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fprintf(f, "  Слот %d: [%d] - %s\n", i, inventory[i], get_item_name(inventory[i]));
    }
    fprintf(f, "\n");
    fclose(f);
    printf("Запись добавлена в diary.txt\n");
}

/* ===== 9. Вариант 7: Расписание ===== */
void process_schedule(void) {
    FILE *fin = fopen("input.txt", "r");
    if (fin == NULL) {
        printf("Ошибка: input.txt не найден.\n");
        return;
    }
    FILE *fout = fopen("output.txt", "w");
    if (fout == NULL) {
        printf("Ошибка: не удалось создать output.txt.\n");
        fclose(fin);
        return;
    }

    char all_npcs[MAX_ITEMS][MAX_NAME_LEN];
    int all_count = 0;

    char active_npcs[MAX_ITEMS][MAX_NAME_LEN];
    char active_places[MAX_ITEMS][MAX_NAME_LEN];
    int active_count = 0;

    char line[256];

    while (fgets(line, sizeof(line), fin)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        char name[MAX_NAME_LEN], place[MAX_NAME_LEN];
        int start_h = 0, end_h = 0;

        char *p1 = strchr(line, ';');
        if (!p1) continue;
        *p1 = '\0';
        strncpy(name, line, MAX_NAME_LEN - 1);
        name[MAX_NAME_LEN - 1] = '\0';

        char *p2 = strchr(p1 + 1, ';');
        if (!p2) continue;
        *p2 = '\0';
        start_h = atoi(p1 + 1);

        char *p3 = strchr(p2 + 1, ';');
        if (!p3) continue;
        *p3 = '\0';
        end_h = atoi(p2 + 1);

        strncpy(place, p3 + 1, MAX_NAME_LEN - 1);
        place[MAX_NAME_LEN - 1] = '\0';

        int exists = 0;
        for (int i = 0; i < all_count; i++) {
            if (strcmp(all_npcs[i], name) == 0) { exists = 1; break; }
        }
        if (!exists && all_count < MAX_ITEMS) {
            strncpy(all_npcs[all_count], name, MAX_NAME_LEN - 1);
            all_count++;
        }

        int in_shift = 0;
        if (start_h <= end_h) {
            if (current_hour >= start_h && current_hour < end_h) in_shift = 1;
        } else {
            if (current_hour >= start_h || current_hour < end_h) in_shift = 1;
        }

        if (in_shift) {
            int found = -1;
            for (int i = 0; i < active_count; i++) {
                if (strcmp(active_npcs[i], name) == 0) { found = i; break; }
            }
            if (found == -1 && active_count < MAX_ITEMS) {
                strncpy(active_npcs[active_count], name, MAX_NAME_LEN - 1);
                strncpy(active_places[active_count], place, MAX_NAME_LEN - 1);
                active_count++;
            } else if (found != -1) {
                strncpy(active_places[found], place, MAX_NAME_LEN - 1);
            }
        }
    }
    fclose(fin);

    printf("\n--- Расписание жителей (День %d, %02d:00) ---\n", current_day, current_hour);
    fprintf(fout, "Расписание жителей (День %d, %02d:00)\n", current_day, current_hour);

    for (int i = 0; i < all_count; i++) {
        int found = -1;
        for (int j = 0; j < active_count; j++) {
            if (strcmp(all_npcs[i], active_npcs[j]) == 0) { found = j; break; }
        }
        if (found != -1) {
            printf("%s: %s\n", all_npcs[i], active_places[found]);
            fprintf(fout, "%s: %s\n", all_npcs[i], active_places[found]);
        } else {
            printf("%s: [Спит дома]\n", all_npcs[i]);
            fprintf(fout, "%s: [Спит дома]\n", all_npcs[i]);
        }
    }

    fclose(fout);
    printf("\nРезультат сохранён в output.txt\n");
}

/* ===== Имя предмета ===== */
const char* get_item_name(int id) {
    if (id < 0 || id >= MAX_ITEMS) return "Неизвестно";
    return item_names[id];
}

/* ===== Карта =================================== */

char** create_map(int height, int width) {
    char **map = (char**)malloc(height * sizeof(char*));
    if (map == NULL) return NULL;
    for (int i = 0; i < height; i++) {
        map[i] = (char*)malloc(width * sizeof(char));
        if (map[i] == NULL) return NULL;
    }
    return map;
}

void free_map(char** map, int height) {
    if (map == NULL) return;
    for (int i = 0; i < height; i++) free(map[i]);
    free(map);
}

void generate_map(char** map, int height, int width) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                map[y][x] = '#';
            } else {
                int r = rand() % 100;
                if (r < TREE_CHANCE) map[y][x] = 'T';
                else if (r < TREE_CHANCE + STONE_CHANCE) map[y][x] = 'R';
                else map[y][x] = '.';
            }
        }
    }
}

void draw_map(char** map, int height, int width, int hero_x, int hero_y, int shout_turns) {
    printf("\n   ");
    for (int x = 0; x < width; x++) {
        printf("%d ", x % 10);
    }
    printf("\n");

    for (int y = 0; y < height; y++) {
        printf("%2d ", y);
        
        for (int x = 0; x < width; x++) {
            char cell = map[y][x];
            
            // Вычисляем расстояние от героя до текущей клетки (по модулю)
            int dist_x = abs(x - hero_x);
            int dist_y = abs(y - hero_y);
            
            // Радиус обычного зрения — 5 клеток
            int view_radius = 5;
            // Радиус крика — 7 клеток
            int shout_radius = 7;

            int is_visible = 0;

            // Если это сам герой — он всегда виден
            if (x == hero_x && y == hero_y) {
                is_visible = 1;
            }
            // Если мы в радиусе обычного зрения
            else if (dist_x <= view_radius && dist_y <= view_radius) {
                is_visible = 1;
            }
            // Если активен крик и мы в радиусе крика
            else if (shout_turns > 0 && dist_x <= shout_radius && dist_y <= shout_radius) {
                is_visible = 1;
            }

            if (is_visible) {
                // Рисуем как обычно
                if (cell == '#') printf("\033[90m# \033[0m");
                else if (cell == 'T') printf("\033[32mT \033[0m");
                else if (cell == 'R') printf("\033[37mR \033[0m");
                else if (cell == '@') printf("\033[33m@ \033[0m");
                else printf(". ");
            } else {
                // Если не видно — рисуем туман
                printf("\033[34m? \033[0m"); // Синий вопросик
            }
        }
        printf("\n");
    }
    
    if (shout_turns > 0) {
        printf("\033[31m[КРИК БЕТМЕНА АКТИВЕН! Осталось ходов: %d]\033[0m\n", shout_turns);
    }
}

void game_loop(char** map, int height, int width, int start_x, int start_y) {
    int hero_x = start_x;
    int hero_y = start_y;
    char input;
    int shout_turns = 0; // Сколько ходов осталось кричать

    while (1) {
        // Передаем координаты героя и статус крика в отрисовку
        draw_map(map, height, width, hero_x, hero_y, shout_turns);
        
        printf("\nW/A/S/D - движение. R - Крик Бетмена. Q - выход.\n");
        printf("Ваш ход: ");
        
        input = getchar();
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        if (input == 'q' || input == 'Q') {
            printf("Вы сбежали домой!\n");
            break; 
        }

        if (input >= 'A' && input <= 'Z') input += 32;

        // Обработка крика
        if (input == 'r') {
            if (shout_turns == 0) {
                shout_turns = 3; // Активируем на 3 хода
                printf("\033[31mВы издали крик! Окрестности озарились светом!\033[0m\n");
            } else {
                printf("Вы уже кричите!\n");
            }
            continue; // Крик не считается ходом, ждем движения
        }

        int new_x = hero_x;
        int new_y = hero_y;

        if (input == 'w') new_y = hero_y - 1;
        else if (input == 's') new_y = hero_y + 1;
        else if (input == 'a') new_x = hero_x - 1;
        else if (input == 'd') new_x = hero_x + 1;
        else {
            printf("Неверная клавиша.\n");
            continue;
        }

        // Проверяем границы и препятствия
        if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height) {
            printf("Там край карты!\n");
        } else {
            char target_cell = map[new_y][new_x];
            if (target_cell == '.' || target_cell == ' ') {
                map[hero_y][hero_x] = '.';
                hero_x = new_x;
                hero_y = new_y;
                map[hero_y][hero_x] = '@';
                
                // Если крик был активен, уменьшаем счетчик (ход сделан)
                if (shout_turns > 0) {
                    shout_turns--;
                    if (shout_turns == 0) {
                        printf("\033[34mТьма снова сгущается...\033[0m\n");
                    }
                }
            } else {
                printf("Там препятствие (%c)!\n", target_cell);
            }
        }
    }
}

void start_adventure(void) {
    printf("\n--- ВЫ ОТПРАВИЛИСЬ В ПРИКЛЮЧЕНИЕ ---\n");
    
    int height = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);
    int width = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);
    
    printf("Сгенерирована карта размером %dx%d\n", height, width);

    char **map = create_map(height, width);
    if (map == NULL) return;

    generate_map(map, height, width);

    int hero_x, hero_y;
    int found = 0;
    while (found == 0) {
        hero_x = rand() % (width - 2) + 1;
        hero_y = rand() % (height - 2) + 1;
        if (map[hero_y][hero_x] == '.') found = 1;
    }
    
    map[hero_y][hero_x] = '@';

    game_loop(map, height, width, hero_x, hero_y);

    free_map(map, height);
    
    printf("\nНажмите Enter, чтобы вернуться в меню...\n");
    getchar(); 
}