#ifndef USER_H
#define USER_H

#define MAX_USERS 100//limit 
#define MAX_UNITS 50
#define MAX_ENROLLMENTS 300
#define MAX_STUDENT_UNITS 3
#define MAX_TEACHER_UNITS 5
#define MAX_STRING_LENGTH 100
#define MAX_LINE_LENGTH 512

typedef struct {
    int user_id;
    char username[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
    char role[3];    // "TA" or "ST"
    char status[10]; // "enabled" or "disabled"
    
    char teaching_units[MAX_TEACHER_UNITS][10];
    int num_teaching_units;
    
    struct {
        char unit_code[10];
        int score; // -1 means no score yet
    } enrolled_units[MAX_STUDENT_UNITS];
    int num_enrolled_units;
} User;

//global variables
extern User users[MAX_USERS];
extern int user_count;

int login(const char* username, const char* password);
int is_user_enabled(int user_index);
void student_menu(int user_index);
void list_my_enrolled_units(int student_index);
void check_my_scores(int student_index, const char* unit_code);
int generate_random_score(int student_index, const char* unit_code);

#endif