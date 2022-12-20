/*============================================================================*/
/* Card_CoPtnPredList.cpp -- Co-Partition Predicate List                      */
/*============================================================================*/

#include "Card_CoPtnPredList.hpp"

#include "Card_CmpPred.hpp"
#include "Card_CoPtnPred.hpp"
#include "Card_JoinPredList.hpp"
#include "Card_TableList.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CoPtnPredList,CK_COPTNPREDLIST)
__aNewFunction(Card_CoPtnPredList,CN_CARD)
__vDeleteFunction(Card_CoPtnPredList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CoPtnPredList::Card_CoPtnPredList(void)
: Card(CK_COPTNPREDLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CoPtnPredList::~Card_CoPtnPredList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtnPredList* Card_CoPtnPredList::pExtract(Card_JoinPredList* p_joinPredList,
                                                 Card_TableList* p_tableList)
{
  _static_

  Card_CoPtnPredList* p_coPtnPredList = new Card_CoPtnPredList;

  i8 nJoinPreds = p_joinPredList->nChildCards();
  for (i8 i_joinPred = 0; i_joinPred < nJoinPreds; i_joinPred++) {
    Card_CmpPred* p_cmpPred =
        _pCard_CmpPred(p_joinPredList->pGetChild(i_joinPred));

    // todo: fix this...
    //if (!T_bTableList_PredCmpIsCoPtn(G.p_tableList, G.p_cmpPred))
    //  continue;

    Card_CoPtnPred* p_coPtnPred = new Card_CoPtnPred;
    p_coPtnPred->vAppend(p_cmpPred->pDupCardAndSubCards());

    p_coPtnPredList->vAppend(p_coPtnPred);
  }

  return p_coPtnPredList;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
