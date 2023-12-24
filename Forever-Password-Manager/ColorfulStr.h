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
    SetPrintColor  设置控制台指定输出颜色

    [in] (int) color  指定颜色

    返回
      无
    */
    void SetPrintColor(int color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    /*
    prints  带颜色输出指定字符串

    [in] (const LPCSTR) str_in    指定字符串
    [in] (int) str_color         指定颜色

    返回 颜色enum值
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
    请使用STRCMP完全比较两个字符串
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
    请使用MEMCMP完全比较两个字符串中子串
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
    分割字符串
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
wprints  带颜色输出指定字符串

[in] (const LPWSTR) str_in    指定字符串
[in] (int) str_color         指定颜色

返回 颜色enum值
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
printa  输出长文本

[in] (const LPCSTR*)str_lines_in   输入二维多行文本字符串
[in] (int)max_lines               输出最大行数

返回 无
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
printLongInt 分段输出长整数

(long long)num   数字
(int)alignWidth  对齐宽度 （0 绝对左对齐, <正整数> 右对齐指定宽度, <负整数> 左对齐指定宽度）
(int)color       颜色 （可缺省）

返回 color值
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
//    if ((num >= 0 && countLength <= 3) || (num < 0 && countLength <= 4)) //小于3位直接返回
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
//    if (num < 0) //处理负数
//    {
//        *p = '-';
//        p++;
//        index = 1;
//    }
//    for (; index <= countLength; index++)
//    {
//        *(p++) = numStr[index];
//        if ((countLength - index + 2) % 3 == 0 && countLength - index > 3) //每3个字符添加一个逗号
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
StrLongInt 分段长整数转为字符串

(long long)num   数字

返回 字符串
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
//    if ((num >= 0 && countLength <= 3) || (num < 0 && countLength <= 4)) //小于3位直接返回
//    {
//        strcpy(strBuf, numStr);
//        free(numStr);
//        return strBuf;
//    }
//    if (num < 0) //处理负数
//    {
//        *p = '-';
//        p++;
//        index = 1;
//    }
//    for (; index <= countLength; index++)
//    {
//        *(p++) = numStr[index];
//        if ((countLength - index + 2) % 3 == 0 && countLength - index > 3) //每3个字符添加一个逗号
//        {
//            *(p++) = ',';
//        }
//    }
//    free(numStr);
//    return strBuf;
//}
//


/*
CheckFileType 检查文件类型

(LPSTR) fileName   文件名
(const LPSTR)type  类型

返回
  类型匹配返回 true
  否则 false
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
宽窄字符转换
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

    // 计算首部不满 3 位的部分
    int firstPartLen = digits % 3;
    if (firstPartLen == 0)
    {
        firstPartLen = 3;
    }
    result.append(numStr, 0, firstPartLen);

    // 添加逗号分隔的部分
    for (int i = firstPartLen; i < digits; i += 3)
    {
        result += ",";
        result.append(numStr, i, 3);
    }

    return result;
}