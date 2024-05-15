/*============================================================================*/
/* XF_Flatten_Reorder_join.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class XF_Flatten_Reorder_join : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Flatten_Reorder_join(CardIter* pCI);
   ~XF_Flatten_Reorder_join(void) noexcept(false);

    static void vXF_Reorder_join(CardIter* pCI);

    void vJoinOrdering(void);

  private:

    void vReorderTables(void);

  protected:
    Footer* aFooter(void);
  };

  XF_Flatten_Reorder_join* _pXF_Flatten_Reorder_join(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
