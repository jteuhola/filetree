#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#define PATH_SIZE 256

void pop_item(char *arr, int index)
{
    for (int i = index; i < strlen(arr); i++)
    {
        arr[i] = arr[i + 1];
    }
}

void trim_string(char *path)
{
    int amount = 0;

    printf("trimming %s\n", path);

    for (int i = 0; i < PATH_SIZE; i++)
    {
        if (path[i] == '"')
        {
            amount++;
            pop_item(path, i);
        }
    }
    printf("amount: %d\n", amount);
    printf("len: %d", strlen(path));
}

void print_tree(const char *path, int depth, int max_depth)
{
    DIR *dir = opendir(path);
    if (!dir)
        return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;
        if (strcmp(entry->d_name, ".git") == 0)
        {
            printf("|- %s\n", entry->d_name);
            continue;
        }

        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("|- %s\n", entry->d_name);

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
        {
            if (depth < max_depth || max_depth == -1)
            {
                print_tree(full_path, depth + 1, max_depth);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    // argc = argument count
    // argv[] = all arguments as strings

    const char *path = ".";
    int max_depth = -1;

    // parse arguments:
    for (int i = 1; i < argc; i++)
    { // i0 = file name (skip it)
        if (strcmp(argv[i], "-v") == 0)
        {
            printf("filemori version 1.0\n");
            return 0;
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            printf("help");
            return 0;
        }
        if (strcmp(argv[i], "--d") == 0 && i + 1 < argc)
        {
            max_depth = atoi(argv[++i]);
        }
        else
        {
            path = argv[i];
        }
    }

    // header:
    printf("##############\n");
    printf("#  filemori  #\n");
    printf("##############\n\n");

    printf("depth: %d\n", max_depth);

    // trimString(path);

    printf("\npath: %s\n\n", path);

    print_tree(path, 0, max_depth);

    printf("ok");
    return 0;
}