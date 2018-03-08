/**
 * pipe.c
 *
 * Demonstrates using pipes for IPC.
 *
 * Compile: gcc -g -Wall pipe.c -o pipe
 * Run: ./pipe
 */

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct tree_node
{
    char buf[1024];
    char name[32];
    int children_no;
    struct tree_node **children;
    struct tree_node *next;
};

struct tree_node* read_tree_file(const char * filename)
{
    struct tree_node *root = NULL;
    struct tree_node *prev;
    struct tree_node *iter;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("File %s does not exist\n", filename);
        exit(1);
    }

    prev = NULL;
    while (!feof(fp)) {
        int i;
        struct tree_node *node = malloc(sizeof(struct tree_node));

        if (prev) {
            prev->next = node;
        }
        node->next = NULL;

        fgets(node->buf, 1024, fp);
        if (node->buf[strlen(node->buf) - 1] == '\n') {
            node->buf[strlen(node->buf) - 1] = '\0';
        }
        sscanf(node->buf, "%s%d", node->name, &node->children_no);
        node->children = malloc(node->children_no * sizeof(struct tree_node *));
        for (i = 0; i < node->children_no; i++) {
            node->children[i] = NULL;
        }
        prev = node;
        if (root == NULL) {
            root = node;
        }
    }

    fclose(fp);

    iter = root;
    while(iter) {
        if (iter->children_no > 0) {
            int i = 0, index = 0;
            char *p = strtok(iter->buf, " ");
            while (p != NULL) {
                if (i++ >= 2) {
                    struct tree_node *inner = root;
                    while (inner) {
                        if (strcmp(inner->name, p) == 0) {
                            iter->children[index++] = inner;
                            break;
                        }
                        inner = inner->next;
                    }

                    if (inner == NULL) {
                        printf("node %s is not found\n", p);
                        exit(1);
                    }
                }
                p = strtok(NULL, " ");
            }
        }
        iter = iter->next;
    }

    return root;
}

void print_tree(struct tree_node *root)
{
    int i;

    printf("%s %d", root->name, root->children_no);
    for (i = 0; i < root->children_no; i++) {
        printfprint_tree(" %s", root->children[i]->name);
    }
    printf("\n");
    for (i = 0; i < root->children_no; i++) {
        print_tree(root->children[i]);
    }
}

int main(int argc, char *argv[]) {

    /* Creates a pipe. */
    int fd[2];

    if (argc < 2) {
        printf("./tree <filename>\n");
        return 1;
    }
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0) {
        /* Child */
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);

        char buf[11];
        while (true) {
            ssize_t bytes = read(fd[0], buf, 10);
            if (bytes == 0) {
                break;
            }
            buf[bytes] = '\0';
            printf("%s", buf);
        }
        close(fd[0]);
    } else {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        struct tree_node *root = read_tree_file(argv[1]);
        print_tree(root);
    }

    return 0;
}
