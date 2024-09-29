#ifndef MLD_H
#define MLD_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ERROR   -1
#define SUCCESS  0
#define THROW_ERROR(status)     { if(status == ERROR) { exit(EXIT_FAILURE); } }
#define ASSERT_PTR(condition)   { if(condition == 0)  { return NULL; } }
#define ASSER_VOID(condition)   { if(condition == 0)  { return; } }
#define ASSERT(condition)       { if(condition == 0)  { return ERROR; } }


#define MAX_STRUCT_NAME_LENGTH      128
#define MAX_FIELD_NAME_LENGTH       128

// macro to compute the offset of a field in a struct
#define OFFSET_OF(struct_name, field_name) (unsigned long int)&(((struct_name *)0)->field_name)
// macro to compute the size of a field in a struct
#define FIELD_SIZE(struct_name, field_name) sizeof(((struct_name *)0)->field_name)
// macro to set all field info in an array
#define FIELD_INFO(struct_name, field_name, field_type, nested_struct_name) \
    { #field_name, field_type, FIELD_SIZE(struct_name, field_name), OFFSET_OF(struct_name, field_name), #nested_struct_name }
// macro to register the struct to the struct database, a macro is used to make it easier for the user
#define REGISTER_STRUCT(struct_db, struct_name_, fields_arr)                            \
    {                                                                                   \
        struct_db_rec_t*  rec = (struct_db_rec_t*)calloc(1, sizeof(struct_db_rec_t));   \
        strncpy(rec->struct_name, #struct_name_, MAX_STRUCT_NAME_LENGTH);               \
        rec->struct_size = sizeof(struct_db_rec_t);                                     \
        rec->nfields = sizeof(fields_arr) / sizeof(field_info_t);                       \
        rec->fields = fields_arr;                                                       \
        THROW_ERROR( register_struct(struct_db, rec) );                                 \
    }                                                                                   \

// define types instead of importing libraries
typedef unsigned int uint32_t;
// typedef unsigned int size_t;
typedef unsigned char status_t;

// record of a struct to be allocated
typedef struct _struct_db_rec_ struct_db_rec_t;
// information about each field in the struct to be allocated
typedef struct _field_info_ field_info_t;
// used to store the head of the records
typedef struct _struct_db_ struct_db_t;
// used to store an object db record
typedef struct _object_db_rec_ object_db_rec_t;
// used to track the allocated objects
typedef struct _object_db_ object_db_t;

// used to identify the type of a struct field
typedef enum {
    UINT8, UINT16, UINT32, UINT64,
    SINT8, SINT16, SINT32, SINT64,
    FLOAT, DOUBLE,
    CHAR,
    OBJ_PTR,    // any pointer field which point to any struct
    OBJ_STRUCT
} data_type_t;

struct _struct_db_rec_ {
    struct_db_rec_t*    next;     // pointer to the next struct
    char                struct_name[MAX_STRUCT_NAME_LENGTH];
    size_t              struct_size;
    uint32_t            nfields;
    field_info_t*       fields;   // an array of the fields in the struct
};
struct _field_info_ {
    char        field_name[MAX_FIELD_NAME_LENGTH];
    data_type_t field_type;
    size_t      field_size;
    uint32_t    offset; // the offset bytes of the field within the struct
    char        nested_struct_name[MAX_STRUCT_NAME_LENGTH]; // in case the struct had a field that is a pointer to another struct
};

struct _struct_db_ {
    struct_db_rec_t* head;
    uint32_t        count;
};

struct _object_db_rec_ {
    object_db_rec_t*    next;
    void*               allocated_object; // a pointer to the allocated object
    uint32_t            units; // number of units the object malloced
    struct_db_rec_t*    struct_rec;
};

struct _object_db_ {
    struct_db_t*        struct_db;
    object_db_rec_t*    head;
    uint32_t            count;
};

status_t register_struct(struct_db_t* struct_db, struct_db_rec_t* rec);
void* c_allocate(object_db_t* object_db, char* struct_name, uint32_t units);

#endif