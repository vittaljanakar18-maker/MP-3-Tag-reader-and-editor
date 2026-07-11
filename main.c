
/* Name: Vittal Janakar
   Batch: Int26001B
   Title of the Project: MP3 Tag Reader and Editor.
   Date: 07 June 2026
   Description: MP3 Tag Reader and Editor is a C-based application that allows users to
               view and modify metadata stored in MP3 files. It supports reading and editing ID3 tags such as song title,
               artist name, album name, year, genre, and composer using file handling and binary file operations.
 */
#include <stdio.h>
#include <string.h>
#include "view.h"
#include "edit.h"
#include "types.h"

// Displays valid command line formats
static void display_usage(void)
{
    printf("\n[!] Invalid arguments\n");
    printf("for Viewing the tags\n");
    printf("./a.out -v file.mp3\n\n");
    printf("for Editing the tags\n");
    printf("./a.out -e <flag> \"New Name\" file.mp3\n");
    printf("flags: -A  -C  -y  -a  -t  -c\n\n");
}

int main(int argc, char *argv[])
{
    OperationType operation;

    // Identify the requested operation
    operation = check_operation_type(argv);

    switch (operation)
    {
        case e_view:
        {
            viewinfo viewData;

            printf("View Mode Selected\n");

            // Validate view mode arguments
            if (read_and_validate(argv, &viewData) != e_success)
            {
                printf("Argument Validation Failed\n");
                return e_failure;
            }

            printf("Arguments Validated Successfully\n");

            // Display MP3 tag details
            if (do_viewing(&viewData) != e_success)
            {
                printf("Unable to Display MP3 Information\n");
                return e_failure;
            }

            printf("MP3 Details Displayed Successfully\n");
            break;
        }

        case e_edit:
        {
            editinfo editData;

            printf("Edit Mode Selected\n");

            // Validate edit mode arguments
            if (read_and_validate1(argv, &editData) != e_success)
            {
                printf("Argument Validation Failed\n");
                return e_failure;
            }

            printf("Arguments Validated Successfully\n");

            // Perform tag modification
            if (do_editing(&editData) != e_success)
            {
                printf("Editing Operation Failed\n");
                return e_failure;
            }

            printf("Editing Completed Successfully\n");
            break;
        }

        default:
            // Invalid command entered
            display_usage();
            return e_failure;
    }

    return e_success;
}

// Determines whether the user selected view or edit mode
OperationType check_operation_type(char *argv[])
{
    if (argv[1] == NULL)
    {
        return e_unsupported;
    }

    // View operation
    if (!strcmp(argv[1], "-v"))
    {
        return e_view;
    }
    // Edit operation
    else if (!strcmp(argv[1], "-e"))
    {
        return e_edit;
    }

    return e_unsupported;
}