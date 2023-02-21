/*============================================================================*/
/* Card_Having.hpp -- Having                                                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class CarryForward;

  class Card_Having : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Having(void);
   ~Card_Having(void) noexcept(false);

    void vAcclimate_foldExpr(CardIter* pCI);

    void vCorrect_After_Inject(CardIter* pCI, bool bBump);

    void vSmooth(CardIter* pCI, bool bSawProject);
    void vSmooth(CardIter* pCI, CarryForward* pCarryForward);
  };

  Card_Having* _pCard_Having(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
