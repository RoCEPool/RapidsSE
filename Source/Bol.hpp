/*============================================================================*/
/* Bol.hpp -- boolean datum                                                   */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Bol
  {
  friend class DtmLib;
  
  protected:
    i8 m_bBol;
  
  public:
    void* operator new(size_t);
  
    Bol(void);
    ~Bol(void) noexcept(false);
  
    bool operator <  (Bol& xBol);
    bool operator <= (Bol& xBol);
    bool operator == (Bol& xBol);
    bool operator != (Bol& xBol);
    bool operator >= (Bol& xBol);
    bool operator >  (Bol& xBol);
  
    void vInitZero(void);
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
  
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static bool bExtract(char** aaSrc, Stz* aDst,
                         MetaData::Column* pColumn, Formatters* pFormatters);
    static bool bExtractDefault(char** aaSrc, Stz* aDst,
                         MetaData::Column* pColumn, Formatters* pFormatters);
    static void vSprint(Stz* aDst, Stz* aSrc);
    
    static bool bLessThan(char* aArgA, char* aArgB);
    static bool bLessEqual(char* aArgA, char* aArgB);
    static bool bEqualTo(char* aArgA, char* aArgB);
    static bool bNotEqual(char* aArgA, char* aArgB);
    static bool bGreaterEqual(char* aArgA, char* aArgB);
    static bool bGreaterThan(char* aArgA, char* aArgB);
    
    static i8 iCmp(char* aArgA, char* aArgB);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
