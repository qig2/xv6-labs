//
// Created by Qi Gao on 2022/8/2.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/**
 * This recursive function only prints the first number from the left neighbor,
 * fork to create the right neighbor and passes potential primes to the right neighbor.
 * @param left_pipe Pointer to the pipe connects to the left neighbor, freed by the callee
 */
void prime(int* left_pipe) {
    int cur_prime; // first number received from the pipe, which must be prime
    int cur_read_num; // later numbers from the left pipe
    int right_pipe[2]; // pipe connects the right neighbor

    // Handle the first number
    if (read(*left_pipe, &cur_prime, 4) != 0) {
        printf("prime %d\n", cur_prime);
        pipe(right_pipe);
    } else {
        close(*left_pipe);
        exit(0);
    }

    // Handle the later numbers
    if (fork() == 0) {
        // Child
        close(right_pipe[1]);
        prime(&(right_pipe[0]));
    } else {
        // Parent
        close(right_pipe[0]);
        while(read(*left_pipe, &cur_read_num, 4) != 0) {
            if (cur_read_num % cur_prime != 0) {
                write(right_pipe[1], &cur_read_num, 4);
            }
        }
        close(right_pipe[1]);
        wait(0); // wait for its child to exit
    }
    close(*left_pipe);
    exit(0);
}

int main() {
    int left_pipe[2];
    int cur_write_num = 2;
    pipe(left_pipe);
    if (fork() == 0) {
        //Child
        close(left_pipe[1]);
        prime(&(left_pipe[0]));
    } else {
        // Parent
        close(left_pipe[0]);
        for (; cur_write_num <= 35; cur_write_num++) {
            write(left_pipe[1], &cur_write_num, 4);
        }
        close(left_pipe[1]);
        wait(0);
    }
    exit(0);
}