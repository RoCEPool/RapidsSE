/*============================================================================*/
/* XF_Flatten_Fold_subquery.hpp -- Flatten                                             */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN.hpp"

namespace STORE
{
  class XF_Flatten_Fold_subquery : public XF_FLATTEN
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Fold_subquery(CardIter* pCI);
   ~XF_Flatten_Fold_subquery(void) noexcept(false);

    static void v10_F(CardIter* pCI);

    void v10_F(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Fold_subquery* _pXF_Flatten_Fold_subquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
