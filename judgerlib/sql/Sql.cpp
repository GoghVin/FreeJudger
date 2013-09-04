﻿#include "Sql.h"

#pragma warning(push)
#pragma warning(disable:4996)

namespace IMUST
{

namespace StringConvert
{


//多字节字符串转换成宽字符字符串
bool narrowStringToWide(std::wstring & dest, const std::string & src, DWORD code/*=0*/)
{
    long lLen = MultiByteToWideChar(code, 0, src.c_str(), -1, NULL, 0); 
    dest.resize(lLen, 0);
    MultiByteToWideChar(code, 0, src.c_str(), -1, &dest[0], lLen);
    
    size_t pos = dest.find(L'\0');
    if(pos != dest.npos)
    {
        dest.erase(pos);
    }

    return false;
}

//宽字符串转换成多字节字符串
bool wideStringToNarrow(std::string & dest, const std::wstring & src, DWORD code/*=0*/)
{
    long lLen = WideCharToMultiByte(code, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
    dest.resize(lLen, 0);
    WideCharToMultiByte(code, 0, src.c_str(), -1, &dest[0], lLen, NULL, NULL);

    size_t pos = dest.find('\0');
    if(pos != dest.npos)
    {
        dest.erase(pos);
    }

    return false;
}

JUDGER_API bool OJStringToNarrowString(std::string & dest, const OJString & src)
{
#ifndef OJ_USE_WIDE_CHAR_SET
    dest = src;
    return true;
#else
    return wideStringToNarrow(dest, src);
#endif
}

JUDGER_API bool NarrowStringToOJString(OJString & dest, const std::string & src)
{
#ifndef OJ_USE_WIDE_CHAR_SET
    dest = src;
    return true;
#else
    return narrowStringToWide(dest, src);
#endif
}

JUDGER_API std::string OJStringToNarrowString(const OJString & src)
{
    std::string tempString;
    OJStringToNarrowString(tempString, src);
    return tempString;
}

JUDGER_API OJString NarrowStringToOJString(const std::string & src)
{
    OJString tempString;
    NarrowStringToOJString(tempString, src);
    return tempString;
}

} //namespace StringConvert



SqlVar::SqlVar(const OJString & v)
    : value_(v)
{}

bool SqlVar::getBool() const
{
    if(value_ == OJStr("true"))
    {
        return true;
    }
    return getInt32() != 0;
}

OJInt32_t SqlVar::getInt32() const
{
    OJInt32_t d = 0;
    OJSscanf(value_.c_str(), OJStr("%d"), &d);
    return d;
}

OJFloat16_t SqlVar::getFloat16() const
{
    OJFloat16_t d = 0;
    OJSscanf(value_.c_str(), OJStr("%f"), &d);
    return d;
}

OJFloat32_t SqlVar::getFloat32() const
{
    OJFloat32_t d = 0;
    OJSscanf(value_.c_str(), OJStr("%lf"), &d);
    return d;
}


const SqlVar & SqlRow::operator[](OJUInt32_t index) const
{
    return getVar(index);
}

} // namespace IMUST

#pragma warning(pop)