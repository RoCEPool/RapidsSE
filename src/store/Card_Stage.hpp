/*============================================================================*/
/* Card_Stage.hpp -- Stage                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Stage : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    static void vDeleteAllStage(CardIter* pCI);

    Card_Stage(void);
   ~Card_Stage(void) noexcept(false);
  };

  Card_Stage* _pCard_Stage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
