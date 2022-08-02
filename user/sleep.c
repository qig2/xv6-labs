//
// Created by Qi Gao on 2022/8/1.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char* argv[]) {
    // Print error message if the user forgets to pass an argument
    if (argc <= 1) {
        fprintf(2, "usage: sleep number_of_seconds\n");
        exit(1);
    }
    int sleep_time;
    sleep_time = atoi(argv[1]);
    sleep(sleep_time);
    exit(0);
}