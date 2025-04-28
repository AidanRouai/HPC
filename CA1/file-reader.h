#ifndef FILE_READER_H
#define FILE_READER_H

int read_num_dims(const char *filename);
int *read_dims(const char *filename, int num_dims);
float *read_array(const char *filename, const int *dims, int num_dims);
void write_to_output_file(const char *filename, const float *array, const int *dims, int num_dims);

#endif // FILE_READER_H