/*============================================================================*/
/* Card_Having.hpp -- Having                                                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Having : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Having(void);
   ~Card_Having(void) noexcept(false);

    void vAcclimate_foldExpr(CardIter* pCI);
    void vSmooth(CardIter* pCI);
  };

  Card_Having* _pCard_Having(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
