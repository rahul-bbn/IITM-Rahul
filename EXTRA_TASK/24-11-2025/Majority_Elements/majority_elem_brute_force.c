#include<stdio.h>

int findMajority(int *arr,int n){
    for (int i = 0; i < n; i++)
    {
        int freq=0;
        for (int j = 0; j < n; j++)
        {
            if (arr[i]==arr[j])
            {
               freq++;
            }
            
        }
        
        if (freq>n/2) return arr[i];   
    }
    return -1;
}

int main(){
    int arr[]={2,2,1,1,3,3,2};
    int n=sizeof(arr)/sizeof(arr[0]);

    int result=findMajority(arr,n);

    if (result!=-1)
    {
        printf("majority element=%d",result);
    }else{
        printf("majority element not found");
    }
    return 0;
}