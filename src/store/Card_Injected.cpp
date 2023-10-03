/*============================================================================*/
/* Card_Injected.cpp -- Injected Columns                                      */
/*============================================================================*/

#include "Card_Injected.hpp"
#include "CardIter.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Injected, CK_INJECTED)
__aNewFunction(Card_Injected, CN_CARD)
__vDeleteFunction(Card_Injected, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Injected::Card_Injected(void)
: Card_EXPRLIST(CK_INJECTED, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Injected::~Card_Injected(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new Card_Injected based on streamDef of table                              */
/*----------------------------------------------------------------------------*/
Card_Injected* Card_Injected::pNew_injected(CardIter* pCI)
{
  _static_;

  Table* pTable = pCI->G.p_table->pGet_Table();
  StreamDef* pStreamDef = pTable->pStreamDef();

  char cTblSyn = pCI->G.p_table->pSymbol()->cSF_char();

  Card_Injected* p_injected = new Card_Injected;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr = new Card_Expr;
    Symbol xSymbol(SF_COL_NO,i_Col+1);
    p_expr->pSymbol()->vSym(&xSymbol);
    p_injected->vAppendChild(p_expr);

    eCT eColumnType = pColumn->eColumnType();

    Card_As *p_as = (new Card_As) ->SYM(SF_AS_NM,pColumn->szColNm());
    p_expr->vAppendChild(p_as);

    Card_RpnCalc *p_rpnCalc = new Card_RpnCalc;
    p_expr->vAppendChild(p_rpnCalc);
    p_rpnCalc->pSymbol()->Y_vColType(szCT_char[eColumnType]);
    p_expr->pSymbol()->Y_vColType(szCT_char[eColumnType]);

    Card_Term *p_term = (new Card_Term) -> SYM(SF_COL_TC,cTblSyn, i_Col + 1);
    p_rpnCalc->vAppendChild(p_term);

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    p_term->pSymbol()->Y_vSet(P_Column);
    p_term->pSymbol()->Y_vColType(szCT_char[eColumnType]);
  }

  return p_injected;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Injected::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pString->vCopy("{injected}"_Stz);
  Card_EXPRLIST::vShortForm(pString);
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Injected::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
