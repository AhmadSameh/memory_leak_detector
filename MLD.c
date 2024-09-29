#include "MLD.h"

static char *data_type[] = {"UINT8", "UINT16", "UINT32", "UINT64",
                            "SINT8", "SINT16", "SINT32", "SINT64",
                            "FLOAT", "DOUBLE",
                            "CHAR",
                            "OBJ_PTR",
                            "OBJ_STRUCT"};

static struct_db_rec_t*
struct_db_look_up(struct_db_t *struct_db, char *struct_name)
{
    ASSERT_PTR( struct_db != NULL );
    ASSERT_PTR( struct_name != NULL);
    struct_db_rec_t* record = struct_db->head;
    for( uint32_t i = 0; i < struct_db->count; i++ ) {
        if( strncmp(struct_name, record->struct_name, MAX_STRUCT_NAME_LENGTH) == 0 ) {
            return record;
        }
        record = record->next;
    }
    return NULL;
}

static object_db_rec_t *
object_db_look_up(object_db_t *object_db, void *ptr)
{
    object_db_rec_t* head = object_db->head;
    // if no object is allocated, return NULL
    if(head == NULL) {
        return NULL;
    }
    while(1) {
        if( head->allocated_object == ptr ) {
            return head->allocated_object;
        }
    }
    return NULL;
}

static void
add_object_to_object_db(object_db_t *object_db,
                        void *ptr,
                        int units,
                        struct_db_rec_t *struct_rec)
{
    object_db_rec_t* object_record = object_db_look_up(object_db, ptr);
    ASSER_VOID( object_record == NULL );

    object_record = calloc(1, sizeof(object_db_rec_t));
    object_record->allocated_object = ptr;
    object_record->units = units;
    object_record->struct_rec = struct_rec;

    if( object_db->head == NULL ) {
        object_record->next = NULL;
        object_db->head = object_record;
        object_db->count = 1;
        return;
    }

    object_record->next = object_db->head;
    object_db->head = object_record;
    object_db->count++;
    return;
}

status_t
register_struct(struct_db_t* struct_db, struct_db_rec_t* rec)
{
    ASSERT( rec != NULL );
    ASSERT( struct_db != NULL );
    // first insert
    if( struct_db->head == NULL ) {
        struct_db->head = rec;
        struct_db->count = 1;
        rec->next = NULL;
        return SUCCESS;
    }

    rec->next = struct_db->head;
    struct_db->head = rec;
    struct_db->count++;

    return SUCCESS;
}

void*
c_allocate( object_db_t* object_db,
            char* struct_name,
            uint32_t units)
{
    ASSERT_PTR( object_db != NULL );
    struct_db_rec_t* struct_record = struct_db_look_up(object_db->struct_db, struct_name);
    ASSERT_PTR( struct_record != NULL );
    void* allocated_ptr = calloc(1, sizeof(struct_record->struct_size));
    add_object_to_object_db(object_db, allocated_ptr, units, struct_record);
    return allocated_ptr;
}