/*============================================================================*/
/* Card_Expr.cpp -- Typed Expression                                     */
/*============================================================================*/

#include "Card_Expr.hpp"
#include "Card_ExprBool.hpp"

#include "Card_Type.hpp"
#include "Card_As.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"
#include "Card_Direction.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Expr,CK_EXPR)
__aNewFunction(Card_Expr,CN_CARD)
__vDeleteFunction(Card_Expr,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(void)
: Card(CK_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(Column* pColumn)
: Card(CK_EXPR)
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
  Card_RpnCalc* pCard_RpnCalc = new Card_RpnCalc(pColumn);
  vAppend(pCard_RpnCalc);

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
  sAs.vSprint("COL_%lld"_Stz,m_iCardNo);
  
  pCard_As = new Card_As(sAs);
  vAppend(pCard_As);
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
