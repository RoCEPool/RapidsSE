/*============================================================================*/
/* XF_Predicate_Reposition.hpp -- Predicate Reposition                        */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Predicate_Reposition : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Predicate_Reposition(CardIter* pCI);
   ~XF_Predicate_Reposition(void) noexcept(false);

    static void vXF_Predicate_Reposition(CardIter* pCI);

    void vXF_Predicate_Reposition(void);

  private:
    Footer* aFooter(void);
  };

  XF_Predicate_Reposition* _pXF_Predicate_Reposition(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
