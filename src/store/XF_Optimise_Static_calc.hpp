/*============================================================================*/
/* XF_Optimise_Static_calc.hpp -- Optimise onetime calculations              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Optimise_Static_calc : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_Static_calc(CardIter* pCI);
   ~XF_Optimise_Static_calc(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Static_calc* _pXF_Optimise_Static_calc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
