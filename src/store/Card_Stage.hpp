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

    Card_Stage(void);
   ~Card_Stage(void) noexcept(false);

    static void vDeleteAllStage(CardIter* pCI);
    static void vReinstate_stages(CardIter* pCI);

  private:
    static void vAddCombined(CardIter* pCI, Card* p_main, i8& ri_main);
    static void vAddNext(CardIter* pCI, Card* p_main, i8& ri_main, Stz* szNextValue);
    static void vAddStage(CardIter* pCI, Card* p_main, i8& ri_main, eAL e_level_running, i8& nStages);
  };

  Card_Stage* _pCard_Stage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
