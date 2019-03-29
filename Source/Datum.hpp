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
      d8   m_dDatum;
      i8   m_iDatum;
      w8   m_wDatum;
      byte m_yaByte[C_BYTES_PER_WORD];
    };
  
    s4K m_sDatum;

    byte* m_ayDatum; //! temporary
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Datum(void);
    ~Datum(void) noexcept(false);
  
    eDM eDatum(void);
    void vDatum(eDM eDatum);

    w8 wDatum(void);
    void vDatum(w8 wDatum);

    s4K& sDatum(void);
    void vDatum(s4K& sDatum);

    Stz* szDatum(void);
    void vDatum(Stz* szDatum);

    byte* ayDatum(void);
    void vDatum(byte* ayDatum);

  private:
    Footer* aFooter(void);
  };
  
  Datum* _pDatum(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
