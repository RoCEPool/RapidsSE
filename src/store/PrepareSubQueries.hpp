/*============================================================================*/
/* PrepareSubQueries.hpp -- Prepare SubQueries                                */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class PrepareSubQueries : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PrepareSubQueries(CardIter* pCI);
   ~PrepareSubQueries(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  PrepareSubQueries* _pPrepareSubQueries(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
