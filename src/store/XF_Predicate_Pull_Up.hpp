/*============================================================================*/
/* XF_Predicate_Pull_Up.hpp -- Predicate Pull Up                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Predicate_Pull_Up : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Predicate_Pull_Up(CardIter* pCI);
   ~XF_Predicate_Pull_Up(void) noexcept(false);

    static void vXF_Predicate_Pull_Up(CardIter* pCI);

    void vXF_Predicate_Pull_Up(void);

  private:
    Footer* aFooter(void);
  };

  XF_Predicate_Pull_Up* _pXF_Predicate_Pull_Up(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
