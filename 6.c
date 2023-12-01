#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>

// Функция для вывода списка файлов и подкаталогов в указанном пути
void listDirectory(const char *path) {
    DIR *dir;
    struct dirent *entry; // Объявление указателя типа struct dirent для представления информации о элементах в каталоге

    // Открываем соединение с каталогом
    dir = opendir(path);

    // Проверяем ошибки при открытии каталога
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    // Выводим содержимое каталога
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            printf("%s\n", entry->d_name); 
        }
    } // Перебор элементов в каталоге и вывод имен тех, которые не начинаются с точки (то есть, не скрытые файлы), на экран.

    // Закрываем соединение с каталогом
    if (closedir(dir) == -1) {
        perror("Error closing directory");
        exit(EXIT_FAILURE);
    }
}

// Главная функция программы
int main(int argc, char *argv[]) {
    // Проверяем количество аргументов (путь к каталогу)
    if (argc < 2) {
        fprintf(stderr,"Usage: %s <path to directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Получаем путь к текущему каталогу
    char currentPath[PATH_MAX];
    if (getcwd(currentPath, sizeof(currentPath)) == NULL) {
        perror("Error getting current directory path");
        exit(EXIT_FAILURE);
    }

    // Выводим содержимое текущего каталога
    printf("Contents of the current directory:\n");
    listDirectory(currentPath);

    // Проверяем и выводим содержимое указанного каталога
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("Error opening specified directory");
        exit(EXIT_FAILURE);
    }
    closedir(dir);

    printf("\nContents of the specified directory (%s):\n", argv[1]);
    listDirectory(argv[1]);

    printf("\n");
    return 0;
}
