#include <stdio.h>
#include <string.h>

typedef struct {
    char name[20];
    int age;
    char city[20];
} Student;

// Table data
Student table[] ={
    {"Alice", 17, "Pune"},
    {"Bob", 20, "Delhi"},
    {"Charlie", 22, "Mumbai"},
    {"Daisy", 16, "Chennai"}
};
int table_size = 4;

// Function prototypes for each operator
void table_scan_open();
Student* table_scan_next();
void table_scan_close();

void filter_open();
Student* filter_next();
void filter_close();
void projection_open();
void projection_next();
void projection_close();

//Table Scan Operator
int current_index=0;

void table_scan_open(){
    current_index=0;
    printf("TableScan: Opened table.\n");
}

Student* table_scan_next(){
    if (current_index>=table_size)
        return NULL;
    return &table[current_index++];
}

void table_scan_close()
{
    printf("TableScan: Closed table.\n");
}

// Filter Operator
Student* current_row = NULL;

void filter_open(){
    printf("Filter: Opened filter (age > 18).\n");
    table_scan_open();
}

Student* filter_next(){
    while ((current_row=table_scan_next())!=NULL)
    {
        if(current_row->age>18)
            return current_row;
    }
    return NULL;
}

void filter_close()
{
    table_scan_close();
    printf("Filter: Closed filter.\n");
}

//Projection
void projection_open() {
    printf("Projection: Opened projection (name, age).\n");
    filter_open();
}

void projection_next(){
    Student* row;
    while ((row=filter_next())!=NULL)
    {
        printf("Result Name: %s, Age: %d\n",row->name,row->age);
    }
}

void projection_close(){
    filter_close();
    printf("Projection: Closed projection.\n");
}

int main(){
    projection_open();
    projection_next();
    projection_close();
    return 0;
}
