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
/* estimate the query cost of this join permutation                           */
/*----------------------------------------------------------------------------*/
void Card_JoinPermSeq::vEstQueryCost(Card_TableList* p_tableList,
                                     Card_FiltPredList* p_filtPredList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nBuff_emitted_prior  = 0;
  i8 nBuff_produced_locus = 0;

  i8 nSumNetworkCost = 0;
  i8 nSumUnicastCost = 0;

  i8 nLocus = 0;

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {

    nBuff_emitted_prior = nBuff_produced_locus;

    // calculate the buffers produced at the start of join sequence
    if (xCI.G.p_MainCard->bIsCard(CK_DATASTART)) {
      char cTblSyn_head = xCI.G.p_dataStart->cValue();
      nBuff_produced_locus = p_tableList->pGetTableSize(cTblSyn_head)->iValue();
      continue;
    }

    if (! xCI.G.p_MainCard->bIsCard(CK_JOINLOCUS))
      continue;

    nLocus++;

    // get info for locus of join sequence
    char cTblSyn_locus = xCI.G.p_joinLocus->cValue();
    eXK eLocusCast = this->eLocusCast(p_tableList, xCI.G.p_joinLocus);
    i8 nBuff_joining_with = p_tableList->pGetTableSize(cTblSyn_locus)->iValue();

    i8 nFiltPred = 0;
    CardIter xCI_filtPred(p_filtPredList);
    while (xCI_filtPred.G_bEachMain()) {
      if (xCI_filtPred.G.p_filtPred->cValue() == cTblSyn_locus) {
        nFiltPred = xCI_filtPred.G.p_filtPred->pSymbol()->Y_iGet(P_Count);
        break;
      }
    }

    // calculate the various costs for locus of join sequence
//    nBuff_produced_locus =
//        xCI.G.p_joinLocus->nEstLocusCosts(nLocus, nBuff_emitted_prior, eLocusCast, nBuff_joining_with, nFiltPred);

    nBuff_produced_locus =
        xCI.G.p_joinLocus->nEstLocusCosts(nLocus,
                                          nBuff_emitted_prior,
                                          eLocusCast,
                                          nBuff_joining_with,
                                          nFiltPred);

    // running sum of the network and unicast costs
    Card_CostLocus* p_costLocus = xCI.G.p_joinLocus->pSub_costLocus();
    Card_CostNetwork* p_costNetwork = p_costLocus->pSub_costNetwork();
    nSumNetworkCost += p_costNetwork->iValue();
    Card_CostUnicast* p_costUnicast = p_costLocus->pSub_costUnicast();
    nSumUnicastCost += p_costUnicast->iValue();
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
/* compute the cast of given join                                             */
/*----------------------------------------------------------------------------*/
eXK Card_JoinPermSeq::eLocusCast(Card_TableList* p_tableList,
                                 Card_JoinLocus* p_joinLocus)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CmpPred* p_cmpPred = p_joinLocus->pSub_cmpPred();
  Card_Term* p_term_0 = p_cmpPred->pGetTerm_0();
  Card_Term* p_term_1 = p_cmpPred->pGetTerm_1();

  char cTblSyn_r = p_joinLocus->cValue();
  char cTblSyn_l = p_cmpPred->cOtherTable(cTblSyn_r);

  Card_TablePtn* p_tablePtn_l = p_tableList->pGetTablePtn(cTblSyn_l);
  Card_TablePtn* p_tablePtn_r = p_tableList->pGetTablePtn(cTblSyn_r);

  // l_ptn == t:$x.1 && r_ptn == t:$y.1
//  if ( (p_tablePtn_l->bSameValue(p_term_0) && p_tablePtn_r->bSameValue(p_term_1)) ||
//       (p_tablePtn_l->bSameValue(p_term_1) && p_tablePtn_r->bSameValue(p_term_0)) )

  if ( (cTblSyn_r >= 'A' && cTblSyn_r <= 'Z') ||
       (cTblSyn_l >= 'A' && cTblSyn_l <= 'Z') )
    return XK_UNICAST;

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

    if ('F' == cTblSyn_ith)
      vDebugPoint();
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (0 == nChildCards()) {
      //+ table is head/first of join sequence

      Card_DataStart *p_dataStart = new Card_DataStart;
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

    Card_JoinLocus *p_joinLocus = new Card_JoinLocus;
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
      Card_DataStart *p_dataStart = new Card_DataStart;
      p_dataStart->vValue(cTblSyn_ith);
      vAppend(p_dataStart);
    }
    else {
      Card_JoinLocus *p_joinLocus = new Card_JoinLocus;
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
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_DATASTART) || xCI.G.p_MainCard->bIsCard(CK_JOINLOCUS))
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
/* does a possible unicast sequence follow after given table?                 */
/*----------------------------------------------------------------------------*/
bool Card_JoinPermSeq::bUnicastSeqFollows(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_DATASTART) || xCI.G.p_MainCard->bIsCard(CK_JOINLOCUS))

    if (xCI.G.p_MainCard->cValue() == cTblSyn)
      break;

    if (xCI.G.p_MainCard->bIsCard(CK_JOINLOCUS)) {

      xCI.G.p_locusCast = xCI.G.p_joinLocus->pSub_locusCast();
      if (xCI.G.p_locusCast && ! xCI.G.p_locusCast->bIsValue("XK_UNICAST"_Stz))
        return false;
    }
  }

  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_DATASTART) || xCI.G.p_MainCard->bIsCard(CK_JOINLOCUS))

    if (xCI.G.p_MainCard->bIsCard(CK_JOINLOCUS)) {
      xCI.G.p_locusCast = xCI.G.p_joinLocus->pSub_locusCast();
      if (xCI.G.p_locusCast && ! xCI.G.p_locusCast->bIsValue("XK_UNICAST"_Stz))
        return false;
    }
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
