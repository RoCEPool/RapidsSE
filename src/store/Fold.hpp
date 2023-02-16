/*============================================================================*/
/* Fold.hpp -- Fold                                                           */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class Fold : public Object
  {
  private:

    SubQuery* m_pSubQuery;
    SubQuery* m_pOuterQuery;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Fold(void);
   ~Fold(void) noexcept(false);

    SubQuery* pOuterQuery(void);
    void vOuterQuery(SubQuery* pOuterQuery);

    SubQuery* pSubQuery(void);
    void vSubQuery(SubQuery* pSubQuery);

  protected:
    Footer* aFooter(void);
  };

  Fold* _pFold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
