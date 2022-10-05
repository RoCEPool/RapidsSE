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

    static void vFormStageNm(String* psStageNm, i8 i_Stage);

    Card_Stage(void);
    Card_Stage(Stz* szStageNm);
   ~Card_Stage(void) noexcept(false);

    void vRenameStage(i8 i_Stage);
  };

  Card_Stage* _pCard_Stage(void* aAddr);
  bool _bIsCard_Stage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
