//
// Created by Qi Gao on 2022/8/2.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
int main(int argc, char *argv[]) {
    char *new_argv[MAXARG]; // new argv[] for commands run on each line
    int argv_counter = 0; // counter for the current argument
    char arg_array[MAXARG][32]; // a two-dimensional array
    char cur_char; // buffer to hold the current character
    int p = 0; // counter in the current argument

    if (argc < 2) {
        fprintf(2, "Usage: xargs command");
        exit(1);
    }
    for (; argv_counter < argc - 1; argv_counter++) {
        new_argv[argv_counter] = argv[argv_counter + 1];
    }
    memset(arg_array, 0, MAXARG * 32);

    while (read(0, &cur_char, 1)) {
        if (cur_char == '\n') {
            // Reach new line
            new_argv[argv_counter] = arg_array[argv_counter]; // store the last argument into argv[]
            new_argv[++argv_counter] = 0;
            if (fork() == 0) {
                // Child
                exec(new_argv[0], new_argv);
            } else {
                // Parent
                wait(0);
                // Clean up the buffers for the next line
                for (; argv_counter >= argc - 1; argv_counter--) {
                    new_argv[argv_counter] = 0;
                }
                argv_counter++;
                p = 0;
                memset(arg_array, 0, MAXARG * 32);
            }
        } else if (cur_char == ' ') {
            // Reach new argument
            new_argv[argv_counter] = arg_array[argv_counter];
            argv_counter++;
            p = 0;
        } else {
            arg_array[argv_counter][p++] = cur_char;
        }
    }
    exit(0);
}
