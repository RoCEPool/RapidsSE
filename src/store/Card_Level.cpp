/*============================================================================*/
/* Card_Level.cpp -- Aggregate Level                                       */
/*============================================================================*/

#include "Card_Level.hpp"
#include "CardIter.hpp"
#include "EnumGroups.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Level, CK_s_LEVEL)
__aNewFunction(Card_Level, CN_CARD)
__vDeleteFunction(Card_Level, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Level::Card_Level(eAL eLevel)
: Card_symbol(CK_s_LEVEL, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef Z_LEVEL_ENUM
  pSymbol()->Y_vSet(P_Level, (i8)eLevel);
  pSymbol()->vApplyFacet(SF_ENUM, EG_AL, eLevel);
#else
  pSymbol()->Y_vSet(P_Level, (i8)eLevel);
#endif
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Level::~Card_Level(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Level::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  eAL eLevel = (eAL)pSymbol()->Y_iGet(P_Level);

  RASSERT(AL_PARTITION == eLevel ||
          AL_NODE      == eLevel ||
          AL_ORIGIN    == eLevel ||
          AL_NONE      == eLevel);

  Card::vValidate(pCI,iDepth,pParent);
  vValidate_noChildren();
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
