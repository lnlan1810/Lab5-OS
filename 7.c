#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

// Функция для поиска файлов в указанном каталоге с размером между min_size и max_size
void search_files(const char* path, int min_size, int max_size, FILE* output_file, int* count);

int main(int argc, char** argv) {
    // Проверка правильности использования программы
    if (argc != 5) {
        printf("Usage: %s <directory> <min_size> <max_size> <output_file>\n", argv[0]);
        return 1;
    }

    // Получение параметров командной строки
    const char* directory = argv[1];
    int min_size = atoi(argv[2]); //берет целочисленное значение из argv[2] и присваивает его переменной min_size.
    int max_size = atoi(argv[3]);
    const char* output_file_name = argv[4];

    // Проверка на неотрицательность min_size и max_size
    if (min_size < 0 || max_size < 0) {
        printf("Error: min_size and max_size should be non-negative.\n");
        return 1;
    }

    // Открытие файла для записи результатов поиска
    FILE* output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
        printf("Error opening output file: %s\n", output_file_name);
        return 1;
    }

    int count = 0; // Счетчик найденных файлов
    search_files(directory, min_size, max_size, output_file, &count); // Вызов функции поиска файлов
    printf("Total files searched: %d\n", count); // Вывод общего количества найденных файлов

    // Закрытие файла
    if (fclose(output_file) != 0) {
        printf("Error closing output file.\n");
        return 1;
    }

    return 0;
}

// Рекурсивная функция поиска файлов в указанном каталоге и его подкаталогах
void search_files(const char* path, int min_size, int max_size, FILE* output_file, int* count) {
    DIR* dir = opendir(path); // Открытие каталога
    if (dir == NULL) {
        printf("Error opening directory.\n");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) { // Перебор элементов каталога
        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name); // Полный путь к текущему элементу

        struct stat st;
        if (lstat(full_path, &st) == -1) { // Получение информации о файле
            printf("Error getting file info for %s.\n", full_path);
            continue;
        }

        if (S_ISDIR(st.st_mode)) { // Если это каталог
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue; // Пропуск . и ..
            }
            search_files(full_path, min_size, max_size, output_file, count); // Рекурсивный вызов для подкаталога
        } else if (S_ISREG(st.st_mode)) { // Если это обычный файл
            if (st.st_size >= min_size && st.st_size <= max_size) {
                // Запись информации о файле в файл результатов
                fprintf(output_file, "%s\t%s\t%ld\n", full_path, entry->d_name, st.st_size);
                (*count)++; // Увеличение счетчика найденных файлов
            }
        }
    }

    closedir(dir); // Закрытие каталога
}
