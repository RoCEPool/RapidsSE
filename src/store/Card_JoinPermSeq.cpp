/*============================================================================*/
/* Card_JoinPermSeq.cpp -- Join Permutation Sequence                          */
/*============================================================================*/

#include "Card_JoinPermSeq.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CmpPred.hpp"
#include "Card_JoinPermAll.hpp"
#include "Card_JoinPredList.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_JoinPermSeq, CK_JOINPERMSEQ)
__aNewFunction(Card_JoinPermSeq, CN_CARD)
__vDeleteFunction(Card_JoinPermSeq, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPermSeq::Card_JoinPermSeq(void)
: Card(CK_JOINPERMSEQ)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPermSeq::~Card_JoinPermSeq(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute the load of this join permutation                                   */
/*----------------------------------------------------------------------------*/
void Card_JoinPermSeq::vCalculateLoad(Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSumLoadNetwork = 0;
  i8 nSumLoadUnicast = 0;

  i8 nBuffersIn = 0;

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {

    if (xCI.G.p_MainCard->bIsCard(CK_JOINHEAD)) {
      char cTblSyn_l = xCI.G.p_joinHead->cValue();
      nBuffersIn = p_tableList->pGetTableSize(cTblSyn_l)->iValue();
      continue;
    }

    if (! xCI.G.p_MainCard->bIsCard(CK_JOINNEXT))
      continue;

    char cTblSyn_r = xCI.G.p_joinNext->cValue();

    eXK eCast = eCastKind(p_tableList, xCI.G.p_joinNext);

    nBuffersIn = xCI.G.p_joinNext->nCalculateLoad(eCast, nBuffersIn, p_tableList, cTblSyn_r);

    Card_Load* p_load_next = xCI.G.p_joinNext->pSub_load();

    Card_LoadNetwork* p_loadNetwork = p_load_next->pSub_loadNetwork();
    nSumLoadNetwork += p_loadNetwork->iValue();
    Card_LoadUnicast* p_loadUnicast = p_load_next->pSub_loadUnicast();
    nSumLoadUnicast += p_loadUnicast->iValue();
  }

  Card_LoadPermSeq* p_loadPermSeq = new Card_LoadPermSeq;

  Card_LoadNetwork* p_loadNetwork = new Card_LoadNetwork;
  p_loadNetwork->vValue(nSumLoadNetwork);
  Card_LoadUnicast* p_loadUnicast = new Card_LoadUnicast;
  p_loadUnicast->vValue(nSumLoadUnicast);

  p_loadPermSeq->vAppend(p_loadNetwork);
  p_loadPermSeq->vAppend(p_loadUnicast);

  vInsert(p_loadPermSeq);
}

/*----------------------------------------------------------------------------*/
/* compute the cast of given join                                             */
/*----------------------------------------------------------------------------*/
eXK Card_JoinPermSeq::eCastKind(Card_TableList* p_tableList,
                                Card_JoinNext*  p_joinNext)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CmpPred* p_cmpPred = p_joinNext->pSub_cmpPred();
  Card_Term* p_term_0 = p_cmpPred->pGetTerm_0();
  Card_Term* p_term_1 = p_cmpPred->pGetTerm_1();

  char cTblSyn_r = p_joinNext->cValue();
  char cTblSyn_l = p_cmpPred->cOtherTable(cTblSyn_r);

  Card_TablePtn* p_tablePtn_l = p_tableList->pGetTablePtn(cTblSyn_l);
  Card_TablePtn* p_tablePtn_r = p_tableList->pGetTablePtn(cTblSyn_r);

  // l_ptn == t:$x.1 && r_ptn == t:$y.1
//  if ( (p_tablePtn_l->bSameValue(p_term_0) && p_tablePtn_r->bSameValue(p_term_1)) ||
//       (p_tablePtn_l->bSameValue(p_term_1) && p_tablePtn_r->bSameValue(p_term_0)) )

  // t:$x.1 == l_ptn && t:$y.1 == r_ptn
  if ( ( (p_term_0->bSameValue(p_tablePtn_l) && p_term_1->bSameValue(p_tablePtn_r)) ||
         (p_term_0->bSameValue(p_tablePtn_r) && p_term_1->bSameValue(p_tablePtn_l)) )
      && bUnicastSeqFollows(cTblSyn_l))
    return XK_UNICAST;

  // r_ptn == t:$x.1
//  if (p_tablePtn_r->bSameValue(p_term_0) || p_tablePtn_r->bSameValue(p_term_1))

  // t:$x.1 == r_ptn
  if (p_term_0->bSameValue(p_tablePtn_r) || p_term_1->bSameValue(p_tablePtn_r))
    return XK_HASHCAST;

  return XK_BROADCAST;
}

/*----------------------------------------------------------------------------*/
/* compute one permuted join sequence                                         */
/*----------------------------------------------------------------------------*/
i8 Card_JoinPermSeq::iOnePermutedJoin(Card_TableList*    p_tableList,
                                        Card_JoinPredList* p_joinPredList,
                                        PermGen*           pPermGen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iNotValid = C_UNKNOWN;

  i8 nTables = p_tableList->nChildCards();

  //vTrace("%lld :"_Stz,pPermGen->nPermutations());
  for (i8 i_Item = 0; i_Item < nTables; i_Item++) {

    // get the ith table of this permutation
    i8 i_Table = pPermGen->iElement(i_Item);
    Card_Table* p_table = _pCard_Table(p_tableList->pGetChild(i_Table));
    char cTblSyn_ith = p_table->cValue();

    if (i_Item)
      vTrace(", "_Stz,i_Table);
    vTrace("%c"_Stz, cTblSyn_ith);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (0 == nChildCards()) {
      //+ table is head/first of join sequence
      Card_JoinHead *p_joinHead = new Card_JoinHead;
      p_joinHead->vValue(cTblSyn_ith);
      vAppend(p_joinHead);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // table is next/subsequent of join sequence

    Card_JoinNext *p_joinNext = new Card_JoinNext;
    p_joinNext->vValue(cTblSyn_ith);
    vAppend(p_joinNext);


    bool bSupported = false;

    // test each join predicate which uses the ith table to see if a join
    // can be supported at this point in the sequence
    CardIter xCI(p_joinPredList);
    while (xCI.G_bEachMain(CK_CMPPRED)) {
      if (! xCI.G.p_cmpPred->bUsesTable(cTblSyn_ith))
        continue;

      // have a join cmpPred which uses the ith table

      // get the other table in the cmpPred
      char cTblSyn_other = xCI.G.p_cmpPred->cOtherTable(cTblSyn_ith);


      if (bHasKnownJoin(cTblSyn_other)) {
        bSupported = true;
        p_joinNext->vAppend(xCI.G.p_cmpPred->pDupCardAndSubCards());
        break;
      }
    }

    if (bSupported)
      //+ join of ith table is possible at this point
      continue;

    // join of ith table is not possible at this point
    p_joinNext->vAppend(new Card_Note("**DOESN'T FOLLOW**"_Stz));
    Y_vSet(P_Invalid);
    break;
  }

  vTrace("\n"_Stz);

  return iNotValid;
}

/*----------------------------------------------------------------------------*/
/* does this join sequence already have a join with given table at this point?*/
/*----------------------------------------------------------------------------*/
bool Card_JoinPermSeq::bHasKnownJoin(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_JOINHEAD) || xCI.G.p_MainCard->bIsCard(CK_JOINNEXT))
    if (xCI.G.p_MainCard->bIsValue(cTblSyn))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is load of this permutation sequence less than given arg?                  */
/*----------------------------------------------------------------------------*/
bool Card_JoinPermSeq::bIsLessLoad(Card_JoinPermSeq* p_joinPermSeq_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_LoadPermSeq* p_loadPermSeq_this = pSub_loadPermSeq();
  Card_LoadPermSeq* p_loadPermSeq_arg  = p_joinPermSeq_arg->pSub_loadPermSeq();

  if (p_loadPermSeq_this->bIsLessLoad(p_loadPermSeq_arg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* does a possible unicast sequence follow after given table?                 */
/*----------------------------------------------------------------------------*/
bool Card_JoinPermSeq::bUnicastSeqFollows(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_JOINHEAD) || xCI.G.p_MainCard->bIsCard(CK_JOINNEXT))

    if (xCI.G.p_MainCard->cValue() == cTblSyn)
      break;

    if (xCI.G.p_MainCard->bIsCard(CK_JOINNEXT)) {

      xCI.G.p_cast = xCI.G.p_joinNext->pSub_cast();
      if (xCI.G.p_cast && ! xCI.G.p_cast->bSame("XK_UNICAST"_Stz))
        return false;
    }
  }

  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_JOINHEAD) || xCI.G.p_MainCard->bIsCard(CK_JOINNEXT))

    if (xCI.G.p_MainCard->bIsCard(CK_JOINNEXT)) {
      xCI.G.p_cast = xCI.G.p_joinNext->pSub_cast();
      if (xCI.G.p_cast && ! xCI.G.p_cast->bSame("XK_UNICAST"_Stz))
        return false;
    }
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
