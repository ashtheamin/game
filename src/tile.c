#ifndef LIBSDEFS_INCLUDED
#define LIBDEFS_INCLUDED
    #include "libsdefs.c"
#endif

enum TILESET {
    TILE_AIR, TILE_PLATFORM
};

struct tilemap {
    int** tileset;
    int num_rows;
    int num_columns;
    struct tilemap* next;
};

struct tilemap* tilemap_init(char* filename) {
    int rows = 0; size_t columns = 0;

    FILE* fp = fopen(filename, "r+");
    if (fp == NULL) {
        printf("Failed to open file '%s'. Exiting.\n", filename);
        return NULL;
    }

    char* buffer = malloc(sizeof(char) * 4096);

    size_t count = 0;
    while (fgets(buffer, 4096, fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (count == 0) columns = strlen(buffer);
        if (count != 0) {
            if (columns != strlen(buffer)) {
                free(buffer);
                printf("tilemap_init(): Inconsistent map dimensions. Exiting.\n");
                fclose(fp);
                return NULL;
            }
        }
        printf("%s", buffer);
        count++;
    }
    fclose(fp);
    free(buffer);

    printf("\nRows: %ld\n", count);
    printf("Columns: %ld\n\n", columns);

    free(buffer);
    if (rows < 1 || columns < 1) {
        printf("tilemap_init(): Rows and/or columns less than 1. Exiting.\n");
        return NULL;
    }

    struct tilemap* tilemap = malloc(sizeof(struct tilemap));
    if (tilemap == NULL) {
        printf("tilemap_init(): Failed to init tilemap. Exit.\n");
        return NULL;
    }

    tilemap->tileset = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        tilemap->tileset[i] = (int*)malloc(columns * sizeof(int));
    }

    tilemap->num_rows = rows;
    tilemap->num_columns = columns;
    tilemap->next = NULL;

    return tilemap;
}

void tilemap_free(struct tilemap* tilemap) {
    if (tilemap == NULL) return;
    for (int i=0; i < tilemap->num_rows; i++) {
        free(tilemap->tileset[i]);
    }
    free(tilemap->tileset);
    free(tilemap);
}