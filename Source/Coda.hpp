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
 
    eDM m_aeDatum[150];
    i2  m_aiAux[150];

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
