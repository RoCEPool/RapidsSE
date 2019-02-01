/*============================================================================*/
/* Ivl.hpp -- interval datum                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Ivl
  {
  friend class DtmLib;
  
  protected:
    union {
      struct {
        u8 m_uMilli  : 10 ; // 0..1023
        u8 m_uSecond : 54 ; // 0..1.8E16
        //            ---
        //             64
        //            ===
      } x;
      i8 m_iValue;
    };
  
  public:
    void* operator new(size_t);
  
    Ivl(void);
    ~Ivl(void) noexcept(false);
  
    bool operator <  (Ivl& xIvl);
    bool operator <= (Ivl& xIvl);
    bool operator == (Ivl& xIvl);
    bool operator != (Ivl& xIvl);
    bool operator >= (Ivl& xIvl);
    bool operator >  (Ivl& xIvl);

    void vInitZero(void);
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
  
    i8   iSecond(void);
    i8   iMilli(void);
  
    void vSet(u8 uSecond, u8 uMilli);
  
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static bool bExtract(char** aaSrc, Stz* aDst,
                         DBsuper::Column* pColumn, Formatters* pFormatters);
    static bool bExtractDefault(char** aaSrc, Stz* aDst,
                         DBsuper::Column* pColumn, Formatters* pFormatters);
    static bool bExtractExtract(char** aaSrc, Stz* aDst,
                         DBsuper::Column* pColumn, Formatters* pFormatters);
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
