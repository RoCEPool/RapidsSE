/*============================================================================*/
/* XF_Flatten_Left_subQuery.hpp -- Left SubQuery                              */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN.hpp"

namespace STORE
{
  class XF_Flatten_Left_subQuery : public XF_FLATTEN
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Left_subQuery(CardIter* pCI);
   ~XF_Flatten_Left_subQuery(void) noexcept(false);

    static void vRemove_LeftMost(CardIter* pCI);

    void vRemove_LeftMost(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Left_subQuery* _pXF_Flatten_Left_subQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
