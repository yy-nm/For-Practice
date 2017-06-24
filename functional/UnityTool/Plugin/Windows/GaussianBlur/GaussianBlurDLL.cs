

using UnityEngine;


using System.Collections;
using System.Runtime.InteropServices;

public class GaussianBlurDLL
{
	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GetGaussianMatrixIn2d(float[] matrix, int matrixlen, float sd, int r);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GetGaussianMatrixIn1d(float[] matrix, int matrixlen, float sd, int r);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur32(byte[] src, int width, int height, float[] matrix, float sd, int radius, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur32Horizontal(byte[] src, int width, int height, float[] matrix, float sd, int radius, int row, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur32HorizontalRange(byte[] src, int width, int height, float[] matrix, float sd, int radius, int row, int rowcount, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur32Vertical(byte[] src, int width, int height, float[] matrix, float sd, int radius, int column, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur32VerticalRange(byte[] src, int width, int height, float[] matrix, float sd, int radius, int column, int columncount, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur24(byte[] src, int width, int height, float[] matrix, float sd, int radius, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur24Horizontal(byte[] src, int width, int height, float[] matrix, float sd, int radius, int row, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur24HorizontalRange(byte[] src, int width, int height, float[] matrix, float sd, int radius, int row, int rowcount, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur24Vertical(byte[] src, int width, int height, float[] matrix, float sd, int radius, int column, byte[] dst);

	[DllImport("GaussianBlur", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
	public static extern int GaussianBlur24VerticalRange(byte[] src, int width, int height, float[] matrix, float sd, int radius, int column, int columncount, byte[] dst);
}
