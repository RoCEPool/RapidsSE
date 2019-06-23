/*============================================================================*/
/* Dtm.hpp -- Complex Datum                                                   */
/*============================================================================*/
#pragma once

namespace Base
{
  enum eDM : u1 // Dtm Kind
  {
  /*  0 */ DM_NONE=0,
  
  /*  1 */ DM_BCD,
  /*  2 */ DM_BOL,
  /*  3 */ DM_CHR,
  /*  4 */ DM_DTE,
  /*  5 */ DM_FLT,
  /*  6 */ DM_INT,
  /*  7 */ DM_IVL,
  /*  8 */ DM_TME,
  /*  9 */ DM_TSP,
  /* 10 */ DM_VBN,
  /* 11 */ DM_VCH,
  
  /* 12 */ DM_ENUMS
  };
  
  static Stz* aDM[]
  {
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "BCD"_Stz,
  /*  2 */ "BOL"_Stz,
  /*  3 */ "CHR"_Stz,
  /*  4 */ "DTE"_Stz,
  /*  5 */ "FLT"_Stz,
  /*  6 */ "INT"_Stz,
  /*  7 */ "IVL"_Stz,
  /*  8 */ "TME"_Stz,
  /*  9 */ "TSP"_Stz,
  /* 10 */ "VBN"_Stz,
  /* 11 */ "VCH"_Stz,
  
  /* 12 */ "ENUMS"_Stz,
  };
  
  //----------------------------------------------------------------------------
  
  class Dtm
  {
  public:
    union {
      
      char* m_azStr;
      i8    m_iI8;
      i8    m_nSize;    // size of vch

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      i8    m_iBcd;
      i8    m_iBol;
      i8    m_iDte;
      i8    m_iEdt;
      d8    m_dFlt;
      i8    m_iInt;
      i8    m_iIvl;
      i8    m_iTme;
      i8    m_iTsp;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      _Dte  xDte;
      _Edt  xEdt;
      _Ivl  xIvl;
      _Tme  xTme;
      _Tsp  xTsp;
      Stz*  m_szVch;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      byte  m_yaByte[C_BYTES_PER_WORD];
      w8    m_w8;

    };
  
  public:
    Dtm(void);
   ~Dtm(void) noexcept(false);
  
    static i8   nCopy(byte* ayDst, byte* aySrc);
    static i8   nCopy(byte* ayDst, i8 iValue);
    static void vCopy(byte* ayDst, byte* aySrc);
    static void vCopy(byte* ayDst, i8 iValue);

    static i8   iDte(i8 iYear, i8 iMonth, i8 iDay);
    static i8   iIvl(i8 iSecond, i8 iMilli);
    static i8   iTme(i8 iHour, i8 iSecond, i8 iMinute, i8 iMicro);
    static i8   iTsp(i8 iYear, i8 iMonth,  i8 iDay,
                     i8 iHour, i8 iSecond, i8 iMinute, i8 iMicro);
  
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);
  
    void operator =(i8 iInt)    { m_iInt  = iInt;  }
    void operator =(Stz* szVch) { m_szVch = szVch; }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
