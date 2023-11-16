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
/* new Card_Injectable based on streamDef of table                            */
/*----------------------------------------------------------------------------*/
Card_Injectable* Card_Injectable::pNew_injectable(CardIter* pCI)
{
  _static_;

  Table* pTable = pCI->G.p_table->pGet_Table();
  StreamDef* pStreamDef = pTable->pStreamDef();

  char cTblSyn = pCI->G.p_table->pSymbol()->cSF_char();

  Card_Injectable* p_injectable = new Card_Injectable;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr_Column =
        Card_Expr::pCast_expr_Column(pColumn, i_Col + 1, cTblSyn, i_Col + 1);

    p_injectable->vAppendChild(p_expr_Column);
  }

  return p_injectable;
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
