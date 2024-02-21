/*============================================================================*/
/* Card_Filter.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Filter.hpp"

#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolPred.hpp"
#include "Card_RlnConj.hpp"
#include "Card_Term.hpp"
#include "Card_SubSet.hpp"
#include "Card_AndConj.hpp"
#include "Card_OrConj.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Filter, CK_o_FILTER)
__aNewFunction(Card_Filter,CN_CARD)
__vDeleteFunction(Card_Filter,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Filter::Card_Filter(void)
: Card_CONDITION(CK_o_FILTER, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Filter::~Card_Filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete empty filters                                                       */
/*----------------------------------------------------------------------------*/
void Card_Filter::vDeleteEmptyFilters(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMain_NONE()) {
    if ((pCI->G.p_filter = pCI->G.p_MainCard->pSub_filter()))
      pCI->G.p_OP->vCullIfEmpty_filter(pCI->G.p_filter);
  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vIndividuate_filter(CardIter* pCI)
{
  _static_

  if (!pCI->G.p_MainCard->pSub_emit())
    pCI->G.p_MainCard->vDump(__func__);

  RASSERT(pCI->G.p_MainCard->pSub_emit());

  pCI->G.p_MainCard->vRemoveChild(pCI->G.p_filter);


  pCI->G.p_filter->vEnsure_stream(pCI);

  pCI->G.p_stream->vAppendChild(pCI->G.p_MainCard->pSub_emit()->pClone());

  pCI->G.pDeck->vInsertChildAfter(pCI->G.p_filter, pCI->G.p_MainCard);

  if (! pCI->G.p_filter->pSub_level()) {
    pCI->G.p_filter->vPushChild(pCI->G.p_MainCard->pSub_level()->pDupCardRecursive());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Filter* Card_Filter::pNew(Card_Constraint* p_constraint)
{
  _static_

  Card_Filter* p_filter = new Card_Filter;

  p_filter->vAppendChild(p_constraint);
  return p_filter;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vCorrect_expCol(Card_Emit* p_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vCorrect_expCol(p_arriving);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Filter::nCullMarked_rlnConj(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolPred* p_boolPred = pSub_boolPred();
  i8 nCulled = p_boolPred->nCullMarked_rlnConj(szWithMark);

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vEarlyProject_add_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vAddTransients(pCI);
}


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Filter::vEarlyProject_recurse_update(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

 // pCI->vPrintBoard(__func__,__LINE__);

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  // check for additional/temporary transients
  vEarlyProject_add_transients(pCI);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

 // pCI->vPrintBoard(__func__,__LINE__);

  pCI->G.p_OP->vRecreate_emit(pCI);

  //pCI->vPrintBoard(__func__,__LINE__);

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vExtend_rlnConj(Card_RlnConj *p_rlnConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_BoolPred* p_calcBool = this->pSub_boolPred();

  p_calcBool->vExtend_andConj(p_rlnConj);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolPred* pCard_calcBool =
      _pCard_BoolPred(this->pFindSubCard(CK_BOOLPRED));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(iDepth);

  s32 sName;

  sName.vSprint("%s%lld]filter"_Stz,
                Stz::szDots(iDepth - 1),
                m_i_Child);

  pRibbon->vSprintSnippet(false,"%c%c%s %llx %lld"_Stz,
                          cMark1(),
                          cMark2(),
                          sName.szStz(),
                          ((char *) this) - (char *) AM::SYSARGS::aRegionAddr(),
                          m_nFunctionCount);

}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_BOOLPRED, CK_NONE);

  if (iDepth > 2) {
   // pCI->vPrintBoard(__func__,__LINE__);
    vValidationFailed("Filter is at depth %lld"_Stz, iDepth);
  }

  if (iDepth > 1)
    (void)Card::eValidate_parentEither(pCI, CK_o_CROSSJOIN, CK_o_FROM, CK_o_HAVING,
                                       CK_o_INDEXJOIN, CK_o_INDEXSCAN, CK_o_SCAN, CK_o_WHERE,
                                       CK_NONE);

  if (CK_DECK == pParent->eCard()) {
    vValidate_mustHave(CK_s_LEVEL, CK_s_STREAM, CK_NONE);
  }

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
