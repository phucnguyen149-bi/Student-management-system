#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include "file_io.h"
#include "user.h"
#include "unit.h"

// LOAD USERS
int load_users(const char* filename){
   FILE *f = fopen("../data/users.csv","r");
    if(f == NULL){
        return -1;   
}
    char line[MAX_LINE_LENGTH];
    
    if(f == NULL){
        printf("Error: Could not open file\n");
        return -1;   
    }
user_count = 0;

char title[200];
fgets(title,200,f);//skip headline
char temp[500];
while(fgets(temp, sizeof(temp),f)!= NULL){
    char *token=strtok(temp,",");
    if(token != NULL){
        users[user_count].user_id= atoi(token);
    }
    token=strtok(NULL,",");
    if(token != NULL){
        strcpy(users[user_count].username,token);
    }
    token=strtok(NULL,",");
    if(token != NULL){
        strcpy(users[user_count].password,token);
    }
    token=strtok(NULL,",");
    if(token != NULL){
        strcpy(users[user_count].role,token);
    }
    token=strtok(NULL,",");
    if(token != NULL){
        strcpy(users[user_count].status,token);
    }
    char *extra_data = strtok(NULL, "\n\r"); // Lấy toàn bộ chuỗi môn học còn lại
        
        users[user_count].num_enrolled_units = 0;
        users[user_count].num_teaching_units = 0;

        if(extra_data != NULL && strlen(extra_data) > 0){
            // Student
            if (strcmp(users[user_count].role, "ST") == 0){
                char *sub_token = strtok(extra_data, ";");
                while (sub_token != NULL){
                    char unit_code[20];
                    int score;
                    if(sscanf(sub_token, "%[^:]:%d", unit_code, &score) == 2){        
                        int current_index = users[user_count].num_enrolled_units;
                        strcpy(users[user_count].enrolled_units[current_index].unit_code, unit_code);
                        users[user_count].enrolled_units[current_index].score = score;
                        users[user_count].num_enrolled_units++;
                    }
                    sub_token = strtok(NULL, ";");
                }
            } 
            // Teacher
            else if (strcmp(users[user_count].role, "TA") == 0) {
                char *sub_token = strtok(extra_data, ";");
                while (sub_token != NULL && users[user_count].num_teaching_units < MAX_TEACHER_UNITS){
                    strcpy(users[user_count].teaching_units[users[user_count].num_teaching_units], sub_token);
                    users[user_count].num_teaching_units++;
                    sub_token = strtok(NULL, ";");
                }
            }
        }
        user_count++;
    }
    
    fclose(f);
    return user_count;
}
//load units
int load_units(const char* filename){
    FILE *f = fopen("../data/units.csv", "r");
    if(f == NULL){
        printf("Error: Could not open file\n");
        return -1;   
    }

unit_count=0;

char title[200];
fgets(title,200,f);
char temp[500];
while(fgets(temp, sizeof(temp),f)!= NULL){
    char *token=strtok(temp,",");
    if(token!=NULL){
    units[unit_count].unit_id=atoi(token);
    }
    token=strtok(NULL,",");
    if(token != NULL){
        strcpy(units[unit_count].unit_code,token);
    }
    token=strtok(NULL,",");
    if(token != NULL){
        strcpy(units[unit_count].unit_name,token);
    }
    token=strtok(NULL,",");
    if(token != NULL){
        units[unit_count].capacity=atoi(token);
    }
    token = strtok(NULL, ",\n\r");
    if(token != NULL){
        units[unit_count].teacher_id= atoi(token);
    }
    unit_count++;
}
fclose(f);
return unit_count;
}
// save users
int save_users(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Cannot save users to %s\n", filename);
        return -1;
    }

    fprintf(fp, "user_id,username,password,role,status,additional_data\n");

    for (int i = 0; i < user_count; i++) {
        char extra[MAX_LINE_LENGTH] = "";

        if (strcmp(users[i].role, "TA") == 0) {
            for (int j = 0; j < users[i].num_teaching_units; j++) {
                strcat(extra, users[i].teaching_units[j]);
                if (j < users[i].num_teaching_units - 1) strcat(extra, ";");
            }
        } else if (strcmp(users[i].role, "ST") == 0) {
            char temp[50];
            for (int j = 0; j < users[i].num_enrolled_units; j++) {
                sprintf(temp, "%s:%d", users[i].enrolled_units[j].unit_code, users[i].enrolled_units[j].score);
                strcat(extra, temp);
                if (j < users[i].num_enrolled_units - 1) strcat(extra, ";");
            }
        }
        fprintf(fp, "%d,%s,%s,%s,%s,%s\n",
                users[i].user_id, users[i].username, users[i].password,
                users[i].role, users[i].status, extra);
    }
    fclose(fp);
    return user_count;
}
//SAVE UNIT
int save_units(const char* filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Cannot save units to %s\n", filename);
        return -1;
    }

    fprintf(fp, "unit_id,unit_code,unit_name,capacity,teacher_id\n");

    for (int i = 0; i < unit_count; i++) {
        fprintf(fp, "%d,%s,%s,%d,%d\n",
                units[i].unit_id,
                units[i].unit_code,
                units[i].unit_name,
                units[i].capacity,
                units[i].teacher_id);
    }
    fclose(fp);
    return unit_count;
}



