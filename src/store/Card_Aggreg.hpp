/*============================================================================*/
/* Card_Aggreg.hpp -- Aggreg                                                  */
/*============================================================================*/
#pragma once

#include "Card_Project.hpp"

namespace STORE
{
  class Card_Aggreg : public Card_Project
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Aggreg(void);
   ~Card_Aggreg(void) noexcept(false);

    void vApplyStream(Card_StreamModel* pStreamModel); // prototype

  };

  Card_Aggreg* _pCard_Aggreg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
