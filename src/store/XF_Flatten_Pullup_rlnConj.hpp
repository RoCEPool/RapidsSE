/*============================================================================*/
/* XF_Flatten_Pullup_rlnConj.hpp -- Predicate Pull Up                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Pullup_rlnConj : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Pullup_rlnConj(CardIter* pCI);
   ~XF_Flatten_Pullup_rlnConj(void) noexcept(false);

    static void vXF_Pullup_rlnConj(CardIter* pCI);

    void vXF_Pullup_rlnConj(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Pullup_rlnConj* _pXF_Flatten_Pullup_rlnConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
