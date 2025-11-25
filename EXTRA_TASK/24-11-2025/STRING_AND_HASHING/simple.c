#include<stdio.h>
#include<string.h>

int main(){
    // char ch[]={'r','a','h','u','l','\0'};
    // for (int i = 0; ch[i]!= '\0'; i++)
    // {
    //     printf("%c",ch[i]);
    // }

    // char str[]="Rahul bansal";

    // char str[10];
    // scanf("%[^\n]s",str);

    // printf("%s",str);

    // char str[50];

    // printf("Enter your name:");
    // fgets(str,sizeof(str),stdin);
    // // str[strcspn(str,"\n")]='\0';

    // printf("%s",str);

    // char str[50]="Rahul";
    // char *ptr="bansal";

    // str[0]='G';
    // ptr="Rahul";

    // printf("%s\n%s",str,ptr);

    char s[] = "a,b,c,d";
    char *p = strtok(s, ",");
    while(p){
    printf("%s", p);
    p = strtok(NULL, ",");
}

    return 0;
}