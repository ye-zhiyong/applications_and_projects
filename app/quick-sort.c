#include <quick-sort.h>

/**
 * @brief  Quick sort based recursive
 * @param  s - the array to be sorted
 *         l - the left pointer
 *         r - the right pointer           
 * @return void
 * @perfmn O(|V|); O(1);
 */
void QuickSortRecursive(int s[], int l, int r)
{
    if (l < r)
    {
        int i = l, j = r, x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x)
                j--;  
            if(i < j) 
                s[i++] = s[j];
            
            while(i < j && s[i] < x) 
                i++;  
            if(i < j) 
                s[j--] = s[i];
        }
        s[i] = x;
        quick_sort(s, l, i - 1);  
        quick_sort(s, i + 1, r);
    }
}

/**
 * @brief  Quick sort based nonrecursive and state manchine
 * @param  s - the array to be sorted
 *         l - the left pointer
 *         r - the right pointer           
 * @return void
 * @perfmn O(|V|); O(1);
 */
void quick_sort_norecursive(int s[], int l, int r){
    top++;
    top->pc = 0;
    top->l = l;
    top->r = r;
    while (1)
    {
        if(top < stk){
            break;
        }
        int l = top->l, r = top->r;
        switch (top->pc){
            case 0:
                top->pc++;
                if(l < r){
                    int i = l, j = r, x = s[l];
                    while (i < j)
                    {
                        while(i < j && s[j] >= x) 
                            j--;  
                        if(i < j) 
                            s[i++] = s[j];
                        
                        while(i < j && s[i] < x)
                            i++;  
                        if(i < j) 
                            s[j--] = s[i];
                    }
                    s[i] = x;
                }
                break;
            case 1:
                top->pc++;
                if(l < r){
                    int i = l, j = r, x = s[l];
                    top++;
                    top->pc = 0;
                    top->l = l;
                    top->r = i - 1;
                }
                break;
            case 2:
                top->pc++;
                if(l < r){
                    int i = l, j = r, x = s[l];
                    top++;
                    top->pc = 0;
                    top->l = i + 1;
                    top->r = r;
                }
                break;
            case 3:
                top->pc++;
                top--;
                break;
            default:
                break;
        }
    }
    

}

int main(void){
    int s[]={3, 0, 2, 7, 6, 1, 4, 8, 9};
    quick_sort_norecursive(s, 0, 8);
    for(int i = 0; i < 9; i++){
        printf("%d ", s[i]);
    }
}