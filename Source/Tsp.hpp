/*============================================================================*/
/* Tsp.hpp -- timestamp datum                                                 */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Tsp
  {
  friend class DtmLib;

  protected:
    union {
      struct {
        u8 m_uSpare  :  6 ; //
        u8 m_uMicro  : 20 ; // 0..1M
        u8 m_uSecond :  6 ; // 0..63
        u8 m_uMinute :  6 ; // 0..63
        u8 m_uHour   :  5 ; // 0..31
        u8 m_uDay    :  5 ; // 0..31
        u8 m_uMonth  :  4 ; // 0..15
        u8 m_uYear   : 12 ; // 0..4095
        //            ---
        //             64
        //            ===
      } x;
      i8 m_iValue;
    };
  
  public:
    void* operator new(size_t);
  
    Tsp(void);
    ~Tsp(void) noexcept(false);
  
    bool operator <  (Tsp& xTsp);
    bool operator <= (Tsp& xTsp);
    bool operator == (Tsp& xTsp);
    bool operator != (Tsp& xTsp);
    bool operator >= (Tsp& xTsp);
    bool operator >  (Tsp& xTsp);
    
    void vInitZero(void);
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSet(Tsp wTspArg);
    void vSetAsNow(void);
    void vSetNull(void);
  
    i8   iYear(void);
    i8   iMonth(void);
    i8   iDay(void);
    i8   iHour(void);
    i8   iMinute(void);
    i8   iSecond(void);
    i8   iMicroseconds(void);
 
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static bool bExtract(char** aaSrc, Stz* aDst,
                         MetaData::Column* pColumn, Formatters* pFormatters);
    static bool bExtractDefault(char** aaSrc, Stz* aDst,
                         MetaData::Column* pColumn, Formatters* pFormatters);
    static void vSprint(Stz* aDst, Stz* aSrc);
    static void vSprintEscaped(Stz* aDst, Stz* aSrc);
    static void vSprintNonSQL(Stz* aDst, Stz* aSrc);
    
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
