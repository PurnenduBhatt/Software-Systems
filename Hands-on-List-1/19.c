/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 19
*/


#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <x86intrin.h> // For rdtsc

int main() {
    uint64_t start, end;
    uint64_t elapsed;

    
    start = __rdtsc();

    getpid();

    end = __rdtsc();

    elapsed = end - start;

    printf("Time taken by getpid: %lu cycles\n", elapsed);

    return 0;
}
/*
Time taken by getpid: 5510 cycles
*/
