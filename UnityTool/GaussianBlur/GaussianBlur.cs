
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

namespace Mard.Tools.Blur
{

	public struct RGBA
	{
		public byte r;
		public byte g;
		public byte b;
		public byte a;

		public void Reset()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 0;
		}

		public void Revert()
		{
			r = (byte)(byte.MaxValue - r);
			g = (byte)(byte.MaxValue - g);
			b = (byte)(byte.MaxValue - b);
			a = (byte)(byte.MaxValue - a);
		}

		public void Parse(int v)
		{
			r = (byte)(v >> 24);
			g = (byte)(v >> 16);
			b = (byte)(v >> 8);
			a = (byte)(v);
		}

		public int Convert2Int()
		{
			return r << 24 | g << 16 | b << 8 | a;
		}

		public static RGBA operator+(RGBA l, RGBA r)
		{
			l.r += r.r;
			l.g += r.g;
			l.b += r.b;
			l.a += r.a;
			return l;
		}

		public static RGBA operator*(RGBA l, float r)
		{
			l.r = (byte)(l.r * r);
			l.g = (byte)(l.g * r);
			l.b = (byte)(l.b * r);
			l.a = (byte)(l.a * r);

			return l;
		}

		public static explicit operator int(RGBA l)
		{
			return l.Convert2Int ();
		}
	}
	public class GaussianBlur
	{

		/// <summary>
		/// default Gaussian matrix which standard deviation=2, radius=3
		/// </summary>
		private static readonly float[] rDefaultMatrix = new float[]{
			0.004193702f,	0.007834867f,	0.01139966f,	0.01291751f,	0.01139966f,	0.007834867f,	0.004193702f,
			0.007834867f,	0.01463746f,	0.02129737f,	0.02413309f,	0.02129737f,	0.01463746f,	0.007834867f,
			0.01139966f,	0.02129737f,	0.0309875f,		0.03511344f,	0.0309875f,		0.02129737f,	0.01139966f,
			0.01291751f,	0.02413309f,	0.03511344f,	0.03978873f,	0.03511344f,	0.02413309f,	0.01291751f,
			0.01139966f,	0.02129737f,	0.0309875f,		0.03511344f,	0.0309875f,		0.02129737f,	0.01139966f,
			0.007834867f,	0.01463746f,	0.02129737f,	0.02413309f,	0.02129737f,	0.01463746f,	0.007834867f,
			0.004193702f,	0.007834867f,	0.01139966f,	0.01291751f,	0.01139966f,	0.007834867f,	0.004193702f,
		};
		private const float cDefaultSd = 2f;
		private const int cDefaultRadius = 3;

		public static void Blur(int[] src, int width, int height, int[] dst)
		{
			Blur (src, width, height, rDefaultMatrix, cDefaultSd, cDefaultRadius, dst);
		}

		public static void Blur(int[] src, int width, int height, float[] matrix, float sd, int radius, int[] dst)
		{
			RGBA tmp = new RGBA();
			RGBA tmp1 = new RGBA();
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

					tmp.Reset ();
					for (int i = 0; i + h_offset < matrixlen && i + h < height; i++) {
						for (int j = 0; j + w_offset < matrixlen && j + w < width; j++) {
							tmp1.Parse (src [(i + h) * width + j + w]);
							//tmp1.Revert ();
							tmp += tmp1 * matrix[(i + h_offset) * matrixlen + j + w_offset];
						}
					}
					//tmp.Revert ();
					dst [y * width + x] = (int)tmp;
				}
			}
		}
	}
}
