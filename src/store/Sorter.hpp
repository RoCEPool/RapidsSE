/*============================================================================*/
/* Sorter.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Sorter : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Sorter(Bench* pBench);
   ~Sorter(void) noexcept(false);

    void vCull_sby_SameAs_gby(void);

  private:

    Footer* aFooter(void);
  };

  Sorter* _pSorter(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
