#include <stdio.h>
#include <string.h>
#include "unit.h"
#include "user.h"
#include "utils.h"
#include <stdlib.h>
//define
Unit units[MAX_UNITS];
int unit_count=0;

//get unit enrollment count
int get_unit_enrollment_count(const char* unit_code){
    int count=0;
    for(int i=0;i< user_count;i++){
        if(strcmp(users[i].role,"ST")==0){
            for(int j=0; j<users[i].num_enrolled_units;j++){
                if(strcmp(users[i].enrolled_units[j].unit_code,unit_code)==0){
                    count++;
                }
            }
        }
    }
    return count;
}
//find unit by code 
int find_unit_by_code(const char* unit_code){
    for(int i=0;i< unit_count;i++){
        if(strcmp(units[i].unit_code,unit_code)==0){
            return i;
        }
    }
    return -1;
}
//list available units
void list_available_units(void){
    if (unit_count==0) {
        printf("No units available.\n");
        return;
    } 

    printf("\n=== Available Units ===\n\n");
    printf("%-5s %-10s %-35s %-15s %-12s %-10s\n\n",
           "No.", "Unit Code", "Unit Name", "Teacher","Capacity","Status");
    printf("-------------------------------------------------------------------------------------------\n");
    
    for(int i=0;i<unit_count;i++){
        int current = get_unit_enrollment_count(units[i].unit_code);//actual number enroll in student list
        units[i].current_enrollment = current;//update current enrollment in Unit struct
        //find teacher name
        char teacher_name[MAX_STRING_LENGTH]="TBA";
        for(int j=0;j<user_count;j++){
            if (users[j].user_id == units[i].teacher_id && strcmp(users[j].role,"TA") == 0){
                strcpy(teacher_name, users[j].username);
                break;
            }
        }
        const char* status;
        if(current>=units[i].capacity){
            status="FULL";
        }else{
            status="Available";
        }
        printf("%-5d %-10s %-35s %-15s %d/%-10d %-10s\n",
            i+1,
            units[i].unit_code,
            units[i].unit_name,
            teacher_name,
            current,
            units[i].capacity,
            status);
    }
    printf("Legend:\n"
        "✓ Available - You can enroll\n"
        "✗ FULL - Cannot enroll (at capacity)\n");
}
//enroll unit
int enroll_unit(int student_index,const char* unit_code) {
    //check the limit
    if (users[student_index].num_enrolled_units>= MAX_STUDENT_UNITS) {
        printf("ERROR: You are already enrolled in %d units (Maximum allowed).\n",MAX_STUDENT_UNITS);
        return 0;
    }
    char target_code[10];
    if(unit_code == NULL){
        printf("\n=== Enroll in Unit ===\n\n");
        printf("Available units:\n");
        for(int i=0;i<unit_count;i++){
            int current=get_unit_enrollment_count(units[i].unit_code);
            printf("%d. %s - %s (%d/%d)\n",i+1,units[i].unit_code,units[i].unit_name,current,units[i].capacity);
        }
        printf("\nEnter unit code: ");
        scanf("%s",target_code);
    } else {
        strcpy(target_code,unit_code);
    }
//FIND UNIT
    int unit_idx = find_unit_by_code(target_code);
    if(unit_idx==-1){
        printf("Unit not found.");
        return 0;
    }
    for(int i=0; i < users[student_index].num_enrolled_units; i++) {
        if(strcmp(users[student_index].enrolled_units[i].unit_code, units[unit_idx].unit_code) == 0) {
            printf("Error: You are already enrolled in %s.\n", units[unit_idx].unit_code);
            return 0;
        }
    }
    int current_enroll= get_unit_enrollment_count(units[unit_idx].unit_code);
    if(current_enroll>= units[unit_idx].capacity){
        printf("Error: Unit is full\n");
    }
//using teacher name to display confirm
    char teacher_name[MAX_STRING_LENGTH] = "TBA";
    for (int j = 0; j < user_count; j++) {
        if (users[j].user_id == units[unit_idx].teacher_id && strcmp(users[j].role, "TA") == 0) {
            strcpy(teacher_name, users[j].username);
            break;
        }
    }
    if(unit_code ==NULL){
        printf("\nEnrolling in:\n");
        printf("Unit Code: %s\n",units[unit_idx].unit_code);
        printf("Unit Name: %s\n",units[unit_idx].unit_name);
        printf("Teacher: %s\n",teacher_name);
        printf("Current Enrollment: %d/%d\n", current_enroll, units[unit_idx].capacity);
    
        char confirm;
        printf("\nConfirm enrollment? (y/n): ");
        scanf(" %c", &confirm);
        if (confirm != 'y' || confirm != 'Y') {
            printf("Enrollment cancelled.\n");
            return 0;
        }
    }
    int student_enroll_idx = users[student_index].num_enrolled_units;//Start enroll in
    strcpy(users[student_index].enrolled_units[student_enroll_idx].unit_code, units[unit_idx].unit_code);//copy vào mảng student
    users[student_index].enrolled_units[student_enroll_idx].score = -1;// Didnt grade => =-1
    users[student_index].num_enrolled_units++;//increase number of units
    units[unit_idx].current_enrollment++;//increase actual enrollment
    printf("✓ Successfully enrolled in %s\n", units[unit_idx].unit_code);
    printf("You are now enrolled in %d/%d units.\n", users[student_index].num_enrolled_units, MAX_STUDENT_UNITS);
    return 1; 
}
//TEACHER UNIT
int generate_unit_id(void) {
    int id;
    int unique;
    do {
        unique = 1;
        id = 1000000 + rand() % 9000000;
        for (int i = 0; i < unit_count; i++) {
            if (units[i].unit_id == id) {
                unique = 0;
                break;
            }
        }
    } while (!unique);
    return id;
}

// 6.5.3 Kiểm tra Unit Code đã tồn tại chưa
int unit_code_exists(const char* unit_code) {
    for (int i = 0; i < unit_count; i++) {
        if (strcmp(units[i].unit_code, unit_code) == 0) {
            return 1;
        }
    }
    return 0;
}

// 6.6.1 Teacher: Xem danh sách môn mình dạy
void list_teaching_units(int teacher_index) {
    printf("\n=== My Teaching Units ===\n");
    if (users[teacher_index].num_teaching_units == 0) {
        printf("You are not teaching any units yet.\n");
        return;
    }

    for (int i = 0; i < users[teacher_index].num_teaching_units; i++) {
        char* code = users[teacher_index].teaching_units[i];
        int idx = find_unit_by_code(code);
        
        if (idx != -1) {
            units[idx].current_enrollment = get_unit_enrollment_count(code);
            float percent = (float)units[idx].current_enrollment / units[idx].capacity * 100;
            printf("Unit Code: %s\n", units[idx].unit_code);
            printf("Unit Name: %s\n", units[idx].unit_name);
            printf("Capacity: %d\n", units[idx].capacity);
            printf("Current Enrollment: %d/%d (%.0f%% full)\n", 
                   units[idx].current_enrollment, units[idx].capacity, percent);
            printf("Status: %s\n", units[idx].current_enrollment >= units[idx].capacity ? "FULL" : "Available");
            printf("----------------------------\n");
        }
    }
    printf("Total units teaching: %d\n", users[teacher_index].num_teaching_units);
}

// 6.6.2 Teacher: Thêm môn học mới
int add_unit(int teacher_index) {
    if (users[teacher_index].num_teaching_units >= MAX_TEACHER_UNITS) {
        printf("Error: You teach too many units (Max %d).\n", MAX_TEACHER_UNITS);
        return 0;
    }
    if (unit_count >= MAX_UNITS) {
        printf("Error: System database full.\n");
        return 0;
    }

    char code[10], name[MAX_STRING_LENGTH];
    int capacity;

    printf("\n=== Add New Unit ===\n");
    printf("Enter unit code (e.g. FIT9136): ");
    scanf("%s", code);
    to_uppercase(code);

    if (!is_valid_unit_code(code)) {
        printf("Error: Invalid format! Must be 3 letters + 4 digits.\n");
        return 0;
    }
    if (unit_code_exists(code)) {
        printf("Error: Unit code %s already exists!\n", code);
        return 0;
    }

    printf("Enter unit name: ");
    while(getchar() != '\n'); // Xóa buffer
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Xóa \n
    trim_whitespace(name);

    printf("Enter capacity (1-100): ");
    scanf("%d", &capacity);
    if (capacity < 1 || capacity > 100) {
        printf("Error: Capacity must be 1-100.\n");
        return 0;
    }

    // Confirm
    printf("\nConfirm adding %s - %s? (y/n): ", code, name);
    char confirm;
    while(getchar() != '\n');
    scanf("%c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        // 1. Thêm vào mảng global units
        units[unit_count].unit_id = generate_unit_id();
        strcpy(units[unit_count].unit_code, code);
        strcpy(units[unit_count].unit_name, name);
        units[unit_count].capacity = capacity;
        units[unit_count].current_enrollment = 0;
        units[unit_count].teacher_id = users[teacher_index].user_id;
        unit_count++;

        // 2. Thêm vào danh sách teaching_units của giáo viên
        strcpy(users[teacher_index].teaching_units[users[teacher_index].num_teaching_units], code);
        users[teacher_index].num_teaching_units++;

        printf("✓ Unit added successfully!\n");
        return 1;
    }
    return 0;
}

// 6.6.3 Teacher: Xóa môn học
int delete_unit(int teacher_index, const char* unit_code_input) {
    char code[10];
    if (unit_code_input == NULL) {
        printf("\nEnter unit code to delete: ");
        scanf("%s", code);
        to_uppercase(code);
    } else {
        strcpy(code, unit_code_input);
    }

    int unit_idx = find_unit_by_code(code);
    if (unit_idx == -1) {
        printf("Error: Unit not found.\n");
        return 0;
    }

    // Check quyền sở hữu
    if (units[unit_idx].teacher_id != users[teacher_index].user_id) {
        printf("Error: You don't teach this unit.\n");
        return 0;
    }

    printf("WARNING: Deleting %s will remove ALL student enrollments.\n", code);
    printf("Are you sure? (yes/no): ");
    char confirm[10];
    scanf("%s", confirm);

    if (strcmp(confirm, "yes") == 0) {
        // 1. Xóa enrollment của TẤT CẢ sinh viên
        for (int i = 0; i < user_count; i++) {
            if (strcmp(users[i].role, "ST") == 0) {
                for (int j = 0; j < users[i].num_enrolled_units; j++) {
                    if (strcmp(users[i].enrolled_units[j].unit_code, code) == 0) {
                        // Dồn mảng enrollment của sinh viên
                        for (int k = j; k < users[i].num_enrolled_units - 1; k++) {
                            users[i].enrolled_units[k] = users[i].enrolled_units[k+1];
                        }
                        users[i].num_enrolled_units--;
                        break;
                    }
                }
            }
        }

        // 2. Xóa khỏi teaching_units của giáo viên
        for (int i = 0; i < users[teacher_index].num_teaching_units; i++) {
            if (strcmp(users[teacher_index].teaching_units[i], code) == 0) {
                for (int j = i; j < users[teacher_index].num_teaching_units - 1; j++) {
                    strcpy(users[teacher_index].teaching_units[j], users[teacher_index].teaching_units[j+1]);
                }
                users[teacher_index].num_teaching_units--;
                break;
            }
        }

        // 3. Xóa khỏi mảng global units
        for (int i = unit_idx; i < unit_count - 1; i++) {
            units[i] = units[i+1];
        }
        unit_count--;

        printf("✓ Unit deleted.\n");
        return 1;
    }
    printf("Deletion cancelled.\n");
    return 0;
}

// 6.6.4 & 6.6.5 Xem sinh viên và thống kê điểm
void list_students_in_unit(const char* unit_code_input) {
    char code[10];
    if (unit_code_input == NULL) {
        printf("\nEnter unit code: ");
        scanf("%s", code);
        to_uppercase(code);
    } else {
        strcpy(code, unit_code_input);
    }

    int idx = find_unit_by_code(code);
    if (idx == -1) { printf("Unit not found.\n"); return; }

    printf("\n=== Students in %s - %s ===\n", code, units[idx].unit_name);
    printf("%-5s %-10s %-15s %-10s %-10s\n", "No.", "ID", "Username", "Score", "Grade");
    printf("----------------------------------------------------------\n");

    int count = 0;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].role, "ST") == 0) {
            for (int j = 0; j < users[i].num_enrolled_units; j++) {
                if (strcmp(users[i].enrolled_units[j].unit_code, code) == 0) {
                    int score = users[i].enrolled_units[j].score;
                    char grade[5];
                    if (score == -1) strcpy(grade, "N/A");
                    else if (score >= 80) strcpy(grade, "HD");
                    else if (score >= 70) strcpy(grade, "D");
                    else if (score >= 60) strcpy(grade, "C");
                    else if (score >= 50) strcpy(grade, "P");
                    else strcpy(grade, "F");

                    printf("%-5d %-10d %-15s %-10d %-10s\n", 
                           ++count, users[i].user_id, users[i].username, score, grade);
                }
            }
        }
    }
    if (count == 0) printf("No students enrolled.\n");
}

void show_unit_statistics(const char* unit_code_input) {
    char code[10];
    if (unit_code_input == NULL) {
        printf("\nEnter unit code: ");
        scanf("%s", code);
        to_uppercase(code);
    } else {
        strcpy(code, unit_code_input);
    }

    int total = 0, graded = 0, sum = 0, max = -1, min = 101;
    int hd=0, d=0, c=0, p=0, f=0;

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].role, "ST") == 0) {
            for (int j = 0; j < users[i].num_enrolled_units; j++) {
                if (strcmp(users[i].enrolled_units[j].unit_code, code) == 0) {
                    total++;
                    int s = users[i].enrolled_units[j].score;
                    if (s != -1) {
                        graded++;
                        sum += s;
                        if (s > max) max = s;
                        if (s < min) min = s;
                        if (s >= 80) hd++;
                        else if (s >= 70) d++;
                        else if (s >= 60) c++;
                        else if (s >= 50) p++;
                        else f++;
                    }
                }
            }
        }
    }

    printf("\n=== Statistics for %s ===\n", code);
    printf("Total Students: %d\n", total);
    printf("Graded: %d\n", graded);
    
    if (graded > 0) {
        printf("Average Score: %.2f\n", (float)sum/graded);
        printf("Max: %d | Min: %d\n", max, min);
        printf("Grade Distribution:\n");
        printf("HD: %d | D: %d | C: %d | P: %d | F: %d\n", hd, d, c, p, f);
        printf("Pass Rate: %.1f%%\n", (float)(hd+d+c+p)/graded * 100);
    } else {
        printf("No graded data available.\n");
    }
}