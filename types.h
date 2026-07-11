#ifndef TYPES_H
#define TYPES_H

/* Status codes returned by project functions */
typedef enum
{
    e_success = 0,
    e_failure
} Status;

/* Supported operation modes */
typedef enum
{
    e_view,
    e_edit,
    e_unsupported
} OperationType;

#endif