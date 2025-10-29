#include<stdio.h>
#include<stdlib.h>

#define MAX_STUDENTS 5

struct Student{
    char name[20];
    int age;
    char city[20];
};

struct Student students[MAX_STUDENTS] ={
    {"Rahul", 19, "Delhi"},
    {"Raman", 27, "Mumbai"},
    {"Sarika", 6, "Pune"},
    {"Monu", 20, "Bangalore"},
    {"Gaurav", 21, "Chennai"}
};

int tableScan(int index,struct Student *row){
    if(index<MAX_STUDENTS)
    {
        *row=students[index];
        return 1;
    }
    return 0;
}

int filter(struct Student *input,struct Student *output){
    if(input->age>18)
    {
        *output=*input;
        return 1;
    }
    return 0;
}

void project(struct Student *row){
    printf("%-10s %d\n",row->name,row->age);
}

int main() {
    struct Student scanRow,filterRow;
    printf("Query: SELECT name, age FROM students WHERE age > 18;\n\n");
    printf("Result:\n");
    printf("-------------------\n");

    for (int i = 0; i < MAX_STUDENTS; i++){
        if(tableScan(i, &scanRow))
        {
            if (filter(&scanRow, &filterRow))
            {
                project(&filterRow);
            }
        }
    }

    printf("-------------------\n");
    return 0;
}



