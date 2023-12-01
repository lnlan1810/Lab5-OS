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
    struct stat st; //stat позволяют процессу определить значения свойств в существующем файле.
    
    if (stat(destinationFileName, &st) == 0 && !(st.st_mode & S_IWUSR))
    //stat(destinationFileName, &st) == 0: функция stat для получения информации о файле с именем DestinationFileName 
    // и сохранения этой информации в переменной st. Если функция выполняется успешно (возвращает 0), условие истинно.
    // !(st.st_mode & S_IWUSR): проверьте, установлено ли разрешение на запись для владельца файла или нет. 
    // Выражение (st.st_mode & S_IWUSR) вернет S_IWUSR (разрешение на запись для пользователя), 
    // если разрешение на запись было установлено, в противном случае оно вернет 0. 
    // Это условие будет истинным, если разрешение на запись для пользователя не установлено.
    {
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
