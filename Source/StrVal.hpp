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
  
    StrVal(void);
  
    operator Stz&(void);
    operator Stz*(void);

    void operator =(i8);
    void operator =(StrVal& rArg);
    void operator =(StrVal* aArg);
    void operator =(Stz*);
  
    i8    nAreaSize(void);
    void  vAreaSize(void);
    void  vAssign(Stz* aStr);
    void  vCopy(Stz* aStr);
    bool  bEqual(Stz* aArg);

    i8    iLength(void);
    char* aChar(void);
    bool  bSame(Stz* aArg);
    i8    nSize(void);
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
