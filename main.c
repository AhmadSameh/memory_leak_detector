#include "MLD.h"

typedef struct _student_ student_t;

struct _student_ {
    char name[32];
    uint32_t student_id;
    uint32_t student_age;
    float gpa;
    student_t* room_mate;
};

int main(int argc, char** argv) {
    struct_db_t* struct_db = calloc(1, sizeof(struct_db_t));

    field_info_t fields[] = {
        FIELD_INFO( student_t, name,        CHAR,    0),
        FIELD_INFO( student_t, student_id,  UINT32,  0),
        FIELD_INFO( student_t, student_age, UINT32,  0),
        FIELD_INFO( student_t, gpa,         FLOAT,   0),
        FIELD_INFO( student_t, room_mate,   OBJ_PTR, student_t),
    };

    REGISTER_STRUCT(struct_db, student_t, fields);

    object_db_t* object_db = calloc(1, sizeof(object_db_t));
    object_db->struct_db = struct_db;

    student_t* ahmad = c_allocate(object_db, "student_t", 1);


    return 0;
}