
/*
* GaussianBlur.cs: implements Gaussian Blur
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/﻿

using System.Collections;
using System;
using System.Threading;

namespace Mard.Tools.Blur
{
	public class GaussianBlur
	{

		/// <summary>
		/// default Gaussian matrix which standard deviation=2, radius=3
		/// </summary>
		public static readonly float[] rDefaultMatrix = new float[]{
			0.00492233f,	0.009196123f,	0.01338028f,	0.01516184f,	0.01338028f,	0.009196123f,	0.00492233f,
			0.009196123f,	0.01718062f,	0.02499765f,	0.02832605f,	0.02499765f,	0.01718062f,	0.009196123f,
			0.01338028f,	0.02499765f,	0.03637137f,	0.04121416f,	0.03637137f,	0.02499765f,	0.01338028f,
			0.01516184f,	0.02832605f,	0.04121416f,	0.04670176f,	0.04121416f,	0.02832605f,	0.01516184f,
			0.01338028f,	0.02499765f,	0.03637137f,	0.04121416f,	0.03637137f,	0.02499765f,	0.01338028f,
			0.009196123f,	0.01718062f,	0.02499765f,	0.02832605f,	0.02499765f,	0.01718062f,	0.009196123f,
			0.00492233f,	0.009196123f,	0.01338028f,	0.01516184f,	0.01338028f,	0.009196123f,	0.00492233f,
		};
		public const float cDefaultSd = 2f;
		public const int cDefaultRadius = 3;

		public static void Blur32(byte[] src, int width, int height, byte[] dst)
		{
			Blur32 (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, dst);
		}

		public static void Blur32(byte[] src, int width, int height, float sd, int radius, byte[] dst)
		{
			if (sd == cDefaultSd && radius == cDefaultRadius)
				Blur32 (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, dst);
			else
				Blur32 (src, width, height, GaussianMatrixGen.GetGaussianMatrixIn2d(sd, radius), sd, radius, dst);
		}

		public static void Blur32(byte[] src, int width, int height, float[] matrix, float sd, int radius, byte[] dst)
		{
			float r, g, b, a;
			float rr, gg, bb, aa;
			int w;
			int w_offset;
			int h;
			int h_offset;

			int matrixlen = radius + radius + 1;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (y - radius >= 0) {
						h = y - radius;
						h_offset = 0;
					} else {
						h = 0;
						h_offset = radius - y;
					}
					if (x - radius >= 0) {
						w = x - radius;
						w_offset = 0;
					} else {
						w = 0;
						w_offset = radius - x;
					}

					r = g = b = a = 0;
					for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
						for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
							rr = src [((i + h) * width + j + w) * 4 + 0];
							gg = src [((i + h) * width + j + w) * 4 + 1];
							bb = src [((i + h) * width + j + w) * 4 + 2];
							aa = src [((i + h) * width + j + w) * 4 + 3];

							r += rr * matrix[(i + h_offset) * matrixlen + j + w_offset];
							g += gg * matrix[(i + h_offset) * matrixlen + j + w_offset];
							b += bb * matrix[(i + h_offset) * matrixlen + j + w_offset];
							a += aa * matrix[(i + h_offset) * matrixlen + j + w_offset];
						}
					}
					dst [(y * width + x) * 4 + 0] = (byte)r;
					dst [(y * width + x) * 4 + 1] = (byte)g;
					dst [(y * width + x) * 4 + 2] = (byte)b;
					dst [(y * width + x) * 4 + 3] = (byte)a;
				}
			}
		}

		public static void Blur32Horizontal(byte[] src, int width, int height, int row, byte[] dst)
		{
			Blur32Horizontal (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, row, dst);
		}

		public static void Blur32Horizontal(byte[] src, int width, int height, float sd, int radius, int row, byte[] dst)
		{
			if (sd == cDefaultSd && radius == cDefaultRadius)
				Blur32Horizontal (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, row, dst);
			else
				Blur32Horizontal (src, width, height, GaussianMatrixGen.GetGaussianMatrixIn2d(sd, radius), sd, radius, row, dst);
		}

		public static void Blur32Horizontal(byte[] src, int width, int height, float[] matrix, float sd, int radius, int row, byte[] dst)
		{
			float r, g, b, a;
			float rr, gg, bb, aa;
			int w;
			int w_offset;
			int h;
			int h_offset;

			int matrixlen = radius + radius + 1;
			int i = row;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (y - radius >= 0) {
						h = y - radius;
						h_offset = 0;
					} else {
						h = 0;
						h_offset = radius - y;
					}
					if (x - radius >= 0) {
						w = x - radius;
						w_offset = 0;
					} else {
						w = 0;
						w_offset = radius - x;
					}

					r = g = b = a = 0;
					if (i + h_offset < matrixlen && i + h < height) {
						for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
							rr = src [((i + h) * width + j + w) * 4 + 0];
							gg = src [((i + h) * width + j + w) * 4 + 1];
							bb = src [((i + h) * width + j + w) * 4 + 2];
							aa = src [((i + h) * width + j + w) * 4 + 3];

							r += rr * matrix[(i + h_offset) * matrixlen + j + w_offset];
							g += gg * matrix[(i + h_offset) * matrixlen + j + w_offset];
							b += bb * matrix[(i + h_offset) * matrixlen + j + w_offset];
							a += aa * matrix[(i + h_offset) * matrixlen + j + w_offset];
						}
						dst [(y * width + x) * 4 + 0] += (byte)r;
						dst [(y * width + x) * 4 + 1] += (byte)g;
						dst [(y * width + x) * 4 + 2] += (byte)b;
						dst [(y * width + x) * 4 + 3] += (byte)a;
					}
				}
			}
		}

		public static void Blur32Vertical(byte[] src, int width, int height, int column, byte[] dst)
		{
			Blur32Vertical (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, column, dst);
		}

		public static void Blur32Vertical(byte[] src, int width, int height, float sd, int radius, int column, byte[] dst)
		{
			if (sd == cDefaultSd && radius == cDefaultRadius)
				Blur32Vertical (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, column, dst);
			else
				Blur32Vertical (src, width, height, GaussianMatrixGen.GetGaussianMatrixIn2d(sd, radius), sd, radius, column, dst);
		}

		public static void Blur32Vertical(byte[] src, int width, int height, float[] matrix, float sd, int radius, int column, byte[] dst)
		{
			float r, g, b, a;
			float rr, gg, bb, aa;
			int w;
			int w_offset;
			int h;
			int h_offset;

			int matrixlen = radius + radius + 1;
			int j = column;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (y - radius >= 0) {
						h = y - radius;
						h_offset = 0;
					} else {
						h = 0;
						h_offset = radius - y;
					}
					if (x - radius >= 0) {
						w = x - radius;
						w_offset = 0;
					} else {
						w = 0;
						w_offset = radius - x;
					}

					r = g = b = a = 0;
					if (j + w_offset < matrixlen && j + w < width) {
						for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {

							rr = src [((i + h) * width + j + w) * 4 + 0];
							gg = src [((i + h) * width + j + w) * 4 + 1];
							bb = src [((i + h) * width + j + w) * 4 + 2];
							aa = src [((i + h) * width + j + w) * 4 + 3];

							r += rr * matrix[(i + h_offset) * matrixlen + j + w_offset];
							g += gg * matrix[(i + h_offset) * matrixlen + j + w_offset];
							b += bb * matrix[(i + h_offset) * matrixlen + j + w_offset];
							a += aa * matrix[(i + h_offset) * matrixlen + j + w_offset];
						}
						dst [(y * width + x) * 4 + 0] += (byte)r;
						dst [(y * width + x) * 4 + 1] += (byte)g;
						dst [(y * width + x) * 4 + 2] += (byte)b;
						dst [(y * width + x) * 4 + 3] += (byte)a;
					}
				}
			}
		}



		public static void Blur24(byte[] src, int width, int height, byte[] dst)
		{
			Blur24 (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, dst);

		}

		public static void Blur24(byte[] src, int width, int height, float sd, int radius, byte[] dst)
		{
			if (sd == cDefaultSd && radius == cDefaultRadius)
				Blur24 (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, dst);
			else
				Blur24 (src, width, height, GaussianMatrixGen.GetGaussianMatrixIn2d(sd, radius), sd, radius, dst);
		}

		public static void Blur24(byte[] src, int width, int height, float[] matrix, float sd, int radius, byte[] dst)
		{
			float r, g, b;
			float rr, gg, bb;
			int w;
			int w_offset;
			int h;
			int h_offset;

			int matrixlen = radius + radius + 1;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (y - radius >= 0) {
						h = y - radius;
						h_offset = 0;
					} else {
						h = 0;
						h_offset = radius - y;
					}
					if (x - radius >= 0) {
						w = x - radius;
						w_offset = 0;
					} else {
						w = 0;
						w_offset = radius - x;
					}

					r = g = b = 0;
					for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
						for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
							rr = src [((i + h) * width + j + w) * 3 + 0];
							gg = src [((i + h) * width + j + w) * 3 + 1];
							bb = src [((i + h) * width + j + w) * 3 + 2];

							r += rr * matrix[(i + h_offset) * matrixlen + j + w_offset];
							g += gg * matrix[(i + h_offset) * matrixlen + j + w_offset];
							b += bb * matrix[(i + h_offset) * matrixlen + j + w_offset];
						}
					}
					dst [(y * width + x) * 3 + 0] = (byte)r;
					dst [(y * width + x) * 3 + 1] = (byte)g;
					dst [(y * width + x) * 3 + 2] = (byte)b;
				}
			}
		}

		public static void Blur24Horizontal(byte[] src, int width, int height, int row, byte[] dst)
		{
			Blur24Horizontal (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, row, dst);
		}

		public static void Blur24Horizontal(byte[] src, int width, int height, float sd, int radius, int row, byte[] dst)
		{
			if (sd == cDefaultSd && radius == cDefaultRadius)
				Blur24Horizontal (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, row, dst);
			else
				Blur24Horizontal (src, width, height, GaussianMatrixGen.GetGaussianMatrixIn2d(sd, radius), sd, radius, row, dst);
		}

		public static void Blur24Horizontal(byte[] src, int width, int height, float[] matrix, float sd, int radius, int row, byte[] dst)
		{
			float r, g, b;
			float rr, gg, bb;
			int w;
			int w_offset;
			int h;
			int h_offset;

			int matrixlen = radius + radius + 1;
			int i = row;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (y - radius >= 0) {
						h = y - radius;
						h_offset = 0;
					} else {
						h = 0;
						h_offset = radius - y;
					}
					if (x - radius >= 0) {
						w = x - radius;
						w_offset = 0;
					} else {
						w = 0;
						w_offset = radius - x;
					}

					r = g = b = 0;
					if (i + h_offset < matrixlen && i + h < height) {
						for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
							rr = src [((i + h) * width + j + w) * 3 + 0];
							gg = src [((i + h) * width + j + w) * 3 + 1];
							bb = src [((i + h) * width + j + w) * 3 + 2];

							r += rr * matrix[(i + h_offset) * matrixlen + j + w_offset];
							g += gg * matrix[(i + h_offset) * matrixlen + j + w_offset];
							b += bb * matrix[(i + h_offset) * matrixlen + j + w_offset];
						}
						dst [(y * width + x) * 3 + 0] += (byte)r;
						dst [(y * width + x) * 3 + 1] += (byte)g;
						dst [(y * width + x) * 3 + 2] += (byte)b;
					}
				}
			}
		}

		public static void Blur24Vertical(byte[] src, int width, int height, int column, byte[] dst)
		{
			Blur24Vertical (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, column, dst);
		}

		public static void Blur24Vertical(byte[] src, int width, int height, float sd, int radius, int column, byte[] dst)
		{
			if (sd == cDefaultSd && radius == cDefaultRadius)
				Blur24Vertical (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, column, dst);
			else
				Blur24Vertical (src, width, height, GaussianMatrixGen.GetGaussianMatrixIn2d(sd, radius), sd, radius, column, dst);
		}

		public static void Blur24Vertical(byte[] src, int width, int height, float[] matrix, float sd, int radius, int column, byte[] dst)
		{
			float r, g, b;
			float rr, gg, bb;
			int w;
			int w_offset;
			int h;
			int h_offset;

			int matrixlen = radius + radius + 1;
			int j = column;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (y - radius >= 0) {
						h = y - radius;
						h_offset = 0;
					} else {
						h = 0;
						h_offset = radius - y;
					}
					if (x - radius >= 0) {
						w = x - radius;
						w_offset = 0;
					} else {
						w = 0;
						w_offset = radius - x;
					}

					r = g = b = 0;
					if (j + w_offset < matrixlen && j + w < width) {
						for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {

							rr = src [((i + h) * width + j + w) * 3 + 0];
							gg = src [((i + h) * width + j + w) * 3 + 1];
							bb = src [((i + h) * width + j + w) * 3 + 2];

							r += rr * matrix[(i + h_offset) * matrixlen + j + w_offset];
							g += gg * matrix[(i + h_offset) * matrixlen + j + w_offset];
							b += bb * matrix[(i + h_offset) * matrixlen + j + w_offset];
						}
						dst [(y * width + x) * 3 + 0] += (byte)r;
						dst [(y * width + x) * 3 + 1] += (byte)g;
						dst [(y * width + x) * 3 + 2] += (byte)b;
					}
				}
			}
		}
	}
}
