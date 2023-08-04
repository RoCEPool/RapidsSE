/*============================================================================*/
/* Card_Hash.cpp -- Hash                                                      */
/*============================================================================*/

#include "Card_Hash.hpp"

#include "Card_HshCol.hpp"
#include "Card_Expr.hpp"
#include "Card_Term.hpp"
#include "Card_Projection.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Hash,CK_HASH)
__aNewFunction(Card_Hash,CN_CARD)
__vDeleteFunction(Card_Hash,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Hash::Card_Hash(void)
: Card_OP(CK_HASH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Hash::~Card_Hash(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Hash* Card_Hash::pDerive(Card_Expr* p_expr,
                              Card_Projection* p_projection_prev)
{
  _static_

  Card_Hash* p_hash = new Card_Hash;

  Card_HshCol* p_hshCol = new Card_HshCol;
  p_hshCol->pSymbol()->vUpdate(p_expr->pGetTerm(0)->pSymbol());

  RASSERT(p_expr->szP_Type());
  p_hshCol->pSymbol()->Y_vSet(P_Type,p_expr->szP_Type());

  p_hshCol->vAppend(p_expr->pGetTerm(0)->pClone());

  p_hshCol->pSymbol()->Y_vSet(P_BreakPt);

  p_hash->vAppend(p_hshCol);

//  if (p_projection_prev)
//    p_hash->vAppend(p_projection_prev->pClone());

  return p_hash;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Hash::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
