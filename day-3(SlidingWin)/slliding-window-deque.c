#include<stdio.h>

void maximumSlidingWindow(int* nums, int numsSize, int k)
{
    int deque[numsSize];
    int front = 0, back = -1;

    for (int iterator = 0; iterator < numsSize; iterator++)
    {
        if(front <= back && deque[front]<= iterator - k)
        {
            front++;
        }

        while(front<= back && nums[deque[back]] <= nums[iterator])
        {
            back--;
        }

        deque[++back] = iterator;

        if (iterator >= k - 1)
        {
            printf("%d", nums[deque[front]]);
        }
    }
    printf("\n");
}

int main()
{
    int nums[] = {1,3,-1,-3,5,3,6,7};
    int k = 3;
    int n = sizeof(nums)/sizeof(nums[0]);

    maximumSlidingWindow(nums, n, k);
    return 0;
}



// deque:[]
// window:

//iterator = 0, nums[iterator] = 1
//deque:[0] //value=1
//window:[1]

//iterator =1, nums[iterator] = 3
//deque[1] //value=3
//window[1,3]

//iterator = 2, nums[iterator] = -1
//deque [1,2] // value = [3,-1]
//window : [1,3,-1]
//max = 3 {nums[1]}

//iterator = 3, nums[iterator] = -3
//deque [1,2,3] // values = [3,-1,-3]
//window:[3,-1,-3]
//max = 3  {nums[1]}

//iterator = 4, nums[iterator] = 5
//deque[4] //value = [5]
//window:[-1,-3,5]
//max = 5 {nums[4]}

//iterator = 5, nums[iterator] = 3
//deque[4,5] //value = [5,3]
//window:[-3,5,3]
//max = 5 {nums[4]}

//iterator = 6, nums[iterator] = 6
//deque[6] //value = [6]
//window:[5,3,6]
//max = 6 {nums[6]}

//iterator = 7, nums[iterator] = 7
//deque[7] //value = [7]
//window:[3,6,7]
//max = 7 {nums[7]}


//335567

//Time complexity - O(N)