/*============================================================================*/
/* Card_Injectable.cpp -- Injectable Columns                                  */
/*============================================================================*/

#include "Card_Injectable.hpp"
#include "CardIter.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Injectable, CK_p_INJECTABLE)
__aNewFunction(Card_Injectable, CN_CARD)
__vDeleteFunction(Card_Injectable, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Injectable::Card_Injectable(void)
: Card_EXPRLIST(CK_p_INJECTABLE, 2)
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

  Table* pTable = pCI->E.p_table->pGet_Table();
  StreamDef* pStreamDef = pTable->pStreamDef();

  char cTblSyn = pCI->E.p_table->pSymbol()->cTblSyn();

  Card_Injectable* p_injectable = new Card_Injectable;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr_Column =
        Card_Expr::pCraft_expr_of_nmdCol(pColumn, i_Col + 1, cTblSyn, i_Col + 1);

    p_injectable->vAppendChild(p_expr_Column);
  }

  return p_injectable;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Injectable::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  (void)Card::eValidate_parent_either(pCI,
                                     CK_o_CROSSJOIN,
                                     CK_o_FROM,
                                     CK_o_INFLATE,
                                     CK_o_INDEXJOIN,
                                     CK_o_INDEXSCAN,
                                     CK_o_KEYEXISTS,
                                     CK_o_KEYNOTEXISTS,
                                     CK_o_SCAN,
                                     CK_NONE);
  vValidate_child_mustBe(CK_EXPR,CK_NONE);

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
