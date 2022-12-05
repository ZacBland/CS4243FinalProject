#ifndef FILE_DATA_HEADER
#define FILE_DATA_HEADER

typedef struct {
    int rows;
    int columns;
    char* refID;
    char*** file;
} file_data;

#endif