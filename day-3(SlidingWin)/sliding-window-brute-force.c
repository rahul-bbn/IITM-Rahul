// You are given an array of integers nums, there is a sliding window of size k which is moving from the very left of the array to the very right. You can only see the k numbers in the window. Each time the sliding window moves right by one position.

// Return the max sliding window.

// Input: nums = [1,3,-1,-3,5,3,6,7], k = 3
// Output: [3,3,5,5,6,7]


#include<stdio.h>

void maximumSlidingWindow(int* nums, int numsSize, int k)
{
    for (int iterator = 0; iterator <= numsSize - k; iterator++)
    {
        int maximumWindowValue = nums[iterator];
        for (int jterator = 1; jterator < k; jterator++)
        {
            if(nums[iterator + jterator] > maximumWindowValue)
            {
                maximumWindowValue = nums [iterator + jterator];
            }
        }
        printf("%d", maximumWindowValue);
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

// time complexity - O(n*k)