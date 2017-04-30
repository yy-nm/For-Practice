#pragma once


#define EXPORT __declspec(dllexport)

#define ATOMIC_BYTE_FETCH_AND_ADD(x, y) (x) = (x) + (y)

typedef unsigned char byte;