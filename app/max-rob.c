#include "max-rob.h"

/**
 * 1. 自上而下top-down分治，递归程序，不记录重复节点。执行效率O(2^n)。执行时间测试97us。
 * 2. 自上而下top-down分治，递归程序，记录重复/等价节点，有stack overflow风险。执行效率O(n)。执行时间测试2us。
 * 3. 自下而上down-top分治，记录重复/等价节点，Dynamic Program，性能较好。执行效率O(n)。执行时间测试1us。 
 */
int MAX(int a, int b){
    return a > b ? a : b;
}

int MaxRobTopDownDivideWithRepetion(int money[], int n){
    if(n == 1){
        return money[0];
    }
    if(n == 2){
        return MAX(money[0], money[1]);
    }
    return MAX(MaxRobTopDownDivideWithRepetion(money, n-1), 
            MaxRobTopDownDivideWithRepetion(money, n-2) + money[n-1]);
}

int MaxRobTopDownDivideWithoutRepetition(int money[], int n){
    static int maxMoney[maxMoneyLength] = {0};
    if (n == 1){
        maxMoney[0] = money[0];
    }
    if(n == 2){
        maxMoney[1] = MAX(money[0], money[1]);
    }
    if(maxMoney[n-1] == 0){
        maxMoney[n-1] = MAX(MaxRobTopDownDivideWithoutRepetition(money, n-1), 
            MaxRobTopDownDivideWithoutRepetition(money, n-2) + money[n-1]);
    }   
    return maxMoney[n-1];
}

int MaxRobDownTopDivideWithoutRepetition(int money[], int n){
    int maxMoney[maxMoneyLength] = {0};
    maxMoney[0] = money[0];
    maxMoney[1] = MAX(money[0], money[1]);
    for(int i = 2; i < n; i++){
        maxMoney[i] = MAX(maxMoney[i-1], maxMoney[i-2] + money[i]);
    }
    return maxMoney[n-1];
}