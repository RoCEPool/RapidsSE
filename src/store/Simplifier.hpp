/*============================================================================*/
/* Simplifier.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Simplifier : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Simplifier(Bench* pBench);
  
   ~Simplifier(void) noexcept(false);

    void vSimplify(void);
  private:

    Footer* aFooter(void);
  };

  Simplifier* _pSimplifier(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
