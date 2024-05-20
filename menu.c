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
    char categories[3]; // Format: 101 for Robotike true, Programim false and Graphic Design true
} Instructor;

typedef struct {
    char id[20];
    char start_date[11]; // Format: DD-MM-YYYY
    char days_of_week[8]; // Format: 1010000 for Monday and Wednesday
    char start_time[6]; // Format: HH:MM
    int category; // 1 for Robotike, 2 for Programim, 3 for Graphic Design
    char instructor_id[20];
} Course;

typedef struct {
    char student_id[20];
    char course_id[20];
} StudentCourse;

const char* get_category_name(int category) {
    switch (category) {
        case 1:
            return "Robotike";
        case 2:
            return "Programim";
        case 3:
            return "Graphic Design";
        default:
            return "Kategori e Gabuar";
    }
}

const char* get_days_of_week(const char *days) { "1010000"
    static char days_of_week[100];
    days_of_week[0] = '\0';

    const char* day_names[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	int i;
    for (i = 0; i < 7; i++) {
        if (days[i] == '1') {
            if (days_of_week[0] != '\0') {
                strcat(days_of_week, ", ");
            }
            strcat(days_of_week, day_names[i]);
        }
    }

    return days_of_week;
}

void list_kurs() {
    FILE *file = fopen("kurs.dat", "rb");
    if (file == NULL) {
        return;
    }

    Course course;
    printf("List of Registered Courses:\n");
    printf("%-10s %-20s %-30s %-10s %-20s %-20s\n", "ID", "Start Date", "Days of Week", "Time", "Category", "Instructor ID");

    while (fread(&course, sizeof(Course), 1, file)) {
        printf("%-10s %-20s %-30s %-10s %-20s %-20s\n", course.id, course.start_date, get_days_of_week(course.days_of_week), course.start_time, get_category_name(course.category), course.instructor_id);
    }
    fclose(file);
}

void list_students() {
    FILE *file = fopen("student.dat", "rb");
    if (file == NULL) {
        return;
    }

    Student student;
    printf("List of Registered Students:\n");
    printf("%-20s %-30s %-30s %-11s\n", "ID", "First Name", "Last Name", "Birth Date");
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("%-20s %-30s %-30s %-11s\n", student.id, student.first_name, student.last_name, student.birth_date);
    }
    fclose(file);
}

// Function to check if a vit i brishte
int vit_brisht(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return 1;
            }
            return 0;
        }
        return 1;
    }
    return 0;
}

int is_valid_date(const char *date) {
    // Get the current time
    time_t current_time;
    time(&current_time);

    // Convert it to local time representation
    struct tm *local_time = localtime(&current_time);

    // Extract the day, month, and year
    int current_day = local_time->tm_mday;
    int current_month = local_time->tm_mon + 1; // tm_mon is 0-based, so add 1
    int current_year = local_time->tm_year + 1900; // tm_year is years since 1900
    
	int i;
    if (strlen(date) != 10)
        return 0;
    if (date[2] != '-' || date[5] != '-')
        return 0;
    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5)
            continue;
        if (!isdigit(date[i]))
            return 0;
    } 
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0')*100 + (date[8] - '0') * 10 + (date[9] - '0');
    // Validate year
    if (year < current_year-100 || year > current_year+1)
        return 0;

    // Validate month
    if (month < 1 || month > 12)
        return 0;

    // Validate day based on the month and leap year status
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (vit_brisht(year)) {
        days_in_month[2] = 29;
    }

    if (day < 1 || day > days_in_month[month])
        return 0;

    return 1;
}

int is_valid_age(const char *date) {
    // Get the current time
    time_t current_time;
    time(&current_time);

    // Convert it to local time representation
    struct tm *local_time = localtime(&current_time);

    // Extract the day, month, and year
    int current_day = local_time->tm_mday;
    int current_month = local_time->tm_mon + 1; // tm_mon is 0-based, so add 1
    int current_year = local_time->tm_year + 1900; // tm_year is years since 1900

    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0')*100 + (date[8] - '0') * 10 + (date[9] - '0');

    // Calculate the age
    int age = current_year - year;
    if (current_month < month || (current_month == month && current_day < day)) {
        age--;
    }

    // Check if the age is within the valid range
    if (age < 5 || age > 14)
        return 0;

    return 1;
}

int is_valid_start_date(const char *date) {
    // Get the current time
    time_t current_time;
    time(&current_time);

    // Convert it to local time representation
    struct tm *local_time = localtime(&current_time);

    // Extract the current day, month, and year
    int current_day = local_time->tm_mday;
    int current_month = local_time->tm_mon + 1; // tm_mon is 0-based, so add 1
    int current_year = local_time->tm_year + 1900; // tm_year is years since 1900

    // Extract day, month, and year from the date string
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    // Validate year, month, and day
    if (year < current_year || (year == current_year && (month < current_month || (month == current_month && day <= current_day))))
        return 0;
	
	// Calculate the maximum allowed year and month
    int max_year = current_year;
    int max_month = current_month + 3;

    if (max_month > 12) {
        max_month -= 12;
        max_year += 1;
    }

    // Check if the provided date is within 3 months from the current date
    if (year > max_year || (year == max_year && month > max_month) || (year == max_year && month == max_month && day > current_day))
        return 0;
    // The date is valid and in the future
    return 1;
}

int is_student_registered(const char *student_id) {
    FILE *file = fopen("student.dat", "rb");
    if (file == NULL) { // kontrollo nese file ekziston
        return 0;
    }

    Student student;
    while (fread(&student, sizeof(Student), 1, file)) {
        if (strcmp(student.id, student_id) == 0) { // kontrollo nese id eshte perdorur ne file
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int is_course_created(const char *course_id) {
    FILE *file = fopen("kurs.dat", "rb");
    if (file == NULL) {
        return 0;
    }

    Course course;
    while (fread(&course, sizeof(Course), 1, file)) {
        if (strcmp(course.id, course_id) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int count_students_in_course(const char *course_id) {
    FILE *file = fopen("student_kurs.dat", "rb");
    if (file == NULL) {
        return 0;
    }

    StudentCourse sc;
    int count = 0;
    while (fread(&sc, sizeof(StudentCourse), 1, file)) {
        if (strcmp(sc.course_id, course_id) == 0) {
            count++;
        }
    }
    fclose(file);
    return count;
}

int is_student_in_course(const char *student_id, const char *course_id) {
    FILE *file = fopen("student_kurs.dat", "rb");
    if (file == NULL) {
        return 0;
    }
    StudentCourse sc;
    while (fread(&sc, sizeof(StudentCourse), 1, file)) {
        if (strcmp(sc.student_id, student_id) == 0 && strcmp(sc.course_id, course_id) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void register_student_in_course() {
    char student_id[20], course_id[20];
    list_students();
    printf("Enter student ID: ");
    scanf("%s", student_id);
    if (!is_student_registered(student_id)) {
        printf("Studenti nuk eshte regjistruar ne sistem!\n");
        return;
    }
    list_kurs();
    printf("Enter course ID: ");
    scanf("%s", course_id);
    if (!is_course_created(course_id)) {
        printf("Kursi nuk eshte krijuar ne sistem!\n");
        return;
    }
    
    if (is_student_in_course(student_id, course_id)) {
        printf("Studenti eshte tashme i regjistruar ne kete kurs!\n");
        return;
    }

    int student_count = count_students_in_course(course_id);
    if (student_count >= 6) {
        printf("Ky grup eshte i plotesuar me studente, zgjidhni nje kurs tjeter!\n");
        return;
    }

    StudentCourse new_sc;
    strcpy(new_sc.student_id, student_id);
    strcpy(new_sc.course_id, course_id);

    FILE *file = fopen("student_kurs.dat", "ab");
    if (file == NULL) {
        return;
    }
    fwrite(&new_sc, sizeof(StudentCourse), 1, file);
    fclose(file);

    printf("Studenti u regjistrua me sukses ne kurs.\n");
}

void list_courses_of_student() {
    char student_id[20];
    list_students();
    printf("Enter student ID: ");
    scanf("%s", student_id);
	
	if (!is_student_registered(student_id)) {
	    printf("Studenti nuk eshte regjistruar ne sistem!\n");
	    return;
	}
    FILE *file = fopen("student_kurs.dat", "rb");
    if (file == NULL) {
        return;
    }
    StudentCourse sc;
    int found = 0;
    printf("List of Courses for Student ID %s:\n", student_id);
    printf("%-10s %-20s %-30s %-10s %-20s %-20s\n", "Course ID", "Start Date", "Days of Week", "Time", "Category", "Instructor ID");

    while (fread(&sc, sizeof(StudentCourse), 1, file)) {
        if (strcmp(sc.student_id, student_id) == 0) {
            found = 1;
            FILE *course_file = fopen("kurs.dat", "rb");
            if (course_file == NULL) {
                fclose(file);
                return;
            }
            Course course;
            while (fread(&course, sizeof(Course), 1, course_file)) {
                if (strcmp(course.id, sc.course_id) == 0) {
                    printf("%-10s %-20s %-30s %-10s %-20s %-20s\n", course.id, course.start_date, get_days_of_week(course.days_of_week), course.start_time, get_category_name(course.category), course.instructor_id);
                    break;
                }
            }
            fclose(course_file);
        }
    }
    fclose(file);

    if (!found) {
        printf("Studenti nuk eshte regjistruar ne asnje kurs.\n");
    }
}

void deregister_student_from_course() {
    char student_id[20], course_id[20];
    list_students();
    printf("Enter student ID: ");
    scanf("%s", student_id);
    list_kurs();
    printf("Enter course ID: ");
    scanf("%s", course_id);
	
	if (!is_student_registered(student_id)) {
	    printf("Studenti nuk eshte regjistruar ne sistem!\n");
	    return;
	}

    if (!is_student_in_course(student_id, course_id)) {
        printf("Studenti nuk eshte regjistruar ne kete kurs!\n");
        return;
    }

    FILE *file = fopen("student_kurs.dat", "rb");
    if (file == NULL) {
        return;
    }

    FILE *temp_file = fopen("temp_student_kurs.dat", "wb");
    if (temp_file == NULL) {
        fclose(file);
        return;
    }
    StudentCourse sc;
    int found = 0;
    while (fread(&sc, sizeof(StudentCourse), 1, file)) {
        if (strcmp(sc.student_id, student_id) == 0 && strcmp(sc.course_id, course_id) == 0) {
            found = 1;
        } else {
            fwrite(&sc, sizeof(StudentCourse), 1, temp_file);
        }
    }
    fclose(file);
    fclose(temp_file);

    if (found) {
        remove("student_kurs.dat");
        rename("temp_student_kurs.dat", "student_kurs.dat");
        printf("Studenti u cregjistrua me sukses nga kursi.\n");
    } else {
        remove("temp_student_kurs.dat");
        printf("Studenti ose kursi nuk u gjet.\n");
    }
}

void register_student() {
    Student new_student;
    printf("Enter student ID: ");
    scanf("%s", new_student.id);
    if (is_student_registered(new_student.id)) {
        printf("Studenti me kete ID eshte i regjistruar ne sistem!\n");
        return;
    }
    printf("Enter first name: "); // mund te validosh emrin
    scanf("%s", new_student.first_name);
    printf("Enter last name: ");
    scanf("%s", new_student.last_name);
    int i =0;
    while (1) {
        printf("Enter birth date (DD-MM-YYYY): ");
        scanf("%s", new_student.birth_date);
        if (is_valid_date(new_student.birth_date)) {
            break;
        } else if(i < 3){
            printf("Data e lindjes nuk eshte ne formatin e sakte. Ju lutem provoni perseri.\n");
    	} else {
    		printf("Provoni perseri me vone...\n");
    		return;
		}
		i++;
	}
	if (!is_valid_age(new_student.birth_date)) {
        printf("Ky student nuk mund te regjistrohet pasi nuk ka moshen e lejuar per kurset!\n");
        return;
    }

    FILE *file = fopen("student.dat", "ab");
    if (file == NULL) {
        return;
    }
    fwrite(&new_student, sizeof(Student), 1, file);
    fclose(file);

    printf("Studenti u regjistrua me sukses.\n");
}

int is_valid_categories(const char *categories) {
	int i;
    if (strlen(categories) != 3)
        return 0;
    for (i = 0; i < 3; i++) {
        if (categories[i] != '0' && categories[i] != '1')
            return 0;
    }
    return 1;
}

void create_instructor() {
    Instructor new_instructor;
    printf("Enter instructor ID: ");
    scanf("%s", new_instructor.id);
    if (is_instructor_created(new_instructor.id)) {
        printf("Instruktori me kete ID eshte regjistruar ne sitem me heret!\n");
        return;
    }
    printf("Enter first name: ");
    scanf("%s", new_instructor.first_name);
    printf("Enter last name: ");
    scanf("%s", new_instructor.last_name);
    int i =0;
	while (1) {
        printf("Enter birth date (DD-MM-YYYY): ");
        scanf("%s", new_instructor.birth_date);
        if (is_valid_date(new_instructor.birth_date)) {
            break;
        } else if(i < 3){
            printf("Data e lindjes nuk eshte ne formatin e sakte. Ju lutem provoni perseri.\n");
        }else {
    		printf("Provoni perseri me vone...\n");
    		return;
		}
		i++;
    }
    while (1) {
        printf("Enter categories (e.g., 101 for Robotike true, Programim false and Graphic Design true): ");
        scanf("%s", new_instructor.categories);
        if (is_valid_categories(new_instructor.categories)) {
            break;
        } else if(i < 3) {
            printf("Kategorite nuk jane ne formatin e sakte. Ju lutem provoni perseri.\n");
        } else {
    		printf("Provoni perseri me vone...\n");
    		return;
		}
		i++;
    }
    
    FILE *file = fopen("instruktor.dat", "ab");
    if (file == NULL) {
        return;
    }
    fwrite(&new_instructor, sizeof(Instructor), 1, file);
    fclose(file);
    
    printf("Instructor created successfully.\n");
}

const char* get_category_names(const char *categories) { "111"
    static char category_names[50];
    category_names[0] = '\0'; 

    if (categories[0] == '1') {
        strcat(category_names, "Robotike ");
    }
    if (categories[1] == '1') {
        strcat(category_names, "Programim ");
    }
    if (categories[2] == '1') {
        strcat(category_names, "Graphic-Design");
    }

    return category_names;
}

void list_instructors() {
    FILE *file = fopen("instruktor.dat", "rb");
    if (file == NULL) {
        return;
    }

    Instructor instructor;
    printf("\nList of Registered Instructors:\n");
    printf("%-20s %-15s %-15s %-15s %-30s\n", "ID", "First Name", "Last Name", "Birth Date", "Categories");
    while (fread(&instructor, sizeof(Instructor), 1, file)) {
        printf("%-20s %-15s %-15s %-15s %-30s", instructor.id, instructor.first_name, instructor.last_name, instructor.birth_date, get_category_names(instructor.categories));
        printf("\n");
    }
    fclose(file);
}

void delete_instructor() {
    char instructor_id[20];
    list_instructors();
    printf("Enter instructor ID to delete: ");
    scanf("%s", instructor_id);

    if (!is_instructor_created(instructor_id)) {
        printf("Instruktori nuk eshte krijuar ne sistem!\n");
        return;
    }
    
    // Check if the instructor is assigned to any course
    FILE *course_file = fopen("kurs.dat", "rb");
    if (course_file == NULL) {
        return;
    }

    Course course;
    while (fread(&course, sizeof(Course), 1, course_file)) {
        if (strcmp(course.instructor_id, instructor_id) == 0) {
            printf("Instruktori nuk mund te fshihet pasi eshte i caktuar ne nje kurs.\n");
            fclose(course_file);
            return;
        }
    }
    fclose(course_file);

    // Read instructors and write back excluding the one to be deleted
    FILE *instructor_file = fopen("instruktor.dat", "rb");
    if (instructor_file == NULL) {
        return;
    }

    FILE *temp_file = fopen("temp.dat", "wb");
    if (temp_file == NULL) {
        fclose(instructor_file);
        return;
    }

    Instructor instructor;
    int found = 0;
    while (fread(&instructor, sizeof(Instructor), 1, instructor_file)) {
        if (strcmp(instructor.id, instructor_id) == 0) {
            found = 1;
        } else {
            fwrite(&instructor, sizeof(Instructor), 1, temp_file);
        }
    }
    fclose(instructor_file);
    fclose(temp_file);

    if (found) {
        remove("instruktor.dat");
        rename("temp.dat", "instruktor.dat");
        printf("Instruktori u fshi me sukses.\n");
    } else {
        remove("temp.dat");
        printf("Instruktori nuk u gjet.\n");
    }
}

int is_instructor_created(const char *instructor_id) {
    FILE *file = fopen("instruktor.dat", "rb");
    if (file == NULL) {
        return 0;
    }

    Instructor instructor;
    while (fread(&instructor, sizeof(Instructor), 1, file)) {
        if (strcmp(instructor.id, instructor_id) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void list_courses_of_instructor() {
    char instructor_id[20];
    list_instructors();
    printf("Enter instructor ID: ");
    scanf("%s", instructor_id);

    if (!is_instructor_created(instructor_id)) {
        printf("Instruktori nuk eshte krijuar ne sistem!\n");
        return;
    }

    FILE *file = fopen("kurs.dat", "rb");
    if (file == NULL) {
        return;
    }

    Course course;
    int found = 0;
    printf("List of Courses for Instructor ID %s:\n", instructor_id);
    printf("%-10s %-20s %-30s %-10s %-20s %-20s\n", "Course ID", "Start Date", "Days of Week", "Time", "Category", "Instructor ID");

    while (fread(&course, sizeof(Course), 1, file)) {
        if (strcmp(course.instructor_id, instructor_id) == 0) {
            printf("%-10s %-20s %-30s %-10s %-20s %-20s\n", course.id, course.start_date, get_days_of_week(course.days_of_week), course.start_time, get_category_name(course.category), course.instructor_id);
            found = 1;
        }
    }
    fclose(file);

    if (found == 0) {
        printf("Instruktori nuk ka asnje kurs te regjistruar.\n");
    }
}

void list_students_of_instructor() {
    char instructor_id[20];
    list_instructors();
    printf("Enter instructor ID: ");
    scanf("%s", instructor_id);

    if (!is_instructor_created(instructor_id)) {
        printf("Instruktori nuk eshte krijuar ne sistem!\n");
        return;
    }

    FILE *course_file = fopen("kurs.dat", "rb");
    if (course_file == NULL) {
        return;
    }

    Course course;
    int found_courses = 0;
    printf("List of Students for Instructor ID %s:\n", instructor_id);
    printf("%-20s %-30s %-30s %-11s\n", "Student ID", "First Name", "Last Name", "Birth Date");

    while (fread(&course, sizeof(Course), 1, course_file)) {
        if (strcmp(course.instructor_id, instructor_id) == 0) {
            found_courses = 1;
            FILE *student_course_file = fopen("student_kurs.dat", "rb");
            if (student_course_file == NULL) {
                fclose(course_file);
                return;
            }
            StudentCourse sc;   
            int found_student = 0;
            while (fread(&sc, sizeof(StudentCourse), 1, student_course_file)) {
                if (strcmp(sc.course_id, course.id) == 0) {
                    FILE *student_file = fopen("student.dat", "rb");
                    if (student_file == NULL) {
                        fclose(student_course_file);
                        fclose(course_file);
                        return;
                    }

                    Student student;
                    while (fread(&student, sizeof(Student), 1, student_file)) {
                        if (strcmp(student.id, sc.student_id) == 0) {
                        	found_student = 1;
                            printf("%-20s %-30s %-30s %-11s\n", student.id, student.first_name, student.last_name, student.birth_date);
                            break;
                        }
                    }
                    fclose(student_file);
                }
            }
            fclose(student_course_file);
            if (!found_student) {
		        printf("Instruktori nuk ka asnje student te regjistruar.\n");
		    }
        }
    }
    fclose(course_file);

    if (!found_courses) {
        printf("Instruktori nuk ka asnje kurs te regjistruar.\n");
    }
}

int is_valid_time(const char *time) {
	int i;
    if (strlen(time) != 5)
        return 0;
    if (time[2] != ':')
        return 0;
    for (i = 0; i < 5; i++) {
        if (i == 2)
            continue;
        if (!isdigit(time[i]))
            return 0;
    }
    int hour = (time[0] - '0') * 10 + (time[1] - '0');
    int minute = (time[3] - '0') * 10 + (time[4] - '0');
    if (hour < 8 || hour > 16 || minute < 0 || minute > 59)
        return 0;
    return 1;
}

int is_valid_category(int category) {
    if(category >= 1 && category <= 3){
        return 1;
    }
    return 0;
}

void krijo_kurs() {
    Course new_course;
    printf("Enter Kurs ID: ");
    scanf("%s", new_course.id);
    if (is_course_created(new_course.id)) {
        printf("Kjo ID eshte perdorur me perpara!\n");
        return;
    }
    int i =0;
	while (1) {
        printf("Enter start date (Format: DD-MM-YYYY): ");
        scanf("%s", new_course.start_date);;
        if (is_valid_date(new_course.start_date)) {
            break;
        } else if(i < 3){
            printf("Data e fillimit nuk eshte ne formatin e sakte. Ju lutem provoni perseri.\n");
        }else {
    		printf("Provoni perseri me vone...\n");
    		return;
		}
		i++;
    }
    if (!is_valid_start_date(new_course.start_date)) {
        printf("Ju nuk mund te vendosni nje date qe ka kaluar ose nje date per pas me shume se 3 muajsh!\n");
        return;
    }
    int j =0;
    while (1) {
        printf("Enter days of the week (e.g., 1010000 for Monday and Wednesday): ");
        scanf("%s", new_course.days_of_week);
        if(j>2){
    		printf("Provoni perseri me vone...\n");
    		return;
		}

        // Check the length of the input
        int length = strlen(new_course.days_of_week);
        if (length != 7) {
            printf("Incorrect format: The input must be exactly 7 characters long.\n");
            j++;
            continue;
        }

        // Check for valid characters and count '1's
        int count = 0;
        int valid = 1;
        int i;
        for (i = 0; i < 7; i++) {
            if (new_course.days_of_week[i] == '1') {
                count++;
            } else if (new_course.days_of_week[i] != '0') {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("Incorrect format: The input must contain only '0' or '1'.\n");
            j++;
            continue;
        }

        if (count != 2) {
            printf("You need to select exactly 2 days of the week.\n");
            j++;
            continue;
        }

        break;
    }
	i =0; 
    while (1) {
    	if(i>3){
    		printf("Provoni perseri me vone...\n");
    		return;
		}
        printf("Enter start time (Format: HH:MM): ");
        scanf("%s", new_course.start_time);
        if (is_valid_time(new_course.start_time)) {
            break;
        } else {
            printf("Ora e fillimit nuk eshte ne formatin e sakte. Ju lutem provoni perseri.\n");
            i++;
        }
    }
    i = 0;
    while (1) {
    	if(i>3){
    		printf("Provoni perseri me vone...\n");
    		return;
		}
        printf("Enter category (1 for Robotike, 2 for Programim and 3 for Graphic Design): ");
        scanf("%d", &new_course.category);
        if (is_valid_category(new_course.category)) {
            break;
        } else {
            printf("Kategoria nuk eshte e vlefshme. Ju lutem provoni perseri.\n");
            i++;
        }
    }
	i =0;
    while (1) {
    	if(i>3){
    		printf("Provoni perseri me vone...\n");
    		return;
		}
        list_instructors();
        printf("Enter instructor ID: ");
        scanf("%s", new_course.instructor_id);
        FILE *file = fopen("instruktor.dat", "rb");
        if (file == NULL) {
            return;
        }

        Instructor instructor;
        int found = 0;
        while (fread(&instructor, sizeof(Instructor), 1, file)) {
            if (strcmp(instructor.id,  new_course.instructor_id) == 0) {
                switch (new_course.category) {
                    case 1:
                        if (instructor.categories[0] == '1') {
                            found = 1;
                        } else {
                            printf("Ky instruktor nuk mund te zhvilloje kete kurs, ju lutem zgjidhni nje instruktor tjeter!\n");
                        }
                        break;
                    case 2:
                        if (instructor.categories[1] == '1') {
                            found = 1;
                        } else {
                            printf("Ky instruktor nuk mund te zhvilloje kete kurs, ju lutem zgjidhni nje instruktor tjeter!\n");
                        }
                        break;
                    case 3:
                        if (instructor.categories[2] == '1') {
                            found = 1;
                        } else {
                            printf("Ky instruktor nuk mund te zhvilloje kete kurs, ju lutem zgjidhni nje instruktor tjeter!\n");
                        }
                        break;
                    default:
                        printf("Invalid option, please try again.\n");
                }
            }
        }
        fclose(file);
        if (found == 1) {
            break;
        } else {
            printf("Invalid Instructor ID, try again.\n");
            i++;
        }
    }

    FILE *file = fopen("kurs.dat", "ab");
    if (file == NULL) {
        return;
    }
    fwrite(&new_course, sizeof(Course), 1, file);
    fclose(file);
    printf("Course created successfully.\n");
}

void list_students_in_course() {
    char course_id[20];
    list_kurs();
    printf("Enter course ID: ");
    scanf("%s", course_id);

    if (!is_course_created(course_id)) {
        printf("Kursi nuk eshte krijuar ne sistem!\n");
        return;
    }
    FILE *file = fopen("student_kurs.dat", "rb");
    if (file == NULL) {
        perror("Cannot open student_kurs file");
        return;
    }
    StudentCourse sc;
    int found = 0;
    printf("List of Students for Course ID %s:\n", course_id);
    printf("%-20s %-30s %-30s %-11s\n", "Student ID", "First Name", "Last Name", "Birth Date");

    while (fread(&sc, sizeof(StudentCourse), 1, file)) {
        if (strcmp(sc.course_id, course_id) == 0) {
            found = 1;
            FILE *student_file = fopen("student.dat", "rb");
            if (student_file == NULL) {
                fclose(file);
                return;
            }

            Student student;
            while (fread(&student, sizeof(Student), 1, student_file)) {
                if (strcmp(student.id, sc.student_id) == 0) {
                    printf("%-20s %-30s %-30s %-11s\n", student.id, student.first_name, student.last_name, student.birth_date);
                    break;
                }
            }
            fclose(student_file);
        }
    }
    fclose(file);

    if (!found) {
        printf("Nuk ka studente te regjistruar ne kete kurs.\n");
    }
}

int is_any_student_registered_in_course(const char *course_id) {
    FILE *file = fopen("student_kurs.dat", "rb");
    if (file == NULL) {
        return 0;
    }
    StudentCourse sc;
    while (fread(&sc, sizeof(StudentCourse), 1, file)) {
        if (strcmp(sc.course_id, course_id) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void delete_course() {
    char course_id[20];
    list_kurs();
    printf("Enter course ID to delete: ");
    scanf("%s", course_id);

    if (!is_course_created(course_id)) {
        printf("Kursi nuk eshte krijuar ne sistem!\n");
        return;
    }

    if (is_any_student_registered_in_course(course_id)) {
        printf("Kursi nuk mund te fshihet pasi ne te jane regjistruar studente.\n");
        return;
    }

    FILE *file = fopen("kurs.dat", "rb");
    if (file == NULL) {
        return;
    }

    FILE *temp_file = fopen("temp_kurs.dat", "wb");
    if (temp_file == NULL) {
        fclose(file);
        return;
    }

    Course course;
    int found = 0;
    while (fread(&course, sizeof(Course), 1, file)) {
        if (strcmp(course.id, course_id) == 0) {
            found = 1;
        } else {
            fwrite(&course, sizeof(Course), 1, temp_file);
        }
    }
    fclose(file);
    fclose(temp_file);

    if (found) {
        remove("kurs.dat");
        rename("temp_kurs.dat", "kurs.dat");
        printf("Kursi u fshi me sukses.\n");
    } else {
        remove("temp_kurs.dat");
        printf("Kursi nuk u gjet.\n");
    }
}

void kurs_menu() {
    int option;
    while (1) {
        printf("\nMenu Kurs\n");
        printf("1. Krijo\n");
        printf("2. Listo\n");
        printf("3. Fshij\n");
        printf("4. Listoje te gjithe femijet qe jane regjistruar ne nje kurs te caktuar\n");
        printf("5. Kthehu Mbrapa\n");
        printf("Zgjidh: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                krijo_kurs();
                break;
            case 2:
                list_kurs();
                break;
            case 3:
				delete_course();
                break;
            case 4:
                list_students_in_course();
                break;
            case 5:
                return;
            default:
                printf("Invalid option, please try again.\n");
        }
    }
}

void student_menu() {
    int option;

    while (1) {
        printf("\nMenu Student\n");
        printf("1. Krijo Student\n");
        printf("2. Listo te gjithe Studentet\n");
        printf("3. Regjistro Student ne Kurs\n");
        printf("4. Cregjistro nje Student nga nje Kurs\n");
		printf("5. Listo kurset e nje studenti\n");
		printf("6. Kthehu Mbrapa\n");
        printf("Zgjidh: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                register_student();
                break;
            case 2:
                list_students();
                break;
            case 3:
            	register_student_in_course();
                break;
            case 4:
            	deregister_student_from_course();
                break;
            case 5:
            	list_courses_of_student();
            	break;
            case 6:
            	return;
            default:
                printf("Invalid option, please try again.\n");
        }
    }
}

void instruktor_menu() {
    int option;
    while (1) {
        printf("\nMenu Instruktor\n");
        printf("1. Krijo\n");
        printf("2. Listo te gjithe Instruktoret\n");
        printf("3. Cregjistro Instruktor\n");
        printf("4. Listo te gjithe kurset qe nje instruktor jep\n");
        printf("5. Listo te gjithe femijet qe nje instruktor ka\n");
        printf("6. Kthehu Mbrapa\n");
        printf("Zgjidh: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                create_instructor();
                break;
            case 2:
                list_instructors();
                break;
            case 3:
                delete_instructor();
                break;
            case 4:
                list_courses_of_instructor();
                break;
            case 5:
                list_students_of_instructor();
                break;
            case 6:
                return;
            default:
                printf("Invalid option, please try again.\n");
        }
    }
}

void main_menu() {
    int option;
    while (1) {
        printf("\nCourse Management System\n");
        printf("1. Student\n");
        printf("2. Instructor\n");
        printf("3. Kurse\n");
        printf("4. Dil\n");
        printf("Zgjidh: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                student_menu();
                break;
            case 2:
                instruktor_menu();
                break;
            case 3:
                kurs_menu();
                break;
            case 4:
                return;
            default:
                printf("Invalid option, please try again.\n");
        }
    }
}

int main() {
    main_menu();
    return 0;
}



