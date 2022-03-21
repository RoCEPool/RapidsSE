/*============================================================================*/
/* Card_Expr.cpp -- Typed Expression                                     */
/*============================================================================*/

#include "Card_Expr.hpp"
#include "Card_ExprBool.hpp"

#include "Card_Type.hpp"
#include "Card_As.hpp"
#include "Card_ExprCalc.hpp"
#include "Card_Direction.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Expr,CK_EXPR)
__aNewFunction(Card_Expr,CN_CARD)
__vDeleteFunction(Card_Expr,CN_CARD)

__bIsCard(STORE,Card_Expr,CK_EXPR)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(void)
: Card_SUB(CK_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(Column* pColumn)
: Card_SUB(CK_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  // type
  Card_Type* pCard_Type = new Card_Type(pColumn);
  vAppend(pCard_Type);
  
  // as
  Card_As* pCard_As = new Card_As(pColumn);
  vAppend(pCard_As);

  // exprCalc
  Card_ExprCalc* pCard_ExprCalc = new Card_ExprCalc(pColumn);
  vAppend(pCard_ExprCalc);

  // direction
  //Card_Direction* pCard_Direction = new Card_Direction(pColumn);
  //vAppend(pCard_Direction);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Expr::~Card_Expr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vAsEnsure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_As* pCard_As = _pCard_As(pFindSubCard(CK_AS));
  if (pCard_As)
    return;
  
  s32 sAs;
  sAs.vSprint("COL__%lld"_Stz,m_iCardNo);
  
  pCard_As = new Card_As(sAs);
  vAppend(pCard_As);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_PredCmp* Card_Expr::pFindBound(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_ExprBool* pCard_ExprBool = _pCard_ExprBool(pFindSubCard(CK_EXPRBOOL));
  
  Card_PredCmp* pCard_PredCmp =
    pCard_ExprBool->pFindBound(pIndex,eBound);
 
  return pCard_PredCmp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_ExprBool* pCard_ExprBool = _pCard_ExprBool(pFindSubCard(CK_EXPRBOOL));
  
  bool bHas = pCard_ExprBool->bHasBounds(pIndex);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vReduceBoolExpr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_ExprBool* pCard_ExprBool = _pCard_ExprBool(pFindSubCard(CK_EXPRBOOL));

  Card_PRED* pCard_ChildPRED = (Card_PRED*)pCard_ExprBool->pGet(0);

  pCard_ChildPRED->vCullByKids();
  
  
  if (1 == pCard_ChildPRED->nItems()) {

    Card_PRED* pCard_KidPRED = (Card_PRED*)pCard_ChildPRED->pPop();

    pCard_ExprBool->vReplace(pCard_ChildPRED,pCard_KidPRED);
    delete pCard_ChildPRED;
    
    pCard_ChildPRED = pCard_KidPRED;
  }
  
  
  if (pCard_ChildPRED->nItems())
    return;
  
  pCard_ExprBool->vRemove(pCard_ChildPRED);
  vRemove(pCard_ExprBool);

  delete pCard_ExprBool;
  delete pCard_ChildPRED;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
