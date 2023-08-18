/*============================================================================*/
/* XF_OT_Join_Ordering.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class XF_OT_Join_Ordering : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_OT_Join_Ordering(CardIter* pCI);
   ~XF_OT_Join_Ordering(void) noexcept(false);

    static void vXF_Join_Ordering(CardIter* pCI);

    void vJoinOrdering(void);

  private:

    void vReorderTables(void);

  protected:
    Footer* aFooter(void);
  };

  XF_OT_Join_Ordering* _pXF_OT_Join_Ordering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
