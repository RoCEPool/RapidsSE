/*============================================================================*/
/* Card_Injectable.cpp -- Injectable Columns                                  */
/*============================================================================*/

#include "Card_Injectable.hpp"
#include "CardIter.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Injectable, CK_INJECTABLE)
__aNewFunction(Card_Injectable, CN_CARD)
__vDeleteFunction(Card_Injectable, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Injectable::Card_Injectable(void)
: Card_EXPRLIST(CK_INJECTABLE, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Injectable::~Card_Injectable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new Card_Injectable based on streamDef of table                              */
/*----------------------------------------------------------------------------*/
Card_Injectable* Card_Injectable::pNew_injected(CardIter* pCI)
{
  _static_;

  Table* pTable = pCI->G.p_table->pGet_Table();
  StreamDef* pStreamDef = pTable->pStreamDef();

  char cTblSyn = pCI->G.p_table->pSymbol()->cSF_char();

  Card_Injectable* p_injected = new Card_Injectable;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr = new Card_Expr;
    Symbol xSymbol(SF_STM_COL, i_Col + 1);
    p_expr->pSymbol()->vFacets(&xSymbol);
    p_injected->vAppendChild(p_expr);

    eCT eColumnType = pColumn->eColumnType();

    p_expr->pSymbol()->vAppendFacet(SF_NMD_COL,pColumn->szColNm());

    Card_RpnCalc *p_rpnCalc = new Card_RpnCalc;
    p_expr->vAppendChild(p_rpnCalc);
    p_rpnCalc->pSymbol()->vColType(eColumnType);
    p_expr->pSymbol()->vColType(eColumnType);

    Card_Term *p_term = (new Card_Term) -> SYM(SF_TBL_COL, cTblSyn, i_Col + 1);
    p_rpnCalc->vAppendChild(p_term);

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    p_term->pSymbol()->Y_vSet(P_Column);
    p_term->pSymbol()->vColType(eColumnType);
  }

  return p_injected;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Injectable::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pString->vCopy("i:"_Stz);
  Card_EXPRLIST::vShortForm(pString);
  //pString->vConcat("}"_Stz);

}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Injectable::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //RASSERT(CK_STREAM == pParent->eCard());
  
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
