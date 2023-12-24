#pragma once
/*
 version 1.5
 edited by Forever
 last edit time -> Sep. 24th, 2023
 for C++
*/

#ifndef _COLORFULSTR_H
#define _COLORFULSTR_H

extern "C"
{
#include <string.h>
}

#define STRCMP(x, y) (EasyStrcmp::strcmp_exact(x, y))
#define MEMCMP(x, y, n) (EasyStrcmp::memcmp_exact(x, y ,n))
#define STRCAT(x, y) (strcat_s(x, y))
#define STRCPY(x, y) (strcpy_s(x, y))

namespace EasyStrcmp {
    /*
    ��ʹ��STRCMP��ȫ�Ƚ������ַ���
    */
    inline bool strcmp_exact(const char* x, const char* y)
    {
        return strcmp(x, y) ? false : true;
    }

    /*
    ��ʹ��MEMCMP��ȫ�Ƚ������ַ������Ӵ�
    */
    inline bool memcmp_exact(const char* x, const char* y, int n)
    {
        return strcmp(x, y) ? false : true;
    }
}
#endif