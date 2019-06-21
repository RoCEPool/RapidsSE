/*============================================================================*/
/* Datum.hpp -- Datum                                                         */
/*============================================================================*/
#pragma once

namespace Data
{
  class Datum : public Object
  {
  public:
  
    Column* m_pCol;
    eDM     m_eDatum;
  
    union {
      i8   m_iBcd;
      i8   m_iBol;
      i8   m_iDte;
      i8   m_iEdt;
      d8   m_dFlt;
      i8   m_iInt;
      i8   m_iIvl;
      i8   m_iTme;
      i8   m_iTsp;
      
      _Dte xDte;
      _Edt xEdt;
      _Ivl xIvl;
      _Tme xTme;
      _Tsp xTsp;
      
      Dtm  m_wDtm;
    };
  
    i8   m_nChars;
    sVCH m_sVch;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Datum(void);
    ~Datum(void) noexcept(false);

    bool bIsNull(void);
    void vSetNull(void);
  
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
