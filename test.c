#include <stdio.h>

int main(void) {
    char buffer[32];
    printf("Введите число: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        int value;
        if (sscanf(buffer, "%d", &value) == 1) {
            printf("Успех! Число = %d\n", value);
        } else {
            printf("Не удалось разобрать: [%s]\n", buffer);
        }
    } else {
        printf("fgets вернул NULL\n");
    }
    return 0;
}