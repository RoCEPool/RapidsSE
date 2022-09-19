/*============================================================================*/
/* Filtrate.hpp --                                                            */
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

    void vBool(void);
    void vRemove_predCmp(void);

  private:
    void vBool(Card* pCard_Filter);
    Card* pTo_CalcBool(Card* pCard_Calc);

    Footer* aFooter(void);
  };

  Filtrate* _pFiltrate(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
