/*============================================================================*/
/* Modifications.hpp --                                                          */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Modifications : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Modifications(eCN eClassNo, Bench* pBench);
   ~Modifications(void) noexcept(false);

    void vCullAll(eCK eCard);
    void vDeleteAll(eCK eCard);
    void vRenumberMainCards(void);

    void R_vReplaceSub(Card* pInCard, Card* pSub, Card* pWith);

    void X_calc_To_calcBool(void);

  private:

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
