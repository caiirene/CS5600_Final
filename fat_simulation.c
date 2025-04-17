#include "fat_simulation.h"

/* Initializes the FAT system with a specified number of clusters */
void initialize_fat(FATSystem* fs, int num_clusters) {
    /* Allocate and mark all clusters as FREE */
    fs->fat_table = malloc(sizeof(int) * num_clusters);
    if (!fs->fat_table) {
        return;
    }
    for (int i = 0; i < num_clusters; i++) {
        fs->fat_table[i] = FREE;
    }
    fs->num_clusters = num_clusters;
    fs->file_count = 0;
}

/* Searches for a file by name and returns its index in the files array */
int find_file_index(FATSystem* fs, const char* filename) {
    for (int i = 0; i < fs->file_count; i++) {
        if (strcmp(fs->files[i].filename, filename) == 0) {
            return i;
        }
    }
    return -1;
}

/* Creates a new file by allocating required clusters in the FAT system */
int allocate_file(FATSystem* fs, const char* filename, int clusters_needed) {
    /* Reject duplicate filenames */
    if (find_file_index(fs, filename) != -1) {
        return 0;
    }
    /* Reject if file table full */
    if (fs->file_count >= MAX_FILES) {
        return 0;
    }
    /* Count free clusters */
    int free_count = 0;
    for (int i = 0; i < fs->num_clusters; i++) {
        if (fs->fat_table[i] == FREE) {
            free_count++;
        }
    }
    if (free_count < clusters_needed) {
        return 0;
    }
    /* Allocate clusters (first-fit, allow fragmentation) */
    int prev = -1;
    int first = -1;
    int allocated = 0;
    for (int i = 0; i < fs->num_clusters && allocated < clusters_needed; i++) {
        if (fs->fat_table[i] == FREE) {
            if (first == -1) {
                first = i;
            }
            if (prev != -1) {
                fs->fat_table[prev] = i;
            }
            prev = i;
            allocated++;
        }
    }
    /* Mark end of chain */
    fs->fat_table[prev] = EOF_MARKER;
    /* Record file entry */
    FileEntry* fe = &fs->files[fs->file_count];
    strncpy(fe->filename, filename, MAX_FILENAME_LENGTH - 1);
    fe->filename[MAX_FILENAME_LENGTH - 1] = '\0';
    fe->starting_cluster = first;
    fe->size_in_clusters = clusters_needed;
    fs->file_count++;
    return 1;
}

/* Removes a file from the FAT system and frees its allocated clusters */
int delete_file(FATSystem* fs, const char* filename) {
    int idx = find_file_index(fs, filename);
    if (idx == -1) {
        return 0;
    }
    /* Traverse chain and free clusters */
    int current = fs->files[idx].starting_cluster;
    while (current != EOF_MARKER) {
        int next = fs->fat_table[current];
        fs->fat_table[current] = FREE;
        if (next == EOF_MARKER) {
            break;
        }
        current = next;
    }
    /* Remove file entry by swapping with last */
    fs->files[idx] = fs->files[fs->file_count - 1];
    fs->file_count--;
    return 1;
}

/* Displays the FAT table to the specified output file */
void display_fat(FATSystem* fs, FILE* output_file) {
    fprintf(output_file, "FAT Table:\n");
    for (int i = 0; i < fs->num_clusters; i++) {
        fprintf(output_file, "Cluster %d: ", i);
        if (fs->fat_table[i] == FREE) {
            fprintf(output_file, "FREE\n");
        } else if (fs->fat_table[i] == EOF_MARKER) {
            fprintf(output_file, "EOF\n");
        } else {
            fprintf(output_file, "-> %d\n", fs->fat_table[i]);
        }
    }
}

/* Lists all files in the FAT system to the specified output file */
void list_files(FATSystem* fs, FILE* output_file) {
    fprintf(output_file, "Files:\n");
    for (int i = 0; i < fs->file_count; i++) {
        FileEntry* fe = &fs->files[i];
        fprintf(output_file, "Filename: %s, Starting Cluster: %d, Clusters: %d\n",
                fe->filename,
                fe->starting_cluster,
                fe->size_in_clusters);
    }
}

/* Cleans up the FAT system by freeing allocated memory */
void clean_up(FATSystem* fs) {
    free(fs->fat_table);
    fs->fat_table = NULL;
    fs->num_clusters = 0;
    fs->file_count = 0;
}
