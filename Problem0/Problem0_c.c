#include <stdio.h>
#include <unistd.h>

static int create_two_child(size_t *backtrace, size_t id, size_t i, size_t n);

int main(void) {
  // we create array to stock id
  size_t backtrace[3];
  size_t n = sizeof backtrace / sizeof *backtrace;
  return create_two_child(backtrace, 1, 0, n);
}

// tail recursive function: child will call this function to create two new
// child
static int create_two_child(size_t *backtrace, size_t id, size_t i, size_t n) {
  if (i < n) {                       // we look our level
    for (size_t j = 0; j < 2; j++) { // we loop to create two child
      pid_t pid = fork();
      if (pid == -1) { // error
        perror("fork()");
        return 1;
      } else if (pid == 0) { // child
        backtrace[i] = id + 1;
        // id * 2 cause we create 2 child each time
        return create_two_child(backtrace, id * 2, i + 1,
                                n); // we call ourself with i + 1 and
                                    // stop function here wich return we
                                    // don't want that child continue the
                                    // loop
      }
      id++;
      // parent will continue the loop to the number of child wanted
    }
  } else {
    // if we are at max level we show our backtrace
    printf("Last Level Children: 1");
    for (size_t j = 0; j < n; j++) {
      printf(", %zu", backtrace[j]);
    }
    printf("\n");
  }
  return 0;
}