/*============================================================================*/
/* XF_Flatten_Reposition_predicate.hpp -- Predicate Reposition                        */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Reposition_predicate : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Reposition_predicate(CardIter* pCI);
   ~XF_Flatten_Reposition_predicate(void) noexcept(false);

    static void vXF_Predicate_Reposition(CardIter* pCI);

    void vXF_Predicate_Reposition(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Reposition_predicate* _pXF_Flatten_Reposition_predicate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
