#include <stdio.h>
#include <string.h>
#include "view.h"
#include "types.h"

// Validates the MP3 file name provided by the user
Status read_and_validate(char *argv[], viewinfo *viewData)
{
    if (argv[2] == NULL)
    {
        return e_failure;
    }

    if (strchr(argv[2], '.') &&
        strcmp(strchr(argv[2], '.'), ".mp3") == 0)
    {
        printf("MP3 file detected\n");
        viewData->src_fname = argv[2];
        return e_success;
    }

    printf("Invalid Arguments!\n");
    printf("1. For View Operation: ./a.out -v filename.mp3\n");
    printf("2. For Edit Operation: ./a.out -e -A/-y/-C/-c/-t/-a \"New Name\" filename.mp3\n");

    return e_failure;
}

// Controls the complete viewing process
Status do_viewing(viewinfo *viewData)
{
    // Open the source file
    if (open_src_file(viewData) != e_success)
    {
        printf("Unable to open source file\n");
        return e_failure;
    }

    printf("Source file opened successfully\n");

    // Verify ID3 information
    if (validate_signature_and_version(viewData) != e_success)
    {
        printf("Invalid MP3 signature/version\n");
        return e_failure;
    }

    printf("Signature verification successful\n");

    // Display tag details
    return print_data(viewData);
}

// Opens the MP3 file in read mode
Status open_src_file(viewinfo *viewData)
{
    viewData->fptr_src_file = fopen(viewData->src_fname, "rb");

    if (viewData->fptr_src_file == NULL)
    {
        return e_failure;
    }

    return e_success;
}

// Checks whether the file contains a valid ID3 header
Status validate_signature_and_version(viewinfo *viewData)
{
    fread(viewData->signature, 3, 1, viewData->fptr_src_file);
    viewData->signature[3] = '\0';

    fread(viewData->version, 2, 1, viewData->fptr_src_file);
    viewData->version[2] = '\0';

    printf("Signature : %s\n", viewData->signature);
    printf("Version   : %d\n", viewData->version[0]);

    if (strcmp(viewData->signature, "ID3") == 0)
    {
        return e_success;
    }

    return e_failure;
}

// Converts 4-byte big endian value into little endian
void con_big_to_small_endian(int *size)
{
    char *ptr = (char *)size;

    for (int i = 0; i < 2; i++)
    {
        char temp = ptr[i];
        ptr[i] = ptr[3 - i];
        ptr[3 - i] = temp;
    }
}

// Reads and prints supported MP3 tags
Status print_data(viewinfo *viewData)
{
    char frame_id[5];
    int frame_count = 6;

    rewind(viewData->fptr_src_file);
    fseek(viewData->fptr_src_file, 10, SEEK_SET);

    while (frame_count--)
    {
        int frame_size = 0;

        fread(frame_id, 4, 1, viewData->fptr_src_file);
        frame_id[4] = '\0';

        fread(&frame_size, 4, 1, viewData->fptr_src_file);

        // Convert frame size format
        con_big_to_small_endian(&frame_size);

        printf("Frame ID = %s  Size = %d\n", frame_id, frame_size);

        fseek(viewData->fptr_src_file, 3, SEEK_CUR);

        if (!strcmp(frame_id, "TPE1"))
            printf("Artist Name : ");
        else if (!strcmp(frame_id, "TIT2"))
            printf("Title       : ");
        else if (!strcmp(frame_id, "TALB"))
            printf("Album       : ");
        else if (!strcmp(frame_id, "TYER"))
            printf("Year        : ");
        else if (!strcmp(frame_id, "TCON"))
            printf("Genre       : ");
        else if (!strcmp(frame_id, "COMM"))
            printf("Composer    : ");
        else
        {
            fseek(viewData->fptr_src_file, frame_size, SEEK_CUR);
            continue;
        }

        // Print tag content
        for (int i = 0; i < frame_size - 1; i++)
        {
            putchar(fgetc(viewData->fptr_src_file));
        }

        printf("\n");
    }

    return e_success;
}