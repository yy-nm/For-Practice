

using System.Collections;
using System;

namespace Mard.Tools.ImageScale
{
	public class BilinearFilter
	{

		/// <summary>
		/// Scale32: scale image which format is RBGA4444 or ARBG4444 in time of pow2, 
		/// </summary>
		/// <param name="src">Source data from image.</param>
		/// <param name="width">Width of image.</param>
		/// <param name="height">Height of image.</param>
		/// <param name="pow2times">time of pow2.</param>
		/// <param name="dst">Destination data which need write to.</param>
		public static void Scale32(byte[] src, int width, int height, int pow2times, byte[] dst) 
		{
			Scale32(src, width, height, pow2times, pow2times, dst);
		}

		/// <summary>
		/// Scale32: scale image which format is RBGA4444 or ARBG4444 in time of pow2, 
		/// </summary>
		/// <param name="src">Source data from image.</param>
		/// <param name="width">Width of image.</param>
		/// <param name="height">Height of image.</param>
		/// <param name="pow2time2ofwidth">time of pow2 scale width.</param>
		/// <param name="pow2timesofheight">time of pow2 scale height.</param>
		/// <param name="dst">Destination data which need write to.</param>
		public static void Scale32(byte[] src, int width, int height, int pow2time2ofwidth, int pow2timesofheight, byte[] dst)
		{
			if (pow2time2ofwidth == 0 && pow2timesofheight == 0) {
				for (int i = 0; i < src.Length; i++) {
					dst [i] = src [i];
				}
				return;
			}

			int targetwidth = 0;
			int targetheight = 0;
			if (pow2time2ofwidth > 0) {
				targetwidth = width << pow2time2ofwidth;
			} else if (pow2time2ofwidth < 0) {
				targetwidth = width >> -pow2time2ofwidth;
			}
			if (pow2timesofheight > 0) {
				targetheight = height << pow2timesofheight;
			} else if (pow2timesofheight < 0) {
				targetheight = height >> -pow2timesofheight;
			}

			ScaleImage32 (src, width, height, targetwidth, targetheight, dst);
		}

		private static void ScaleImage32(byte[] s, int w, int h, int tw, int th, byte[] d) 
		{
			double u, v;
			int x, y;
			double x_coefficient, x_opposite_coefficient;
			double y_coefficient, y_opposite_coefficient;
			int pos1, pos2, pos3, pos4;
			int pos;

			for (int i = 0; i < th; i++) {
				for (int j = 0; j < tw; j++) {
					u = i * 1.0 / th * h - 0.5;
					v = j * 1.0 / tw * w - 0.5;
					y = (int)Math.Floor(u);
					x = (int)Math.Floor(v);
					y_coefficient = u - y;
					y_opposite_coefficient = 1 - y_coefficient;
					x_coefficient = v - x;
					x_opposite_coefficient = 1 - x_coefficient;

					pos = ((i * tw) + j) * 4;

					if (x < 0 && y < 0) {
						pos1 = (((h - 1) * w) + w - 1) * 4;
						pos2 = (((h - 1) * w) + x + 1) * 4;
						pos3 = (((y + 1) * w) + w - 1) * 4;
					} else if (x < 0) {
						pos1 = ((y * w) + w - 1) * 4;
						pos3 = (((y + 1) * w) + w - 1) * 4;

						pos2 = ((y * w) + x + 1) * 4;
					} else if (y < 0) {
						pos1 = (((h - 1) * w) + x) * 4;
						pos2 = (((h - 1) * w) + x + 1) * 4;

						pos3 = (((y + 1) * w) + x) * 4;
					} else {
						pos1 = ((y * w) + x) * 4;
						pos2 = ((y * w) + x + 1) * 4;
						pos3 = (((y + 1) * w) + x) * 4;
					}
					pos4 = (((y + 1) * w) + x + 1) * 4;

					d[pos + 0] = (byte)((s[pos1 + 0] * x_opposite_coefficient + s[pos2 + 0] * x_coefficient) * y_opposite_coefficient 
						+ (s[pos3 + 0] * x_opposite_coefficient + s[pos4 + 0] * x_coefficient) * y_coefficient);
					d[pos + 1] = (byte)((s[pos1 + 1] * x_opposite_coefficient + s[pos2 + 1] * x_coefficient) * y_opposite_coefficient 
						+ (s[pos3 + 1] * x_opposite_coefficient + s[pos4 + 1] * x_coefficient) * y_coefficient);
					d[pos + 2] = (byte)((s[pos1 + 2] * x_opposite_coefficient + s[pos2 + 2] * x_coefficient) * y_opposite_coefficient 
						+ (s[pos3 + 2] * x_opposite_coefficient + s[pos4 + 2] * x_coefficient) * y_coefficient);
					d[pos + 3] = (byte)((s[pos1 + 3] * x_opposite_coefficient + s[pos2 + 3] * x_coefficient) * y_opposite_coefficient 
						+ (s[pos3 + 3] * x_opposite_coefficient + s[pos4 + 3] * x_coefficient) * y_coefficient);
				}
			}
		}


		/// <summary>
		/// Scale24: scale image which format is RBG444 in time of pow2, 
		/// </summary>
		/// <param name="src">Source data from image.</param>
		/// <param name="width">Width of image.</param>
		/// <param name="height">Height of image.</param>
		/// <param name="pow2times">time of pow2.</param>
		/// <param name="dst">Destination data which need write to.</param>
		public static void Scale24(byte[] src, int width, int height, int pow2times, byte[] dst) 
		{
			Scale24(src, width, height, pow2times, pow2times, dst);
		}

		/// <summary>
		/// Scale24: scale image which format is RBG444 in time of pow2, 
		/// </summary>
		/// <param name="src">Source data from image.</param>
		/// <param name="width">Width of image.</param>
		/// <param name="height">Height of image.</param>
		/// <param name="pow2time2ofwidth">time of pow2 scale width.</param>
		/// <param name="pow2timesofheight">time of pow2 scale height.</param>
		/// <param name="dst">Destination data which need write to.</param>
		public static void Scale24(byte[] src, int width, int height, int pow2time2ofwidth, int pow2timesofheight, byte[] dst)
		{
			if (pow2time2ofwidth == 0 && pow2timesofheight == 0) {
				for (int i = 0; i < src.Length; i++) {
					dst [i] = src [i];
				}
				return;
			}

			int targetwidth = 0;
			int targetheight = 0;
			if (pow2time2ofwidth > 0) {
				targetwidth = width << pow2time2ofwidth;
			} else if (pow2time2ofwidth < 0) {
				targetwidth = width >> -pow2time2ofwidth;
			}
			if (pow2timesofheight > 0) {
				targetheight = height << pow2timesofheight;
			} else if (pow2timesofheight < 0) {
				targetheight = height >> -pow2timesofheight;
			}

			ScaleImage24 (src, width, height, targetwidth, targetheight, dst);
		}

		private static void ScaleImage24(byte[] s, int w, int h, int tw, int th, byte[] d) 
		{
			double u, v;
			int x, y;
			double x_coefficient, x_opposite_coefficient;
			double y_coefficient, y_opposite_coefficient;
			int pos1, pos2, pos3, pos4;
			int pos;

			for (int i = 0; i < th; i++) {
				for (int j = 0; j < tw; j++) {
					u = i * 1.0 / th * h - 0.5;
					v = j * 1.0 / tw * w - 0.5;
					y = (int)Math.Floor(u);
					x = (int)Math.Floor(v);
					y_coefficient = u - y;
					y_opposite_coefficient = 1 - y_coefficient;
					x_coefficient = v - x;
					x_opposite_coefficient = 1 - x_coefficient;

					pos = ((i * tw) + j) * 3;

					if (x < 0 && y < 0) {
						pos1 = (((h - 1) * w) + w - 1) * 3;
						pos2 = (((h - 1) * w) + x + 1) * 3;
						pos3 = (((y + 1) * w) + w - 1) * 3;
					} else if (x < 0) {
						pos1 = ((y * w) + w - 1) * 3;
						pos3 = (((y + 1) * w) + w - 1) * 3;

						pos2 = ((y * w) + x + 1) * 3;
					} else if (y < 0) {
						pos1 = (((h - 1) * w) + x) * 3;
						pos2 = (((h - 1) * w) + x + 1) * 3;

						pos3 = (((y + 1) * w) + x) * 3;
					} else {
						pos1 = ((y * w) + x) * 3;
						pos2 = ((y * w) + x + 1) * 3;
						pos3 = (((y + 1) * w) + x) * 3;
					}
					pos4 = (((y + 1) * w) + x + 1) * 3;

					d[pos + 0] = (byte)((s[pos1 + 0] * x_opposite_coefficient + s[pos2 + 0] * x_coefficient) * y_opposite_coefficient 
						+ (s[pos3 + 0] * x_opposite_coefficient + s[pos4 + 0] * x_coefficient) * y_coefficient);
					d[pos + 1] = (byte)((s[pos1 + 1] * x_opposite_coefficient + s[pos2 + 1] * x_coefficient) * y_opposite_coefficient 
						+ (s[pos3 + 1] * x_opposite_coefficient + s[pos4 + 1] * x_coefficient) * y_coefficient);
					d[pos + 2] = (byte)((s[pos1 + 2] * x_opposite_coefficient + s[pos2 + 2] * x_coefficient) * y_opposite_coefficient 
						+ (s[pos3 + 2] * x_opposite_coefficient + s[pos4 + 2] * x_coefficient) * y_coefficient);
				}
			}
		}
	}
}