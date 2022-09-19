/*============================================================================*/
/* Filtrate.hpp --                                                             */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Filtrate : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filtrate(Bench* pBench);
  
   ~Filtrate(void) noexcept(false);

    void vDelete(void);
  private:

    Footer* aFooter(void);
  };

  Filtrate* _pFiltrate(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
