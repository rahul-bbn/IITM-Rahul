#include<stdio.h>
#include<limits.h>

void secondLargestNum(int *arr,int n){

    int max = INT_MIN, secondMax = INT_MIN;
    for(int i=0;i<n;i++){
        if(arr[i] > max){
            secondMax = max;
            max = arr[i];
        }else if(arr[i] > secondMax && arr[i] != max){
            secondMax = arr[i];
        }
    }
    printf("Max= %d\nsecond Max= %d",max,secondMax);
}

int main(){
    int arr[]={20,55,3,21,18,6};
    int n=sizeof(arr)/sizeof(arr[0]);

    secondLargestNum(arr,n);
    return 0;
}