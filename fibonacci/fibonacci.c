#include "fibonacci.h"

/**
 * 1. 递归转状态机，迭代程序，无栈溢风险，但执行速度慢。执行效率O(2^n)。执行时间测试46ms。
 * 2. top-down分治，递归程序，但不进行等价/重复节点的计算，执行效率大幅提升。执行效率O(n)。
 *    执行时间1us。
 * 3. top-down分治，递归程序，会计算重复节点，有栈溢风险。执行效率O(2^n)。执行时间测试6ms。
 * 4. down-top分治，且记录等价/重复节点，避免重复计算，以空间换时间，性能最佳(Dynamic Pro
 *    gram)。执行效率O(n)。执行时间测试0ns。
 */

typedef struct stackFrame{
    int pc;
    int n;
    int a,b;
}stackFrame;
stackFrame stack[64];
stackFrame *top = stack - 1;
int retval = 0;
int FiboStateMachineNonRecursive(int n){
    //initial the first stackFrame
    top++;
    (*top).pc = 0;
    (*top).n = n;

    //state transform
    while(1){
        if (top < stack) {
            // No top-most frame any more; we're done.
            break;
        }

        int n = top->n;

        switch(top->pc){
            case 0: 
                top->pc++;
                if(n == 0){
                    top--;
                    retval = 0;
                }
                break;
            case 1:
                top->pc++;
                if(n == 1){
                    top--;
                    retval = 1;
                }
                break;
            case 2:  top->pc++; top++; top->pc=0; top->n = n-1; break;
            case 3:  top->pc++; top->a = retval; break;
            case 4:  top->pc++; top++; top->pc=0; top->n = n-2; break;
            case 5:  top->pc++; top->b = retval; break;
            case 6:  top->pc++; retval = top->a + top->b; top--; break;
            default: assert(0);
        }

    }
    return retval;
}

int FiboRecursiveWithoutRepetition(int n){
    static int f[maxLength] = {0};
    if(n == 0){
        f[0] = 0;
        return f[0];
    }
    if(n == 1){
        f[1] = 1;
        return f[1];
    }
    if(f[n-1] ==0){
        f[n-1] = FiboRecursiveWithoutRepetition(n-1);
    }
    if(f[n-2] == 0){
        f[n-2] = FiboRecursiveWithoutRepetition(n-2);
    }
    return f[n-1] + f[n-2];
}

int FiboRecursiveWithRepetition(int n){
    if(n == 0){
        return 0;
    }
    if(n == 1){
        return 1;
    }
    return FiboRecursiveWithRepetition(n-1) + FiboRecursiveWithRepetition(n-2);;
}

int FiboDynamicProgram(int n){
    int f[maxLength] = {0};
    f[0] = 0;
    f[1] = 1;
    for(int i = 2; i < n + 1; i++){
        f[i] = f[i-1] + f[i-2];
    }
    return f[n];
}