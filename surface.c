#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void surface_atoms(
    const double *positions,
    int n_atoms,
    double min_r,
    double max_r,
    int n_tries,
    bool *is_surface
)
{
    int i, j, k;

    double rmin[3], rmax[3];
    double *rand_points;
    bool *too_close;

    rand_points = malloc(sizeof(double) * n_tries * 3);
    too_close = malloc(sizeof(bool) * n_tries);

    for (i = 0; i < n_tries; i++)
        too_close[i] = false;

    for (k = 0; k < 3; k++)
        rmin[k] = rmax[k] = positions[k];

    for (i = 1; i < n_atoms; i++)
    {
        for (k = 0; k < 3; k++)
        {
            double coord = positions[3 * i + k];

            if (coord < rmin[k])
                rmin[k] = coord;

            if (coord > rmax[k])
                rmax[k] = coord;
        }
    }

    for (k = 0; k < 3; k++)
    {
        rmin[k] -= min_r;
        rmax[k] += min_r;
    }

    for (i = 0; i < n_tries; i++)
    {
        for (k = 0; k < 3; k++)
        {
            double r = (double)rand() / RAND_MAX;

            rand_points[3 * i + k] =
                rmin[k] + r * (rmax[k] - rmin[k]);
        }
    }

    for (i = 0; i < n_tries; i++)
    {
        double *p = &rand_points[3 * i];

        for (j = 0; j < n_atoms; j++)
        {
            double dx = positions[3 * j + 0] - p[0];
            double dy = positions[3 * j + 1] - p[1];
            double dz = positions[3 * j + 2] - p[2];

            double d = sqrt(dx * dx + dy * dy + dz * dz);

            if (d < min_r)
            {
                too_close[i] = true;
                break;
            }
        }
    }

    for (j = 0; j < n_atoms; j++)
        is_surface[j] = false;

    for (j = 0; j < n_atoms; j++)
    {
        for (i = 0; i < n_tries; i++)
        {
            if (too_close[i])
                continue;

            double dx = positions[3 * j + 0] - rand_points[3 * i + 0];
            double dy = positions[3 * j + 1] - rand_points[3 * i + 1];
            double dz = positions[3 * j + 2] - rand_points[3 * i + 2];

            double d = sqrt(dx * dx + dy * dy + dz * dz);

            if (d < max_r)
            {
                is_surface[j] = true;
                break;
            }
        }
    }

    free(rand_points);
    free(too_close);
}