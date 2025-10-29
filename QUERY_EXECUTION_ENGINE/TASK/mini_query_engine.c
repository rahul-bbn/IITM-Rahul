#include <stdio.h>
#include <string.h>

typedef struct{
    char name[20];
    int age;
    char city[20];
}Student;

Student table[]={
    {"Rahul", 23, "Pune"},
    {"Gaurav", 20, "Delhi"},
    {"Kanha", 21, "Mumbai"},
    {"Gulshan", 22, "Chennai"}
};

int table_size=4;

// Table Scan
int current_index=0;
void table_scan_open()
{
    current_index=0;
    printf("TableScan: Opened table.\n");
}

Student* table_scan_next()
{
    if (current_index>=table_size)
    return NULL;
    return &table[current_index++];
}

void table_scan_close()
{
    printf("TableScan: Closed table.\n");
}

//Filter
Student* current_row=NULL;
void filter_open()
{
    printf("Filter: Opened filter.\n");
    table_scan_open();
}

Student* filter_next()
{
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

// Projection
void projection_open()
{
    printf("Projection: Opened projection.\n");
    filter_open();
}

void projection_next()
{
    Student* row;
    while ((row=filter_next())!=NULL) {
        printf("Name: %s, Age: %d\n",row->name,row->age);
    }
}

void projection_close()
{
    filter_close();
    printf("Projection: Closed projection.\n");
}

int main(){
    projection_open();
    printf("Result:\n");
    projection_next();
    projection_close();
    
    return 0;
}