
#include "PerlinNoiseLib.h"
#include <cmath>

int PerlinNoise::p[512];

PerlinNoise::PerlinNoise()
{
    for (int i = 0; i < 256; i++) {
        p[i] = permutation[i];
        p[i + 256] = permutation[i];
    }
}
double PerlinNoise::noise(double x, double y)
{
    int X = (int)std::floor(x) & 255;
    int Y = (int)std::floor(y) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    double u = fade(x);
    double v = fade(y);

    int A = p[X] + Y;
    int B = p[X + 1] + Y;

    return lerp(v, lerp(u, grad(p[A], x, y),
           grad(p[B], x - 1, y)),
           lerp(u, grad(p[A + 1], x, y - 1),
           grad(p[B + 1], x - 1, y - 1)));
}

double PerlinNoise::grad(int hash, double x, double y)
{
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	
}