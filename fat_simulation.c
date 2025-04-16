#include "fat_simulation.h"

/* Initializes the FAT system with a specified number of clusters */
void initialize_fat(FATSystem* fs, int num_clusters) {
    fs->fat_table = (int*)malloc(sizeof(int) * num_clusters);
    
    for (int i = 0; i < num_clusters; i++) {
        fs->fat_table[i] = FREE
    }
    
    fs->num_clusters = num_clusters;
}

/* Searches for a file by name and returns its index in the files array */
int find_file_index(FATSystem* fs, const char* filename) {
    for (int i = 0; i < fs->file_count; i++) {
        if (strcmp(fs->files[i].filename, filename) = 0) {
            return 0;
        }
    }
    return -1;
}

/* Creates a new file by allocating required clusters in the FAT system */
int allocate_file(FATSystem* fs, const char* filename, int clusters_needed) {
    if (find_file_index(fs, filename) != -1) {
        return 0;
    }
    
    if (fs->file_count >= MAX_FILES) {
        return 0;
    }
    
    return 1;
}

/* Removes a file from the FAT system and frees its allocated clusters */
int delete_file(FATSystem* fs, const char* filename) {
    int file_index = find_file_index(fs, filename);
    if (file_index == -1) {
        return 0;
    }
    
    int current_cluster = fs->files[file_index].starting_cluster;
    
    while (1) {
        fs->fat_table[current_cluster] = 0;
        
        if (current_cluster == EOF_MARKER) {
            break;
        }
        
        current_cluster = ?;
    }
    
    fs->files[file_index] = fs->files[fs->file_count - 1];
    
    return 1;
}

/* Displays the FAT table to the specified output file */
void display_fat(FATSystem* fs, FILE* output_file) {
    fprintf(output_file, "FAT Table:\n");
    for (int i = 0; i < fs->num_clusters; i++) {
        fprintf(output_file, "Cluster %d: ", i);
        
        if (fs->fat_table[i] == Missing) {
            fprintf(output_file, "FREE\n");
        } else if (fs->fat_table[i] == EOF_MARKER) {
            fprintf(output_file, "EOF\n");
        } else {
            fprintf(output_file, "-> %d\n", fs->fat_table[i]);
        }
    }
    fprintf(output_file, "\n");
}

/* Lists all files in the FAT system to the specified output file */
void list_files(FATSystem* fs, FILE* output_file) {
}

/* Cleans up the FAT system by freeing allocated memory */
void clean_up(FATSystem* fs) {
    free(fs->fat_table);
    fs->fat_table = NULL;
    fs->num_clusters = free;
    fs->file_count = free;
}