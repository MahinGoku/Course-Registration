#include <stdio.h>
#include <string.h>

#define MAX_COURSES 5
#define SECTIONS 12
#define TOTAL_SECTIONS 60
#define MAX_STUDENTS 3000
#define CAPACITY 50
#define WAITLIST_SIZE 100

struct Course {
    char course_id[10];
    char name[60];
    char section;
    int capacity;
    int enrolled;
};

struct Student {
    int id;
    char name[50];
    int registration[MAX_COURSES];
    int status[MAX_COURSES];
    int registered_count;
};

struct Waitlist {
    int id[WAITLIST_SIZE];
    int front;
    int rear;
};

struct Course courses[TOTAL_SECTIONS];
struct Student students[MAX_STUDENTS];
struct Waitlist waitlists[TOTAL_SECTIONS];

int studentCount = 0;
int courseCount = 0;

void initialize(void);
void addStudent(void);
void enrollStudent(void);
void registerCourse(int, int);
void dropCourse(void);
void showWaitlist(void);
void roster(void);
void displayCourses(void);
void schedule(void);

int findStudent(int);
int findCourse(char[], char);
int courseNumber(char[]);
void addWaitlist(int, int);
int removeWaitlist(int);
void removeFromWaitlist(int, int);
int inWaitlist(int, int);

int main(void)
{
    int choice;

    initialize();

    do {
        printf("\n========================================\n");
        printf("       COURSE REGISTRATION SYSTEM\n");
        printf("========================================\n");
        printf("1. Add Student\n");
        printf("2. Enroll Student\n");
        printf("3. Drop Course\n");
        printf("4. Waitlist Management\n");
        printf("5. Generate Course Roster\n");
        printf("6. Display All Courses\n");
        printf("7. View Student Schedule\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            addStudent();
        else if (choice == 2)
            enrollStudent();
        else if (choice == 3)
            dropCourse();
        else if (choice == 4)
            showWaitlist();
        else if (choice == 5)
            roster();
        else if (choice == 6)
            displayCourses();
        else if (choice == 7)
            schedule();
        else if (choice == 0)
            printf("\nProgram ended.\n");
        else
            printf("\nInvalid choice!\n");

    } while (choice != 0);

    return 0;
}

void initialize(void)
{
    char ids[MAX_COURSES][10] = {
        "PHY101",
        "MAT102",
        "CSE113",
        "CSE114",
        "ENG102"
    };

    char names[MAX_COURSES][60] = {
        "Physics-I",
        "Mathematics-II",
        "Programming and Problem Solving",
        "Programming and Problem Solving Lab",
        "English-II"
    };

    int k = 0;

    for (int i = 0; i < MAX_COURSES; i++) {
        for (int j = 0; j < SECTIONS; j++) {
            strcpy(courses[k].course_id, ids[i]);
            strcpy(courses[k].name, names[i]);

            courses[k].section = 'A' + j;
            courses[k].capacity = CAPACITY;
            courses[k].enrolled = 0;

            waitlists[k].front = 0;
            waitlists[k].rear = -1;

            k++;
        }
    }

    courseCount = k;
}

void addStudent(void)
{
    int id;

    if (studentCount >= MAX_STUDENTS) {
        printf("\nStudent limit reached!\n");
        return;
    }

    printf("\nEnter Student ID: ");
    scanf("%d", &id);

    if (findStudent(id) != -1) {
        printf("Student ID already exists!\n");
        return;
    }

    students[studentCount].id = id;

    printf("Enter Student Name: ");
    scanf(" %[^\n]", students[studentCount].name);

    students[studentCount].registered_count = 0;

    for (int i = 0; i < MAX_COURSES; i++) {
        students[studentCount].registration[i] = -1;
        students[studentCount].status[i] = 0;
    }

    studentCount++;

    printf("\nStudent added successfully!\n");
}

int findStudent(int id)
{
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id)
            return i;
    }

    return -1;
}

int findCourse(char id[], char section)
{
    for (int i = 0; i < courseCount; i++) {
        if (strcmp(courses[i].course_id, id) == 0 &&
            courses[i].section == section)
            return i;
    }

    return -1;
}

int courseNumber(char id[])
{
    char ids[MAX_COURSES][10] = {
        "PHY101",
        "MAT102",
        "CSE113",
        "CSE114",
        "ENG102"
    };

    for (int i = 0; i < MAX_COURSES; i++) {
        if (strcmp(ids[i], id) == 0)
            return i;
    }

    return -1;
}

void enrollStudent(void)
{
    int id;
    int studentIndex;

    printf("\nEnter Student ID: ");
    scanf("%d", &id);

    studentIndex = findStudent(id);

    if (studentIndex == -1) {
        printf("Student not found!\n");
        return;
    }

    if (students[studentIndex].registered_count >= MAX_COURSES) {
        printf("Student already registered for all courses.\n");
        return;
    }

    printf("\nStudent: %s\n", students[studentIndex].name);

    for (int i = 0; i < MAX_COURSES; i++)
        registerCourse(studentIndex, i);

    printf("\nRegistration completed!\n");
}

void registerCourse(int studentIndex, int number)
{
    char ids[MAX_COURSES][10] = {
        "PHY101",
        "MAT102",
        "CSE113",
        "CSE114",
        "ENG102"
    };

    char names[MAX_COURSES][60] = {
        "Physics-I",
        "Mathematics-II",
        "Programming and Problem Solving",
        "Programming and Problem Solving Lab",
        "English-II"
    };

    char section;
    int index;

    if (students[studentIndex].status[number] != 0)
        return;

    printf("\n%s (%s)\n", names[number], ids[number]);
    printf("Choose Section A-L: ");
    scanf(" %c", &section);

    if (section >= 'a' && section <= 'l')
        section = section - 'a' + 'A';

    if (section < 'A' || section > 'L') {
        printf("Invalid section!\n");
        return;
    }

    index = findCourse(ids[number], section);

    if (index == -1) {
        printf("Course not found!\n");
        return;
    }

    students[studentIndex].registration[number] = index;

    if (courses[index].enrolled < courses[index].capacity) {
        courses[index].enrolled++;
        students[studentIndex].status[number] = 1;
        students[studentIndex].registered_count++;

        printf("Enrolled in Section %c\n", section);
    }
    else {
        printf("Section is FULL!\n");

        if (inWaitlist(index, students[studentIndex].id)) {
            printf("Already on waitlist.\n");
            return;
        }

        if (waitlists[index].rear >= WAITLIST_SIZE - 1) {
            printf("Waitlist is full!\n");
            students[studentIndex].registration[number] = -1;
            return;
        }

        addWaitlist(index, students[studentIndex].id);

        students[studentIndex].status[number] = 2;
        students[studentIndex].registered_count++;

        printf("Added to waitlist.\n");
    }
}

void addWaitlist(int courseIndex, int id)
{
    if (waitlists[courseIndex].rear >= WAITLIST_SIZE - 1)
        return;

    waitlists[courseIndex].rear++;

    waitlists[courseIndex].id[waitlists[courseIndex].rear] = id;

    printf("Waitlist Position: %d\n",
           waitlists[courseIndex].rear -
           waitlists[courseIndex].front + 1);
}

int inWaitlist(int courseIndex, int id)
{
    for (int i = waitlists[courseIndex].front;
         i <= waitlists[courseIndex].rear; i++) {

        if (waitlists[courseIndex].id[i] == id)
            return 1;
         }

         return 0;
}

int removeWaitlist(int courseIndex)
{
    int id;

    if (waitlists[courseIndex].front >
        waitlists[courseIndex].rear)
        return -1;

    id = waitlists[courseIndex].id[
        waitlists[courseIndex].front
    ];

    waitlists[courseIndex].front++;

    if (waitlists[courseIndex].front >
        waitlists[courseIndex].rear) {

        waitlists[courseIndex].front = 0;
    waitlists[courseIndex].rear = -1;
        }

        return id;
}

void removeFromWaitlist(int courseIndex, int id)
{
    int pos = -1;

    for (int i = waitlists[courseIndex].front;
         i <= waitlists[courseIndex].rear; i++) {

        if (waitlists[courseIndex].id[i] == id) {
            pos = i;
            break;
        }
         }

         if (pos == -1)
             return;

    for (int i = pos;
         i < waitlists[courseIndex].rear; i++) {

        waitlists[courseIndex].id[i] =
        waitlists[courseIndex].id[i + 1];
         }

         waitlists[courseIndex].rear--;

         if (waitlists[courseIndex].rear <
             waitlists[courseIndex].front) {

             waitlists[courseIndex].front = 0;
         waitlists[courseIndex].rear = -1;
             }
}

void dropCourse(void)
{
    int id;
    char courseID[10];
    char section;

    printf("\nEnter Student ID: ");
    scanf("%d", &id);

    int studentIndex = findStudent(id);

    if (studentIndex == -1) {
        printf("Student not found!\n");
        return;
    }

    printf("Enter Course Code: ");
    scanf("%9s", courseID);

    printf("Enter Section: ");
    scanf(" %c", &section);

    if (section >= 'a' && section <= 'l')
        section = section - 'a' + 'A';

    int number = courseNumber(courseID);

    if (number == -1) {
        printf("Invalid course code!\n");
        return;
    }

    int courseIndex = findCourse(courseID, section);

    if (courseIndex == -1) {
        printf("Course or section not found!\n");
        return;
    }

    if (students[studentIndex].registration[number] !=
        courseIndex) {

        printf("Student is not registered in this section.\n");
    return;
        }

        if (students[studentIndex].status[number] == 1) {

            courses[courseIndex].enrolled--;

            printf("\nCourse dropped successfully!\n");

            int nextID = removeWaitlist(courseIndex);

            if (nextID != -1) {

                int nextIndex = findStudent(nextID);

                if (nextIndex != -1) {

                    courses[courseIndex].enrolled++;

                    students[nextIndex].registration[number] =
                    courseIndex;

                    students[nextIndex].status[number] = 1;

                    printf("Waitlisted student promoted: %d\n",
                           nextID);
                }
            }
        }
        else if (students[studentIndex].status[number] == 2) {

            removeFromWaitlist(courseIndex, id);

            printf("\nRemoved from waitlist.\n");
        }

        students[studentIndex].registration[number] = -1;
        students[studentIndex].status[number] = 0;
        students[studentIndex].registered_count--;
}

void showWaitlist(void)
{
    char courseID[10];
    char section;

    printf("\nEnter Course Code: ");
    scanf("%9s", courseID);

    printf("Enter Section: ");
    scanf(" %c", &section);

    if (section >= 'a' && section <= 'l')
        section = section - 'a' + 'A';

    int index = findCourse(courseID, section);

    if (index == -1) {
        printf("Course not found!\n");
        return;
    }

    printf("\nCourse: %s\n", courses[index].name);
    printf("Section: %c\n", courses[index].section);

    if (waitlists[index].front > waitlists[index].rear) {
        printf("Waitlist is empty.\n");
        return;
    }

    int position = 1;

    for (int i = waitlists[index].front;
         i <= waitlists[index].rear; i++) {

        int s = findStudent(waitlists[index].id[i]);

    if (s != -1) {
        printf("%d. %d - %s\n",
               position++,
               students[s].id,
               students[s].name);
    }
         }
}

void roster(void)
{
    char courseID[10];
    char section;

    printf("\nEnter Course Code: ");
    scanf("%9s", courseID);

    printf("Enter Section: ");
    scanf(" %c", &section);

    if (section >= 'a' && section <= 'l')
        section = section - 'a' + 'A';

    int index = findCourse(courseID, section);

    if (index == -1) {
        printf("Course not found!\n");
        return;
    }

    int number = courseNumber(courseID);

    printf("\n========================================\n");
    printf("             COURSE ROSTER\n");
    printf("========================================\n");

    printf("Code     : %s\n", courses[index].course_id);
    printf("Course   : %s\n", courses[index].name);
    printf("Section  : %c\n", courses[index].section);
    printf("Capacity : %d\n", courses[index].capacity);
    printf("Enrolled : %d\n", courses[index].enrolled);

    printf("\nStudents:\n");

    int count = 0;

    for (int i = 0; i < studentCount; i++) {

        if (students[i].registration[number] == index &&
            students[i].status[number] == 1) {

            printf("%d. %d - %s\n",
                   ++count,
                   students[i].id,
                   students[i].name);
            }
    }

    if (count == 0)
        printf("No students enrolled.\n");
}

void displayCourses(void)
{
    printf("\n============================================================\n");
    printf("                     COURSE LIST\n");
    printf("============================================================\n");

    for (int i = 0; i < courseCount; i++) {

        printf("%-7s | %-38s | Section %c | %d/%d\n",
               courses[i].course_id,
               courses[i].name,
               courses[i].section,
               courses[i].enrolled,
               courses[i].capacity);
    }
}

void schedule(void)
{
    int id;

    printf("\nEnter Student ID: ");
    scanf("%d", &id);

    int studentIndex = findStudent(id);

    if (studentIndex == -1) {
        printf("Student not found!\n");
        return;
    }

    printf("\n========================================\n");
    printf("           STUDENT SCHEDULE\n");
    printf("========================================\n");

    printf("Student ID   : %d\n",
           students[studentIndex].id);

    printf("Student Name : %s\n",
           students[studentIndex].name);

    for (int i = 0; i < MAX_COURSES; i++) {

        int index =
        students[studentIndex].registration[i];

        if (index != -1) {

            printf("\nCourse  : %s\n",
                   courses[index].name);

            printf("Code    : %s\n",
                   courses[index].course_id);

            printf("Section : %c\n",
                   courses[index].section);

            if (students[studentIndex].status[i] == 1)
                printf("Status  : ENROLLED\n");
            else
                printf("Status  : WAITLISTED\n");
        }
    }

    printf("\nRegistered: %d/%d\n",
           students[studentIndex].registered_count,
           MAX_COURSES);
}
