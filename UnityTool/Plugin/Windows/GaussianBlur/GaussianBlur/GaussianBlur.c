
#include "PreDefine.h"


static inline int GetRightPos(int width, int height, int x, int y)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
		return y * width + x;
	else if (x >= 0 && x < width && y < 0)
		return (height + y) * width + x;
	else if (x >= 0 && x < width && y >= height)
		return (y - height) * width + x;
	else if (x < 0 && y < 0)
		return (height + y) * width + x + width;
	else if (x < 0 && y >= 0 && y < height)
		return y * width + x + width;
	else if (x < 0 && y >= height)
		return (y - height) * width + x + width;
	else if (x >= width && y < 0)
		return (height + y) * width + x - width;
	else if (x >= width && y >= 0 && y < height)
		return y * width + x - width;
	else //if (x >= width && y >= height)
		return (y - height) * width + x - width;
}

EXPORT int GaussianBlur32(byte src[], int width, int height, float matrix[], float sd, int radius, byte dst[])
{

	float r, g, b, a;
	float rr, gg, bb, aa;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 4;

	int matrixlen = radius + radius + 1;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			r = g = b = a = 0;
			for (int i = 0; i < matrixlen; i++) {
				posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
				posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
				for (int j = 0; j < matrixlen; j++) {
					mvalue = matrix[i * matrixlen + j];
					pos = posInSrc * channel_count;
					rr = src[pos + 0];
					gg = src[pos + 1];
					bb = src[pos + 2];
					aa = src[pos + 3];

					r += mvalue * rr;
					g += mvalue * gg;
					b += mvalue * bb;
					a += mvalue * aa;

					if (++posInSrc >= posInEdge)
						posInSrc -= width;
				}
			}

			pos = (y * width + x) * channel_count;
			dst[pos + 0] = (byte)r;
			dst[pos + 1] = (byte)g;
			dst[pos + 2] = (byte)b;
			dst[pos + 3] = (byte)a;
		}
	}

	return 0;
}

EXPORT int GaussianBlur32Horizontal(byte src[], int width, int height, float matrix[], float sd, int radius, int row, byte dst[])
{

	if (row < 0 || row >= height)
		return 1;

	float r, g, b, a;
	float rr, gg, bb, aa;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 4;

	int matrixlen = radius + radius + 1;

	int y = row;
	for (int x = 0; x < width; x++) {

		r = g = b = a = 0;
		for (int i = 0; i < matrixlen; i++) {
			posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
			posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
			for (int j = 0; j < matrixlen; j++) {
				mvalue = matrix[i * matrixlen + j];
				pos = posInSrc * channel_count;
				rr = src[pos + 0];
				gg = src[pos + 1];
				bb = src[pos + 2];
				aa = src[pos + 3];

				r += mvalue * rr;
				g += mvalue * gg;
				b += mvalue * bb;
				a += mvalue * aa;

				if (++posInSrc >= posInEdge)
					posInSrc -= width;
			}
		}

		pos = (y * width + x) * channel_count;
		dst[pos + 0] = (byte)r;
		dst[pos + 1] = (byte)g;
		dst[pos + 2] = (byte)b;
		dst[pos + 3] = (byte)a;
	}

	return 0;
}

EXPORT int GaussianBlur32HorizontalRange(byte src[], int width, int height, float matrix[], float sd, int radius, int row, int rowcount, byte dst[])
{
	if (row < 0 || rowcount < 0 || row + rowcount > height)
		return 1;

	float r, g, b, a;
	float rr, gg, bb, aa;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 4;

	int matrixlen = radius + radius + 1;

	for (int y = row; y < row + rowcount; y++) {
		for (int x = 0; x < width; x++) {

			r = g = b = a = 0;
			for (int i = 0; i < matrixlen; i++) {
				posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
				posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
				for (int j = 0; j < matrixlen; j++) {
					mvalue = matrix[i * matrixlen + j];
					pos = posInSrc * channel_count;
					rr = src[pos + 0];
					gg = src[pos + 1];
					bb = src[pos + 2];
					aa = src[pos + 3];

					r += mvalue * rr;
					g += mvalue * gg;
					b += mvalue * bb;
					a += mvalue * aa;

					if (++posInSrc >= posInEdge)
						posInSrc -= width;
				}
			}

			pos = (y * width + x) * channel_count;
			dst[pos + 0] = (byte)r;
			dst[pos + 1] = (byte)g;
			dst[pos + 2] = (byte)b;
			dst[pos + 3] = (byte)a;
		}
	}

	return 0;
}

EXPORT int GaussianBlur32Vertical(byte src[], int width, int height, float matrix[], float sd, int radius, int column, byte dst[])
{
	if (column < 0 || column >= width)
		return 1;

	float r, g, b, a;
	float rr, gg, bb, aa;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 4;

	int matrixlen = radius + radius + 1;

	for (int y = 0; y < height; y++) {
		int x = column;

		r = g = b = a = 0;
		for (int i = 0; i < matrixlen; i++) {
			posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
			posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
			for (int j = 0; j < matrixlen; j++) {
				mvalue = matrix[i * matrixlen + j];
				pos = posInSrc * channel_count;
				rr = src[pos + 0];
				gg = src[pos + 1];
				bb = src[pos + 2];
				aa = src[pos + 3];

				r += mvalue * rr;
				g += mvalue * gg;
				b += mvalue * bb;
				a += mvalue * aa;

				if (++posInSrc >= posInEdge)
					posInSrc -= width;
			}
		}

		pos = (y * width + x) * channel_count;
		dst[pos + 0] = (byte)r;
		dst[pos + 1] = (byte)g;
		dst[pos + 2] = (byte)b;
		dst[pos + 3] = (byte)a;
	}

	return 0;
}

EXPORT int GaussianBlur32VerticalRange(byte src[], int width, int height, float matrix[], float sd, int radius, int column, int columncount, byte dst[])
{
	if (column < 0 || columncount < 0 || column + columncount > width)
		return 1;

	float r, g, b, a;
	float rr, gg, bb, aa;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 4;

	int matrixlen = radius + radius + 1;

	for (int y = 0; y < height; y++) {
		for (int x = column; x < column + columncount; x++) {

			r = g = b = a = 0;
			for (int i = 0; i < matrixlen; i++) {
				posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
				posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
				for (int j = 0; j < matrixlen; j++) {
					mvalue = matrix[i * matrixlen + j];
					pos = posInSrc * channel_count;
					rr = src[pos + 0];
					gg = src[pos + 1];
					bb = src[pos + 2];
					aa = src[pos + 3];

					r += mvalue * rr;
					g += mvalue * gg;
					b += mvalue * bb;
					a += mvalue * aa;

					if (++posInSrc >= posInEdge)
						posInSrc -= width;
				}
			}

			pos = (y * width + x) * channel_count;
			dst[pos + 0] = (byte)r;
			dst[pos + 1] = (byte)g;
			dst[pos + 2] = (byte)b;
			dst[pos + 3] = (byte)a;
		}
	}

	return 0;
}

EXPORT int GaussianBlur24(byte src[], int width, int height, float matrix[], float sd, int radius, byte dst[])
{

	float r, g, b;
	float rr, gg, bb;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 3;

	int matrixlen = radius + radius + 1;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			r = g = b = 0;
			for (int i = 0; i < matrixlen; i++) {
				posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
				posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
				for (int j = 0; j < matrixlen; j++) {
					mvalue = matrix[i * matrixlen + j];
					pos = posInSrc * channel_count;
					rr = src[pos + 0];
					gg = src[pos + 1];
					bb = src[pos + 2];

					r += mvalue * rr;
					g += mvalue * gg;
					b += mvalue * bb;

					if (++posInSrc >= posInEdge)
						posInSrc -= width;
				}
			}

			pos = (y * width + x) * channel_count;
			dst[pos + 0] = (byte)r;
			dst[pos + 1] = (byte)g;
			dst[pos + 2] = (byte)b;
		}
	}

	return 0;
}

EXPORT int GaussianBlur24Horizontal(byte src[], int width, int height, float matrix[], float sd, int radius, int row, byte dst[])
{
	if (row < 0 || row >= height)
		return 1;

	float r, g, b;
	float rr, gg, bb;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 3;

	int matrixlen = radius + radius + 1;

	int y = row;
	for (int x = 0; x < width; x++) {

		r = g = b = 0;
		for (int i = 0; i < matrixlen; i++) {
			posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
			posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
			for (int j = 0; j < matrixlen; j++) {
				mvalue = matrix[i * matrixlen + j];
				pos = posInSrc * channel_count;
				rr = src[pos + 0];
				gg = src[pos + 1];
				bb = src[pos + 2];

				r += mvalue * rr;
				g += mvalue * gg;
				b += mvalue * bb;

				if (++posInSrc >= posInEdge)
					posInSrc -= width;
			}
		}

		pos = (y * width + x) * channel_count;
		dst[pos + 0] = (byte)r;
		dst[pos + 1] = (byte)g;
		dst[pos + 2] = (byte)b;
	}

	return 0;
}

EXPORT int GaussianBlur24HorizontalRange(byte src[], int width, int height, float matrix[], float sd, int radius, int row, int rowcount, byte dst[])
{
	if (row < 0 || rowcount < 0 || row + rowcount > height)
		return 1;

	float r, g, b;
	float rr, gg, bb;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 3;

	int matrixlen = radius + radius + 1;

	for (int y = row; y < row + rowcount; y++) {
		for (int x = 0; x < width; x++) {

			r = g = b = 0;
			for (int i = 0; i < matrixlen; i++) {
				posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
				posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
				for (int j = 0; j < matrixlen; j++) {
					mvalue = matrix[i * matrixlen + j];
					pos = posInSrc * channel_count;
					rr = src[pos + 0];
					gg = src[pos + 1];
					bb = src[pos + 2];

					r += mvalue * rr;
					g += mvalue * gg;
					b += mvalue * bb;

					if (++posInSrc >= posInEdge)
						posInSrc -= width;
				}
			}

			pos = (y * width + x) * channel_count;
			dst[pos + 0] = (byte)r;
			dst[pos + 1] = (byte)g;
			dst[pos + 2] = (byte)b;
		}
	}

	return 0;
}

EXPORT int GaussianBlur24Vertical(byte src[], int width, int height, float matrix[], float sd, int radius, int column, byte dst[])
{
	if (column < 0 || column >= width)
		return 1;

	float r, g, b;
	float rr, gg, bb;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 3;

	int matrixlen = radius + radius + 1;

	for (int y = 0; y < height; y++) {
		int x = column;

		r = g = b = 0;
		for (int i = 0; i < matrixlen; i++) {
			posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
			posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
			for (int j = 0; j < matrixlen; j++) {
				mvalue = matrix[i * matrixlen + j];
				pos = posInSrc * channel_count;
				rr = src[pos + 0];
				gg = src[pos + 1];
				bb = src[pos + 2];

				r += mvalue * rr;
				g += mvalue * gg;
				b += mvalue * bb;

				if (++posInSrc >= posInEdge)
					posInSrc -= width;
			}
		}

		pos = (y * width + x) * channel_count;
		dst[pos + 0] = (byte)r;
		dst[pos + 1] = (byte)g;
		dst[pos + 2] = (byte)b;
	}

	return 0;
}

EXPORT int GaussianBlur24VerticalRange(byte src[], int width, int height, float matrix[], float sd, int radius, int column, int columncount, byte dst[])
{
	if (column < 0 || columncount < 0 || column + columncount > width)
		return 1;

	float r, g, b;
	float rr, gg, bb;

	float mvalue;
	int posInSrc;
	int posInEdge;
	int pos;

	int channel_count = 3;

	int matrixlen = radius + radius + 1;

	for (int y = 0; y < height; y++) {
		for (int x = column; x < column + columncount; x++) {

			r = g = b = 0;
			for (int i = 0; i < matrixlen; i++) {
				posInSrc = GetRightPos(width, height, x + 0 - radius, y + i - radius);
				posInEdge = GetRightPos(width, height, 0, y + i - radius) + width;
				for (int j = 0; j < matrixlen; j++) {
					mvalue = matrix[i * matrixlen + j];
					pos = posInSrc * channel_count;
					rr = src[pos + 0];
					gg = src[pos + 1];
					bb = src[pos + 2];

					r += mvalue * rr;
					g += mvalue * gg;
					b += mvalue * bb;

					if (++posInSrc >= posInEdge)
						posInSrc -= width;
				}
			}

			pos = (y * width + x) * channel_count;
			dst[pos + 0] = (byte)r;
			dst[pos + 1] = (byte)g;
			dst[pos + 2] = (byte)b;
		}
	}

	return 0;
}