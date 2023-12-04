/*============================================================================*/
/* Card_From.cpp -- From                                                      */
/*============================================================================*/

#include "Card_From.hpp"
#include "Card_Table.hpp"

#include "Table.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_From, CK_m_FROM)
__aNewFunction(Card_From,CN_CARD)
__vDeleteFunction(Card_From,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_From::Card_From(void)
: Card_OP(CK_m_FROM, 1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_From::~Card_From(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* table projection analysis                                                  */
/*----------------------------------------------------------------------------*/
Stz* Card_From::szColNm(i8 i_Col_base0)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_TableNm* p_tableNm = pSub_tableNm();

  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szFacet(SF_NONE));

  StreamDef* pStreamDef = pTable->pStreamDef();

  Column* pColumn = pStreamDef->pGetColumn(i_Col_base0);

  Stz* szColNm = pColumn->szColNm();

  return szColNm;
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_From::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pSymbol()->Y_vSet(P_Smoothed);

  pCI->G.p_table = pSub_table();

  vUpdate_emitted(pCI);
 // pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_From::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pSymbol()->Y_vSet(P_Smoothed);

  pCI->G.p_table = pSub_table();

  RASSERT(pCarryForward->Table.m_bReproject);
  vUpdate_emitted(pCI);
 // pCI->vRenumberCards();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_From::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card::vValidate(pCI,iDepth,pParent);

  Card_OP::vValidate(pCI,iDepth,pParent);

  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
