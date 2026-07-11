#include <stdio.h>
#include "types.h"
#ifndef EDIT_H
#define EDIT_H



/* Structure used for MP3 tag editing operations */
typedef struct
{
    char modify_tag[5];         // Stores the frame/tag to be modified
    FILE *fptr1_src_file;       // Source MP3 file pointer
    char *src_fname1;           // Source MP3 file name

    FILE *fptr_temp_file;       // Temporary file pointer

    char signature[4];          // ID3 signature
    char version[3];            // ID3 version information

    char *new_name;             // New value entered by user

} editinfo;

/* Reads and validates edit operation arguments */
Status read_and_validate1(char *argv[], editinfo *editData);

/* Performs the complete edit operation */
Status do_editing(editinfo *editData);

/* Opens source and temporary files */
Status open_files(editinfo *editData);

/* Verifies MP3 signature and version */
Status validate_signature_and_version1(editinfo *editData);

/* Updates the selected frame data */
Status edit_data(editinfo *editData);

/* Copies remaining file contents */
void copy_remaining_data(editinfo *editData);

/* Deletes the original MP3 file */
Status remove_original_file(editinfo *editData);

/* Renames temporary file to original file name */
Status rename_temp_file_as_source_file(editinfo *editData);

#endif