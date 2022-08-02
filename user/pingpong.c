//
// Created by Qi Gao on 2022/8/2.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p_1[2];
    int p_2[2];
    pipe(p_1); // pipe from parent[1] to child[0]
    pipe(p_2); // pipe from child to parent
    if (fork() == 0) {
        // Child
        int child_id = getpid();
        char byte;
        // Receive byte through pipe p_1
        close(p_1[1]);
        read(p_1[0], &byte, 1);
        close(p_1[0]);
        printf("%d: received ping\n", child_id);

        // Write byte through pipe p_2
        close(p_2[0]);
        write(p_2[1], &byte, 1);
        close(p_2[1]);
    } else {
        // Parent
        int parent_id = getpid();
        char byte = 'a';
        // Write byte through pipe p_1
        close(p_1[0]);
        write(p_1[1], &byte, 1);
        close(p_1[1]);

        // Read byte through pipe p_2
        close(p_2[1]);
        read(p_2[0], &byte, 1);
        close(p_2[0]);
        printf("%d: received pong\n", parent_id);
    }
    exit(0);
}
