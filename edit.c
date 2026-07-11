#include <stdio.h>
#include <string.h>
#include "edit.h"
#include "types.h"

// Display usage information
static void print_usage(void)
{
    printf("Invalid Arguments!\n");
    printf("1. For View Operation: ./a.out -v filename.mp3\n");
    printf("2. For Edit Operation: ./a.out -e -A/-y/-C/-c/-t/-a \"New Name\" filename.mp3\n");
}

// Read and validate edit operation arguments
Status read_and_validate1(char *argv[], editinfo *editData)
{
    // Check which tag the user wants to modify
    if (argv[2] && !strcmp(argv[2], "-A"))
        strcpy(editData->modify_tag, "TALB");
    else if (argv[2] && !strcmp(argv[2], "-y"))
        strcpy(editData->modify_tag, "TYER");
    else if (argv[2] && !strcmp(argv[2], "-C"))
        strcpy(editData->modify_tag, "COMM");
    else if (argv[2] && !strcmp(argv[2], "-c"))
        strcpy(editData->modify_tag, "TCON");
    else if (argv[2] && !strcmp(argv[2], "-t"))
        strcpy(editData->modify_tag, "TIT2");
    else if (argv[2] && !strcmp(argv[2], "-a"))
        strcpy(editData->modify_tag, "TPE1");
    else
    {
        print_usage();
        return e_failure;
    }

    // Store the new value entered by user
    if (!argv[3])
    {
        print_usage();
        return e_failure;
    }

    editData->new_name = argv[3];

    // Verify that the source file is an MP3 file
    char *ext = (argv[4]) ? strchr(argv[4], '.') : NULL;

    if (!argv[4] || !ext || strcmp(ext, ".mp3"))
    {
        print_usage();
        return e_failure;
    }

    editData->src_fname1 = argv[4];

    return e_success;
}

// Performs the complete editing process
Status do_editing(editinfo *editData)
{
    // Open required files
    if (open_files(editData) != e_success)
        return e_failure;

    // Validate MP3 signature and version
    if (validate_signature_and_version1(editData) != e_success)
        return e_failure;

    // Modify the selected frame data
    if (edit_data(editData) != e_success)
        return e_failure;

    // Remove original file after editing
    if (remove_original_file(editData) != e_success)
        return e_failure;

    // Rename temporary file as original file
    if (rename_temp_file_as_source_file(editData) != e_success)
        return e_failure;

    return e_success;
}
// Open source and temporary files required for editing
Status open_files(editinfo *editData)
{
    // Open the original MP3 file in binary read mode
    editData->fptr1_src_file = fopen(editData->src_fname1, "rb");

    if (!editData->fptr1_src_file)
        return e_failure;

    // Create a temporary file to store updated data
    editData->fptr_temp_file = fopen("temp.mp3", "wb");

    if (!editData->fptr_temp_file)
        return e_failure;

    return e_success;
}

// Verify ID3 signature and version information
Status validate_signature_and_version1(editinfo *editData)
{
    fread(editData->signature, 3, 1, editData->fptr1_src_file);
    editData->signature[3] = '\0';

    fread(editData->version, 2, 1, editData->fptr1_src_file);
    editData->version[2] = '\0';

    // Check whether the file contains a valid ID3 tag
    return (!strcmp(editData->signature, "ID3")) ? e_success : e_failure;
}

// Modify the selected frame data in the MP3 file
Status edit_data(editinfo *editData)
{
    char header[10];

    // Copy ID3 header to temporary file
    rewind(editData->fptr1_src_file);
    fread(header, 10, 1, editData->fptr1_src_file);
    fwrite(header, 10, 1, editData->fptr_temp_file);

    // Search for the frame selected by the user
    while (1)
    {
        char tag[5];
        char size_bytes[4];
        char flags[2];

        if (fread(tag, 4, 1, editData->fptr1_src_file) != 1)
            return e_failure;

        tag[4] = '\0';

        // Check whether the current frame matches the requested frame
        if (!strcmp(tag, editData->modify_tag))
        {
            int old_size = 0;
            int new_size;

            fwrite(tag, 4, 1, editData->fptr_temp_file);

            fread(size_bytes, 4, 1, editData->fptr1_src_file);

            old_size |= ((unsigned char)size_bytes[0] << 24);
            old_size |= ((unsigned char)size_bytes[1] << 16);
            old_size |= ((unsigned char)size_bytes[2] << 8);
            old_size |= ((unsigned char)size_bytes[3]);

            // Calculate size of new content
            new_size = strlen(editData->new_name) + 1;

            size_bytes[0] = (new_size >> 24) & 0xFF;
            size_bytes[1] = (new_size >> 16) & 0xFF;
            size_bytes[2] = (new_size >> 8) & 0xFF;
            size_bytes[3] = new_size & 0xFF;

            fwrite(size_bytes, 4, 1, editData->fptr_temp_file);

            // Copy frame flags
            fread(flags, 2, 1, editData->fptr1_src_file);
            fwrite(flags, 2, 1, editData->fptr_temp_file);

            // Preserve text encoding byte
            char encoding;
            fread(&encoding, 1, 1, editData->fptr1_src_file);
            fwrite(&encoding, 1, 1, editData->fptr_temp_file);

            // Write updated tag content
            fwrite(editData->new_name,
                   strlen(editData->new_name),
                   1,
                   editData->fptr_temp_file);

            // Skip old frame content
            fseek(editData->fptr1_src_file, old_size - 1, SEEK_CUR);

            // Copy remaining MP3 data
            copy_remaining_data(editData);
            break;
        }
        else
        {
            int frame_size = 0;
            char byte;

            // Copy frames that are not being modified
            fwrite(tag, 4, 1, editData->fptr_temp_file);

            fread(size_bytes, 4, 1, editData->fptr1_src_file);
            fwrite(size_bytes, 4, 1, editData->fptr_temp_file);

            frame_size |= ((unsigned char)size_bytes[0] << 24);
            frame_size |= ((unsigned char)size_bytes[1] << 16);
            frame_size |= ((unsigned char)size_bytes[2] << 8);
            frame_size |= ((unsigned char)size_bytes[3]);

            fread(flags, 2, 1, editData->fptr1_src_file);
            fwrite(flags, 2, 1, editData->fptr_temp_file);

            // Copy frame data byte by byte
            while (frame_size--)
            {
                fread(&byte, 1, 1, editData->fptr1_src_file);
                fwrite(&byte, 1, 1, editData->fptr_temp_file);
            }
        }
    }

    return e_success;
}
// Copy all remaining data from source file to temporary file
void copy_remaining_data(editinfo *editData)
{
    char byte;

    while (fread(&byte, 1, 1, editData->fptr1_src_file))
    {
        fwrite(&byte, 1, 1, editData->fptr_temp_file);
    }
}

// Delete the original MP3 file
Status remove_original_file(editinfo *editData)
{
    return (remove(editData->src_fname1) == 0) ?
            e_success : e_failure;
}

// Rename temporary file with original file name
Status rename_temp_file_as_source_file(editinfo *editData)
{
    return (rename("temp.mp3",
                   editData->src_fname1) == 0) ?
            e_success : e_failure;
}