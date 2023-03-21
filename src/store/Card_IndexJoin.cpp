/*============================================================================*/
/* Card_IndexJoin.cpp -- Index Join                                           */
/*============================================================================*/

#include "Card_IndexJoin.hpp"
#include "CardIter.hpp"
#include "Board.hpp"
#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexJoin,CK_INDEXJOIN)
__aNewFunction(Card_IndexJoin,CN_CARD)
__vDeleteFunction(Card_IndexJoin,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::Card_IndexJoin(void)
: Card_OP(CK_INDEXJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::~Card_IndexJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // extends the projection key

  RASSERT(bPeg_Chosen_index(pCI));

  // snap onto <indexJoin>/<projection>, delete existing <indexJoin>/<projection>/<key>
  pCI->G.p_projection = pSub_projection();
  pCI->G.p_projection->vDelete_key(pCI);

  if (pCI->StreamOrder.m_pKey_running) {
    pCI->G.p_key = _pCard_Key(pCI->StreamOrder.m_pKey_running->pDupCardAndSubCards());
    pCI->G.p_projection->vAppend(pCI->G.p_key);
  }
  else {
    pCI->G.p_key = new Card_Key;
    pCI->G.p_projection->vAppend(pCI->G.p_key);
  }
  pCI->G.p_index->vAdd_key_exprs(pCI);

  // set running projection for use by next op
  pCI->StreamOrder.m_pProjection_running = pCI->G.p_projection;
  pCI->StreamOrder.m_pKey_running = pCI->G.p_key;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vEarly_project_recurse(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->vSave();
  Card_OP::vEarly_project_test(pCI);
  pCI->vRestore();

  // arrived back ...

  pCI->G.p_filter = pSub_filter();
  pCI->G.p_filter->vTermsBecome(pCI->EarlyProject.m_pProjection_reduced);

  pCI->G.p_indexList = pSub_indexList();
  pCI->G.p_indexList->vTermsBecome(pCI->EarlyProject.m_pProjection_reduced);


  pCI->G.p_projection = pSub_projection();
  Card_Project* p_project = Card_Project::pNew_project_early(pCI->G.p_projection,
                                                             pCI->EarlyProject.m_pProjection_reduced);
  pCI->G_vInsertAfter(p_project,this);


  pCI->ProjectionAnalysis.m_pProjection_running = p_project->pSub_projection();


  vProjectionAnalysis(pCI);

  pCI->vPrintBoard();

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/* individuate filter                                                         */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vIndividuateFilter(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    Board::vIndividuate_indexJoin_filter_b(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* table morphs to indexJoin                                                  */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vTableMorphsToIndexJoin(STORE::CardIter *pCI)
{
  _static_

  if (! pCI->G.p_bounds) {
    s256 sErrorMsg;
    sErrorMsg.vSprint("No index for table(%c) %s"_Stz,
                      pCI->G.p_table->cValue(),
                      pCI->G.p_table->pSub_tableNm()->szValue());
    Error xError(EC_MISSING_JOIN_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

  pCI->G.p_indexList->vChoose_index_For_indexJoin();

  pCI->G_vNewMember(CK_INDEXJOIN);

  pCI->G.p_indexJoin->vTransferChildren(pCI->G.p_table);

  pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table, pCI->G.p_indexJoin);
  pCI->G.p_indexJoin->vInsert(pCI->G.p_table);

  //pCI->G.p_indexJoin->vRemoveSub(pCI->G.p_index);
  //pCI->G.p_indexJoin->vAppend(pCI->G.p_index);

  pCI->G.p_MainCard = pCI->G.p_indexJoin;

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
