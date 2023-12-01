#include <stdio.h>
#include <stdlib.h>

// Функция для отображения содержимого файла с паузой каждые N символов
void displayFile(char *filename, int N) {
    // Открываем файл в режиме чтения
    FILE *file = fopen(filename, "r");

    // Проверяем успешное открытие файла
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char c;
    int count = 0;

    // Цикл по каждому символу в файле
    while ((c = fgetc(file)) != EOF) {
        // Проверяем наличие ошибок при чтении файла
        if (ferror(file)) {
            perror("Error reading file");
            break;
        }

        // Выводим символ
        putchar(c);
        count++;

        // Если N больше 0 и count кратен N, делаем паузу и ждем Enter
        if (N > 0 && count % N == 0) {
            printf("\nPress Enter to continue...");
            int ch;

            // Ждем, пока не будет нажат Enter
            while ((ch = getchar()) != '\n' && ch != EOF) {
                // Проверяем ошибки при чтении из stdin
                if (ch == EOF) {
                    perror("Error reading from stdin");
                    break;
                }
            }
        }
    }

    // Закрываем файл
    if (fclose(file) != 0) {
        perror("Error closing file");
    }

    // Добавляем новую строку в конце файла, если ее еще нет
    if (count > 0 && c != '\n') {
        putchar('\n');
    }
}

// Основная функция
int main(int argc, char *argv[]) {
    // Проверяем, что передано правильное количество аргументов командной строки
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <N>\n", argv[0]);
        return 1;
    }

    // Извлекаем имя файла и N из аргументов командной строки
    char *filename = argv[1];
    int N = atoi(argv[2]);

    // Проверяем, что N - неотрицательное значение
    if (N < 0) {
        fprintf(stderr, "Invalid value for N\n");
        return 1;
    }

    // Вызываем функцию displayFile для отображения содержимого файла
    displayFile(filename, N);

    // Возвращаем 0, чтобы указать успешное выполнение программы
    return 0;
}
