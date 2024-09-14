#include "MLD.h"

status_t
register_struct(struct_db_t* struct_db, struct_db_rec_t* rec)
{
    ASSERT(rec != NULL);
    ASSERT(struct_db != NULL);
    // first insert
    if( struct_db->head == NULL ) {
        struct_db->head = rec;
        struct_db->count = 1;
        rec->next = NULL;
    } else {
        rec->next = struct_db->head;
        struct_db->head = rec;
        struct_db->count++;
    }
    return SUCCESS;
}

