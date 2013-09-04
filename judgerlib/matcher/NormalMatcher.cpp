﻿#include "NormalMatcher.h"

#include "../filetool/FileTool.h"

namespace IMUST
{
    
namespace
{

const OJString WhiteSpaces = OJStr(" \r\n\t");

    
bool isWhiteSpace(OJChar_t ch)
{
    return WhiteSpaces.find(ch) != WhiteSpaces.npos;
}

}

NormalMatcher::NormalMatcher(void)
{
}


NormalMatcher::~NormalMatcher(void)
{
}

bool NormalMatcher::run(
    const OJString & answerOutputFile, 
    const OJString & userOutputFile)
{
    result_ = compare(answerOutputFile, userOutputFile);

    return isAccept();
}

OJInt32_t NormalMatcher::compare(const OJString & srcFile, const OJString & destFile)
{
    std::vector<OJChar_t> srcBuffer;
    if (!FileTool::ReadFile(srcBuffer, srcFile, false))
    {
        return MatcherCode::SystemError;
    }

    std::vector<OJChar_t> dstBuffer;
    if (!FileTool::ReadFile(dstBuffer, destFile, false))
    {
        return MatcherCode::SystemError;
    }

    return compare(srcBuffer, dstBuffer);
}

OJInt32_t NormalMatcher::compare(std::vector<OJChar_t> & srcBuffer, std::vector<OJChar_t> & dstBuffer)
{
    bool presentError = false;

    OJUInt32_t srcLen = srcBuffer.size();
    OJUInt32_t dstLen = dstBuffer.size();

    OJChar_t srcChr, dstChr;

    OJUInt32_t i=0, k=0;
    while(i<srcLen && k<dstLen)
    {
        srcChr = srcBuffer[i];
        dstChr = dstBuffer[k];

        if (srcChr == dstChr)
        {
            ++i;
            ++k;
            continue;
        }

        //srcChr != dstChr

        if (srcChr == OJCh('\r') && dstChr == OJCh('\n'))
        {
            ++i;
        }
        else if (dstChr == OJCh('\r') && srcChr == OJCh('\n'))
        {
            ++k;
        }
        else if (isWhiteSpace(srcChr))
        {
            presentError = true;
            ++i;
        }
        else if (isWhiteSpace(dstChr))
        {
            presentError = true;
            ++k;
        }
        else
        {
            return MatcherCode::WrongAnswer;
        }
    }

    while(i < srcLen)
    {
        if (!isWhiteSpace(srcBuffer[i]))
        {
            return MatcherCode::WrongAnswer;
        }
        presentError = true;
        ++i;
    }
        
    while(k < dstLen)
    {
        if (!isWhiteSpace(dstBuffer[k]))
        {
            return MatcherCode::WrongAnswer;
        }
        presentError = true;
        ++k;
    }

    if(presentError)
    {
        return MatcherCode::PressentError;
    }

    return MatcherCode::Success;
}


}//namespace IMUST