
/*
* GaussianMatrixGen.cs: use Gaussian Distribution to generate matrix for Gaussian Blur
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/﻿

using System.Collections;
using System;

namespace Mard.Tools.Blur
{

	/// <summary>
	/// GaussianMatrixGen: generate Gaussian Matrix[nxn] by given standard deviation(σ) and radius(r), (n = 2 * r + 1)
	/// Gaussian distribution formula in two-dimensions: G(x, y) = 1 * (e ^ (-(x*x + y*y) / (2 * σ * σ))) / (2 * pi * σ * σ)
	/// Gaussian distribution formula in one-dimensions: G(x) = 1 * (e ^ (-(x*x) / (2 * σ * σ))) / (2 * pi * σ * σ)^(1/2)
	/// </summary>
	public class GaussianMatrixGen
	{
		/// <summary>
		/// GetGaussianMatrixIn2d: Gets the gaussian matrix[nxn] by given standard deviation(σ) and radius(r), (n = 2 * r + 1)
		/// </summary>
		/// <returns>gaussian matrix[nxn] in array</returns>
		/// <param name="sd">standard deviation(σ)</param>
		/// <param name="r">radius</param>
		public static float[] GetGaussianMatrixIn2d(float sd, int r)
		{
			float[] result = new float[(r + r + 1) * (r + r + 1)];

			// 8/8 section
			for (int x = 0; x <= r; x++) {
				for (int y = 0; y <= x; y++) {
					result [(r + r + 1) * (r + y) + x + r] = (float)(Math.Exp (-(y * y + x * x) / (2 * sd * sd)) / (2 * Math.PI * sd * sd));
				}
			}

			// 8/8 -> 1/8 section
			for (int x = 0; x <= r; x++) {
				for (int y = 1; y <= x; y++) {
					result [(r + r + 1) * (r - y) + x + r] = result [(r + r + 1) * (r + y) + x + r];
				}
			}

			// 8/8 + 1/8 -> 5/8 + 4/8
			for (int x = 1; x <= r; x++) {
				for (int y = -x; y <= x; y++) {
					result [(r + r + 1) * (r + y) + r - x] = result [(r + r + 1) * (r + y) + r + x];
				}
			}

			// 1/8 -> 2/8 section
			for (int y = 0; y <= r; y++) {
				for (int x = 0; x < r - y; x++) {
					result [(r + r + 1) * y + x + r] = result [(r + r + 1) * (r - x) + r - y + r];
				}
			}

			// 2/8 -> 3/8 section
			for (int y = 0; y <= r; y++) {
				for (int x = 1; x < r - y; x++) {
					result [(r + r + 1) * y + r - x] = result [(r + r + 1) * y + r + x];
				}
			}

			// 2/8 + 3/8 -> 7/8 + 6/8
			for (int y = 1; y <= r; y++) {
				for (int x = -y + 1; x < y; x++) {
					result [(r + r + 1) * (r + y) + r + x] = result [(r + r + 1) * (r - y) + r + x];
				}
			}

			float total = 0.0f;
			int count = result.Length;
			for (int i = 0; i < count; i++) {
				total += result [i];
			}
			for (int i = 0; i < count; i++) {
				result [i] /= total;
			}

			return result;
		}

		/// <summary>
		/// GetGaussianMatrixIn1d: Gets the gaussian matrix[1xn] by given standard deviation(σ) and radius(r), (n = 2 * r + 1)
		/// </summary>
		/// <returns>gaussian matrix[1xn] in array</returns>
		/// <param name="sd">standard deviation(σ)</param>
		/// <param name="r">radius</param>
		public static float[] GetGaussianMatrixIn1d(float sd, int r)
		{
			float[] result = new float[(r + r + 1)];

			for (int x = 0; x <= r; x++) {
				result [x + r] = (float)(Math.Exp (-(x * x) / (2 * sd * sd)) / Math.Pow(2 * Math.PI * sd * sd, 1/2));
			}

			for (int x = 0; x < r; x++) {
				result [x] = result [r + r - x];
			}

			float total = 0.0f;
			int count = result.Length;
			for (int i = 0; i < count; i++) {
				total += result [i];
			}
			for (int i = 0; i < count; i++) {
				result [i] /= total;
			}

			return result;
		}
	}

}
