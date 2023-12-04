/*============================================================================*/
/* Card_Needed.cpp -- Needed                                                  */
/*============================================================================*/

#include "Card_Needed.hpp"

#include "Card_EXPRLIST.hpp"
#include "Card_Project.hpp"
#include "Card_Level.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Needed, CK_s_NEEDED)
__aNewFunction(Card_Needed, CN_CARD)
__vDeleteFunction(Card_Needed, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Needed::Card_Needed(void)
: Card_EXPRLIST(CK_s_NEEDED, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Needed::~Card_Needed(void) noexcept(false)
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
Card_Needed* Card_Needed::pClone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Needed(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Project* Card_Needed::pNew_project_early(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Project* p_project_early = new Card_Project(CK_m_PROJECT);
  p_project_early->pSymbol()->Y_vSet(P_Early);

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  p_project_early->vAppendChild(p_level);

  p_project_early->vAppendChild(pCI->Early.m_pNeeded_later->pClone());

  return p_project_early;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Needed::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  if (! pCI->G.pBoard->pState()->bHasName(aXF[XF_EPILOG_X_TBLCOL_TO_STMCOL])) {
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
