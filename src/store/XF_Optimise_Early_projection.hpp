/*============================================================================*/
/* XF_Optimise_Early_projection.hpp -- Early projection                                */
/*============================================================================*/
#pragma once

#include "XF_OPTIMISE.hpp"

namespace STORE
{
  class XF_Optimise_Early_projection : public XF_OPTIMISE
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    static void vBackstep(CardIter* pCI, i8 iRoot, i8 iDepth,
                          const char* azCaller, i8 iLine);
    static void vEarlyRoot(CardIter* pCI, i8 iRoot);

    XF_Optimise_Early_projection(CardIter* pCI);
   ~XF_Optimise_Early_projection(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:


    Footer* aFooter(void);

  };

  XF_Optimise_Early_projection* _pXF_Optimise_Early_projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
