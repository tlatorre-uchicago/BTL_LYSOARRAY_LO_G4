#include "util.hh"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int read_tsv_file(const char *filename, double *energy, double *values, double xunit, double yunit)
{
    int i, j;
    char line[256];
    char *str;
    double value;
    int n;

    FILE *f = fopen(filename, "r");

    if (!f) {
        fprintf(stderr, "failed to '%s': %s\n", filename, strerror(errno));
        return -1;
    }

    i = 0;
    n = 0;
    /* For the first pass, we just count how many values there are. */
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        if (len && (line[len-1] != '\n')) {
            fprintf(stderr, "got incomplete line on line %i: '%s'\n", i, line);
            goto err;
        }

        i += 1;

        if (!len) continue;
        else if (line[0] == '#') continue;

        str = strtok(line," \n");

        while (str) {
            value = strtod(str, NULL);
            str = strtok(NULL," \n");
        }

        n += 1;
    }

    i = 0;
    n = 0;
    /* Now, we actually store the values. */
    rewind(f);
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        if (len && (line[len-1] != '\n')) {
            fprintf(stderr, "got incomplete line on line %i: '%s'\n", i, line);
            goto err;
        }

        i += 1;

        if (!len) continue;
        else if (line[0] == '#') continue;

        str = strtok(line," \n");

        j = 0;
        while (str) {
            value = strtod(str, NULL);
            switch (j) {
            case 0:
                energy[n] = value*xunit;
                break;
            case 1:
                values[n] = value*yunit;
                break;
            }
            j += 1;
            str = strtok(NULL," \n");
        }

        n += 1;
    }

    fclose(f);

    return n;

err:
    fclose(f);

    return -1;
}
