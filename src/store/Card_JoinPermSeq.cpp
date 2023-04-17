/*============================================================================*/
/* Card_JoinPermSeq.cpp -- Join Permutation Sequence                          */
/*============================================================================*/

#include "Card_JoinPermSeq.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CmpPred.hpp"
#include "Card_FiltPred.hpp"
#include "Card_FiltPredList.hpp"
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
/* estimate the query cost of this join permutation sequence                  */
/*----------------------------------------------------------------------------*/
void Card_JoinPermSeq::vEstQueryCost(Card_TableList* p_tableList,
                                     Card_FiltPredList* p_filtPredList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_JoinPermSeq = pSymbol()->Y_iGet(P_Permutation);
  if (686 == i_JoinPermSeq)
    vDebugPoint();

  i8 nBuff_prev_locus  = 0;
  i8 nBuff_producedBy_locus = 0;

  i8 nSumNetworkCost = 0;
  i8 nSumUnicastCost = 0;

  i8 i_Locus = 0;

  i8 nFilterPred_prev = 0;

  Symbol* p_Y_streamPtn_running = nullptr;

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {

    nBuff_prev_locus = nBuff_producedBy_locus;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream start

    // calculate the buffers produced at the start of join sequence
    if (xCI.G.p_MainCard->bIsCard(CK_STREAMHEAD)) {

      // get info of stream start
      char cTblSyn_head = xCI.G.p_dataStart->cValue();

      nBuff_producedBy_locus = p_tableList->nGetTableSize(cTblSyn_head) * C_EXPANSION_COEFF;

      nFilterPred_prev = p_filtPredList->nFiltersFor(cTblSyn_head);

      p_Y_streamPtn_running = p_tableList->pGetTablePtn(cTblSyn_head)->pSymbol();

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // join locus

    if (! xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))
      continue;

    // get info for locus of join sequence
    char cTblSyn_locus = xCI.G.p_joinLocus->cValue();

    eXK eLocusCast = this->eLocusCast(p_Y_streamPtn_running,
                                      p_tableList,
                                      xCI.G.p_joinLocus);

    // calculate the various costs for locus of join sequence
    i8 nBuff_joining_with = p_tableList->nGetTableSize(cTblSyn_locus);

    nBuff_producedBy_locus =
        xCI.G.p_joinLocus->nEstLocusCosts(i_JoinPermSeq,
                                          i_Locus,
                                          nBuff_prev_locus,
                                          nFilterPred_prev,
                                          eLocusCast,
                                          nBuff_joining_with
                                          );

    // running sum of the network and unicast costs
    Card_JoinEstimate* p_costLocus = xCI.G.p_joinLocus->pSub_joinCost();
    Card_CostNetwork* p_costNetwork = p_costLocus->pSub_costNetwork();
    nSumNetworkCost += p_costNetwork->iValue();
    Card_CostUnicast* p_costUnicast = p_costLocus->pSub_costUnicast();
    nSumUnicastCost += p_costUnicast->iValue();

    nFilterPred_prev = p_filtPredList->nFiltersFor(cTblSyn_locus);

    i_Locus++;

    if (eLocusCast != XK_UNICAST)
      p_Y_streamPtn_running = p_tableList->pGetTablePtn(cTblSyn_locus)->pSymbol();
  }

  // record the sum network and unicast costs
  Card_CostNetwork* p_costNetwork = new Card_CostNetwork;
  p_costNetwork->vValue(nSumNetworkCost);

  Card_CostUnicast* p_costUnicast = new Card_CostUnicast;
  p_costUnicast->vValue(nSumUnicastCost);

  Card_CostQuery* p_costQuery = new Card_CostQuery;
  p_costQuery->vAppend(p_costNetwork);
  p_costQuery->vAppend(p_costUnicast);

  vInsert(p_costQuery);
}

/*----------------------------------------------------------------------------*/
/* compute the cast from prev join to curr join locus                         */
/*----------------------------------------------------------------------------*/
eXK Card_JoinPermSeq::eLocusCast(Symbol*         p_Y_streamPtn_running,
                                 Card_TableList* p_tableList,
                                 Card_StreamJoin* p_joinLocus)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CmpPred* p_cmpPred = p_joinLocus->pSub_cmpPred();
  Card_Term* p_term_0_cmp = p_cmpPred->pGetTerm_0();
  Card_Term* p_term_1_cmp = p_cmpPred->pGetTerm_1();

  char cTblSyn_locus = p_joinLocus->cValue();
  char cTblSyn_prior = p_cmpPred->cOtherTable(cTblSyn_locus);

  Card_TablePtn* p_tablePtn_prior = p_tableList->pGetTablePtn(cTblSyn_prior);
  Card_TablePtn* p_tablePtn_locus = p_tableList->pGetTablePtn(cTblSyn_locus);



  Symbol* p_Y_tablePtn_prev = p_tablePtn_prior->pSymbol();
  Symbol* p_Y_tablePtn_curr = p_tablePtn_locus->pSymbol();

  Symbol* p_Y_term_0_cmp = p_term_0_cmp->pSymbol();
  Symbol* p_Y_term_1_cmp = p_term_1_cmp->pSymbol();


  Symbol* p_Y_tblCol_prev = nullptr;
  Symbol* p_Y_idxCol_curr = nullptr;

  if (Card_COL::bMatchingTblSyn(p_Y_tablePtn_prev,p_Y_term_0_cmp)) {
    p_Y_tblCol_prev = p_Y_term_0_cmp;
    p_Y_idxCol_curr = p_Y_term_1_cmp;
  }
  else {
    p_Y_tblCol_prev = p_Y_term_1_cmp;
    p_Y_idxCol_curr = p_Y_term_0_cmp;
  }

  eXK eStageCast = Card_Cast::eStageCast(p_Y_streamPtn_running,
                                         p_Y_tablePtn_prev,
                                         p_Y_tblCol_prev,
                                         p_Y_tablePtn_curr,
                                         p_Y_idxCol_curr);

  return eStageCast;
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

    if ('F' == cTblSyn_ith)
      vDebugPoint();
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (0 == nChildCards()) {
      //+ table is head/first of join sequence

      Card_StreamHead *p_dataStart = new Card_StreamHead;
      p_dataStart->vValue(cTblSyn_ith);
      vAppend(p_dataStart);

      p_dataStart->vAppend(new Card_TableNm(p_table->pSub_tableNm()->szValue()));

      if (p_table->pSub_keyExists() || p_table->pSub_keyNotExists()) {
        p_dataStart->vAppend(new Card_Note("**CAN'T BE FIRST**"_Stz));
        pSymbol()->Y_vSet(P_Invalid);
        iNotValid = i_Item;
        break;
      }

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // table is next/subsequent of join sequence

    Card_StreamJoin *p_joinLocus = new Card_StreamJoin;
    p_joinLocus->vValue(cTblSyn_ith);
    vAppend(p_joinLocus);

    p_joinLocus->vAppend(new Card_TableNm(p_table->pSub_tableNm()->szValue()));


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
        p_joinLocus->vAppend(xCI.G.p_cmpPred->pDupCardRecursive());
        break;
      }
    }

    if (bSupported)
      //+ join of ith table is possible at this point
      continue;

    // join of ith table is not possible at this point
    p_joinLocus->vAppend(new Card_Note("**DOESN'T FOLLOW**"_Stz));
    pSymbol()->Y_vSet(P_Invalid);

    iNotValid = i_Item;
    break;
  }

//  vTrace("\n"_Stz);

  return iNotValid;
}

/*----------------------------------------------------------------------------*/
/* compute one permuted join sequence                                         */
/*----------------------------------------------------------------------------*/
i8 Card_JoinPermSeq::iOnePermutedJoin_v2(Card_TableList*    p_tableList,
                                         Card_JoinPredList* p_joinPredList,
                                         PermGen*           pPermGen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_NotValid = C_UNKNOWN;

  i8 nTables = p_tableList->nChildCards();

  i8 nCards = 0;
  for (i8 i_Item = 0; i_Item < nTables; i_Item++) {

    // get the ith table of this permutation
    i8 i_Table = pPermGen->iElement(i_Item);
    Card_Table* p_table = _pCard_Table(p_tableList->pGetChild(i_Table));
    char cTblSyn_ith = p_table->cValue();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (0 == nCards) {
      //+ table is head/first of join sequence
      nCards++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // table is next/subsequent of join sequence

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
        nCards++;
        break;
      }
    }

    if (! bSupported) {
      i_NotValid = i_Item;
      break;
    }
  }

//  vTrace("\n"_Stz);

  for (i8 i_Item = 0; i_Item < nTables; i_Item++) {

    // get the ith table of this permutation
    i8 i_Table = pPermGen->iElement(i_Item);
    Card_Table *p_table = _pCard_Table(p_tableList->pGetChild(i_Table));
    char cTblSyn_ith = p_table->cValue();

    if (0 == i_Item) {
      Card_StreamHead *p_dataStart = new Card_StreamHead;
      p_dataStart->vValue(cTblSyn_ith);
      vAppend(p_dataStart);
    }
    else {
      Card_StreamJoin *p_joinLocus = new Card_StreamJoin;
      p_joinLocus->vValue(cTblSyn_ith);
      vAppend(p_joinLocus);

      if (i_NotValid == i_Item) {
        p_joinLocus->vAppend(new Card_Note("**DOESN'T FOLLOW**"_Stz));
        pSymbol()->Y_vSet(P_Invalid);
        break;
      }

      CardIter xCI(p_joinPredList);
      while (xCI.G_bEachMain(CK_CMPPRED)) {
        if (! xCI.G.p_cmpPred->bUsesTable(cTblSyn_ith))
          continue;

        // have a join cmpPred which uses the ith table

        // get the other table in the cmpPred
        char cTblSyn_other = xCI.G.p_cmpPred->cOtherTable(cTblSyn_ith);

        if (bHasKnownJoin(cTblSyn_other)) {
          p_joinLocus->vAppend(xCI.G.p_cmpPred->pDupCardRecursive());
          break;
        }
      }
    }
  }

  return i_NotValid;
}

/*----------------------------------------------------------------------------*/
/* does this join sequence already have a join with given table at this point?*/
/*----------------------------------------------------------------------------*/
bool Card_JoinPermSeq::bHasKnownJoin(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_STREAMHEAD) || xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))
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

  Card_CostQuery* p_costQuery_this = pSub_costQuery();
  Card_CostQuery* p_costQuery_arg  = p_joinPermSeq_arg->pSub_costQuery();

  if (p_costQuery_this->bIsLessLoad(p_costQuery_arg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is a possible unicast permitted at table r ?                               */
/*----------------------------------------------------------------------------*/
bool Card_JoinPermSeq::bUnicastPermitted(char cTblSyn_prior,
                                         char cTblSyn_prev,
                                         char cTblSyn_locus)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);

  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_STREAMHEAD) || xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))

    if (xCI.G.p_MainCard->cValue() == cTblSyn_prior)
      break;

    if (! xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))
      continue;

 }

  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_STREAMHEAD) || xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))

    if (! xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))
      continue;

    xCI.G.p_locusCast = xCI.G.p_joinLocus->pSub_locusCast();
    if (xCI.G.p_locusCast && ! xCI.G.p_locusCast->bIsValue("XK_UNICAST"_Stz))
      return false;

    if (xCI.G.p_MainCard->cValue() == cTblSyn_prev)
      break;
  }

  return true;
}
/*----------------------------------------------------------------------------*/
//bool Card_JoinPermSeq::bUnicastPermitted(char cTblSyn_l,
//                                         char cTblSyn_prev, char cTblSyn_r)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
//
//  CardIter xCI(this);
//  while (xCI.G_bEachMain()) {
//    ASSERT(xCI.G.p_MainCard->bIsCard(CK_STREAMHEAD) || xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))
//
//    if (xCI.G.p_MainCard->cValue() == cTblSyn_l)
//      break;
//
//    if (xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN)) {
//
//      xCI.G.p_locusCast = xCI.G.p_joinLocus->pSub_locusCast();
//      if (xCI.G.p_locusCast && xCI.G.p_locusCast->bIsValue("XK_BROADCAST"_Stz))
//        return false;
//    }
//  }
//
//  while (xCI.G_bEachMain()) {
//    ASSERT(xCI.G.p_MainCard->bIsCard(CK_STREAMHEAD) || xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))
//
//    if (xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN)) {
//      xCI.G.p_locusCast = xCI.G.p_joinLocus->pSub_locusCast();
//      if (xCI.G.p_locusCast && xCI.G.p_locusCast->bIsValue("XK_BROADCAST"_Stz))
//        return false;
//    }
//  }
//
//  return true;
//}
/*----------------------------------------------------------------------------*/
//bool Card_JoinPermSeq::bUnicastPermitted(char cTblSyn_l, char cTblSyn_r)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
//
//  CardIter xCI(this);
//  while (xCI.G_bEachMain()) {
//    ASSERT(xCI.G.p_MainCard->bIsCard(CK_STREAMHEAD) || xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))
//
//    if (xCI.G.p_MainCard->cValue() == cTblSyn_l)
//      break;
//
//    if (xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN)) {
//
//      xCI.G.p_locusCast = xCI.G.p_joinLocus->pSub_locusCast();
//      if (xCI.G.p_locusCast && ! xCI.G.p_locusCast->bIsValue("XK_UNICAST"_Stz))
//        return false;
//    }
//  }
//
//  while (xCI.G_bEachMain()) {
//    ASSERT(xCI.G.p_MainCard->bIsCard(CK_STREAMHEAD) || xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN))
//
//    if (xCI.G.p_MainCard->bIsCard(CK_STREAMJOIN)) {
//      xCI.G.p_locusCast = xCI.G.p_joinLocus->pSub_locusCast();
//      if (xCI.G.p_locusCast && ! xCI.G.p_locusCast->bIsValue("XK_UNICAST"_Stz))
//        return false;
//    }
//  }
//
//  return true;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
