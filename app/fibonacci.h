#ifndef FIBONACCI_H_
#define FIBONACCI_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define maxLength 200

int FiboStateMachineNonRecursive(int n);
int FiboRecursiveWithoutRepetition(int n);
int FiboRecursiveWithRepetition(int n);
int FiboDynamicProgram(int n);

#endif