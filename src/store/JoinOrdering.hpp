/*============================================================================*/
/* JoinOrdering.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class JoinOrdering : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    JoinOrdering(CardIter* pCI);
   ~JoinOrdering(void) noexcept(false);

    void vJoinOrdering(void);

  private:

    void vReorderTables(void);

  protected:
    Footer* aFooter(void);
  };

  JoinOrdering* _pJoinOrdering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
