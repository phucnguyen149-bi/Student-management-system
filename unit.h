#ifndef UNIT_H
#define UNIT_H

#include "user.h"

typedef struct {
    int unit_id;
    char unit_code[10];
    char unit_name[MAX_STRING_LENGTH];
    int capacity;
    int current_enrollment;
    int teacher_id;
} Unit;

extern Unit units[MAX_UNITS];
extern int unit_count;

void list_available_units(void);
int enroll_unit(int student_index, const char* unit_code);
int find_unit_by_code(const char* unit_code);
int get_unit_enrollment_count(const char* unit_code);
int generate_unit_id(void);
int unit_code_exists(const char* unit_code);

// Teacher functions
void list_teaching_units(int teacher_index);
int add_unit(int teacher_index);
int delete_unit(int teacher_index, const char* unit_code);
void list_students_in_unit(const char* unit_code);
void show_unit_statistics(const char* unit_code);

#endif