/*============================================================================*/
/* Card_Stage.hpp -- Stage                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_UNIT.hpp"

namespace STORE
{
  class Card_Stage : public Card_UNIT
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Stage(Stz* szStageNm, eYG eStageTopology);
   ~Card_Stage(void) noexcept(false);
  };

  Card_Stage* _pCard_Stage(void* aAddr);
  bool _bIsCard_Stage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
