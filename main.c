#include "fat_simulation.h"

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"
#define MAX_COMMAND_LENGTH 256

int process_command(FATSystem* fs, char* command, FILE* output_file);

/* Reads and processes FAT system commands from an input file */
int main() {
    FATSystem fs = {0}; // 初始化结构体所有字段为0/NULL
    char command[MAX_COMMAND_LENGTH];

    FILE* input_file = fopen(INPUT_FILE, "r");
    if (!input_file) {
        printf("Error: Could not open input file %s\n", INPUT_FILE);
        return 1;
    }

    FILE* output_file = fopen(OUTPUT_FILE, "w");
    if (!output_file) {
        printf("Error: Could not open output file %s\n", OUTPUT_FILE);
        fclose(input_file);
        return 1;
    }

    while (fgets(command, MAX_COMMAND_LENGTH, input_file)) {
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        if (strlen(command) == 0) {
            continue;
        }

        fprintf(output_file, "> %s\n", command);

        if (!process_command(&fs, command, output_file)) {
            fprintf(output_file, "Error processing command: %s\n", command);
        }

        fprintf(output_file, "\n");  // 每条命令输出后空一行
    }

    clean_up(&fs);
    fclose(input_file);
    fclose(output_file);

    return 0;
}

int process_command(FATSystem* fs, char* command, FILE* output_file) {
    char cmd[MAX_COMMAND_LENGTH];
    char arg1[MAX_FILENAME_LENGTH];
    int arg2;

    int args_read = sscanf(command, "%s %s %d", cmd, arg1, &arg2);
    if (args_read < 1) {
        fprintf(output_file, "Invalid command format\n");
        return 0;
    }

    if (strcmp(cmd, "init") == 0) {
        if (args_read != 2) {
            fprintf(output_file, "Usage: init <num_clusters>\n");
            return 0;
        }

        int num_clusters = atoi(arg1);
        if (num_clusters <= 0) {
            fprintf(output_file, "Number of clusters must be positive\n");
            return 0;
        }

        initialize_fat(fs, num_clusters);
        fprintf(output_file, "FAT initialized with %d clusters\n", num_clusters);
        return 1;
    } else if (strcmp(cmd, "alloc") == 0) {
        if (args_read != 3) {
            fprintf(output_file, "Usage: alloc <filename> <num_clusters>\n");
            return 0;
        }

        if (allocate_file(fs, arg1, arg2)) {
            fprintf(output_file, "File '%s' allocated with %d clusters\n", arg1, arg2);
            return 1;
        } else {
            fprintf(output_file, "Failed to allocate file '%s'\n", arg1);
            return 0;
        }
    } else if (strcmp(cmd, "delete") == 0) {
        if (args_read != 2) {
            fprintf(output_file, "Usage: delete <filename>\n");
            return 0;
        }

        if (delete_file(fs, arg1)) {
            fprintf(output_file, "File '%s' deleted\n", arg1);
            return 1;
        } else {
            fprintf(output_file, "Failed to delete file '%s'\n", arg1);
            return 0;
        }
    } else if (strcmp(cmd, "display") == 0) {
        display_fat(fs, output_file);
        return 1;
    } else if (strcmp(cmd, "list") == 0) {
        list_files(fs, output_file);
        return 1;
    } else {
        fprintf(output_file, "Unknown command: %s\n", cmd);
        return 0;
    }
}
