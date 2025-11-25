#include<stdio.h>

int main(){
    int arr[]={10,4,80,5,48};
    int n=sizeof(arr)/sizeof(arr[0]);

    for (int i = 0; i < n-1; i++)
    {
        for (int j = i; j < n-i-1; j++)
        {
            if (arr[j]>arr[j+1])
            {
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
            
        }
        
    }
    for (int i = 0; i < n; i++)
    {
        printf("%d\t",arr[i]);
    }
    return 0;
}

// :-repeatedly swap the adjacent element that are in wrong order

// time: O(n^2), space: O(1), stable: yes