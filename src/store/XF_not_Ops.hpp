/*============================================================================*/
/* XF_not_Ops.hpp -- Not Ops                                                  */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_not_Ops : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_not_Ops(CardIter* pCI);
   ~XF_not_Ops(void) noexcept(false);

    static void vXF_not_Ops(CardIter* pCI);

    void vXF_not_Ops(void);

  private:
    Footer* aFooter(void);
  };

  XF_not_Ops* _pXF_not_Ops(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
