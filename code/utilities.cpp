#include "utilities.h"


void CatChars(char * sourceA, ui32 sizeA,
              char * sourceB, ui32 sizeB,
              char * dest)
{
    for (ui32 i = 0; i < sizeA; i++)
    {
        *dest++ = *sourceA++;
    }
    for (ui32 i = 0; i < sizeB; i++)
    {
        *dest++ = *sourceB++;
    }
    *dest++ = 0;
}

ui32 CharStarSize(char * pointer)
{
    ui32 size = 0;
    while (*pointer++)
    {
        size++;
    }
    return size;
}

