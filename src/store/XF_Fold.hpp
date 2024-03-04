/*============================================================================*/
/* XF_Fold.hpp -- XF_Fold                                                           */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class CarryForward;

  class XF_Fold : public Object
  {
  private:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Fold(void);
   ~XF_Fold(void) noexcept(false);


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    void vDerive_Correleted_fold_projection(CardIter* pCI);

    void vFold_basic(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ);

    CarryForward* pFold_from(Stz* szFoldNm, CardIter* pCI);

    CarryForward* pFold_nc_subQuery(CardIter* pCI, Stz* szFoldNm,
                                    eCK eCardWithSQ, eQO eWhatOp);

  protected:
    Footer* aFooter(void);
  };

  XF_Fold* _pXF_Fold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
