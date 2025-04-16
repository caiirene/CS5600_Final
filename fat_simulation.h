#ifndef FAT_SIMULATION_H
#define FAT_SIMULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 32
#define MAX_FILES 100
#define FREE 0
#define EOF_MARKER -1

// File entry structure
typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    int starting_cluster;
    int size_in_clusters;
} FileEntry;

// FAT filesystem structure
typedef struct {
    int* fat_table;
    int num_clusters;
    FileEntry files[MAX_FILES];
    int file_count;
} FATSystem;

// Function declarations
void initialize_fat(FATSystem* fs, int num_clusters);
int allocate_file(FATSystem* fs, const char* filename, int clusters_needed);
int delete_file(FATSystem* fs, const char* filename);
void display_fat(FATSystem* fs, FILE* output_file);
void list_files(FATSystem* fs, FILE* output_file);
void clean_up(FATSystem* fs);
int find_file_index(FATSystem* fs, const char* filename);

#endif /* FAT_SIMULATION_H */ 