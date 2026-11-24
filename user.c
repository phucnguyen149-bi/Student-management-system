#include <stdio.h>
#include <string.h>
#include <time.h>
#include "user.h"
#include "unit.h"
#include "utils.h"
User users[MAX_USERS];
int user_count = 0;
//user_enabled
int is_user_enabled(int user_index) {
    if (user_index < 0 || user_index >= user_count) return 0;
    if (strcmp(users[user_index].status, "enabled") == 0) {
        return 1;
    }
    return 0;
}

int user_index(int input_id, User users[], int count){
    for(int i=0; i< count;i++){
        if( users[i].user_id == input_id)
           return i;
    }
    return -1;
}
//login
int login(const char *username, const char *password){
    for(int i=0;i < user_count; i++){
        if(strcmp(users[i].username,username)==0){
            if(strcmp(users[i].password,password)==0){
                if(strcmp(users[i].status,"enabled")==0){
                    return i;
                }else {
                    printf("[Function call: is_user_disable]\n");
                    return -1;
                }
            }else {
                printf("Wrong password.\n");
                return -1;
            }
        }
    }
    return -1;
}
//list enrolled unit
void list_my_enrolled_units(int student_index){
    if(users[student_index].num_enrolled_units == -1){
        printf("\n=== My Enrolled Units ===\n\n");
        printf("You are not enrolled in any units yet.\n"
               "You can enroll in up to 3 units.\n");
    } else {
        printf("\n=== My Enrolled Units ===\n");
        printf("%-5s %-10s %-35s %-10s\n", "No.", "Code", "Unit Name", "Score");
        printf("-------------------------------------------------------------\n");
        int total_score = 0;
        int graded_count = 0;
        for (int i= 0;i< users[student_index].num_enrolled_units;i++) {
            int unit_index= find_unit_by_code(users[student_index].enrolled_units[i].unit_code);
            if (unit_index != -1) {
                printf("%-5d %-10s %-35s %-10d\n",
                    i + 1,
                    users[student_index].enrolled_units[i].unit_code,
                    units[unit_index].unit_name, 
                    users[student_index].enrolled_units[i].score
                );
            }
        if(users[student_index].enrolled_units[i].score != -1) {
                total_score += users[student_index].enrolled_units[i].score;
                graded_count++;
            }
        }
        float GPA = (graded_count > 0) ? (float)total_score / graded_count : 0.0;
        printf("-------------------------------------------------------------\n");
        if(users[student_index].num_enrolled_units >= MAX_STUDENT_UNITS){
            printf("Enrolled: %d/%d (FULL - Cannot enroll in more units)\n",users[student_index].num_enrolled_units, MAX_STUDENT_UNITS);
        }else{
            printf("Enrolled: %d/%d (You can enroll in more units)\n",users[student_index].num_enrolled_units, MAX_STUDENT_UNITS);
        }
        printf("GPA: %.2f (Based on %d graded units)\n", GPA, graded_count);
        printf("\n");
    }
}
//check my scores
const char* get_grade(int score){
    if (score == -1) return "-";
    if (score >= 80) return "HD";
    return "D";
}

const char* get_status(int score){
    if (score == -1) return "Not graded";
    return "Graded";
}
void check_my_scores(int student_index, const char* unit_code){
    int total_score = 0;
    int graded_count = 0;
    int total_units = users[student_index].num_enrolled_units;
    if (total_units == 0) {
        printf("\nYou are not enrolled in any units yet.\n\n");
        return;
    }
    printf("\n=== My Scores ===\n\n");
    printf("%-10s %-35s %-10s %-10s %-12s\n", "Unit Code", "Unit Name", "Score", "Grade", "Status");
    printf("-----------------------------------------------------------------------------\n");
    for (int i = 0; i < total_units; i++) {
        char* code = users[student_index].enrolled_units[i].unit_code;
        int score = users[student_index].enrolled_units[i].score;
        int unit_idx = find_unit_by_code(code);
        if (unit_idx != -1) {
            printf("%-10s %-35s %-10d %-10s %-12s\n",
                   code,
                   units[unit_idx].unit_name,
                   score,
                   get_grade(score),
                   get_status(score));
            if(score != -1) {
                total_score += score;
                graded_count++;
            }
        }
    }
    printf("----------------------------------------------------------------------------\n");
    printf("Overall Performance:\n");
    float GPA = 0.0;
    int percent = 0;
    if (graded_count > 0) {
        GPA = (float)total_score / graded_count;
        percent = (int)((float)graded_count / total_units * 100);
    }
    printf("Graded Units: %d/%d (%d%%)\n", graded_count, total_units, percent);
    printf("Average score: %.2f\n", GPA);
    printf("GPA: %s\n", get_grade((int)GPA));
}
//generate random score
int generate_random_score(int student_index,const char* unit_code){
    for (int i = 0;i< users[student_index].num_enrolled_units; i++){
        if (strcmp(users[student_index].enrolled_units[i].unit_code, unit_code) == 0) {
            printf("\nWARNING: This is for TESTING only!\n");
            if(users[student_index].enrolled_units[i].score == -1){
                printf("Current score: Not graded\n");
            } else {
                printf("Current score: %d\n", users[student_index].enrolled_units[i].score);
            }
            char confirm;
            printf("Are you sure you want to generate a random score? (y/n): ");
            scanf(" %c", &confirm);
            if(confirm == 'y' || confirm == 'Y') {
                int new_score = get_random_int(0, 100); 
                users[student_index].enrolled_units[i].score = new_score;
                printf("✓ Score generated: %d\n", new_score);
                return new_score;
            }else{
                printf("Operation cancelled.\n");
                return -1;
            }
        }
    }
    printf("Error: Unit code %s not found in your enrollments.\n", unit_code);
    return -1;
}
int enrolled_table(int student_index) {
    printf("\nYour enrolled units:\n");
    if (users[student_index].num_enrolled_units == 0) {
        printf("No units enrolled yet.\n");
        return -1;
    }
    for(int i = 0; i < users[student_index].num_enrolled_units; i++) {
        char* code = users[student_index].enrolled_units[i].unit_code;
        int score = users[student_index].enrolled_units[i].score;
        int unit_idx = find_unit_by_code(code);
        char* name = (unit_idx != -1) ? units[unit_idx].unit_name : "Unknown Unit";
        printf("\n%d. %s - %s (Score: ", i + 1, code, name);
        if(score == -1){
            printf("Not graded)\n"); 
        }else{
            printf("%d)\n", score);
        }
    }
    return 0;
}
