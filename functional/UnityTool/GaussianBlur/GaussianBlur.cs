
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
			float mvalue;
			int pos;

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
							mvalue = matrix[(i + h_offset) * matrixlen + j + w_offset];
							pos = ((i + h) * width + j + w) * 4;
							rr = src [pos + 0];
							gg = src [pos + 1];
							bb = src [pos + 2];
							aa = src [pos + 3];

							r += rr * mvalue;
							g += gg * mvalue;
							b += bb * mvalue;
							a += aa * mvalue;
						}
					}

					if (h_offset > 0 || w_offset > 0) {
						// handle with matrix which x < 0 and y < 0 part
						for (int i = h_offset - 1; i >= 0; i--) {
							for (int j = w_offset - 1; j >= 0; j--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((height + i - h_offset - 1) * width + width + j -  w_offset - 1) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
						// handle with matrix which x < 0 and y >= 0 part
						for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
							for (int j = w_offset - 1; j >= 0; j--) {
								mvalue = matrix[(i + h_offset) * matrixlen + j];
								pos = ((i + h) * width + width + j -  w_offset - 1) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x >= 0 and y < 0 part
						for (int i = h_offset - 1; i >= 0; i--) {
							for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
								mvalue = matrix[i * matrixlen + j + w_offset];
								pos = ((height + i - h_offset - 1) * width + j + w) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
					}

					if (h + matrixlen > height || w + matrixlen > width) {
						// handle with matrix which x >= width and y >= height part
						for (int i = 0; h + matrixlen - i > height; i++) {
							for (int j = 0; w + matrixlen - j > width; j++) {
								mvalue = matrix[(matrixlen - i - 1) * matrixlen + matrixlen - j - 1];
								pos = ((h + matrixlen - i - height - 1) * width + w + matrixlen - j - 1) * 4;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x >= width and y < height part
						for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
							for (int j = 0; w + matrixlen - j > width; j++) {
								mvalue = matrix[(i + h_offset) * matrixlen + matrixlen - j - 1];
								pos = ((i + h - 1) * width + w + matrixlen - j - 1) * 4;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x < width and y >= height part
						for (int i = 0; h + matrixlen - i > height; i++) {
							for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
								mvalue = matrix[(matrixlen - i - 1) * matrixlen + j + w_offset];
								pos = ((h + matrixlen - i - height - 1) * width + j + w) * 4;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
					}

					pos = (y * width + x) * 4;
					dst [pos + 0] = (byte)r;
					dst [pos + 1] = (byte)g;
					dst [pos + 2] = (byte)b;
					dst [pos + 3] = (byte)a;
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
			float mvalue;
			int pos;

			int matrixlen = radius + radius + 1;
			int i = row;

			if (row >= matrixlen || row < 0)
				return;

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
							mvalue = matrix[(i + h_offset) * matrixlen + j + w_offset];
							pos = ((i + h) * width + j + w) * 4;
							rr = src [pos + 0];
							gg = src [pos + 1];
							bb = src [pos + 2];
							aa = src [pos + 3];

							r += rr * mvalue;
							g += gg * mvalue;
							b += bb * mvalue;
							a += aa * mvalue;
						}
					}

					if (h_offset >= i || w_offset > 0) {
						// handle with matrix which x < 0 and y < 0 part
						if (h_offset > i) {
							for (int j = w_offset - 1; j >= 0; j--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((height + i - h_offset) * width + width + j -  w_offset - 1) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
						// handle with matrix which x < 0 and y >= 0 part
						if (h_offset <= i && i + h < height && w_offset > 0) {
							for (int j = w_offset - 1; j >= 0; j--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((i + h - h_offset) * width + width + j -  w_offset - 1) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x >= 0 and y < 0 part
						if  (h_offset > i) {
							for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
								mvalue = matrix[i * matrixlen + j + w_offset];
								pos = ((height + i - h_offset) * width + j + w) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
					}

					if (h + i + 1 > height || w + matrixlen > width) {
						// handle with matrix which x >= width and y >= height part
						if (h + i + 1 > height) {
							for (int j = 0; w + matrixlen - j > width; j++) {
								mvalue = matrix[i * matrixlen + matrixlen - j - 1];
								pos = ((h + i - height) * width + w + matrixlen - j - 1) * 4;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x >= width and y < height part
						if (h + i + 1 <= height && w + matrixlen > width) {
							for (int j = 0; w + matrixlen - j > width; j++) {
								mvalue = matrix[i * matrixlen + matrixlen - j - 1];
								pos = ((h + i - 1) * width + w + matrixlen - j - 1) * 4;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x < width and y >= height part
						if (h + i + 1 > height) {
							for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
								mvalue = matrix[i * matrixlen + j + w_offset];
								pos = ((h + i - height) * width + j + w) * 4;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
					}

					pos = (y * width + x) * 4;
					dst [pos + 0] += (byte)r;
					dst [pos + 1] += (byte)g;
					dst [pos + 2] += (byte)b;
					dst [pos + 3] += (byte)a;
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
			float mvalue;
			int pos;

			int matrixlen = radius + radius + 1;
			int j = column;

			if (column >= matrixlen || column < 0)
				return;

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
							mvalue = matrix[(i + h_offset) * matrixlen + j + w_offset];
							pos = ((i + h) * width + j + w) * 4;
							rr = src [pos + 0];
							gg = src [pos + 1];
							bb = src [pos + 2];
							aa = src [pos + 3];

							r += rr * mvalue;
							g += gg * mvalue;
							b += bb * mvalue;
							a += aa * mvalue;
						}

					}

					if (h_offset > 0 || w_offset >= j) {
						// handle with matrix which x < 0 and y < 0 part
						if (w_offset > j) {
							for (int i = h_offset - 1; i >= 0; i--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((height + i - h_offset - 1) * width + width + j - w_offset) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
						// handle with matrix which x < 0 and y >= 0 part
						if (w_offset > j) {
							for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
								mvalue = matrix[(i + h_offset) * matrixlen + j];
								pos = ((i + h) * width + width + j - w_offset) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x >= 0 and y < 0 part
						if (w_offset <= j && h_offset > 0) {
							for (int i = h_offset - 1; i >= 0; i--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((height + i - h_offset - 1) * width + j + w - w_offset) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
					}
						
					if (h + matrixlen > height || w + j + 1 > width) {
						// handle with matrix which x >= width and y >= height part
						if (w + j + 1 > width) {
							for (int i = 0; h + matrixlen - i > height; i++) {
								mvalue = matrix[(matrixlen - i - 1) * matrixlen + j];
								pos = ((h + matrixlen - i - height - 1) * width + w + j - width) * 4;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x >= width and y < height part
						if (w + j + 1 > width) {
							for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
								mvalue = matrix[(i + h_offset) * matrixlen + j];
								pos = ((i + h) * width + w + j - width) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}

						// handle with matrix which x < width and y >= height part
						if (w + j + 1 <= width && h + matrixlen > height) {
							for (int i = 0; h + matrixlen - i > height; i++) {
								mvalue = matrix[(matrixlen - i - 1) * matrixlen + j];
								pos = ((h + matrixlen - i - height - 1) * width + j + w) * 4;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];
								aa = src [pos + 3];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
								a += aa * mvalue;
							}
						}
					}


					pos = (y * width + x) * 4;
					dst [pos + 0] += (byte)r;
					dst [pos + 1] += (byte)g;
					dst [pos + 2] += (byte)b;
					dst [pos + 3] += (byte)a;
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
			float mvalue;
			int pos;

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
							mvalue = matrix[(i + h_offset) * matrixlen + j + w_offset];
							pos = ((i + h) * width + j + w) * 3;
							rr = src [pos + 0];
							gg = src [pos + 1];
							bb = src [pos + 2];

							r += rr * mvalue;
							g += gg * mvalue;
							b += bb * mvalue;
						}
					}

					if (h_offset > 0 || w_offset > 0) {
						// handle with matrix which x < 0 and y < 0 part
						for (int i = h_offset - 1; i >= 0; i--) {
							for (int j = w_offset - 1; j >= 0; j--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((height + i - h_offset - 1) * width + width + j -  w_offset - 1) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
						// handle with matrix which x < 0 and y >= 0 part
						for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
							for (int j = w_offset - 1; j >= 0; j--) {
								mvalue = matrix[(i + h_offset) * matrixlen + j];
								pos = ((i + h) * width + width + j -  w_offset - 1) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x >= 0 and y < 0 part
						for (int i = h_offset - 1; i >= 0; i--) {
							for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
								mvalue = matrix[i * matrixlen + j + w_offset];
								pos = ((height + i - h_offset - 1) * width + j + w) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
					}

					if (h + matrixlen > height || w + matrixlen > width) {
						// handle with matrix which x >= width and y >= height part
						for (int i = 0; h + matrixlen - i > height; i++) {
							for (int j = 0; w + matrixlen - j > width; j++) {
								mvalue = matrix[(matrixlen - i - 1) * matrixlen + matrixlen - j - 1];
								pos = ((h + matrixlen - i - height - 1) * width + w + matrixlen - j - 1) * 3;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x >= width and y < height part
						for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
							for (int j = 0; w + matrixlen - j > width; j++) {
								mvalue = matrix[(i + h_offset) * matrixlen + matrixlen - j - 1];
								pos = ((i + h - 1) * width + w + matrixlen - j - 1) * 3;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x < width and y >= height part
						for (int i = 0; h + matrixlen - i > height; i++) {
							for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
								mvalue = matrix[(matrixlen - i - 1) * matrixlen + j + w_offset];
								pos = ((h + matrixlen - i - height - 1) * width + j + w) * 3;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
					}

					pos = (y * width + x) * 3;
					dst [pos + 0] = (byte)r;
					dst [pos + 1] = (byte)g;
					dst [pos + 2] = (byte)b;
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
			float mvalue;
			int pos;

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
							mvalue = matrix[(i + h_offset) * matrixlen + j + w_offset];
							pos = ((i + h) * width + j + w) * 3;
							rr = src [pos + 0];
							gg = src [pos + 1];
							bb = src [pos + 2];

							r += rr * mvalue;
							g += gg * mvalue;
							b += bb * mvalue;
						}
					}
						
					if (h_offset >= i || w_offset > 0) {
						// handle with matrix which x < 0 and y < 0 part
						if (h_offset > i) {
							for (int j = w_offset - 1; j >= 0; j--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((height + i - h_offset) * width + width + j -  w_offset - 1) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
						// handle with matrix which x < 0 and y >= 0 part
						if (h_offset <= i && i + h < height && w_offset > 0) {
							for (int j = w_offset - 1; j >= 0; j--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((i + h - h_offset) * width + width + j -  w_offset - 1) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x >= 0 and y < 0 part
						if  (h_offset > i) {
							for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
								mvalue = matrix[i * matrixlen + j + w_offset];
								pos = ((height + i - h_offset) * width + j + w) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
					}

					if (h + i + 1 > height || w + matrixlen > width) {
						// handle with matrix which x >= width and y >= height part
						if (h + i + 1 > height) {
							for (int j = 0; w + matrixlen - j > width; j++) {
								mvalue = matrix[i * matrixlen + matrixlen - j - 1];
								pos = ((h + i - height) * width + w + matrixlen - j - 1) * 3;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x >= width and y < height part
						if (h + i + 1 <= height && w + matrixlen > width) {
							for (int j = 0; w + matrixlen - j > width; j++) {
								mvalue = matrix[i * matrixlen + matrixlen - j - 1];
								pos = ((h + i - 1) * width + w + matrixlen - j - 1) * 3;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x < width and y >= height part
						if (h + i + 1 > height) {
							for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
								mvalue = matrix[i * matrixlen + j + w_offset];
								pos = ((h + i - height) * width + j + w) * 3;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
					}

					pos = (y * width + x) * 3;
					dst [pos + 0] += (byte)r;
					dst [pos + 1] += (byte)g;
					dst [pos + 2] += (byte)b;
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
			float mvalue;
			int pos;

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
							mvalue = matrix[(i + h_offset) * matrixlen + j + w_offset];
							pos = ((i + h) * width + j + w) * 3;
							rr = src [pos + 0];
							gg = src [pos + 1];
							bb = src [pos + 2];

							r += rr * mvalue;
							g += gg * mvalue;
							b += bb * mvalue;
						}
					}

					if (h_offset > 0 || w_offset >= j) {
						// handle with matrix which x < 0 and y < 0 part
						if (w_offset > j) {
							for (int i = h_offset - 1; i >= 0; i--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((height + i - h_offset - 1) * width + width + j - w_offset) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
						// handle with matrix which x < 0 and y >= 0 part
						if (w_offset > j) {
							for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
								mvalue = matrix[(i + h_offset) * matrixlen + j];
								pos = ((i + h) * width + width + j - w_offset) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x >= 0 and y < 0 part
						if (w_offset <= j && h_offset > 0) {
							for (int i = h_offset - 1; i >= 0; i--) {
								mvalue = matrix[i * matrixlen + j];
								pos = ((height + i - h_offset - 1) * width + j + w - w_offset) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
					}

					if (h + matrixlen > height || w + j + 1 > width) {
						// handle with matrix which x >= width and y >= height part
						if (w + j + 1 > width) {
							for (int i = 0; h + matrixlen - i > height; i++) {
								mvalue = matrix[(matrixlen - i - 1) * matrixlen + j];
								pos = ((h + matrixlen - i - height - 1) * width + w + j - width) * 3;
								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x >= width and y < height part
						if (w + j + 1 > width) {
							for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
								mvalue = matrix[(i + h_offset) * matrixlen + j];
								pos = ((i + h) * width + w + j - width) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}

						// handle with matrix which x < width and y >= height part
						if (w + j + 1 <= width && h + matrixlen > height) {
							for (int i = 0; h + matrixlen - i > height; i++) {
								mvalue = matrix[(matrixlen - i - 1) * matrixlen + j];
								pos = ((h + matrixlen - i - height - 1) * width + j + w) * 3;

								rr = src [pos + 0];
								gg = src [pos + 1];
								bb = src [pos + 2];

								r += rr * mvalue;
								g += gg * mvalue;
								b += bb * mvalue;
							}
						}
					}

					pos = (y * width + x) * 3;
					dst [pos + 0] += (byte)r;
					dst [pos + 1] += (byte)g;
					dst [pos + 2] += (byte)b;
				}
			}
		}

	}
}
