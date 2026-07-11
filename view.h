
#include <stdio.h>

#ifndef VIEW_H
#define VIEW_H

#include "types.h"

// Structure used during MP3 tag viewing operation
typedef struct
{
    char *src_fname;            // Source MP3 file name
    FILE *fptr_src_file;        // File pointer for MP3 file
    char signature[4];          // Stores ID3 signature
    char version[3];            // Stores version information

} viewinfo;

/* Function to identify the selected operation */
OperationType check_operation_type(char *argv[]);

/* Reads and validates command line arguments */
Status read_and_validate(char *argv[], viewinfo *viewData);

/* Performs view operation */
Status do_viewing(viewinfo *viewData);

/* Opens the source MP3 file */
Status open_src_file(viewinfo *viewData);

/* Verifies ID3 signature and version */
Status validate_signature_and_version(viewinfo *viewData);

/* Displays the extracted tag information */
Status print_data(viewinfo *viewData);

/* Converts big-endian value into little-endian format */
void con_big_to_small_endian(int *size);

#endif