/*============================================================================*/
/* XF_Flatten_subquery.hpp -- Flatten SubQueries                                         */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"


namespace STORE
{
  class XF_Flatten_subquery : public XF_TRANSFORM
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Flatten_subquery(CardIter* pCI);
   ~XF_Flatten_subquery(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vFlatten(void);

  private:

    void vFlatten_recurse(i8 iDepth);

    void vFlattenByDecisionTree(void);

  protected:
    Footer* aFooter(void);
  };

  XF_Flatten_subquery* _pXF_Flatten_subquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
