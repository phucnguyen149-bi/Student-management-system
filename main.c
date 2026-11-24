#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "file_io.h"
#include "user.h"
#include "unit.h"
#include "utils.h"

int main(void) {
    int login_choice;
    int menu_choice;
    char input_username[50];
    char input_password[50];
    int input_id; //index user
    srand(time(NULL));
    printf("=================================\n");
    printf("Loading data...\n");
    // Load 
    load_users("../data/users.csv"); 
    load_units("../data/units.csv");
    printf("✓ Loaded %d users\n", user_count);
    printf("✓ Loaded %d units\n", unit_count);
    printf("System ready!\n");
    printf("=================================\n\n");
    int system_running = 1;

    while(system_running){
        clear_screen();
        printf("=================================\n");
        printf("    STUDENT MANAGEMENT SYSTEM\n");
        printf("=================================\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("=================================\n");
        printf("Enter choice: ");
        
        if(scanf("%d", &login_choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        printf("\n");
        
        if(login_choice == 1){
            printf("Enter username: ");
            scanf("%s", input_username);
            printf("Enter password: ");
            scanf("%s", input_password);
            
            input_id = login(input_username, input_password);
            
            if(input_id != -1) {
                printf("\n✓ Login successful!\n");
                printf("Welcome, %s (%s)\n", users[input_id].username, users[input_id].role);
                pause_screen(); 

                int is_running = 1;

                if (strcmp(users[input_id].role, "TA") == 0) {
                    // ================= TEACHER MENU =================
                    while (is_running) {
                        clear_screen();
                        printf("\n=================================\n");
                        printf("    TEACHER MENU (%s)\n", users[input_id].username);
                        printf("=================================\n");
                        printf("1. List my teaching units\n");
                        printf("2. Add a new unit\n");
                        printf("3. Delete a unit\n");
                        printf("4. List students in a unit\n");
                        printf("5. Show unit statistics\n");
                        printf("6. Logout\n");
                        printf("=================================\n");
                        printf("Enter choice: ");
                        scanf("%d", &menu_choice);

                        switch(menu_choice) {
                            case 1: list_teaching_units(input_id); pause_screen(); break;
                            case 2: add_unit(input_id); pause_screen(); break;
                            case 3: 
                                list_teaching_units(input_id);
                                delete_unit(input_id, NULL); 
                                pause_screen(); 
                                break;
                            case 4: 
                                list_teaching_units(input_id);
                                list_students_in_unit(NULL); 
                                pause_screen(); 
                                break;
                            case 5: 
                                list_teaching_units(input_id);
                                show_unit_statistics(NULL); 
                                pause_screen(); 
                                break;
                            case 6: is_running = 0; break;
                            default: printf("Invalid choice!\n"); pause_screen();
                        }
                    }
                } 
                else if (strcmp(users[input_id].role, "ST") == 0) {
                    // ================= STUDENT MENU =================
                    while (is_running) {
                        clear_screen();
                        printf("\n=================================\n");
                        printf("    STUDENT MENU (%s)\n", users[input_id].username);
                        printf("=================================\n");
                        printf("1. List available units\n");
                        printf("2. List my enrolled units\n");
                        printf("3. Enroll in a unit\n");
                        printf("4. Drop a unit\n");
                        printf("5. Check my scores\n");
                        printf("6. Generate random score (TEST)\n");
                        printf("7. Logout\n");
                        printf("=================================\n");
                        printf("Enter choice: ");
                        scanf("%d", &menu_choice);

                        if(menu_choice == 1){
                            list_available_units();
                            pause_screen();
                        } else if(menu_choice == 2){
                            list_my_enrolled_units(input_id);
                            pause_screen();
                        } else if(menu_choice == 3){
                            enroll_unit(input_id, NULL);
                            pause_screen();
                        } else if(menu_choice == 4){
                            list_my_enrolled_units(input_id); 
                            drop_unit(input_id);
                            pause_screen();
                        } else if(menu_choice == 5){
                            check_my_scores(input_id, NULL);
                            pause_screen();
                        } else if(menu_choice == 6){
                            list_my_enrolled_units(input_id);
                            char target_code[10];
                            printf("\n=== Generate Random Score (TESTING) ===\n");
                            printf("Enter unit code: ");
                            scanf("%s", target_code);
                            to_uppercase(target_code); 
                            generate_random_score(input_id, target_code);
                            pause_screen();
                        } else if(menu_choice == 7){
                            is_running = 0;
                        } else {
                            printf("Invalid choice!\n");
                            pause_screen();
                        }
                    }
                }
                printf("\nLogging out... Thank you, %s!\n", users[input_id].username);
                pause_screen();
            } else {
                // Login failed handled in login func or here
                printf("Login failed! Check username/password/status.\n");
                pause_screen(); 
            }
        } 
        else if(login_choice == 2){
            char confirm;
            printf("Are you sure you want to exit? (y/n): ");        
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') {
                printf("\nSaving data...\n");
                save_users("../data/users.csv"); 
                save_units("../data/units.csv");
                printf("✓ Saved data.\nGoodbye!\n");
                system_running = 0;
            }
        }
    }
    return 0;
}