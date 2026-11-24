#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "user.h"
#include <stdlib.h>
#include <stdlib.h>

int get_random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}
void pause_screen(void) {
    printf("\nPress Enter to continue...");
    getchar();
    while (getchar() != '\n');
}
void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void to_uppercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}
void trim_whitespace(char* str) {
    char* end;
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return; // All spaces
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    // Write new null terminator
    *(end+1) = 0;
}

int is_valid_unit_code(const char* unit_code) {
    if (strlen(unit_code) != 7) return 0;
    for (int i = 0; i < 3; i++) {
        if (!isalpha(unit_code[i])) return 0;
    }
    for (int i = 3; i < 7; i++) {
        if (!isdigit(unit_code[i])) return 0;
    }
    return 1;
}
void drop_unit(int user_index) {
    char target_code[20];
    printf("\n=== DROP A UNIT ===\n");
    if (users[user_index].num_enrolled_units == 0) {
        printf("You are not enrolled in any units currently.\n");
        return;
    }

    printf("Enter the unit code you want to drop: ");
    scanf("%s", target_code);
    to_uppercase(target_code); 

    int found_index = -1;
    
    for (int i = 0; i < users[user_index].num_enrolled_units; i++) {
        if (strcmp(users[user_index].enrolled_units[i].unit_code, target_code) == 0) {
            found_index = i;
            break; 
        }
    }

    if (found_index == -1) {
        printf("Error: You are not enrolled in unit %s.\n", target_code);
        return;
    }

    char confirm;
    printf("Are you sure you want to drop %s? (y/n): ", target_code);
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        for (int i = found_index; i < users[user_index].num_enrolled_units - 1; i++) {
            users[user_index].enrolled_units[i] = users[user_index].enrolled_units[i + 1];
        }
    
        users[user_index].num_enrolled_units--;
        printf("✓ Successfully dropped unit %s.\n", target_code);
    } else {
        printf("Drop cancelled.\n");
    }
}