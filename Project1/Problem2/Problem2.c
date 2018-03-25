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

// void fork_n_children(int n){
//     int i, pid;

//     if (n == 0)
//         sleep(10);
//     for(i = 0; i < n; i++) {
//         pid = fork();
//         if(pid < 0) {
//             printf("Error");
//             exit(1);
//         } else if (pid == 0) {
//             printf("Child: pid %d, ppid: %d\n", getpid(), getppid());
//             exit(0); 
//         } else  {
//             wait(NULL);
//         }
//     }
// }

void print_tree(struct tree_node *root)
{
    int i;

    printf("%s %d", root->name, root->children_no);
    for (i = 0; i < root->children_no; i++) {
        printf(" %s", root->children[i]->name);
    }
    printf("\n");
    for (i = 0; i < root->children_no; i++) {
        print_tree(root->children[i]);
    }
}

void fork_tree(struct tree_node *root){
    int i;
    pid_t pid;
    // printf("%s %d", root->name, root->children_no);
    if(root == NULL)
        return;

    if(root -> children_no == 0){
        sleep(3);

        //printf("Child:%s: pid %d, ppid: %d\n", root-> name, getpid(), getppid());
                //printf("Process pid %d exiting.......\n", getpid());

        // exit(0);
        exit(0);
        return;
    }
    else{
        wait(NULL);
        //printf("\n\n************Parent: %s, length: %d\n", root -> name, root ->children_no );
        for(i = 0; i < root->children_no; i++){
            pid = fork();

            //printf("How many forks???\n");

            if(pid < 0) {
                printf("Error");
                exit(1);
            } else if (pid == 0) {
                printf("Child:%s: pid %d, ppid: %d\n", root->children[i]-> name, getpid(), getppid());

                fork_tree(root -> children[i]);
                exit(0);
                // sleep(10);
            } else  {
                wait(NULL);
               // printf("Process pid %d exiting.......\n", getpid());
                //exit(0);
            }
        }
        return;
    }
}


int main(int argc, char* argv[])
{
    
   // read the input file
    struct tree_node *root = read_tree_file(argv[1]);
    printf("Printing the tree:\n");
    
    // print the tree
    print_tree(root);
    
    printf("\nStart printing the process tree...\n");

    printf("Child:%s: pid %d, ppid: %d\n", root-> name, getpid(), getppid());
    // print the process based on the tree
    fork_tree(root);
}