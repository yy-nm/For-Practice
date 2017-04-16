
/*
	GaussianBlurMatrixGen: generate Gaussian Matrix[nxn] by given standard deviation(σ) and radius(r), (n = 2 * r + 1)
	Gaussian distribution formula in two-dimensions: G(x, y) = 1 * (e ^ (-(x*x + y*y) / (2 * σ * σ))) / (2 * pi * σ * σ)
	Gaussian distribution formula in one-dimensions: G(x) = 1 * (e ^ (-(x*x) / (2 * σ * σ))) / (2 * pi * σ * σ)^(1/2)
*/

#include <stdio.h>
#include <math.h>

#include "PreDefine.h"

static const double PI = 3.14159265358979;


EXPORT int GetGaussianMatrixIn2d(float matrix[], int matrixlen, float sd, int r)
{
	int len = (r + r + 1) * (r + r + 1);
	if (len > matrixlen)
		return 1;

	// 8/8 section
	for (int x = 0; x <= r; x++) {
		for (int y = 0; y <= x; y++) {
			matrix[(r + r + 1) * (r + y) + x + r] = (float)(exp(-(y * y + x * x) / (2 * sd * sd)) / (2 * PI * sd * sd));
		}
	}

	// 8/8 -> 1/8 section
	for (int x = 0; x <= r; x++) {
		for (int y = 1; y <= x; y++) {
			matrix[(r + r + 1) * (r - y) + x + r] = matrix[(r + r + 1) * (r + y) + x + r];
		}
	}

	// 8/8 + 1/8 -> 5/8 + 4/8
	for (int x = 1; x <= r; x++) {
		for (int y = -x; y <= x; y++) {
			matrix[(r + r + 1) * (r + y) + r - x] = matrix[(r + r + 1) * (r + y) + r + x];
		}
	}

	// 1/8 -> 2/8 section
	for (int y = 0; y <= r; y++) {
		for (int x = 0; x < r - y; x++) {
			matrix[(r + r + 1) * y + x + r] = matrix[(r + r + 1) * (r - x) + r - y + r];
		}
	}

	// 2/8 -> 3/8 section
	for (int y = 0; y <= r; y++) {
		for (int x = 1; x < r - y; x++) {
			matrix[(r + r + 1) * y + r - x] = matrix[(r + r + 1) * y + r + x];
		}
	}

	// 2/8 + 3/8 -> 7/8 + 6/8
	for (int y = 1; y <= r; y++) {
		for (int x = -y + 1; x < y; x++) {
			matrix[(r + r + 1) * (r + y) + r + x] = matrix[(r + r + 1) * (r - y) + r + x];
		}
	}

	float total = 0.0f;
	for (int i = 0; i < len; i++) {
		total += matrix[i];
	}
	for (int i = 0; i < len; i++) {
		matrix[i] /= total;
	}

	return 0;
}

EXPORT int GetGaussianMatrixIn1d(float matrix[], int matrixlen, float sd, int r)
{
	int len = (r + r + 1);
	if (len > matrixlen)
		return 1;

	for (int x = 0; x <= r; x++) {
		matrix[x + r] = (float)(exp(-(x * x) / (2 * sd * sd)) / pow(2 * PI * sd * sd, 1 / 2));
	}

	for (int x = 0; x < r; x++) {
		matrix[x] = matrix[r + r - x];
	}

	float total = 0.0f;
	for (int i = 0; i < len; i++) {
		total += matrix[i];
	}
	for (int i = 0; i < len; i++) {
		matrix[i] /= total;
	}

	return 0;
}