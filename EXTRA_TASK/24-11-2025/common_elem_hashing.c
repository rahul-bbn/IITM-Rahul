#include<stdio.h>

int main(){
    int a[] = {1,2,2,3,4,5};
    int b[] = {2,2,3,6};
    int freq[1000]={0};

    int n=sizeof(a)/sizeof(a[0]);
    int m=sizeof(b)/sizeof(b[0]);

    for (int i = 0; i < n; i++)
    {
        freq[a[i]]++;
    }

    printf("Common element:");

    for (int i = 0; i < m; i++)
    {
        if (freq[b[i]]>0)
        {
            printf("%d\t",b[i]);
            freq[b[i]]--;
        }    
    }
    return 0;
}