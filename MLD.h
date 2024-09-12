#ifndef MLD_H
#define MLD_H

#include <stdint.h>
#include <stddef.h>

#define MAX_STRUCTURE_NAME_LENGTH   128
#define MAX_FIELD_NAME_LENGTH       128

typedef struct _struct_db_rec_ struct_db_rec_t;
typedef struct _field_info_ field_info_t;

// used to identify the type of a struct field
typedef enum {
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    SINT8,
    SINT16,
    SINT32,
    SINT64,
    CHAR,
    FLOAT,
    DOUBLE,
    OBJ_PTR,    // any pointer field which point to any struct
    OBJ_STRUCT
} data_type_t;

struct _struct_db_rec_ {
    struct_db_rec_t*    next;     /* pointer to the next struct */      
    char                struct_name[MAX_STRUCTURE_NAME_LENGTH];
    size_t              struct_size; 
    uint32_t            nfields;
    field_info_t*       fields; /* an array of the fields in the struct */
};

struct _field_info_ {
    char field_name[MAX_FIELD_NAME_LENGTH];
    size_t field_size;
    uint32_t offset; /* the offset bytes of the field within the struct */
    data_type_t field_type;
    char nested_struct_name[MAX_STRUCTURE_NAME_LENGTH]; /* in case the struct had a field that is a pointer to another struct */
};


#endif