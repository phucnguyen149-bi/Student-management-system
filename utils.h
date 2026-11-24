#ifndef UTILS_H
#define UTILS_H
void clear_screen(void);
void pause_screen(void);
int get_random_int(int min, int max);
// string processing 
void to_uppercase(char* str);
void trim_whitespace(char* str);
// valid check
int is_valid_username(const char* username);
int is_valid_unit_code(const char* unit_code);
void drop_unit(int user_index);
#endif