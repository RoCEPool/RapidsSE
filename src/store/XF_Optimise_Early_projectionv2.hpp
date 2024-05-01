/*============================================================================*/
/* XF_Optimise_Early_projectionv2.hpp -- Early projection                                */
/*============================================================================*/
#pragma once

#include "XF_OPTIMISE.hpp"

namespace STORE
{
  class XF_Optimise_Early_projectionv2 : public XF_OPTIMISE
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    XF_Optimise_Early_projectionv2(CardIter* pCI);
   ~XF_Optimise_Early_projectionv2(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

  private:
    void vNeeded_rloop(void);
    void vNeeded_root(i8 iRoot);
    void vNeeded_arced(i8 iRoot);


    Footer* aFooter(void);

  };

  XF_Optimise_Early_projectionv2* _pXF_Optimise_Early_projectionv2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
