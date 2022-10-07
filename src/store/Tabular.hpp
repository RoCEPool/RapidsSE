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

    void vAdd_tablePtn_To_table(void);
    void vAdd_tableSyn_To_table(void);

  private:

    Footer* aFooter(void);
  };

  Tabular* _pTabular(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
