/*============================================================================*/
/* Card_Aggreg.hpp -- Aggreg                                                  */
/*============================================================================*/
#pragma once

#include "Card_AGG.hpp"

namespace STORE
{
  class Card_Aggreg : public Card_AGG
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Aggreg(void);
   ~Card_Aggreg(void) noexcept(false);


    void vBump_Rightwards(i8 nBumpBy);

    void vComputeProjection(CardIter* pCI); // prototype
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    void vDerive_streamKey(CardIter* pCI);

    void vEarlyProject_root(CardIter* pCI);

    void vSmooth(CardIter* pCI);
    void vUpdate_projection(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Aggreg* _pCard_Aggreg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
