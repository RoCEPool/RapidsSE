/*============================================================================*/
/* XF_Optimise_Cut_sort.hpp -- Cut sort                                                */
/*============================================================================*/
#pragma once

#include "XF_Optimise.hpp"

namespace STORE
{
  class XF_Optimise_Cut_sort : public XF_Optimise
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_Cut_sort(CardIter* pCI);
   ~XF_Optimise_Cut_sort(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Cut_sort* _pXF_Optimise_Cut_sort(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
