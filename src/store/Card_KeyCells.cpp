/*============================================================================*/
/* Card_KeyCells.cpp -- Key Cells                                             */
/*============================================================================*/

#include "Card_KeyCells.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_KeyCells,CK_KEYCELLS)
__aNewFunction(Card_KeyCells,CN_CARD)
__vDeleteFunction(Card_KeyCells,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_KeyCells::Card_KeyCells(void)
: Card_CELLS(CK_KEYCELLS,C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_KeyCells::~Card_KeyCells(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a particular expr                                                      */
/*----------------------------------------------------------------------------*/
Card_Map* Card_KeyCells::pGetMap(i8 i_Map)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Map* p_map = _pCard_Map(pGetChild(i_Map));

  return p_map;
}

/*----------------------------------------------------------------------------*/
/* get map relating to ordinal symbol                                         */
/*----------------------------------------------------------------------------*/
Card_Map* Card_KeyCells::pGet_map(Symbol* pSymbol_keyCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Map  = pSymbol_keyCol->iTo_ordinal(SF_KEY_COL) - 1;

  Card_Map* p_map = _pCard_Map(pGetChild(i_Map));

  return p_map;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_KeyCells::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
