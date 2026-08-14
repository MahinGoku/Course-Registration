#include <stdio.h>
#include <string.h>

#define MAX_COURSES 5
#define SECTIONS 12
#define TOTAL_SECTIONS 60
#define MAX_STUDENTS 3000
#define CAPACITY 50
#define WAITLIST_SIZE 100

/* =========================================================
   COURSE STRUCTURE
   ========================================================= */

struct Course
{
    char course_id[10];
    char name[60];
    char section;
    unsigned char capacity;
    unsigned char enrolled;
};


/* =========================================================
   STUDENT STRUCTURE

   registration[5] stores the selected section for each
   of the 5 courses.

   status:
   0 = not registered
   1 = enrolled
   2 = waitlisted
   ========================================================= */

struct Student
{
    unsigned int student_id;
    char name[50];

    int registration[MAX_COURSES];
    int status[MAX_COURSES];

    int registered_count;
};


/* =========================================================
   WAITLIST QUEUE
   ========================================================= */

struct Waitlist
{
    unsigned int student_id[WAITLIST_SIZE];
    int front;
    int rear;
};


/* =========================================================
   GLOBAL VARIABLES
   ========================================================= */

struct Course courses[TOTAL_SECTIONS];

struct Student students[MAX_STUDENTS];

struct Waitlist waitlists[TOTAL_SECTIONS];

int courseCount = 0;
int studentCount = 0;


/* =========================================================
   FUNCTION PROTOTYPES
   ========================================================= */

void initialize_courses(void);

void add_student(void);

void enroll_student(void);

void drop_course(void);

void waitlist_management(void);

void generate_roster(void);

void display_courses(void);

void student_schedule(void);

/* Register a single course (prototype added to avoid implicit declaration) */
void register_one_course(int studentIndex,
                         int courseNumber,
                         const char courseID[],
                         const char courseName[]);

int find_student(unsigned int student_id);

int find_course(const char course_id[], char section);

int get_course_number(const char course_id[]);

int is_student_registered(int studentIndex, int courseNumber);

void add_to_waitlist(int courseIndex,
                     unsigned int student_id);

int remove_from_waitlist(int courseIndex);

int is_student_in_waitlist(int courseIndex,
                           unsigned int student_id);

void remove_student_from_waitlist(int courseIndex,
                                  unsigned int student_id);


/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{
    int choice;

    initialize_courses();

    do
    {
        printf("\n");
        printf("============================================\n");
        printf("       COURSE REGISTRATION SYSTEM\n");
        printf("============================================\n");

        printf("1. Add Student\n");
        printf("2. Enroll Student in 5 Courses\n");
        printf("3. Drop Course\n");
        printf("4. Waitlist Management\n");
        printf("5. Generate Course Roster\n");
        printf("6. Display All Courses\n");
        printf("7. View Student Schedule\n");
        printf("0. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                add_student();
                break;

            case 2:
                enroll_student();
                break;

            case 3:
                drop_course();
                break;

            case 4:
                waitlist_management();
                break;

            case 5:
                generate_roster();
                break;

            case 6:
                display_courses();
                break;

            case 7:
                student_schedule();
                break;

            case 0:
                printf("\nProgram ended.\n");
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}


/* =========================================================
   INITIALIZE 5 COURSES × 12 SECTIONS = 60 SECTIONS
   ========================================================= */

void initialize_courses(void)
{
    char courseNames[MAX_COURSES][60] =
    {
        "Physics-I",
        "Mathematics-II",
        "Programming and Problem Solving",
        "Programming and Problem Solving Lab",
        "English-II"
    };

    char courseIDs[MAX_COURSES][10] =
    {
        "PHY101",
        "MAT102",
        "CSE113",
        "CSE114",
        "ENG102"
    };

    int index = 0;

    for (int i = 0; i < MAX_COURSES; i++)
    {
        for (int j = 0; j < SECTIONS; j++)
        {
            strcpy(courses[index].course_id,
                   courseIDs[i]);

            strcpy(courses[index].name,
                   courseNames[i]);

            courses[index].section = 'A' + j;

            courses[index].capacity = CAPACITY;

            courses[index].enrolled = 0;

            waitlists[index].front = 0;
            waitlists[index].rear = -1;

            index++;
        }
    }

    courseCount = index;
}


/* =========================================================
   ADD STUDENT
   ========================================================= */

void add_student(void)
{
    if (studentCount >= MAX_STUDENTS)
    {
        printf("\nMaximum student limit reached!\n");
        return;
    }

    unsigned int id;

    printf("\nEnter Student ID: ");
    scanf("%u", &id);

    if (find_student(id) != -1)
    {
        printf("\nStudent ID already exists!\n");
        return;
    }

    students[studentCount].student_id = id;

    printf("Enter Student Name: ");
    scanf(" %[^\n]", students[studentCount].name);

    students[studentCount].registered_count = 0;

    for (int i = 0; i < MAX_COURSES; i++)
    {
        students[studentCount].registration[i] = -1;
        students[studentCount].status[i] = 0;
    }

    studentCount++;

    printf("\nStudent added successfully!\n");
}


/* =========================================================
   FIND STUDENT
   ========================================================= */

int find_student(unsigned int student_id)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].student_id == student_id)
        {
            return i;
        }
    }

    return -1;
}


/* =========================================================
   FIND COURSE SECTION
   ========================================================= */

int find_course(const char course_id[], char section)
{
    for (int i = 0; i < courseCount; i++)
    {
        if (strcmp(courses[i].course_id, course_id) == 0 &&
            courses[i].section == section)
        {
            return i;
        }
    }

    return -1;
}


/* =========================================================
   GET COURSE NUMBER

   PHY101 = 0
   MAT102 = 1
   CSE113 = 2
   CSE114 = 3
   ENG102 = 4
   ========================================================= */

int get_course_number(const char course_id[])
{
    if (strcmp(course_id, "PHY101") == 0)
        return 0;

    if (strcmp(course_id, "MAT102") == 0)
        return 1;

    if (strcmp(course_id, "CSE113") == 0)
        return 2;

    if (strcmp(course_id, "CSE114") == 0)
        return 3;

    if (strcmp(course_id, "ENG102") == 0)
        return 4;

    return -1;
}


/* =========================================================
   CHECK IF STUDENT IS ALREADY REGISTERED
   ========================================================= */

int is_student_registered(int studentIndex,
                          int courseNumber)
{
    if (students[studentIndex]
            .status[courseNumber] != 0)
    {
        return 1;
    }

    return 0;
}


/* =========================================================
   ENROLL STUDENT IN ALL 5 COURSES
   ========================================================= */

void enroll_student(void)
{
    unsigned int studentID;

    printf("\nEnter Student ID: ");
    scanf("%u", &studentID);

    int studentIndex = find_student(studentID);

    if (studentIndex == -1)
    {
        printf("\nStudent not found!\n");
        printf("Please add the student first.\n");
        return;
    }

    if (students[studentIndex].registered_count >= MAX_COURSES)
    {
        printf("\nStudent already has registrations for all 5 courses.\n");
        return;
    }

    printf("\n============================================\n");
    printf("       COURSE ENROLLMENT\n");
    printf("============================================\n");

    printf("\nStudent: %s\n", students[studentIndex].name);
    printf("Student ID: %u\n", studentID);

    printf("\nYou must select one section (A-L)\n");
    printf("for each of the 5 courses.\n");


    /* -----------------------------------------------------
       COURSE 1 - PHYSICS
       ----------------------------------------------------- */

    register_one_course(studentIndex,
                        0,
                        "PHY101",
                        "Physics-I");


    /* -----------------------------------------------------
       COURSE 2 - MATHEMATICS
       ----------------------------------------------------- */

    register_one_course(studentIndex,
                        1,
                        "MAT102",
                        "Mathematics-II");


    /* -----------------------------------------------------
       COURSE 3 - PROGRAMMING
       ----------------------------------------------------- */

    register_one_course(studentIndex,
                        2,
                        "CSE113",
                        "Programming and Problem Solving");


    /* -----------------------------------------------------
       COURSE 4 - PROGRAMMING LAB
       ----------------------------------------------------- */

    register_one_course(studentIndex,
                        3,
                        "CSE114",
                        "Programming and Problem Solving Lab");


    /* -----------------------------------------------------
       COURSE 5 - ENGLISH
       ----------------------------------------------------- */

    register_one_course(studentIndex,
                        4,
                        "ENG102",
                        "English-II");


    printf("\n============================================\n");
    printf("        REGISTRATION COMPLETED\n");
    printf("============================================\n");

    printf("\nStudent: %s\n",
           students[studentIndex].name);

    printf("Student ID: %u\n\n",
           students[studentIndex].student_id);

    printf("Course registration status:\n");

    for (int i = 0; i < MAX_COURSES; i++)
    {
        int index =
            students[studentIndex].registration[i];

        if (index != -1)
        {
            printf("%-40s Section %c - ",
                   courses[index].name,
                   courses[index].section);

            if (students[studentIndex].status[i] == 1)
            {
                printf("ENROLLED\n");
            }
            else if (students[studentIndex].status[i] == 2)
            {
                printf("WAITLISTED\n");
            }
        }
    }
}


/* =========================================================
   REGISTER ONE COURSE

   Student chooses one section from A-L.
   ========================================================= */

void register_one_course(int studentIndex,
                         int courseNumber,
                         const char courseID[],
                         const char courseName[])
{
    char section;

    printf("\n--------------------------------------------\n");
    printf("%s (%s)\n", courseName, courseID);
    printf("--------------------------------------------\n");

    printf("Choose Section (A-L): ");
    scanf(" %c", &section);

    if (section >= 'a' && section <= 'l')
    {
        section = section - 'a' + 'A';
    }

    if (section < 'A' || section > 'L')
    {
        printf("Invalid section! Please choose A-L.\n");
        return;
    }

    int courseIndex =
        find_course(courseID, section);

    if (courseIndex == -1)
    {
        printf("Course section not found!\n");
        return;
    }


    /* Check if already registered */
    if (is_student_registered(studentIndex,
                              courseNumber))
    {
        printf("Student is already registered for this course.\n");
        return;
    }


    /* Check capacity */

    if (courses[courseIndex].enrolled
        < courses[courseIndex].capacity)
    {
        courses[courseIndex].enrolled++;

        students[studentIndex]
            .registration[courseNumber] =
            courseIndex;

        students[studentIndex]
            .status[courseNumber] = 1;

        students[studentIndex].registered_count++;

        printf("\nSuccessfully enrolled!\n");
        printf("Course: %s\n", courseName);
        printf("Section: %c\n", section);
        printf("Seats remaining: %d\n",
               courses[courseIndex].capacity -
               courses[courseIndex].enrolled);
    }
    else
    {
        printf("\nSection %c is FULL!\n",
               section);

        printf("Student will be added to the waitlist.\n");

        add_to_waitlist(courseIndex,
                        students[studentIndex].student_id);

        students[studentIndex]
            .registration[courseNumber] =
            courseIndex;

        students[studentIndex]
            .status[courseNumber] = 2;

        students[studentIndex].registered_count++;

        printf("Student has been added to the waitlist.\n");
    }
}


/* =========================================================
   ADD STUDENT TO WAITLIST
   FIFO QUEUE
   ========================================================= */

void add_to_waitlist(int courseIndex,
                     unsigned int student_id)
{
    if (waitlists[courseIndex].rear
        >= WAITLIST_SIZE - 1)
    {
        printf("\nWaitlist is full!\n");
        return;
    }

    if (is_student_in_waitlist(courseIndex,
                               student_id))
    {
        printf("\nStudent is already on the waitlist.\n");
        return;
    }

    waitlists[courseIndex].rear++;

    waitlists[courseIndex]
        .student_id[
            waitlists[courseIndex].rear
        ] = student_id;

    printf("Waitlist position: %d\n",
           waitlists[courseIndex].rear -
           waitlists[courseIndex].front + 1);
}


/* =========================================================
   CHECK WAITLIST
   ========================================================= */

int is_student_in_waitlist(int courseIndex,
                           unsigned int student_id)
{
    for (int i = waitlists[courseIndex].front;
         i <= waitlists[courseIndex].rear;
         i++)
    {
        if (waitlists[courseIndex].student_id[i]
            == student_id)
        {
            return 1;
        }
    }

    return 0;
}


/* =========================================================
   REMOVE STUDENT FROM WAITLIST

   Used if the student drops a waitlisted course.
   ========================================================= */

void remove_student_from_waitlist(int courseIndex,
                                  unsigned int student_id)
{
    int position = -1;

    for (int i = waitlists[courseIndex].front;
         i <= waitlists[courseIndex].rear;
         i++)
    {
        if (waitlists[courseIndex].student_id[i]
            == student_id)
        {
            position = i;
            break;
        }
    }

    if (position == -1)
    {
        return;
    }

    for (int i = position;
         i < waitlists[courseIndex].rear;
         i++)
    {
        waitlists[courseIndex].student_id[i] =
            waitlists[courseIndex].student_id[i + 1];
    }

    waitlists[courseIndex].rear--;

    if (waitlists[courseIndex].rear <
        waitlists[courseIndex].front)
    {
        waitlists[courseIndex].front = 0;
        waitlists[courseIndex].rear = -1;
    }
}


/* =========================================================
   REMOVE FIRST STUDENT FROM WAITLIST

   FIFO
   ========================================================= */

int remove_from_waitlist(int courseIndex)
{
    if (waitlists[courseIndex].front >
        waitlists[courseIndex].rear)
    {
        return -1;
    }

    unsigned int studentID =
        waitlists[courseIndex]
        .student_id[
            waitlists[courseIndex].front
        ];

    waitlists[courseIndex].front++;

    if (waitlists[courseIndex].front >
        waitlists[courseIndex].rear)
    {
        waitlists[courseIndex].front = 0;
        waitlists[courseIndex].rear = -1;
    }

    return studentID;
}


/* =========================================================
   DROP COURSE
   ========================================================= */

void drop_course(void)
{
    unsigned int studentID;
    char courseID[10];
    char section;

    printf("\nEnter Student ID: ");
    scanf("%u", &studentID);

    int studentIndex = find_student(studentID);

    if (studentIndex == -1)
    {
        printf("\nStudent not found!\n");
        return;
    }

    printf("Enter Course Code: ");
    scanf("%9s", courseID);

    printf("Enter Section (A-L): ");
    scanf(" %c", &section);

    if (section >= 'a' && section <= 'l')
    {
        section = section - 'a' + 'A';
    }

    int courseNumber =
        get_course_number(courseID);

    if (courseNumber == -1)
    {
        printf("\nInvalid course code!\n");
        return;
    }

    int courseIndex =
        find_course(courseID, section);

    if (courseIndex == -1)
    {
        printf("\nCourse or section not found!\n");
        return;
    }

    if (students[studentIndex]
            .registration[courseNumber] !=
        courseIndex)
    {
        printf("\nStudent is not registered for this section.\n");
        return;
    }


    /* -----------------------------------------------
       If student is enrolled
       ----------------------------------------------- */

    if (students[studentIndex]
            .status[courseNumber] == 1)
    {
        courses[courseIndex].enrolled--;

        printf("\nCourse dropped successfully.\n");


        /* Promote first waitlisted student */

        int nextStudent =
            remove_from_waitlist(courseIndex);

        if (nextStudent != -1)
        {
            int nextStudentIndex =
                find_student(nextStudent);

            if (nextStudentIndex != -1)
            {
                courses[courseIndex].enrolled++;

                students[nextStudentIndex]
                    .registration[courseNumber] =
                    courseIndex;

                students[nextStudentIndex]
                    .status[courseNumber] = 1;

                printf("\n============================================\n");
                printf("        WAITLIST STUDENT PROMOTED\n");
                printf("============================================\n");

                printf("Student ID: %u\n",
                       nextStudent);

                printf("Student Name: %s\n",
                       students[nextStudentIndex].name);

                printf("Course: %s\n",
                       courses[courseIndex].name);

                printf("Section: %c\n",
                       courses[courseIndex].section);
            }
        }
    }


    /* -----------------------------------------------
       If student was waitlisted
       ----------------------------------------------- */

    else if (students[studentIndex]
                 .status[courseNumber] == 2)
    {
        remove_student_from_waitlist(
            courseIndex,
            studentID
        );

        printf("\nStudent removed from waitlist.\n");
    }


    /* Clear student's registration */

    students[studentIndex]
        .registration[courseNumber] = -1;

    students[studentIndex]
        .status[courseNumber] = 0;

    students[studentIndex]
        .registered_count--;
}


/* =========================================================
   WAITLIST MANAGEMENT
   ========================================================= */

void waitlist_management(void)
{
    char courseID[10];
    char section;

    printf("\nEnter Course Code: ");
    scanf("%9s", courseID);

    printf("Enter Section (A-L): ");
    scanf(" %c", &section);

    if (section >= 'a' && section <= 'l')
    {
        section = section - 'a' + 'A';
    }

    int courseIndex =
        find_course(courseID, section);

    if (courseIndex == -1)
    {
        printf("\nCourse or section not found!\n");
        return;
    }

    printf("\n============================================\n");
    printf("                 WAITLIST\n");
    printf("============================================\n");

    printf("Course: %s\n",
           courses[courseIndex].name);

    printf("Section: %c\n\n",
           courses[courseIndex].section);


    if (waitlists[courseIndex].front >
        waitlists[courseIndex].rear)
    {
        printf("Waitlist is empty.\n");
        return;
    }

    int position = 1;

    for (int i = waitlists[courseIndex].front;
         i <= waitlists[courseIndex].rear;
         i++)
    {
        int studentIndex =
            find_student(
                waitlists[courseIndex]
                    .student_id[i]
            );

        if (studentIndex != -1)
        {
            printf("%d. Student ID: %u | Name: %s\n",
                   position,
                   students[studentIndex].student_id,
                   students[studentIndex].name);

            position++;
        }
    }
}


/* =========================================================
   GENERATE ROSTER
   ========================================================= */

void generate_roster(void)
{
    char courseID[10];
    char section;

    printf("\nEnter Course Code: ");
    scanf("%9s", courseID);

    printf("Enter Section (A-L): ");
    scanf(" %c", &section);

    if (section >= 'a' && section <= 'l')
    {
        section = section - 'a' + 'A';
    }

    int courseIndex =
        find_course(courseID, section);

    if (courseIndex == -1)
    {
        printf("\nCourse or section not found!\n");
        return;
    }

    printf("\n============================================\n");
    printf("                  ROSTER\n");
    printf("============================================\n");

    printf("Course Code : %s\n",
           courses[courseIndex].course_id);

    printf("Course Name : %s\n",
           courses[courseIndex].name);

    printf("Section     : %c\n",
           courses[courseIndex].section);

    printf("Capacity    : %d\n",
           courses[courseIndex].capacity);

    printf("Enrolled    : %d\n",
           courses[courseIndex].enrolled);

    printf("\n----------- ENROLLED STUDENTS ------------\n");

    int count = 0;

    for (int i = 0; i < studentCount; i++)
    {
        int courseNumber =
            get_course_number(courseID);

        if (students[i]
                .registration[courseNumber]
            == courseIndex &&
            students[i]
                .status[courseNumber] == 1)
        {
            count++;

            printf("%d. ID: %u | Name: %s\n",
                   count,
                   students[i].student_id,
                   students[i].name);
        }
    }

    if (count == 0)
    {
        printf("No students enrolled.\n");
    }
}


/* =========================================================
   DISPLAY ALL 60 SECTIONS
   ========================================================= */

void display_courses(void)
{
    printf("\n");
    printf("============================================================\n");
    printf("                    COURSE LIST\n");
    printf("============================================================\n");

    for (int i = 0; i < courseCount; i++)
    {
        printf("Code: %-7s | %-40s | Section: %c | %d/%d\n",
               courses[i].course_id,
               courses[i].name,
               courses[i].section,
               courses[i].enrolled,
               courses[i].capacity);
    }
}


/* =========================================================
   VIEW STUDENT SCHEDULE
   ========================================================= */

void student_schedule(void)
{
    unsigned int studentID;

    printf("\nEnter Student ID: ");
    scanf("%u", &studentID);

    int studentIndex =
        find_student(studentID);

    if (studentIndex == -1)
    {
        printf("\nStudent not found!\n");
        return;
    }

    printf("\n============================================\n");
    printf("             STUDENT SCHEDULE\n");
    printf("============================================\n");

    printf("Student ID : %u\n",
           students[studentIndex].student_id);

    printf("Student Name : %s\n\n",
           students[studentIndex].name);

    for (int i = 0; i < MAX_COURSES; i++)
    {
        int courseIndex =
            students[studentIndex].registration[i];

        if (courseIndex != -1)
        {
            printf("%-40s | Section %c | ",
                   courses[courseIndex].name,
                   courses[courseIndex].section);

            if (students[studentIndex].status[i] == 1)
            {
                printf("ENROLLED\n");
            }
            else if (students[studentIndex].status[i] == 2)
            {
                printf("WAITLISTED\n");
            }
        }
    }

    printf("\nRegistered courses: %d/%d\n",
           students[studentIndex].registered_count,
           MAX_COURSES);
}