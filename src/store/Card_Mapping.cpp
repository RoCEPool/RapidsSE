/*============================================================================*/
/* Card_Mapping.cpp -- Mapping                                                */
/*============================================================================*/

#include "Card_Mapping.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Mapping, CK_s_MAPPING)
__aNewFunction(Card_Mapping,CN_CARD)
__vDeleteFunction(Card_Mapping,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Mapping::Card_Mapping(void)
: Card_symbol(CK_s_MAPPING, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Mapping::~Card_Mapping(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert to streamDef                                                       */
/*----------------------------------------------------------------------------*/
void Card_Mapping::vToColMap(ColMap* pColMap)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKeyCells(pSub_keyCells()); xKeyCells.bEach(CK_MAP); /**/) {
    Card_Map* p_map = xKeyCells.p_map();

    i8 i_EmittedCol = p_map->pSymbol()->Y_iGet(P_EmittedCol);
    i8 i_Cell       = p_map->pSymbol()->Y_iGet(P_Cell);

    pColMap->vAdd(i_EmittedCol, false, i_Cell);
  }

  for (Kid xPayloadCells(pSub_payloadCells()); xPayloadCells.bEach(CK_MAP); /**/) {
    Card_Map* p_map = xPayloadCells.p_map();

    i8 i_EmittedCol = p_map->pSymbol()->Y_iGet(P_EmittedCol);
    i8 i_Cell       = p_map->pSymbol()->Y_iGet(P_Cell);

    pColMap->vAdd(i_EmittedCol, true, i_Cell);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Mapping::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
