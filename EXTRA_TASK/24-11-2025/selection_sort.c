#include<stdio.h>

int main(){
    int arr[]={10,4,80,5,48};
    int n=sizeof(arr)/sizeof(arr[0]);

    for (int i = 0; i < n-1; i++)
    {
        int min_pos=i;
        for (int j = i+1; j < n; j++)
        {
            if (arr[j]<arr[min_pos])
            {
                min_pos=j;
            }
            
        }
        int temp=arr[i];
        arr[i]=arr[min_pos];
        arr[min_pos]=temp;
        
    }

    for (int i = 0; i < n; i++)
    {
        printf("%d\t",arr[i]);
    }
    return 0;
    
}