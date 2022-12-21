/*============================================================================*/
/* Card_JoinOrderSeq.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinOrderSeq.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "Card_PtndJoin.hpp"
#include "Card_JoinOrder.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinOrderSeq,CK_JOINORDERSEQ)
__aNewFunction(Card_JoinOrderSeq,CN_CARD)
__vDeleteFunction(Card_JoinOrderSeq,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinOrderSeq::Card_JoinOrderSeq(void)
: Card(CK_JOINORDERSEQ)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinOrderSeq::~Card_JoinOrderSeq(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinOrderSeq::vConcat(Card_PtndJoin* p_PtndJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eChild;

  for (i8 i_Child = 0; i_Child < p_PtndJoin->nChildCards(); i_Child++) {
    Card_CmpPred* p_predCmp = _pCard_CmpPred(p_PtndJoin->pGetChild(i_Child));

    for (i8 i_Kid = p_predCmp->nChildCards() - 1; i_Kid >= 0; i_Kid--) {
      Card *pChild = p_predCmp->pGetChild(i_Kid, eChild);
      if (CK_TERM != eChild)
        continue;

      Card_Term* p_term = _pCard_Term(pChild);
      s32 sTableSyn;
      //E_vColumnRef_TableSyn_x(p_columnRef, sTableSyn);
      sTableSyn.vSprint("%c"_Stz, p_term->sz()->azChar()[1]);

      if (bHasChildWithValue(sTableSyn))
        continue;

      Card_JoinOrder* p_joinOrder = new Card_JoinOrder(sTableSyn);
      vAppend(p_joinOrder);
    }
  }
}


//*===========================================================================*/
/* end                                                                        */
/*============================================================================*/
