#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // Проверяем, было ли передано правильное количество аргументов командной строки (должно быть 3).
    if (argc == 3) {
        // Переменные для функции strtol, которая преобразует второй аргумент в длинное целое число.
        char *endptr;
        errno = 0;
        long N = strtol(argv[2], &endptr, 10);

        // Проверяем ошибки во время преобразования strtol.
        if ((errno == ERANGE && (N == LONG_MAX || N == LONG_MIN))
            || (errno != 0 && N == 0)) {
            perror("strtol");
            exit(EXIT_FAILURE);
        }

        // Проверяем, есть ли во втором параметре нечисловые символы.
        if (*endptr != '\0') {
            fprintf(stderr, "Invalid character in the second parameter\n");
            exit(EXIT_FAILURE);
        }

        // Проверяем, находится ли преобразованное значение в допустимом диапазоне.
        if (N < 0 || N > INT_MAX) {
            fputs("Second param must be between 0 and INT_MAX\n", stderr);
            exit(EXIT_FAILURE);
        }

        // Открываем файл, указанный в первом параметре, для чтения.
        FILE *file = fopen(argv[1], "r");

        // Проверяем ошибки при открытии файла.
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // Счетчик для отслеживания количества строк (используется, когда N > 0).
        int i = 0;
        int c;

        // Цикл по каждому символу в файле до его конца.
        while ((c = getc(file)) != EOF) {
            // Если N равно 0, выводим символы непрерывно.
            if (N == 0) {
                if (putc(c, stdout) == EOF) {
                    perror("Error writing to stdout");
                    exit(EXIT_FAILURE);
                }
            } else {
                // Если N больше 0, выводим символы группами по N строк.
                if (putc(c, stdout) == EOF) {
                    perror("Error writing to stdout");
                    exit(EXIT_FAILURE);
                }
                // Если встречен символ новой строки, увеличиваем счетчик строк.
                if (c == '\n' && N > 0) {
                    i++;
                    // Если счетчик достигает указанного размера группы (N), приглашаем пользователя продолжить.
                    if (i >= N) {
                        printf("\nPress Enter to continue...");
                        int ch;
                        // Ждем, пока пользователь нажмет Enter.
                        while ((ch = getc(stdin)) != '\n' && ch != EOF) {
                            // Обрабатываем ошибки ввода пользователя.
                            if (ch == EOF && ferror(stdin)) {
                                perror("Error reading from stdin");
                                exit(EXIT_FAILURE);
                            }
                        }
                        // Сбрасываем счетчик строк.
                        i = 0;
                    }
                }
            }
        }

        // Закрываем файл.
        if (fclose(file) != 0) {
            perror("Error closing file");
            exit(EXIT_FAILURE);
        }

        // Возвращаем 0 для указания успешного выполнения.
        return 0;
    } else {
        // Если количество аргументов командной строки неверное, выводим сообщение о использовании.
        printf("Usage: %s <filename> <group_size>\n", argv[0]);
        // Возвращаем EXIT_FAILURE для указания ошибки.
        return EXIT_FAILURE;
    }
}
