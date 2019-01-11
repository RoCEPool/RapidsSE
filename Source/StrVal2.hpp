/*============================================================================*/
/* StrVal2.hpp -- null terminated string value                                */
/*============================================================================*/
#pragma once

#ifdef STRING_LENGTH_TEST
  #define STRLEN_TEST() ASSERT(nAreaSize() == StrLib::iLength(aStz() + 1));
#else
  #define STRLEN_TEST() ;
#endif

namespace Values
{
  // the strval class
  class StrVal2 : public Value
  {
  protected:
    WspVbl m_xWsp;
    //i8   m_nAreaUsed;

  public:
    void* operator new(size_t);
  
    StrVal2(void);
  
    operator Stz&(void);
    operator Stz*(void);
  
    static i8 nBytesRequired(i8 iLength);
  
    void operator =(i8);
    void operator =(StrVal2& rArg);
    void operator =(StrVal2* aArg);
    void operator =(Stz*);
  
    i8    nAreaSize(void);
    void  vAreaSize(void);
    void  vAssign(Stz* aStr);
    void  vCat(Stz* aStr, ...);
    void  vCatSprint(Stz* aFormat, ...);
    void  vConcat(Stz* aStr);
    bool  bContains(Stz* aArg);
    void  vCopy(Stz* aStr);
    void  vCopyCat(Stz* aStr, ...);
    void  vCopyTill(Stz* aSrc, char cTill);
    bool  bEnds(Stz* aSuffix);
    bool  bIsEqual(StrVal2& sArg, i8 iCase);
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

  StrVal2* _aStrVal2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
