/*============================================================================*/
/* Card_CrossJoin.cpp -- Cross Join                                           */
/*============================================================================*/

#include "Card_CrossJoin.hpp"
#include "CardIter.hpp"
#include "Board.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CrossJoin, CK_o_CROSSJOIN)
__aNewFunction(Card_CrossJoin,CN_CARD)
__vDeleteFunction(Card_CrossJoin,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CrossJoin::Card_CrossJoin(void)
: Card_OP(CK_o_CROSSJOIN, 1)
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

  pCI->E.p_indexList = pCI->E.p_table->pSub_indexList();

//  pCI->vNewMember(CK_o_CROSSJOIN);
//  pCI->E.p_table->vRelegate_table(pCI, pCI->E.p_crossJoin);


  pCI->E.p_crossJoin = _pCard_CrossJoin(pCI->E.p_from->pCard(CK_o_CROSSJOIN));
  pCI->E.p_index = pCI->E.p_crossJoin->pSub_index();

  pCI->vNewMember(CK_BOUNDS,Symbol(SF_CROSS,"cross"_Stz));
  pCI->E.p_index->vAppendChild(pCI->E.p_bounds);

  pCI->vNewMember(CK_BOUND);
  pCI->E.p_bounds->vAppendChild(pCI->E.p_bound);

  pCI->vNewMember(CK_TERM,Symbol(SF_IDX_COL,1));
  pCI->E.p_bound->vAppendChild(pCI->E.p_term);

  pCI->E.p_indexList->vChoose_index_For_crossJoin();
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_CrossJoin::vEarlyProject_recurse_update(CardIter* pCI)
{
   _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

 // pCI->PRINTBOARD_dbg();

  Card_OP::vEarlyProject_backstep(pCI);

  pCI->E.p_Op = this;
  pCI->E.e_Op = CK_o_CROSSJOIN;

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...


 // pCI->PRINTBOARD_dbg();

  ((Card_OP*)pCI->E.p_Op)->vRestate_emit(pCI);

  // pCI->PRINTBOARD_dbg();

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CrossJoin::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_s_LEVEL, CK_s_TABLE, CK_NONE);

  RASSERT(pSub_table()->pSub_indexList());

  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
