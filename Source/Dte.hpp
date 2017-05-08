/*============================================================================*/
/* Dte.hpp -- date datum                                                      */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Dte
  {
  friend class DtmLib;
  
  private:
    struct {      
      u8 m_uYear  : 12 ; // 0..4095
      u8 m_uMonth :  4 ; // 0..15
      u8 m_uDay   :  5 ; // 0..31
      u8 m_uSpare : 43 ; //
      //           ---
      //            64
      //           ===
    } x;
  
  public:
    void* operator new(size_t);
  
    Dte(void);
    ~Dte(void);
  
    bool operator <  (Dte& xDte);
    bool operator <= (Dte& xDte);
    bool operator == (Dte& xDte);
    bool operator != (Dte& xDte);
    bool operator >= (Dte& xDte);
    bool operator >  (Dte& xDte);
  
    void vInitZero(void);
  
    static bool bExtract(Str* aDst, Str* aSrc, i8& nBytesTaken);
    static void vSprint(Str* aDst, Str* aSrc, i8& nBytesTaken);
    
    static bool bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    
    static i8 iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
