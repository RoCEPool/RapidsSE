/*============================================================================*/
/* StrVal.hpp -- null terminated string value                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  // the strval inspector
  class StrSee
  {
    Value  m_xValue;      // the Value header
    i8     m_nSize;       // size of str
    char   m_cChr[256];   // str contents
  };

  // the strval class
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
  
    void operator =(i8);
    void operator =(StrVal& rArg);
    void operator =(StrVal* pArg);
    void operator =(Str*);
    void operator =(ValWrk& rArg);
  
    void  vAppend(const char cChar);
    i8    nAreaSize(void);
    void  vAreaSize(void);
    void  vAssign(Str* aStr);
    void  vAssign(Str* aStr, i8 iLength);
    void  vCat(Str* aStr, ...);
    void  vCatSprint(Str* aFormat, ...);
    void  vConcat(Str* aStr);
    bool  bContains(Str* aArg);
    void  vCopy(Str* aStr);
    void  vCopyCat(Str* aStr, ...);
    void  vCopyTill(Str* aSrc, char cTill);
    bool  bEnds(Str* aSuffix);
    bool  bIsEqual(StrVal& sArg, i8 iCase);
    bool  bEqual(Str* aArg);

    i8    iLength(void);
    char* s(void);
    bool  bSame(Str* aArg);
    i8    nSize(void);
    void  vSprint(Str* aFormat, ...);
    Str*  aStr(void);
    i8    iToI8(void);
    void  vToLower(void);
    void  vToString(Str* aStr);
  

  private:
    Footer* pFooter(void);
  };

  StrVal* _pStrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
