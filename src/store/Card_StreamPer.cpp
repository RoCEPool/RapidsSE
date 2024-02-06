/*============================================================================*/
/* Card_StreamPer.cpp -- Join Permutation Sequence                          */
/*============================================================================*/

#include "Card_StreamPer.hpp"

#include "Card_BoolPred.hpp"
#include "Card_RlnConj.hpp"
#include "Card_RlnConj.hpp"
#include "Card_FiltPred.hpp"
#include "Card_FiltPredList.hpp"
#include "Card_StreamSet.hpp"
#include "Card_JoinPredList.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_StreamPer, CK_STREAMPER)
__aNewFunction(Card_StreamPer, CN_CARD)
__vDeleteFunction(Card_StreamPer, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamPer::Card_StreamPer(void)
: Card_symbol(CK_STREAMPER, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamPer::~Card_StreamPer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate the query cost of this join permutation sequence                  */
/*----------------------------------------------------------------------------*/
void Card_StreamPer::vEstStreamCost(Card_TableList* p_tableList,
                                    Card_FiltPredList* p_filtPredList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Permutation = pSymbol()->Y_iGet(P_Permutation);
  if (571 == i_Permutation)
    vDebugPoint();

  i8 nUnfiltBuff_prev_join  = 0;
  i8 nUnfiltBuff_producedBy_join = 0;

  i8 nSumNetworkCost = 0;
  i8 nSumUnicastCost = 0;

  i8 i_Join = 0;

  i8 nFilterPred_prev = 0;

  Card_StreamPtn* p_streamPtn = new Card_StreamPtn;

  Kid xKid(this);
  while (xKid.bEach()) {

    nUnfiltBuff_prev_join = nUnfiltBuff_producedBy_join;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream head

    // calculate the buffers produced at the start of join sequence
    if (xKid.pChild()->bIsCard(CK_STREAMHEAD)) {

      // get info of stream start
      char cTblSyn_head = xKid.p_streamHead()->pSymbol()->cTblSyn();

      nUnfiltBuff_producedBy_join = p_tableList->nGetTableSize(cTblSyn_head) * C_EXPANSION_COEFF;

      nFilterPred_prev = p_filtPredList->nFiltersFor(cTblSyn_head);

      p_streamPtn->vAddKey(p_tableList->pGetTablePtn(cTblSyn_head)->pSymbol());
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream join

    if (! xKid.pChild()->bIsCard(CK_STREAMJOIN))
      continue;

    // get info for this join
    char cTblSyn_join = xKid.p_streamJoin()->pSymbol()->cTblSyn();

    eXK eJoinCast = this->eJoinCast(p_streamPtn,
                                    p_tableList,
                                    xKid.p_streamJoin());

    // calculate the various costs of join sequence
    i8 nBuff_joining_with = p_tableList->nGetTableSize(cTblSyn_join);

    nUnfiltBuff_producedBy_join =
        xKid.p_streamJoin()->nEstJoinCost(i_Permutation,
                                         i_Join,
                                         nUnfiltBuff_prev_join,
                                         nFilterPred_prev,
                                         eJoinCast,
                                         nBuff_joining_with
        );

    // running sum of the network and unicast costs
    Card_JoinEstimate* p_joinEstimate = xKid.p_streamJoin()->pSub_joinEstimate();
    Card_CostNetwork* p_costNetwork = p_joinEstimate->pSub_costNetwork();
    nSumNetworkCost += p_costNetwork->pSymbol()->iSF_number();
    Card_CostUnicast* p_costUnicast = p_joinEstimate->pSub_costUnicast();
    nSumUnicastCost += p_costUnicast->pSymbol()->iSF_number();

    nFilterPred_prev = p_filtPredList->nFiltersFor(cTblSyn_join);

    i_Join++;
  }

  // record the sum network and unicast costs
  Card_CostNetwork* p_costNetwork = new Card_CostNetwork;
  p_costNetwork->SYM(SF_INT_LIT,nSumNetworkCost);

  Card_CostUnicast* p_costUnicast = new Card_CostUnicast;
  p_costUnicast->SYM(SF_INT_LIT,nSumUnicastCost);

  Card_CostStream* p_costQuery = new Card_CostStream;
  p_costQuery->vAppendChild(p_costNetwork);
  p_costQuery->vAppendChild(p_costUnicast);

  vPushChild(p_costQuery);

  delete p_streamPtn;
}

/*----------------------------------------------------------------------------*/
/* compute the cast from prev join to curr/given stream join                  */
/*----------------------------------------------------------------------------*/
eXK Card_StreamPer::eJoinCast(Card_StreamPtn*  p_streamPtn,
                              Card_TableList*  p_tableList,
                              Card_StreamJoin* p_streamJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RlnConj* p_rlnConj = p_streamJoin->pSub_rlnConj();
  Card_Term* p_term_0_cmp = p_rlnConj->pGetTerm_0();
  Card_Term* p_term_1_cmp = p_rlnConj->pGetTerm_1();

  char cTblSyn_join  = p_streamJoin->pSymbol()->cTblSyn();
  char cTblSyn_prior = p_rlnConj->cOtherTable(cTblSyn_join);

  Card_TablePtn* p_tablePtn_prior = p_tableList->pGetTablePtn(cTblSyn_prior);
  Card_TablePtn* p_tablePtn_join  = p_tableList->pGetTablePtn(cTblSyn_join);

  Symbol* p_Y_tablePtn_prior = p_tablePtn_prior->pSymbol();
  Symbol* p_Y_tablePtn_join  = p_tablePtn_join->pSymbol();

  Symbol* p_Y_term_0_cmp = p_term_0_cmp->pSymbol();
  Symbol* p_Y_term_1_cmp = p_term_1_cmp->pSymbol();

  Symbol* p_Y_tblCol_prior = nullptr;
  Symbol* p_Y_idxCol_join  = nullptr;

  if (Card_COLUMN::bMatchingTblSyn(p_Y_tablePtn_prior, p_Y_term_0_cmp)) {
    p_Y_tblCol_prior = p_Y_term_0_cmp;
    p_Y_idxCol_join  = p_Y_term_1_cmp;
  }
  else {
    p_Y_tblCol_prior = p_Y_term_1_cmp;
    p_Y_idxCol_join  = p_Y_term_0_cmp;
  }

  eXK eStageCast = Card_Cast::eStageCast(p_streamPtn,
                                         p_Y_tblCol_prior,
                                         p_Y_tablePtn_join,
                                         p_Y_idxCol_join);
  return eStageCast;
}

/*----------------------------------------------------------------------------*/
/* create the join sequence for given stream permutation                      */
/*----------------------------------------------------------------------------*/
i8 Card_StreamPer::iCreatePerSeq(Card_TableList*    p_tableList,
                                 Card_JoinPredList* p_joinPredList,
                                 PermGen*           pPermGen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_InvalidAtJoin = C_UNKNOWN;

  i8 nTables   = p_tableList->nChildCards();
  i8 nElements = pPermGen->nElements();
  RASSERT(nTables == pPermGen->nElements());

  //vTrace("%lld :"_Stz,pPermGen->nPermutations());
  for (i8 i_Elm = 0; i_Elm < nElements; i_Elm++) {

    // get the ith table of this permutation
    i8 i_Table = pPermGen->iElement(i_Elm);
    Card_Table* p_table_ith = _pCard_Table(p_tableList->pGetChild(i_Table));
    char cTblSyn_ith = p_table_ith->pSymbol()->cTblSyn();

    if ('F' == cTblSyn_ith)
      vDebugPoint();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream head : table is the head of this stream permutation

    if (0 == nChildCards()) {
      //+ table is the head of this stream permutation

      Card_StreamHead *p_streamHead = new Card_StreamHead;
      p_streamHead->SYM(SF_TBL_SYN,cTblSyn_ith);
      vAppendChild(p_streamHead);

      p_streamHead->vAppendChild((new Card_TableNm) ->SYM(SF_TBL_NM, p_table_ith->pSub_tableNm()->szFacet(SF_TBL_NM)));

      if (p_table_ith->pSub_keyExists() || p_table_ith->pSub_keyNotExists()) {
        p_streamHead->vAppendChild((new Card_Note) ->SYM(SF_NOTE,"**CAN'T BE FIRST**"_Stz));
        pSymbol()->Y_vSet(P_Invalid);
        i_InvalidAtJoin = i_Elm;
        break;
      }

      if (p_table_ith->pSymbol()->Y_bHas(P_FixedPos)) {
        if (p_table_ith->pSymbol()->Y_iGet(P_FixedPos) != i_Elm) {
          p_streamHead->vAppendChild((new Card_Note) ->SYM(SF_NOTE,"**BAD POS**"_Stz));
          pSymbol()->Y_vSet(P_Invalid);
          i_InvalidAtJoin = i_Elm;
          break;
        }
      }


      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream join : table is a join in this stream permutation

    Card_StreamJoin *p_streamJoin = new Card_StreamJoin;
    p_streamJoin->SYM(SF_TBL_SYN,cTblSyn_ith);
    vAppendChild(p_streamJoin);

    p_streamJoin->vAppendChild((new Card_TableNm) ->SYM(SF_TBL_NM, p_table_ith->pSub_tableNm()->szFacet(SF_TBL_NM)));

    if (p_table_ith->pSymbol()->Y_bHas(P_FixedPos)) {
      if (p_table_ith->pSymbol()->Y_iGet(P_FixedPos) != i_Elm) {
        p_streamJoin->vAppendChild((new Card_Note) ->SYM(SF_NOTE,"**BAD POS**"_Stz));
        pSymbol()->Y_vSet(P_Invalid);
        i_InvalidAtJoin = i_Elm;
        break;
      }
    }


    bool bSupported = false;

    // test each join predicate which uses the ith table to see if a join
    // can be supported at this point in the sequence
    Kid xKid(p_joinPredList);
    while (xKid.bEach(CK_RLNCONJ)) {
      if (!xKid.p_rlnConj()->bUsesTable(cTblSyn_ith))
        continue;

      // have a join cmpPred which uses the ith table

      // get the other table in the cmpPred
      char cTblSyn_other = xKid.p_rlnConj()->cOtherTable(cTblSyn_ith);

      if (bHasKnownJoin(cTblSyn_other)) {
        bSupported = true;
        p_streamJoin->vAppendChild(xKid.p_rlnConj()->pDupCardRecursive());
        break;
      }
    }

    if (bSupported)
      //+ join of ith table is possible at this point
      continue;

    // join of ith table is not possible at this point
    p_streamJoin->vAppendChild((new Card_Note) ->SYM(SF_NOTE,"**DOESN'T FOLLOW**"_Stz));
    pSymbol()->Y_vSet(P_Invalid);

    i_InvalidAtJoin = i_Elm;
    break;
  }

//  vTrace("\n"_Stz);

  return i_InvalidAtJoin;
}

/*----------------------------------------------------------------------------*/
/* compute one permuted join sequence                                         */
/*----------------------------------------------------------------------------*/
i8 Card_StreamPer::iOnePermutedJoin_v2(Card_TableList*    p_tableList,
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
    char cTblSyn_ith = p_table->pSymbol()->cTblSyn();

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
    Kid xKid(p_joinPredList);
    while (xKid.bEach(CK_RLNCONJ)) {
      if (!xKid.p_rlnConj()->bUsesTable(cTblSyn_ith))
        continue;

      // have a join cmpPred which uses the ith table

      // get the other table in the cmpPred
      char cTblSyn_other = xKid.p_rlnConj()->cOtherTable(cTblSyn_ith);

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
    char cTblSyn_ith = p_table->pSymbol()->cTblSyn();

    if (0 == i_Item) {
      Card_StreamHead *p_dataStart = new Card_StreamHead;
      p_dataStart->SYM(SF_TBL_SYN,cTblSyn_ith);
      vAppendChild(p_dataStart);
    }
    else {
      Card_StreamJoin *p_joinLocus = new Card_StreamJoin;
      p_joinLocus->SYM(SF_TBL_SYN,cTblSyn_ith);
      vAppendChild(p_joinLocus);

      if (i_NotValid == i_Item) {
        p_joinLocus->vAppendChild((new Card_Note) ->SYM(SF_NOTE,"**DOESN'T FOLLOW**"_Stz));
        pSymbol()->Y_vSet(P_Invalid);
        break;
      }

      Kid xKid(p_joinPredList);
      while (xKid.bEach(CK_RLNCONJ)) {
        if (!xKid.p_rlnConj()->bUsesTable(cTblSyn_ith))
          continue;

        // have a join cmpPred which uses the ith table

        // get the other table in the cmpPred
        char cTblSyn_other = xKid.p_rlnConj()->cOtherTable(cTblSyn_ith);

        if (bHasKnownJoin(cTblSyn_other)) {
          p_joinLocus->vAppendChild(xKid.p_rlnConj()->pDupCardRecursive());
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
bool Card_StreamPer::bHasKnownJoin(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach()) {
    ASSERT(xKid.pChild()->bIsCard(CK_STREAMHEAD) || xKid.pChild()->bIsCard(CK_STREAMJOIN))
    if (xKid.pChild()->pSymbol()->bSame(cTblSyn))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is load of this permutation sequence greater than given arg?               */
/*----------------------------------------------------------------------------*/
bool Card_StreamPer::bIsHigherCost(Card_StreamPer* p_joinPermSeq_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CostStream* p_costQuery_this = pSub_costQuery();
  Card_CostStream* p_costQuery_arg  = p_joinPermSeq_arg->pSub_costQuery();

  if (p_costQuery_this->bIsHigherCost(p_costQuery_arg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is load of this permutation sequence less than given arg?                  */
/*----------------------------------------------------------------------------*/
bool Card_StreamPer::bIsLowerCost(Card_StreamPer* p_joinPermSeq_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CostStream* p_costQuery_this = pSub_costQuery();
  Card_CostStream* p_costQuery_arg  = p_joinPermSeq_arg->pSub_costQuery();

  if (p_costQuery_this->bIsLowerCost(p_costQuery_arg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is a possible unicast permitted at table r ?                               */
/*----------------------------------------------------------------------------*/
bool Card_StreamPer::bUnicastPermitted(char cTblSyn_prior,
                                       char cTblSyn_prev,
                                       char cTblSyn_locus)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(cTblSyn_locus);

  Kid xKid(this);

  while (xKid.bEach()) {

    if (xKid.pChild()->pSymbol()->cTblSyn() == cTblSyn_prior)
      break;

    if (! xKid.pChild()->bIsCard(CK_STREAMJOIN))
      continue;

 }

  while (xKid.bEach()) {

    if (! xKid.pChild()->bIsCard(CK_STREAMJOIN))
      continue;

    Card_JoinCast* p_joinCast = xKid.p_streamJoin()->pSub_joinCast();
    if (p_joinCast && ! p_joinCast->pSymbol()->bSame("XK_UNICAST"_Stz))
      return false;

    if (xKid.pChild()->pSymbol()->cTblSyn() == cTblSyn_prev)
      break;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_StreamPer::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
