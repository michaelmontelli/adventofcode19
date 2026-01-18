#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE_NAME "input.txt"
#define INPUT_BUF_LENGTH 1024

#define OP_ADD 1
#define OP_MULT 2
#define OP_HALT 99

void getInput(int inputbuf[], char *filename) {
    FILE *fd = fopen(filename, "r");
    if (fd == NULL) {
        fprintf(stderr,"Error: file %s not found.\n",filename);
        exit(1);
    }

    int i = 0;
    while ((fscanf(fd, "%d,", inputbuf+i)) != EOF) {
        printf("%d: %d\n", i, inputbuf[i]);
        i++;
    }
    fclose(fd);
}

int computeResult(int arr[]) {
    int i = 0;
    while (arr[i] != OP_HALT) {
        int opcode = arr[i];
        int arg1loc = arr[i+1];
        int arg2loc = arr[i+2];
        int dest = arr[i+3];

        switch (opcode) {
            case OP_ADD:
            {
                printf(
                    "ADD on args %d and %d, storing at %d\n",
                    arg1loc,
                    arg2loc,
                    dest
                );
                arr[dest] = arr[arg1loc] + arr[arg2loc];
                break;
            }
            case OP_MULT:
            {
                printf(
                    "MULT on args %d and %d, storing at %d\n",
                    arg1loc,
                    arg2loc,
                    dest
                );
                arr[dest] = arr[arg1loc] * arr[arg2loc];
                break;
            }
        }
        i += 4;
    }
    printf("Position 0 holds %d\n", arr[0]);
    return arr[0];
}

int main(void) {
    int inputbuf[INPUT_BUF_LENGTH];
    getInput(inputbuf, INPUT_FILE_NAME);

    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            int arr[INPUT_BUF_LENGTH];
            memcpy(arr,inputbuf,sizeof(*inputbuf)*INPUT_BUF_LENGTH);
            arr[1] = noun;
            arr[2] = verb;

            int result = computeResult(arr);
            if (result == 19690720) {
                printf("FOUND, noun=%d, verb=%d\n", noun, verb);
                printf("Answer: %d\n", 100*noun+verb);
                return 0;
            }
        }
    }

    printf("Answer not found.\n");
    return 0;
}
