#include<stdio.h>

void repeatMissing(int* arr,int n){
    long long S=(long long)n*(n+1)/2;
    long long P=(long long)n*(n+1)*(2*n+1)/6;

    long long sumArr=0,sumSqrArr=0;
    for (int i = 0; i < n; i++)
    {
        sumArr+=arr[i];
        sumSqrArr+=(long long)arr[i]*arr[i];
    }

    long long val1=sumArr-S;
    long long val2=sumSqrArr-P;

    long long val3=val2/val1;

    long long R=(val1+val3)/2;
    long long M=R-val1;

    printf("Repeated: %lld\n", R);
    printf("Missing : %lld\n", M);
    
}

int main(){
    int arr[]={3, 1, 2, 5, 3};
    int n=sizeof(arr)/sizeof(arr[0]);

    repeatMissing(arr,n);

    return 0;
}