/*============================================================================*/
/* Datum.hpp -- Datum                                                         */
/*============================================================================*/
#pragma once

namespace Data
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Datum : public Object
  {
  public:
  
    eDM m_eDatum;
  
    union {
      i8   m_iBcd;
      i8   m_iBol;
      i8   m_iDte;
      d8   m_dFlt;
      i8   m_iInt;
      i8   m_iIvl;
      i8   m_iTme;
      i8   m_iTsp;
      
      struct {
        u8 m_uSpare    : 47 ; //
        
        u8 m_uDay      :  5 ; // 0..31   same as xTsp.dte
        u8 m_uMonth    :  4 ; // 0..15
        u8 m_uYearDelta:  8 ; // 0..255  (1900+8b == 2155)
        //              ---
        //               64
        //              ===
      } xDte;
      
      struct {
        u8 m_uSpare    : 27 ; //
        
        u8 m_uMicro    : 20 ; // 0..1M  same as xTsp.tme
        u8 m_uSecond   :  6 ; // 0..63
        u8 m_uMinute   :  6 ; // 0..63
        u8 m_uHour     :  5 ; // 0..31
        //              ---
        //               64
        //              ===
      } xTme;

      struct {
        u8 m_uSpare    : 10 ; //           // 10b
        
        u8 m_uMicro    : 20 ; // 0..1M
        u8 m_uSecond   :  6 ; // 0..63
        u8 m_uMinute   :  6 ; // 0..63
        u8 m_uHour     :  5 ; // 0..31     // 37b
        
        u8 m_uDay      :  5 ; // 0..31
        u8 m_uMonth    :  4 ; // 0..15
        u8 m_uYearDelta:  8 ; // 0..255    // 17b  (1900+8b == 2155)
        //              ---
        //               64
        //              ===
      } xTsp;

      struct {
        u8 m_uMilli    : 10 ; // 0..1023
        u8 m_uSecond   : 54 ; // 0..1.8E16
        //              ---
        //               64
        //              ===
      } xIvl;
      
      Bcd  m_wBcd;
      Bol  m_wBol;
      Dte  m_wDte;
      Flt  m_wFlt;
      Int  m_wInt;
      Ivl  m_wIvl;
      Tme  m_wTme;
      Tsp  m_wTsp;

      w8   m_wWord;
      byte m_yaByte[C_BYTES_PER_WORD];
    };
  
    sVCH m_sVch;
    i8 m_nChars;

    eMN m_eEncoding;
    i8  m_nEncodedBytes;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Datum(void);
    ~Datum(void) noexcept(false);

    bool bIsNull(void);
    void vSetNull(void);
  
    void vSprint(Stz* szDst, Column* pCol);
    void vSprintBcd(Stz* szDst, Column* pCol);
    void vSprintBol(Stz* szDst, Column* pCol);
    void vSprintDte(Stz* szDst, Column* pCol);
    void vSprintFlt(Stz* szDst, Column* pCol);
    void vSprintInt(Stz* szDst, Column* pCol);
    void vSprintIvl(Stz* szDst, Column* pCol);
    void vSprintTme(Stz* szDst, Column* pCol);
    void vSprintTsp(Stz* szDst, Column* pCol);
    void vSprintVch(Stz* szDst, Column* pCol);

  private:
    Footer* aFooter(void);
  };
  
  Datum* _pDatum(void* aAddr);
}

#pragma pack(push, 1)
class Nibbler1 {
  struct {
    u8 m_uNibble : 4 ; //
  } m_xaNibbles[128];
};
class Nibbler2 {
  struct {
    u8 m_uNibbleL : 4 ; //
    u8 m_uNibbleR : 4 ; //
  } m_xaNibbles[64];
};
#pragma pack(pop)


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
