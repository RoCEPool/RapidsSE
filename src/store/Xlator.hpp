/*============================================================================*/
/* Xlator.hpp --                                                              */
/*============================================================================*/
#pragma once

#include "Accessor.hpp"

namespace STORE
{
  class Xlator : public Accessor
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xlator(eCN eClassNo, Bench* pBench);
   ~Xlator(void) noexcept(false);

    void vCullAll(eCK eCard);
    void vDeleteAll(eCK eCard);
    void vRenumberMainCards(void);

    void R_vReplaceSub(Card* pSub, Card* pInParent, Card* pWith);

    void X_calc_To_calcBool(void);

  private:

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
