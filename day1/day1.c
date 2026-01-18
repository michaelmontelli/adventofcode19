#include <stdio.h>

#define INPUT_FILE_NAME "input.txt"

/* Fuel required to launch a given module is based on its mass.
 * Specifically, to find the fuel required for a module,
 * take its mass, divide by three, round down, and subtract 2. 
 *
 * Fuel also requires fuel, and that fuel requires fuel, and so on.
 * Any mass that would require negative fuel should instead
 * be treated as if it requires zero fuel */
int calculateFuel(int mass) {
    int fuel = 0;
    while ((mass = (mass / 3) - 2) > 0) {
        fuel += mass;
    }

    return fuel;
}

int main(void) {
    FILE *fd = fopen(INPUT_FILE_NAME, "r");
    if (fd == NULL) {
        fprintf(stderr, "No file named %s\n", INPUT_FILE_NAME);
        return 1;
    }

    int mass;
    int totalFuel = 0;
    while (fscanf(fd, "%d\n", &mass) != EOF) {
        totalFuel += calculateFuel(mass);
    }
    fclose(fd);
    printf("Total fuel required: %d\n", totalFuel);

    return 0;
}
