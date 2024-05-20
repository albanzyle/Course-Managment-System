#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 100
#define MAX_INSTRUCTORS 50
#define MAX_COURSES 50
#define MAX_STUDENTS_PER_COURSE 6

typedef struct {
    char id[20];
    char first_name[30];
    char last_name[30];
    char birth_date[11]; // Format: DD-MM-YYYY
} Student;

typedef struct {
    char id[20];
    char first_name[30];
    char last_name[30];
    char birth_date[11]; // Format: DD-MM-YYYY
    char categories[100];
} Instructor;

typedef struct {
    char id[20];
    char start_date[11];  // Format: DD-MM-YYYY
    int days_of_week;     // Bitmask representing days of the week
    char start_time[6];   // Format: HH:MM
    char category[30];
    char instructor_id[20];
    int student_count;
    char student_ids[MAX_STUDENTS_PER_COURSE][20];
} Course;


void save_students(Student students[], int student_count);
void save_instructors(Instructor instructors[], int instructor_count);
void save_courses(Course courses[], int course_count);
int calculate_age(const char *birth_date);
void register_student(Student students[], int *student_count);
void create_instructor(Instructor instructors[], int *instructor_count);
void create_course(Course courses[], int *course_count, Instructor instructors[], int instructor_count);
void list_students(Student students[], int student_count);
void list_instructors(Instructor instructors[], int instructor_count);
void list_courses(Course courses[], int course_count);
void register_student_to_course(Course courses[], int course_count, Student students[], int student_count);
void delete_instructor(Instructor instructors[], int *instructor_count, Course courses[], int course_count);
void delete_course(Course courses[], int *course_count);
void unregister_student_from_course(Course courses[], int course_count);
void list_students_in_course(Course courses[], int course_count, Student students[], int student_count);
void list_courses_by_instructor(Course courses[], int course_count, Instructor instructors[], int instructor_count);
void list_students_by_instructor(Course courses[], int course_count, Student students[], int student_count, Instructor instructors[], int instructor_count);
void main_menu();

int main() {
    main_menu();
    return 0;
}

void save_students(Student students[], int student_count) {
    FILE *file = fopen("student.dat", "wb");
    if (file != NULL) {
        fwrite(students, sizeof(Student), student_count, file);
        fclose(file);
    } else {
        perror("Cannot open file");
    }
}

void save_instructors(Instructor instructors[], int instructor_count) {
    FILE *file = fopen("instructor.dat", "wb");
    if (file != NULL) {
        fwrite(instructors, sizeof(Instructor), instructor_count, file);
        fclose(file);
    } else {
        perror("Cannot open file");
    }
}

void save_courses(Course courses[], int course_count) {
    FILE *file = fopen("course.dat", "wb");
    if (file != NULL) {
        fwrite(courses, sizeof(Course), course_count, file);
        fclose(file);
    } else {
        perror("Cannot open file");
    }
}

int calculate_age(const char *birth_date) {
    int day, month, year;
    sscanf(birth_date, "%d-%d-%d", &day, &month, &year);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int current_year = tm.tm_year + 1900;
    int current_month = tm.tm_mon + 1;
    int current_day = tm.tm_mday;

    int age = current_year - year;
    if (current_month < month || (current_month == month && current_day < day)) {
        age--;
    }
    return age;
}

void register_student(Student students[], int *student_count) {
    if (*student_count >= MAX_STUDENTS) {
        printf("Nuk ka vend për më shumë studentë!\n");
        return;
    }

    Student new_student;
    printf("Enter student ID: ");
    scanf("%s", new_student.id);
    printf("Enter first name: ");
    scanf("%s", new_student.first_name);
    printf("Enter last name: ");
    scanf("%s", new_student.last_name);
    printf("Enter birth date (DD-MM-YYYY): ");
    scanf("%s", new_student.birth_date);

    int age = calculate_age(new_student.birth_date);
    if (age < 5 || age > 14) {
        printf("Ky student nuk mund te regjistrohet pasi nuk ka moshen e lejuar per kurset!\n");
        return;
    }

    students[*student_count] = new_student;
    (*student_count)++;
    save_students(students, *student_count);

    printf("Student registered successfully.\n");
}

void create_instructor(Instructor instructors[], int *instructor_count) {
    if (*instructor_count >= MAX_INSTRUCTORS) {
        printf("Nuk ka vend për më shumë instruktorë!\n");
        return;
    }

    Instructor new_instructor;
    printf("Enter instructor ID: ");
    scanf("%s", new_instructor.id);
    printf("Enter first name: ");
    scanf("%s", new_instructor.first_name);
    printf("Enter last name: ");
    scanf("%s", new_instructor.last_name);
    printf("Enter birth date (DD-MM-YYYY): ");
    scanf("%s", new_instructor.birth_date);
    printf("Enter categories (comma-separated: Robotike, Programim, Graphic Design): ");
    scanf(" %[^\n]", new_instructor.categories);

    instructors[*instructor_count] = new_instructor;
    (*instructor_count)++;
    save_instructors(instructors, *instructor_count);

    printf("Instructor created successfully.\n");
}

void create_course(Course courses[], int *course_count, Instructor instructors[], int instructor_count) {
    if (*course_count >= MAX_COURSES) {
        printf("Nuk ka vend për më shumë kurse!\n");
        return;
    }

    Course new_course;
    printf("Enter course ID: ");
    scanf("%s", new_course.id);
    printf("Enter start date (DD-MM-YYYY): ");
    scanf("%s", new_course.start_date);
    printf("Enter days of week: ");
    scanf("%s", new_course.days_of_week);
    printf("Enter start time (HH:MM): ");
    scanf("%s", new_course.start_time);
    printf("Enter category (Robotike, Programim, Graphic Design): ");
    scanf("%s", new_course.category);
    printf("Enter instructor ID: ");
    scanf("%s", new_course.instructor_id);

    int instructor_found = 0;
    int i;
    for (i = 0; i < instructor_count; i++) {
        if (strcmp(instructors[i].id, new_course.instructor_id) == 0) {
            instructor_found = 1;
            if (strstr(instructors[i].categories, new_course.category) == NULL) {
                printf("Ky instruktor nuk mund te zhvilloje kete kurs, ju lutem zgjidhni nje instruktor tjeter!\n");
                return;
            }
            break;
        }
    }

    if (!instructor_found) {
        printf("Instruktori nuk u gjet!\n");
        return;
    }

    new_course.student_count = 0;
    courses[*course_count] = new_course;
    (*course_count)++;
    save_courses(courses, *course_count);

    printf("Course created successfully.\n");
}

void list_students(Student students[], int student_count) {
    printf("List of Registered Students:\n");
    printf("%-20s %-30s %-30s %-11s\n", "ID", "First Name", "Last Name", "Birth Date");
    int i;
	for (i = 0; i < student_count; i++) {
        printf("%-20s %-30s %-30s %-11s\n", students[i].id, students[i].first_name, students[i].last_name, students[i].birth_date);
    }
}

void list_instructors(Instructor instructors[], int instructor_count) {
    printf("List of Registered Instructors:\n");
    printf("%-20s %-30s %-30s %-11s %-100s\n", "ID", "First Name", "Last Name", "Birth Date", "Categories");
    int i;
	for (i = 0; i < instructor_count; i++) {
        printf("%-20s %-30s %-30s %-11s %-100s\n", instructors[i].id, instructors[i].first_name, instructors[i].last_name, instructors[i].birth_date, instructors[i].categories);
    }
}

void list_courses(Course courses[], int course_count) {
    printf("List of Created Courses:\n");
    printf("%-20s %-11s %-20s %-6s %-30s %-20s\n", "ID", "Start Date", "Days of Week", "Time", "Category", "Instructor ID");
    int i;
	for (i = 0; i < course_count; i++) {
        printf("%-20s %-11s %-20s %-6s %-30s %-20s\n", courses[i].id, courses[i].start_date, courses[i].days_of_week, courses[i].start_time, courses[i].category, courses[i].instructor_id);
    }
}

void register_student_to_course(Course courses[], int course_count, Student students[], int student_count) {
    char student_id[20], course_id[20];
    printf("Enter student ID: ");
    scanf("%s", student_id);
    printf("Enter course ID: ");
    scanf("%s", course_id);

    int student_found = 0, course_found = 0;
    int i;
	for (i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            student_found = 1;
            break;
        }
    }

    if (!student_found) {
        printf("Studenti nuk u gjet!\n");
        return;
    }
    for (i = 0; i < course_count; i++) {
        if (strcmp(courses[i].id, course_id) == 0) {
            course_found = 1;
            if (courses[i].student_count >= MAX_STUDENTS_PER_COURSE) {
                printf("Ky grup eshte i plotesuar me studente, zgjidhni nje kurs tjeter!\n");
                return;
            }

            strcpy(courses[i].student_ids[courses[i].student_count], student_id);
            courses[i].student_count++;
            save_courses(courses, course_count);
            printf("Studenti u regjistrua me sukses ne kurs.\n");
            return;
        }
    }

    if (!course_found) {
        printf("Kursi nuk u gjet!\n");
    }
}

void delete_instructor(Instructor instructors[], int *instructor_count, Course courses[], int course_count) {
    char instructor_id[20];
    printf("Enter instructor ID to delete: ");
    scanf("%s", instructor_id);
	int i;
    for (i = 0; i < course_count; i++) {
        if (strcmp(courses[i].instructor_id, instructor_id) == 0) {
            printf("Instruktori nuk mund te fshihet pasi instrukton nje kurs.\n");
            return;
        }
    }
    for (i = 0; i < *instructor_count; i++) {
        if (strcmp(instructors[i].id, instructor_id) == 0) {
            instructors[i] = instructors[*instructor_count - 1];
            (*instructor_count)--;
            save_instructors(instructors, *instructor_count);
            printf("Instruktori u fshi me sukses.\n");
            return;
        }
    }

    printf("Instruktori nuk u gjet!\n");
}

void delete_course(Course courses[], int *course_count) {
    char course_id[20];
    printf("Enter course ID to delete: ");
    scanf("%s", course_id);
	int i;
    for (i = 0; i < *course_count; i++) {
        if (strcmp(courses[i].id, course_id) == 0) {
            if (courses[i].student_count > 0) {
                printf("Kursi nuk mund te fshihet pasi ka studente te regjistruar.\n");
                return;
            }
            courses[i] = courses[*course_count - 1];
            (*course_count)--;
            save_courses(courses, *course_count);
            printf("Kursi u fshi me sukses.\n");
            return;
        }
    }

    printf("Kursi nuk u gjet!\n");
}

void unregister_student_from_course(Course courses[], int course_count) {
    char student_id[20], course_id[20];
    printf("Enter student ID to unregister: ");
    scanf("%s", student_id);
    printf("Enter course ID: ");
    scanf("%s", course_id);
	int i;
	int j;
	int k;
    for (i = 0; i < course_count; i++) {
        if (strcmp(courses[i].id, course_id) == 0) {
            for (j = 0; j < courses[i].student_count; j++) {
                if (strcmp(courses[i].student_ids[j], student_id) == 0) {
                    courses[i].student_ids[j][0] = '\0';
                    for (k = j; k < courses[i].student_count - 1; k++) {
                        strcpy(courses[i].student_ids[k], courses[i].student_ids[k + 1]);
                    }
                    courses[i].student_count--;
                    save_courses(courses, course_count);
                    printf("Studenti u çregjistrua me sukses nga kursi.\n");
                    return;
                }
            }
        }
    }

    printf("Studenti ose kursi nuk u gjet!\n");
}

void list_students_in_course(Course courses[], int course_count, Student students[], int student_count) {
    char course_id[20];
    printf("Enter course ID: ");
    scanf("%s", course_id);
	int i;
	int j;
	int k;
    for (i = 0; i < course_count; i++) {
        if (strcmp(courses[i].id, course_id) == 0) {
            printf("List of Students in Course %s:\n", course_id);
            printf("%-20s %-30s %-30s %-11s\n", "ID", "First Name", "Last Name", "Birth Date");
            for (j = 0; j < courses[i].student_count; j++) {
                for (k = 0; k < student_count; k++) {
                    if (strcmp(courses[i].student_ids[j], students[k].id) == 0) {
                        printf("%-20s %-30s %-30s %-11s\n", students[k].id, students[k].first_name, students[k].last_name, students[k].birth_date);
                        break;
                    }
                }
            }
            return;
        }
    }

    printf("Kursi nuk u gjet!\n");
}

void list_courses_by_instructor(Course courses[], int course_count, Instructor instructors[], int instructor_count) {
    char instructor_id[20];
    printf("Enter instructor ID: ");
    scanf("%s", instructor_id);

    int instructor_found = 0;
    int i;
    for (i = 0; i < instructor_count; i++) {
        if (strcmp(instructors[i].id, instructor_id) == 0) {
            instructor_found = 1;
            break;
        }
    }

    if (!instructor_found) {
        printf("Instruktori nuk u gjet!\n");
        return;
    }

    printf("List of Courses by Instructor %s:\n", instructor_id);
    printf("%-20s %-11s %-20s %-6s %-30s %-20s\n", "ID", "Start Date", "Days of Week", "Time", "Category", "Instructor ID");
	for (i = 0; i < course_count; i++) {
        if (strcmp(courses[i].instructor_id, instructor_id) == 0) {
            printf("%-20s %-11s %-20s %-6s %-30s %-20s\n", courses[i].id, courses[i].start_date, courses[i].days_of_week, courses[i].start_time, courses[i].category, courses[i].instructor_id);
        }
    }
}

void list_students_by_instructor(Course courses[], int course_count, Student students[], int student_count, Instructor instructors[], int instructor_count) {
    char instructor_id[20];
    printf("Enter instructor ID: ");
    scanf("%s", instructor_id);

    int instructor_found = 0;
    int i;
    for (i = 0; i < instructor_count; i++) {
        if (strcmp(instructors[i].id, instructor_id) == 0) {
            instructor_found = 1;
            break;
        }
    }

    if (!instructor_found) {
        printf("Instruktori nuk u gjet!\n");
        return;
    }

    printf("List of Students by Instructor %s:\n", instructor_id);
    printf("%-20s %-30s %-30s %-11s\n", "ID", "First Name", "Last Name", "Birth Date");
	int j;
	int k;
    for (i = 0; i < course_count; i++) {
        if (strcmp(courses[i].instructor_id, instructor_id) == 0) {
            for (j = 0; j < courses[i].student_count; j++) {
                for (k = 0; k < student_count; k++) {
                    if (strcmp(courses[i].student_ids[j], students[k].id) == 0) {
                        printf("%-20s %-30s %-30s %-11s\n", students[k].id, students[k].first_name, students[k].last_name, students[k].birth_date);
                        break;
                    }
                }
            }
        }
    }
}

void main_menu() {
    Student students[MAX_STUDENTS];
    int student_count = 0;
    FILE *file = fopen("student.dat", "rb");
    if (file != NULL) {
        student_count = fread(students, sizeof(Student), MAX_STUDENTS, file);
        fclose(file);
    }

    Instructor instructors[MAX_INSTRUCTORS];
    int instructor_count = 0;
    file = fopen("instructor.dat", "rb");
    if (file != NULL) {
        instructor_count = fread(instructors, sizeof(Instructor), MAX_INSTRUCTORS, file);
        fclose(file);
    }

    Course courses[MAX_COURSES];
    int course_count = 0;
    file = fopen("course.dat", "rb");
    if (file != NULL) {
        course_count = fread(courses, sizeof(Course), MAX_COURSES, file);
        fclose(file);
    }

    int option;
    while (1) {
        printf("\nCourse Management System\n");
        printf("1. Student\n");
        printf("2. Instructor\n");
        printf("3. Courses\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                while (1) {
                    printf("\nStudent Menu\n");
                    printf("1. Register Student\n");
                    printf("2. List Students\n");
                    printf("3. Register Student to Course\n");
                    printf("4. Unregister Student from Course\n");
                    printf("5. Back to Main Menu\n");
                    printf("Choose an option: ");
                    scanf("%d", &option);

                    switch (option) {
                        case 1:
                            register_student(students, &student_count);
                            break;
                        case 2:
                            list_students(students, student_count);
                            break;
                        case 3:
                            register_student_to_course(courses, course_count, students, student_count);
                            break;
                        case 4:
                            unregister_student_from_course(courses, course_count);
                            break;
                        case 5:
                            goto student_menu_end;
                        default:
                            printf("Invalid option, please try again.\n");
                    }
                }
                student_menu_end:
                break;
            case 2:
                while (1) {
                    printf("\nInstructor Menu\n");
                    printf("1. Create Instructor\n");
                    printf("2. List Instructors\n");
                    printf("3. Delete Instructor\n");
                    printf("4. List Courses by Instructor\n");
                    printf("5. List Students by Instructor\n");
                    printf("6. Back to Main Menu\n");
                    printf("Choose an option: ");
                    scanf("%d", &option);

                    switch (option) {
                        case 1:
                            create_instructor(instructors, &instructor_count);
                            break;
                        case 2:
                            list_instructors(instructors, instructor_count);
                            break;
                        case 3:
                            delete_instructor(instructors, &instructor_count, courses, course_count);
                            break;
                        case 4:
                            list_courses_by_instructor(courses, course_count, instructors, instructor_count);
                            break;
                        case 5:
                            list_students_by_instructor(courses, course_count, students, student_count, instructors, instructor_count);
                            break;
                        case 6:
                            goto instructor_menu_end;
                        default:
                            printf("Invalid option, please try again.\n");
                    }
                }
                instructor_menu_end:
                break;
            case 3:
                while (1) {
                    printf("\nCourse Menu\n");
                    printf("1. Create Course\n");
                    printf("2. List Courses\n");
                    printf("3. Delete Course\n");
                    printf("4. List Students in Course\n");
                    printf("5. Back to Main Menu\n");
                    printf("Choose an option: ");
                    scanf("%d", &option);

                    switch (option) {
                        case 1:
                            create_course(courses, &course_count, instructors, instructor_count);
                            break;
                        case 2:
                            list_courses(courses, course_count);
                            break;
                        case 3:
                            delete_course(courses, &course_count);
                            break;
                        case 4:
                            list_students_in_course(courses, course_count, students, student_count);
                            break;
                        case 5:
                            goto course_menu_end;
                        default:
                            printf("Invalid option, please try again.\n");
                    }
                }
                course_menu_end:
                break;
            case 4:
                return;
            default:
                printf("Invalid option, please try again.\n");
        }
    }
}

