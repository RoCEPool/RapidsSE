/*============================================================================*/
/* XF_Optimise_from_To_OP.hpp -- Morph table to EE op                         */
/*============================================================================*/
#pragma once

#include "XF_Optimise.hpp"

namespace STORE
{
  class XF_Optimise_from_To_OP : public XF_Optimise
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_from_To_OP(CardIter* pCI);
   ~XF_Optimise_from_To_OP(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_from_To_OP* _pXF_Optimise_from_To_OP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
