#include <stdio.h>
#include <string.h>

typedef struct {
    char name[20];
    int age;
    char city[20];
}Student;

// Table data
Student table[] ={
    {"Alice", 17, "Pune"},
    {"Bob", 20, "Delhi"},
    {"Charlie", 22, "Mumbai"},
    {"Daisy", 16, "Chennai"}
};
int table_size=4;

//Table Scan Operator
int current_index = 0;

void table_scan_open(){
    current_index=0;
    printf("TableScan: Opened table.\n");
}

Student* table_scan_next(){
    if(current_index>=table_size)
        return NULL;
    return &table[current_index++];
}

void table_scan_close(){
    printf("TableScan: Closed table.\n");
}

//Filter Operator
Student* current_row=NULL;

void filter_open(){
    printf("Filter: Opened filter (age > 18).\n");
    table_scan_open();
}

Student* filter_next() {
    while((current_row=table_scan_next())!=NULL)
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

//Limit Operator
int limit_count = 0;
int limit_value = 1;

void limit_open() {
    printf("Limit: Opened (LIMIT %d).\n",limit_value);
    filter_open();
    limit_count=0;
}

Student* limit_next(){
    if(limit_count>=limit_value)
        return NULL;
    Student* row=filter_next();
    if(row!=NULL)
        limit_count++;
    return row;
}

void limit_close(){
    filter_close();
    printf("Limit: Closed.\n");
}

// Projection Operator
void projection_open(){
    printf("Projection: Opened projection (name, age).\n");
    limit_open();
}

void projection_next(){
    Student* row;
    while((row=limit_next())!=NULL)
    {
        printf("Result → Name: %s, Age: %d\n", row->name, row->age);
    }
}

void projection_close()
{
    limit_close();
    printf("Projection: Closed projection.\n");
}

int main() {
    projection_open();
    projection_next();
    projection_close();
    return 0;
}