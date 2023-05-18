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
/* individuate filter                                                         */
/*----------------------------------------------------------------------------*/
void Card_IndexScan::vIndividuateFilter(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMainWithSub(CK_INDEXSCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_IndexScan::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bPeg_Chosen_index(pCI));

  // snap onto <indexScan>/<projection>, delete existing <indexScan>/<projection>/<key>
  pCI->G.p_projection = pSub_projection();
  pCI->G.p_projection->vDeletePrjKey(pCI);

  // derive new <indexScan>/<projection>/<key>
  pCI->G.p_key = new Card_Key;
  pCI->G.p_projection->vAppend(pCI->G.p_key);
  pCI->G.p_index->vAdd_key_exprs(pCI);

  // set running projection for use by next op
  pCI->StreamOrder.m_pProjection_running = pCI->G.p_projection;
  pCI->StreamOrder.m_pKey_running = pCI->G.p_key;
}

/*----------------------------------------------------------------------------*/
/* add an early project after this index scan                                 */
/*----------------------------------------------------------------------------*/
void Card_IndexScan::vEarlyProject_leaf(CardIter* pCI)
{
  vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->Early.m_pOP_leaf = this;

  pCI->Analysis.m_pProjection_prior = pSub_projection();

  pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexScan::vValidate(i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
