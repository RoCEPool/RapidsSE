/*============================================================================*/
/* Curator.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Curator : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Curator(Bench* pBench);
   ~Curator(void) noexcept(false);

    void vChangeAll_scan_To_retrieve(void);
  private:

    Footer* aFooter(void);
  };

  Curator* _pCurator(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
