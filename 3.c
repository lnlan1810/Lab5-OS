#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Проверяем, передано ли имя файла в аргументах командной строки
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Получаем имя файла из аргументов командной строки
    char *filename = argv[1];

    // Открываем файл для записи данных (режим "a" - добавление к существующему или создание нового файла)
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        // В случае ошибки при открытии файла выводим сообщение об ошибке и завершаем программу
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Печатаем приглашение для пользователя
    printf("Enter characters. To finish, press Ctrl-D (EOF):\n");

    int ch;
    // Считываем символы из стандартного ввода и записываем их в файл до достижения конца файла (EOF)
    while ((ch = getc(stdin)) != EOF) {
        // Пишем символ в файл
        if (fputc(ch, file) == EOF) {
            // В случае ошибки при записи символа выводим сообщение об ошибке, закрываем файл и завершаем программу
            perror("Error writing to file");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    // Закрываем файл после успешной записи данных
    if (fclose(file) == EOF) {
        // В случае ошибки при закрытии файла выводим сообщение об ошибке и завершаем программу
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    // Выводим сообщение об успешной записи данных в файл
    printf("Data successfully written to file %s\n", filename);

    // Завершаем программу с кодом возврата 0 (успех)
    return 0;
}
