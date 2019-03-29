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
      Stz* m_szVch;

      w8   m_wWord;
      byte m_yaByte[C_BYTES_PER_WORD];
    };
  
    s4K m_sStr;

    byte* m_ayDatum; //! temporary
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Datum(void);
    ~Datum(void) noexcept(false);
  

  private:
    Footer* aFooter(void);
  };
  
  Datum* _pDatum(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
