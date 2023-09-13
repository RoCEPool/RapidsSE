/*============================================================================*/
/* Card_SubQuery.cpp -- SubQuery                                              */
/*============================================================================*/

#include "Card_SubQuery.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_SubQuery,CK_SUBQUERY)
__aNewFunction(Card_SubQuery,CN_CARD)
__vDeleteFunction(Card_SubQuery,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_SubQuery::Card_SubQuery(void)
: Card_CONSTRUCTION(CK_SUBQUERY, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_SubQuery::~Card_SubQuery(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_SubQuery::vPopulate_subQuery(CardIter* pCI, SubQuery* pSubQuery)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pCI);
  UNUSED(pSubQuery);

  vAppendChild((new Card_SqyNo)       ->SYM(SF_INT_LIT,pCI->G.pBoard->i_Board()));
  vAppendChild((new Card_Caller)      ->SYM(SF_INT_LIT,pSubQuery->i_Caller()));
  vAppendChild((new Card_Clause)      ->SYM(SF_NOTE,   aQC[pSubQuery->eClause()]));
  vAppendChild((new Card_CallOp)      ->SYM(SF_NOTE,   aQO[pSubQuery->eCallOp()]));
  vAppendChild((new Card_RelnOp)      ->SYM(SF_NOTE,   aRO[pSubQuery->eRelnOp()]));
  vAppendChild((new Card_CorrCols)    ->SYM(SF_INT_LIT,pSubQuery->nCorrCols()));
  vAppendChild((new Card_Disposition) ->SYM(SF_NOTE,   aQO[pSubQuery->eDisposition()]));
  vAppendChild((new Card_SqyCalls)    ->SYM(SF_INT_LIT,pSubQuery->nSubQueries()));
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_SubQuery::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
