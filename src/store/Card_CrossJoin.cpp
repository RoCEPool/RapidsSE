/*============================================================================*/
/* Card_CrossJoin.cpp -- Cross Join                                           */
/*============================================================================*/

#include "Card_CrossJoin.hpp"
#include "CardIter.hpp"
#include "Board.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CrossJoin,CK_CROSSJOIN)
__aNewFunction(Card_CrossJoin,CN_CARD)
__vDeleteFunction(Card_CrossJoin,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CrossJoin::Card_CrossJoin(void)
: Card_OP(CK_CROSSJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CrossJoin::~Card_CrossJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* table morphs to crossJoin                                                  */
/*----------------------------------------------------------------------------*/
void Card_CrossJoin::vMorph_table_To_crossJoin(STORE::CardIter *pCI)
{
  _static_
    //Board::vChoose_index_For_crossJoin(pCI);

  pCI->G.p_indexList = pCI->G.p_table->pSub_indexList();

//  pCI->G_vNewMember(CK_CROSSJOIN);
//  pCI->G.p_table->vRelegate_table(pCI, pCI->G.p_crossJoin);


  pCI->G.p_crossJoin = _pCard_CrossJoin(pCI->G.p_from->pCard(CK_CROSSJOIN));
  pCI->G.p_index = pCI->G.p_crossJoin->pSub_index();

  pCI->G_vNewMember(CK_BOUNDS,Symbol(SF_CROSS,"cross"_Stz));
  pCI->G.p_index->vAppend(pCI->G.p_bounds);

  pCI->G_vNewMember(CK_BOUND);
  pCI->G.p_bounds->vAppend(pCI->G.p_bound);

  pCI->G_vNewMember(CK_TERM,Symbol(SF_IDX_COL,1));
  pCI->G.p_bound->vAppend(pCI->G.p_term);

  pCI->G.p_indexList->vChoose_index_For_crossJoin();
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_CrossJoin::vEarlyProject_recurse_update(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  pCI->vPrintBoard();

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...


  pCI->vPrintBoard();

  vUpdate_projection(pCI);

  vCorrectStmOdn(pCI->Prior.m_pProjection);

  pCI->vPrintBoard();

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;

  pCI->Prior.m_pProjection = pSub_projection();

  pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CrossJoin::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_LEVEL,CK_TABLE,CK_NONE);

  RASSERT(pSub_table()->pSub_indexList());

  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
