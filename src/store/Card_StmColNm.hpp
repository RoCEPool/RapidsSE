/*============================================================================*/
/* Card_StmColNm.hpp -- Stream Column Name                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StmColNm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StmColNm(void);
   ~Card_StmColNm(void) noexcept(false);
  };

  Card_StmColNm* _pCard_StmColNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
