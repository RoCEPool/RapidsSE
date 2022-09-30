/*============================================================================*/
/* Tabular.hpp --                                                             */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Tabular : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Tabular(Bench* pBench);
   ~Tabular(void) noexcept(false);

    void vAdd_TablePtn(void);
    void vAdd_TableSyn(void);

  private:

    Footer* aFooter(void);
  };

  Tabular* _pTabular(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
