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
    s32 m_sStageNm;
    eYG m_eStageTopology;

  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Stage(eCK eKind, Stz* szStageNm, eYG eStageTopology);
   ~Card_Stage(void) noexcept(false);
    
    Stz* szStageNm(void);
    void vStageNm(Stz* szStageNm);
    eYG  eStageTopology(void);
    void vStageTopology(eYG  eStageTopology);

    void vPrintStage(Ribbon* pRibbon);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_Stage* _pCard_Stage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
