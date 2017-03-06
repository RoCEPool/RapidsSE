/*============================================================================*/
/* StrVbl.hpp -- null terminated variable-length string                       */
/*============================================================================*/
#pragma once

namespace Values
{
  class StrVbl : public Value
  {
  protected:
    WspVbl m_xWsp;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, StrVbl* pThis);
  
    StrVbl(eCN eClassNo);
    StrVbl(eCN eClass, i8 nBytes);
    StrVbl(StrVbl* pThis);
    StrVbl(StrVbl* pThis, i8 nBytes);
  
    ~StrVbl(void);
  
    operator Str& (void);
    operator Str* (void);
  
  
    static i8 nAreaRequired(i8 iLength);
  
    void operator =(const char* aArg);
    void operator =(char* aArg);
    void operator =(StrVbl& rArg);
  
    void  vAppend(const char cChar);
    void  vAppend(const char* aStr);
    char* aArea(void);
    i8    nAreaSize(void);
    void  vAreaSize(void);
    void  vAssign(const char* aStr);
    void  vAssign(const char* aStr, i8 iLength);
    void  vCopy(const char* aStr);
    void  vCopy(const char* aStr, i8 iLength);
    bool  bEnds(const char* aSuffix);
    bool  bIsEqual(StrVbl& sArg, i8 iCase);
    i8    iLength(void);
    i8    nSize(void);
    char* aStr(void);
    void  vToCommand(char* aStr);
    void  vToString(char* aStr);
  
    Str* aStrArea(void);
  private:
  
  
    Footer* pFooter(void);
  };

  StrVbl* _pStrVbl(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
