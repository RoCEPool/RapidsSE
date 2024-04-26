/*============================================================================*/
/* Card_Neededv2.cpp -- Neededv2                                                  */
/*============================================================================*/

#include "Card_Neededv2.hpp"

#include "Card_EXPRLIST.hpp"
#include "Card_Project.hpp"
#include "Card_Level.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Neededv2, CK_c_NEEDEDv2)
__aNewFunction(Card_Neededv2, CN_CARD)
__vDeleteFunction(Card_Neededv2, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Neededv2::Card_Neededv2(void)
: Card_EXPRLIST(CK_c_NEEDEDv2, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Neededv2::~Card_Neededv2(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Neededv2* Card_Neededv2::pClone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Neededv2(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* test if given expr is a needed expr                                        */
/*----------------------------------------------------------------------------*/
bool Card_Neededv2::bNeededv2_expr(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid x_needed(this); x_needed.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_needed = x_needed.p_expr();

    //p_expr_needed->vDump(__func__);
    if (p_expr_needed->bMatches(p_expr_arg))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Project* Card_Neededv2::pNew_project_early(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Project* p_project_early = new Card_Project(CK_o_PROJECT);
  p_project_early->pSymbol()->Y_vSet(P_Early);

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  p_project_early->vAppendChild(p_level);

  p_project_early->vAppendChild(pCI->Early.m_pNeeded_later->pClone());

  return p_project_early;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Neededv2::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
//  if (! pCI->G.pBoard->pState()->bHasName(aXF[XF_EPILOG_X_TBLCOL_TO_EXPCOL])) {
//    Card::vValidate(pCI,iDepth,pParent);
//    pCI->vValidateChildren(this, iDepth + 1);
//  }
//  else {
//    vMark1Recursive(MARK_BLANK);
//  }

  if (pCI->E.pBoard->pState()->bHasName(aXF[XF_EPILOG_X_TBLCOL_TO_EXPCOL])) {
    Card::vValidate(pCI,iDepth,pParent);
    pCI->vValidateChildren(this, iDepth + 1);
  }
  else {
    vMark1Recursive(MARK_BLANK);
  }

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
