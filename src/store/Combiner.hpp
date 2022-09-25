/*============================================================================*/
/* Combiner.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Combiner : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Combiner(Bench* pBench);
  
   ~Combiner(void) noexcept(false);

    void vAdd_combined(void);
    void vAdd_combined2(void);
    void vDelete_Combines(void);

  private:

    Footer* aFooter(void);
  };

  Combiner* _pCombiner(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
