#include "pch.h"
#include "BEUtil.h"

std::string BEUtil::ToString(const std::wstring& wstr)
{
    int tgtlength;
    int srclength = (int)wstr.length();
    tgtlength = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), srclength, 0, 0, 0, 0);
    std::string r(tgtlength, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), srclength, &r[0], tgtlength, 0, 0);
    return r;
}

std::wstring BEUtil::ToWString(const std::string& str)
{
    int tgtlength;
    int srclength = (int)str.length();
    tgtlength = MultiByteToWideChar(CP_ACP, 0, str.c_str(), srclength, 0, 0);
    std::wstring r(tgtlength, L'\0');
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), srclength, &r[0], tgtlength);
    return r;
}
