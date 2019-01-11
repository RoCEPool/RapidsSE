/*============================================================================*/
/* StrVal.hpp -- null terminated string value                                 */
/*============================================================================*/
#pragma once

#ifdef STRING_LENGTH_TEST
  #define STRLEN_TEST() ASSERT(nAreaSize() == StrLib::iLength(aStz() + 1));
#else
  #define STRLEN_TEST() ;
#endif

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
  
    ~StrVal(void) noexcept(false);
  
    operator Stz&(void);
    operator Stz*(void);

    static i8 nBytesRequired(i8 iLength);
  
    void operator =(i8);
    void operator =(StrVal& rArg);
    void operator =(StrVal* pArg);
    void operator =(Stz*);
    //void operator =(GenVar& rArg);
  
    void  vAppend(const char cChar);
    i8    nAreaSize(void);
    void  vAreaSize(void);
    void  vAssign(Stz* aStr);
    void  vAssign(Stz* aStr, i8 iLength);
    void  vCat(Stz* aStr, ...);
    void  vCatSprint(Stz* aFormat, ...);
    void  vConcat(Stz* aStr);
    bool  bContains(Stz* aArg);
    void  vCopy(Stz* aStr);
    void  vCopyCat(Stz* aStr, ...);
    void  vCopyTill(Stz* aSrc, char cTill);
    bool  bEnds(Stz* aSuffix);
    bool  bIsEqual(StrVal& sArg, i8 iCase);
    bool  bEqual(Stz* aArg);

    i8    iLength(void);
    char* aChar(void);
    bool  bSame(Stz* aArg);
    i8    nSize(void);
    void  vSprint(Stz* aFormat, ...);
    Stz*  aStz(void);
    i8    iToI8(void);
    void  vToLower(void);
    void  vToString(Stz* aStr);
  
    bool  bIsNumeric(void);

  private:
    Footer* aFooter(void);
  };

  StrVal* _aStrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
