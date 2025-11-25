#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Book{
    int price;
    char title[50];
    char auther[50];
    struct Book* link;
};

struct Book* createBook(char *title, char *auther, int price){
    struct Book* newBook=(struct Book*)malloc(sizeof(struct Book));

    strcpy(newBook->title,title);
    strcpy(newBook->auther,auther);
    newBook->price=price;
    newBook->link=NULL;

    return newBook;
}

int main(){
    struct Book* head=NULL, *temp=NULL,*ptr=NULL;

    int n,price;
    char title[50],auther[50];

    printf("Enter number of book:");
    scanf("%d",&n);
    getchar();

    for (int i = 0; i < n; i++)
    {
        printf("Enter details of book %d\n",i+1);

        printf("Enter title:");
        fgets(title,sizeof(title),stdin);
        title[strcspn(title,"\n")]='\0';

        printf("Enter auther:");
        fgets(auther,sizeof(auther),stdin);
        auther[strcspn(auther,"\n")]='\0';

        printf("Enter price:");
        scanf("%d",&price);
        getchar();

        struct Book* newBook=createBook(title,auther,price);

        if (head==NULL)
        {
           head=newBook;
        }else{
            temp=head;
            while (temp->link!=NULL)
            {
                temp=temp->link;
            }
            temp->link=newBook;
        } 
    }

    char searchTitle[50];
    printf("Enter book name to search:");
    fgets(searchTitle,sizeof(searchTitle),stdin);
    searchTitle[strcspn(searchTitle,"\n")]='\0';

    int found=0;
    ptr=head;

    while (ptr!=NULL)
    {
        if (strcmp(ptr->title,searchTitle)==0)
        {
            printf("\nBook found!\n");
            printf("Title: %s\n",ptr->title);
            printf("Auther: %s\n",ptr->auther);
            printf("Price: %d\n",ptr->price);

            found=1;
        }
        
        ptr=ptr->link;
    }
    if (!found)
    {
        printf("\nBook not found!\n");
    }
    
    return 0;
}