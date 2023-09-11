/*============================================================================*/
/* XF_Flatten_Left_subQuery.hpp -- Left SubQuery                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Left_subQuery : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Left_subQuery(CardIter* pCI);
   ~XF_Flatten_Left_subQuery(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vFlatten(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Left_subQuery* _pXF_Flatten_Left_subQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
