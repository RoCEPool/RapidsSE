/*============================================================================*/
/* Card_IndexScan.cpp -- Index Scan                                           */
/*============================================================================*/

#include "Card_IndexScan.hpp"
#include "CardIter.hpp"
#include "Card_Filter.hpp"
#include "Card_Project.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexScan,CK_INDEXSCAN)
__aNewFunction(Card_IndexScan,CN_CARD)
__vDeleteFunction(Card_IndexScan,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexScan::Card_IndexScan(void)
: Card_OP(CK_INDEXSCAN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexScan::~Card_IndexScan(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_IndexScan::vDerive_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bPeg_Chosen_index(pCI));

  // snap onto <indexScan>/<projection>, delete existing <indexScan>/<projection>/<key>
  pCI->G.p_projection = pSub_projection();
  pCI->G.p_projection->vDelete_streamKey(pCI);

  // derive new <indexScan>/<projection>/<key>
  pCI->G.p_key = new Card_Key;
  pCI->G.p_projection->vAppend(pCI->G.p_key);
  pCI->G.p_index->vAdd_key_exprs(pCI);


  pCI->StreamOrder.m_pStreamKey_running = Card_StreamKey::pTo_streamKey(pCI->G.p_key);
  pCI->G.p_projection->vAppend(pCI->StreamOrder.m_pStreamKey_running);


  // set running projection for use by next op
  pCI->StreamOrder.m_pProjection_running = pCI->G.p_projection;
  pCI->StreamOrder.m_pKey_running = pCI->G.p_key;
}

/*----------------------------------------------------------------------------*/
/* add an early project after this index scan                                 */
/*----------------------------------------------------------------------------*/
void Card_IndexScan::vEarlyProject_leaf(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->Early.m_pOP_leaf = this;

  pCI->Prior.m_pProjection = pSub_projection();

  pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexScan::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_LEVEL,CK_TABLE,CK_INDEXLIST,CK_NONE);
  RASSERT(pFindPpyRecursive(P_ChosenIndex,CK_INDEX));

  UNUSED(pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
