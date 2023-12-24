#pragma once
/*
 version 1.5
 edited by Forever
 last edit time -> Sep. 24th, 2023
 for C++
*/

#ifndef _COLORFULSTR_H
#define _COLORFULSTR_H

#define sINTENSE FOREGROUND_INTENSITY

enum str_color
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    LAKE_BLUE = 3,
    RED = 4,
    VIOLET = 5,
    YELLOW = 6,
    RESET = 7,
    GREY = 8,
    CYAN = 9,
    LIGHT_GREEN = 10,
    LIGHTER_GREEN = 11,
    LIGHT_RED = 12,
    LIGHT_VIOLET = 13,
    LIGHT_YELLOW = 14,
    LIGHT_WHITE = 15,
    WHITE = 7
};

#include <iostream>
#include <sstream>
#include <string>
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <windows.h>
}

#define STRCMP(x, y) (ColorfulStr::strcmp_exact(x, y))
#define MEMCMP(x, y, n) (ColorfulStr::memcmp_exact(x, y ,n))
#define STRCAT(x, y) (strcat_s(x, y))
#define STRCPY(x, y) (strcpy_s(x, y))

namespace ColorfulStr {
    /*
    SetPrintColor  ���ÿ���ָ̨�������ɫ

    [in] (int) color  ָ����ɫ

    ����
      ��
    */
    void SetPrintColor(int color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    /*
    prints  ����ɫ���ָ���ַ���

    [in] (const LPCSTR) str_in    ָ���ַ���
    [in] (int) str_color         ָ����ɫ

    ���� ��ɫenumֵ
    */
    int prints(const LPCSTR str_in, int str_color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), str_color);
        printf("%s", str_in);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        return str_color;
    }

    int prints(const size_t int_in, int str_color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), str_color);
        printf("%lld", int_in);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        return str_color;
    }

    int prints(const long double float_in, int str_color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), str_color);
        printf("%lf", float_in);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        return str_color;
    }

    /*
    ��ʹ��STRCMP��ȫ�Ƚ������ַ���
    */
    bool strcmp_exact(const char* x, const char* y)
    {
        int ret;
        ret = strcmp(x, y);
        if (ret == 0)
            return true;
        else
            return false;
    }

    /*
    ��ʹ��MEMCMP��ȫ�Ƚ������ַ������Ӵ�
    */
    bool memcmp_exact(const char* x, const char* y, int n)
    {
        int ret;
        ret = memcmp(x, y, n);
        if (ret == 0)
            return true;
        else
            return false;
    }


    /*
    �ָ��ַ���
    */
    std::vector<std::string> StringSplit(std::string input, const std::string delimiter) {
        std::vector<std::string> tokens;

        size_t pos = 0;
        std::string token;
        while ((pos = input.find(delimiter)) != std::string::npos) {
            token = input.substr(0, pos);
            tokens.push_back(token);
            input.erase(0, pos + delimiter.length());
        }
        tokens.push_back(input);

        return tokens;
    }

    void printEnter(void)
    {
        putchar('\r');
        putchar('\n');
    }

}

    

/*
wprints  ����ɫ���ָ���ַ���

[in] (const LPWSTR) str_in    ָ���ַ���
[in] (int) str_color         ָ����ɫ

���� ��ɫenumֵ
*/
//int wprints(const LPWSTR str_in, int str_color)
//{
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), str_color);
//    wprintf(L"%s", str_in);
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
//    return str_color;
//}
//
/*
printa  ������ı�

[in] (const LPCSTR*)str_lines_in   �����ά�����ı��ַ���
[in] (int)max_lines               ����������

���� ��
*/
//void printa(const LPCSTR* str_lines_in, int max_lines)
//{
//    if (max_lines == NULL)
//    {
//        int lines = 0;
//        while (str_lines_in[lines] != NULL)
//        {
//            printf("%s\n", str_lines_in[lines]);
//            lines++;
//        }
//    }
//    for (int i = 0; i < max_lines; i++)
//        printf("%s\n", str_lines_in[i]);
//    return;
//}

/*
printLongInt �ֶ����������

(long long)num   ����
(int)alignWidth  ������ ��0 ���������, <������> �Ҷ���ָ�����, <������> �����ָ����ȣ�
(int)color       ��ɫ ����ȱʡ��

���� colorֵ
*/
//int printLongInt(long long num, int alignWidth, int color = RESET)
//{
//    int countLength = 0;
//    int index = 0;
//    long double temp = num;
//    char outputFormat[18] = "%";
//    char alignWidthStr[16] = { 0 };
//
//    while (1)
//    {
//        temp /= 10.0;
//        countLength++;
//        if (temp <= 1.0)
//            break;
//    }
//
//    char* numStr = (char*)malloc(countLength * sizeof(char));
//    char strBuf[1024] = { 0 };
//    char* p = strBuf;
//    sprintf(numStr, "%lld", num);
//    if ((num >= 0 && countLength <= 3) || (num < 0 && countLength <= 4)) //С��3λֱ�ӷ���
//    {
//        strcpy(strBuf, numStr);
//        free(numStr);
//        SetPrintColor(color);
//        if (alignWidth == 0)
//            printf("%s", strBuf);
//        else
//        {
//            _itoa(alignWidth, alignWidthStr, 10);
//            STRCAT(outputFormat, alignWidthStr);
//            STRCAT(outputFormat, "s");
//            printf((const char*)outputFormat, strBuf);
//        }
//        SetPrintColor(RESET);
//        return color;
//    }
//    if (num < 0) //������
//    {
//        *p = '-';
//        p++;
//        index = 1;
//    }
//    for (; index <= countLength; index++)
//    {
//        *(p++) = numStr[index];
//        if ((countLength - index + 2) % 3 == 0 && countLength - index > 3) //ÿ3���ַ����һ������
//        {
//            *(p++) = ',';
//        }
//    }
//    free(numStr);
//    SetPrintColor(color);
//    if (alignWidth == 0)
//        printf("%s", strBuf);
//    else
//    {
//        _itoa(alignWidth, alignWidthStr, 10);
//        STRCAT(outputFormat, alignWidthStr);
//        STRCAT(outputFormat, "s");
//        printf("%-10s", strBuf);
//    }
//    SetPrintColor(RESET);
//    return color;
//}

/*
StrLongInt �ֶγ�����תΪ�ַ���

(long long)num   ����

���� �ַ���
*/
//LPSTR StrLongInt(long long num)
//{
//    int countLength = 0;
//    int index = 0;
//    long double temp = num;
//    char outputFormat[18] = "%";
//    char alignWidthStr[16] = { 0 };
//
//    while (1)
//    {
//        temp /= 10.0;
//        countLength++;
//        if (temp <= 1.0)
//            break;
//    }
//
//    char* numStr = (char*)malloc(countLength * sizeof(char));
//    char strBuf[1024] = { 0 };
//    char* p = strBuf;
//    sprintf(numStr, "%lld", num);
//    if ((num >= 0 && countLength <= 3) || (num < 0 && countLength <= 4)) //С��3λֱ�ӷ���
//    {
//        strcpy(strBuf, numStr);
//        free(numStr);
//        return strBuf;
//    }
//    if (num < 0) //������
//    {
//        *p = '-';
//        p++;
//        index = 1;
//    }
//    for (; index <= countLength; index++)
//    {
//        *(p++) = numStr[index];
//        if ((countLength - index + 2) % 3 == 0 && countLength - index > 3) //ÿ3���ַ����һ������
//        {
//            *(p++) = ',';
//        }
//    }
//    free(numStr);
//    return strBuf;
//}
//


/*
CheckFileType ����ļ�����

(LPSTR) fileName   �ļ���
(const LPSTR)type  ����

����
  ����ƥ�䷵�� true
  ���� false
*/
//bool CheckFileType(LPSTR fileName, const LPSTR type)
//{
//    const char* pFile;
//    pFile = strrchr(fileName, '.');
//    if (pFile != NULL)
//    {
//        if (strcmp(pFile, type) == 0)
//        {
//            return true;
//        }
//        else
//            return false;
//    }
//}
//
//
#endif

/*
BLACK------|---GREY
BLUE-------|---CYAN
GREEN------|---LIGHT_GREEN
LAKE_BLUE--|---LIGHTER_GREEN
RED--------|---LIGHT_RED
VIOLET-----|---LIGHT_VIOLET
YELLOW-----|---LIGHT_YELLOW
RESET------|---LIGHT_WHITE
*/

/*
��խ�ַ�ת��
*/
class Char
{
public:
	static std::wstring AtoW(const std::string& str)
	{
		int wcLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		std::wstring newBuf;
		newBuf.resize(wcLen);
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)newBuf.c_str(), wcLen);
		return newBuf;
	}

	static std::string& WtoA(const std::wstring& str)
	{
		int cLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, 0, NULL);
		std::string newBuf;
		newBuf.resize(cLen);
		WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, (char*)newBuf.c_str(), cLen, 0, NULL);
		return newBuf;
	}

	static std::string& WtoU8(const std::wstring& str)
	{
		int cLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, 0, NULL);
		std::string newBuf;
		newBuf.resize(cLen);
		WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, (LPSTR)newBuf.c_str(), cLen, 0, NULL);
		return newBuf;
	}

	static std::wstring& U8toW(const std::string& str)
	{
		int wcLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		std::wstring newBuf;
		newBuf.resize(wcLen);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (WCHAR*)newBuf.c_str(), wcLen);
		return newBuf;
	}

	static std::string& AtoU8(const std::string& str)
	{
		return WtoU8(AtoW(str));
	}

	static std::string& U8toA(const std::string& str)
	{
		return WtoA(U8toW(str));
	}
};

std::string IntString(unsigned long long num)
{
    std::string numStr = std::to_string(num);
    int digits = numStr.length();

    if (digits <= 3)
    {
        return numStr;
    }

    std::string result;

    // �����ײ����� 3 λ�Ĳ���
    int firstPartLen = digits % 3;
    if (firstPartLen == 0)
    {
        firstPartLen = 3;
    }
    result.append(numStr, 0, firstPartLen);

    // ��Ӷ��ŷָ��Ĳ���
    for (int i = firstPartLen; i < digits; i += 3)
    {
        result += ",";
        result.append(numStr, i, 3);
    }

    return result;
}