/*============================================================================*/
/* StrVal.hpp -- null terminated string value                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class StrVal : public Value
  {
  protected:
    WspVbl m_xWsp;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, StrVal* pThis);
  
    StrVal(eCN eClassNo);
    StrVal(eCN eClass, i8 nBytes);
    StrVal(StrVal* pThis);
    StrVal(StrVal* pThis, i8 nBytes);
  
    ~StrVal(void);
  
    operator Str&(void);
    operator Str*(void);

    static i8 nAreaRequired(i8 iLength);
  
    void operator =(char* aArg);
    void operator =(StrVal& rArg);
    void operator =(StrVal* pArg);
    void operator =(Str*);
    void operator =(ValWsp& rArg);
  
    void  vAppend(const char cChar);
    void  vAppend(const char* aStr);
    char* aArea(void);
    i8    nAreaSize(void);
    void  vAreaSize(void);
    void  vAssign(Str* aStr);
    void  vAssign(Str* aStr, i8 iLength);
    void  vCat(Str* aStr, ...);
    void  vCatSprint(Str* aFormat, ...);
    void  vConcat(Str* aStr);
    bool  bContains(Str* aArg);
    void  vCopy(const char* aStr);
    void  vCopy(const char* aStr, i8 iLength);
    void  vCopy(Str* aStr);
    void  vCopyCat(Str* aStr, ...);
    void  vCopyTill(Str* aSrc, char cTill);
    bool  bEnds(const char* aSuffix);
    bool  bIsEqual(StrVal& sArg, i8 iCase);
    bool  bEqual(Str* aArg);

    i8    iLength(void);
    bool  bSame(Str* aArg);
    i8    nSize(void);
    void vSprint(Str* aFormat, ...);
    char* aStr(void);
    void  vToCommand(char* aStr);
    i8    iToI8(void);
    void  vToLower(void);
    void  vToString(char* aStr);
  
    Str* aStrArea(void);

  private:
    Footer* pFooter(void);
  };

  StrVal* _pStrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
