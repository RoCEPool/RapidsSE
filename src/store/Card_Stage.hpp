/*============================================================================*/
/* Card_Stage.hpp -- Stage                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"
#include "eYG.h"


namespace STORE
{
  class Card_Stage : public Card_Unit
  {  
  protected:

  private:
    

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Stage(eCK eKind, Stz* szStageNm, eYG eStageTopology);
   ~Card_Stage(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_Stage* _pCard_Stage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
