#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// Функция для копирования содержимого одного файла в другой
void copyFile(const char *sourceFileName, const char *destinationFileName);

int main(int argc, char *argv[]) {
    // Проверка количества аргументов командной строки
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *sourceFileName = argv[1];
    const char *destinationFileName = argv[2];

    // Выполнение копирования файла
    copyFile(sourceFileName, destinationFileName);

    printf("File copy completed successfully.\n");
    return EXIT_SUCCESS;
}

// Функция для копирования содержимого одного файла в другой
void copyFile(const char *sourceFileName, const char *destinationFileName) {
    FILE *sourceFile, *destinationFile;
    int character;

    // Проверка существования и возможности чтения исходного файла
    if ((sourceFile = fopen(sourceFileName, "rb")) == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    // Проверка прав на чтение и открытие целевого файла для записи
    struct stat st;
    if (stat(destinationFileName, &st) == 0 && !(st.st_mode & S_IWUSR)) {
        fprintf(stderr, "Error: No write permission for the destination file\n");
        fclose(sourceFile);
        exit(EXIT_FAILURE);
    }
    if ((destinationFile = fopen(destinationFileName, "wb")) == NULL) {
        perror("Error opening destination file ");
        fclose(sourceFile);
        exit(EXIT_FAILURE);
    }

    // Копирование содержимого файла
    while ((character = fgetc(sourceFile)) != EOF) {
        if (fputc(character, destinationFile) == EOF) {
            perror("Error writing to the destination file");
            fclose(sourceFile);
            fclose(destinationFile);
            exit(EXIT_FAILURE);
        }
    }

    // Проверка ошибок записи
    if (ferror(destinationFile)) {
        perror("Error writing to the destination file");
        fclose(sourceFile);
        fclose(destinationFile);
        exit(EXIT_FAILURE);
    }

    // Закрытие исходного и целевого файлов
    if (fclose(sourceFile) == EOF || fclose(destinationFile) == EOF) {
        perror("Error closing files");
        exit(EXIT_FAILURE);
    }
}
