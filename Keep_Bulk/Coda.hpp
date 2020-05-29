/*============================================================================*/
/* Coda.cpp -- Coda                                                           */
/*============================================================================*/
#pragma once

namespace Data
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Coda : public Object
  {
  public:
 
    eDM m_aeExtDatum[C_MAX_COLUMNS];
  
    union {
      i8   m_iBcd;
      i8   m_iBol;
      i8   m_iDte;
      d8   m_dFlt;
      i8   m_iInt;
      i8   m_iIvl;
      i8   m_iTme;
      i8   m_iTsp;
      char m_caChr[C_BYTES_PER_WORD];
      
    } m_aiAux[C_MAX_COLUMNS];

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddr);
    void  operator delete(void*);

    Coda(void);
    ~Coda(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };
  
  Coda* _pCoda(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
