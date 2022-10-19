/*============================================================================*/
/* Card_TstPred.hpp -- Test Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_TstPred : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TstPred(void);
   ~Card_TstPred(void) noexcept(false);
  };

  Card_TstPred* _pCard_TstPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
