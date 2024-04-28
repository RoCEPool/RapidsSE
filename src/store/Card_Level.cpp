/*============================================================================*/
/* Card_Level.cpp -- Aggregate Level                                          */
/*============================================================================*/

#include "Card_Level.hpp"
#include "CardIter.hpp"
#include "EnumGroups.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Level, CK_p_LEVEL)
__aNewFunction(Card_Level, CN_CARD)
__vDeleteFunction(Card_Level, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Level::Card_Level(eAL eLevel)
: Card_symbol(CK_p_LEVEL, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pSymbol()->Y_vSet(P_Level, (i8)eLevel);
  pSymbol()->vApplyFacet(SF_ENUM, EG_AL, eLevel);

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
/* convert eAL to eYG                                                         */
/*----------------------------------------------------------------------------*/
eYG Card_Level::e_aggLevel_To_stageTopo(eAL eLevel)
{
  _static_

  switch (eLevel) {
    case AL_PARTITION : return YG_ALL_PARTITIONS;
    case AL_NODE      : return YG_ALL_NODES;
    case AL_ORIGIN    : return YG_ORIGIN_NODE;
    default           : return YG_ALL_PARTITIONS;
  }
}

/*----------------------------------------------------------------------------*/
/* convert eYG to eAL                                                         */
/*----------------------------------------------------------------------------*/
eAL Card_Level::e_stageTopo_To_aggLevel(eYG eStageTopo)
{
  _static_

  eAL eLevel;

  switch (eStageTopo) {
    case YG_ALL_PARTITIONS : eLevel = AL_PARTITION; break;
    case YG_ALL_NODES      : eLevel = AL_NODE;      break;
    case YG_ORIGIN_NODE    : eLevel = AL_ORIGIN;    break;
    default                : eLevel = AL_NONE;      break;
  }

  return eLevel;
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
  vValidate_hasNoChildren();
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
