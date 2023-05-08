/*============================================================================*/
/* OuterQuery.hpp -- Outer Query                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CardIter;

  class OuterQuery : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    OuterQuery(void);
   ~OuterQuery(void) noexcept(false);

    void vQueryAnalysis(CardIter* pCI);

  private:
    void vAnalysis_Of_from_call(CardIter* pCI);
    void vAnalysis_Of_having_boolCalc(CardIter* pCI);
    void vAnalysis_Of_where_boolCalc(CardIter*  pCI);

  private:
    Footer* aFooter(void);
  };

  OuterQuery* _pOuterQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
