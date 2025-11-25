#include<stdio.h>

int mojorityElem(int *arr,int n){
    int freq=0,ans=0;

    for (int i = 0; i < n; i++)
    {
        if(freq==0){
            ans=arr[i];
        }
        if (ans==arr[i])
        {
            freq++;
        }else{
            freq--;
        }
        
    }
    int count=0;
    for (int i = 0; i < n; i++)
    {
        if (ans==arr[i])
        {
            count++;
        }
    }
    if (count>n/2)
    {
        return ans;
    }else{
        return -1;
    }
    
    return ans;
    
}

int main(){
    int arr[]={2,2,1,1,2,2,2};
    int n=sizeof(arr)/sizeof(arr[0]);

    int result=mojorityElem(arr,n);

    if (result!=-1)
    {
        printf("majority element=%d",result);
    }else{
        printf("majority element not found");
    }
    

    return 0;

}