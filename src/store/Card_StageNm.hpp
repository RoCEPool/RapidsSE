/*============================================================================*/
/* Card_StageNm.hpp -- Name                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StageNm : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StageNm(Stz* szName);
   ~Card_StageNm(void) noexcept(false);
  };

  Card_StageNm* _pCard_StageNm(void* aAddr);
  bool _bIsCard_StageNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
