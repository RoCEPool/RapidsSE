/*============================================================================*/
/* Card_HshColList.cpp -- Index Column List                                   */
/*============================================================================*/

#include "Card_IdxCol.hpp"
#include "Card_HshColList.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_HshColList, CK_s_HSHCOLLIST)
__aNewFunction(Card_HshColList,CN_CARD)
__vDeleteFunction(Card_HshColList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_HshColList::Card_HshColList(void)
    : Card_symbol(CK_s_HSHCOLLIST, 2)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_HshColList::~Card_HshColList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_HshColList::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_t_HSHCOL)) {
    pCI->G.p_hshCol = xKid.p_hshCol();
    pCI->G.p_hshCol->vX_tblCol_To_expCol(pCI);
  }

  pCI->G.p_hshCol = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_HshColList::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
