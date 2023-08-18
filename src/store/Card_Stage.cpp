/*============================================================================*/
/* Card_Stage.cpp -- Stage                                                    */
/*============================================================================*/

#include "Card_Stage.hpp"

#include "CardIter.hpp"
#include "SawOpCards.hpp"

#include "CardUsage.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Stage,CK_STAGE)
__aNewFunction(Card_Stage,CN_CARD)
__vDeleteFunction(Card_Stage,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stage::Card_Stage(void)
: Card_symbol(CK_STAGE, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Stage::~Card_Stage(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* delete the <absJoin>                                                       */
/*----------------------------------------------------------------------------*/
void Card_Stage::vDeleteAllStage(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMain(CK_STAGE)) {
    pCI->G_vRemoveDeleteMain(CK_STAGE);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vReinstate_stages(CardIter* pCI)
{
  _static_

  i8 nStages = 0;

  Card_Stage* p_stage = new Card_Stage;
  p_stage->SYM(SF_STG_NO,++nStages);
  Card_Topology* p_topology = new Card_Topology(YG_ALL_PARTITIONS);
  p_stage->vAppend(p_topology);
  pCI->G.pDeck->vPush(p_stage);

  eAL e_level_running = Card::e_stageTopo_To_aggLevel(YG_ALL_PARTITIONS);

  bool bSawAggOp = false;
  bool bSawFilterOp = false;
  bool bSawIndexJoinOp = false;
  bool bSawProjectOp = false;
  bool bSawScanOp = false;
  bool bSawSortOp = false;

  pCI->vClearRestart();
  while (pCI->G_bEachMain(CK_FOLD))
    pCI->G_vRemoveDeleteMain(CK_FOLD);
  pCI->vClearRestart();
  while (pCI->G_bEachMain(CK_FLAT))
    pCI->G_vRemoveDeleteMain(CK_FLAT);

//  Card* p_prev = nullptr;
  eCK   e_prev = CK_NONE;
  Card* p_main = p_stage;
  eCK   e_main = p_main->eCard();

  for (i8 i_Main = 1; i_Main < pCI->G.pDeck->nChildCards(); i_Main++) {

//    p_prev = p_main;
    e_prev = e_main;

    p_main = pCI->G.pDeck->pGetChild(i_Main);
    e_main = p_main->eCard();



    //RASSERT(Card_OP::bIsOp(p_main));



    eAL e_level_main = (eAL) p_main->pSub_level()->pSymbol()->Y_iGet(P_Level);

    // change of level
    if (e_level_main != e_level_running) {
      if (CK_SCAN != e_prev)// && CK_PROJECT != e_prev)
        vAddCombined(pCI, p_main, i_Main);
      if (e_level_running == AL_ORIGIN &&
          e_level_main == AL_PARTITION)
        vAddNext(pCI, p_main, i_Main,"broadcast"_Stz);
      else
        vAddNext(pCI, p_main, i_Main,""_Stz);
      e_level_running = e_level_main;
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawFilterOp = false;
      bSawScanOp = false;
      if (CK_GBYAGG == e_main || CK_AGGREG == e_main || CK_ORDAGG == e_main)
        bSawAggOp = true;
      else
        bSawAggOp = false;
      if (CK_PROJECT == e_main)
        bSawProjectOp = true;
      else
        bSawProjectOp = false;
      continue;
    }

    if (CK_CROSSJOIN == e_main || CK_INDEXJOIN == e_main
        || CK_KEYEXISTS == e_main
        || CK_KEYNOTEXISTS == e_main) {
      if (CK_SCAN != e_prev)// && ((CK_PROJECT == e_prev && bSawFilterOp) || CK_PROJECT != e_prev))
        vAddCombined(pCI, p_main, i_Main);
      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = true;
      bSawProjectOp = false;
      bSawScanOp = false;
      bSawSortOp = false;
      continue;
    }

//    if ((CK_PROJECT == e_main) &&
//        ! ((bSawAggOp || bSawIndexJoinOp || bSawProjectOp || bSawScanOp || bSawSortOp))) {
//      vAddCombined(pCI, p_main, i_Main);
//    }


    if ((CK_AGGREG == e_main)
        && (bSawAggOp || bSawFilterOp || bSawIndexJoinOp || bSawProjectOp || bSawSortOp) ) {
      if (CK_SCAN != e_prev)// && CK_PROJECT != e_prev)
        vAddCombined(pCI, p_main, i_Main);
      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = true;
      bSawScanOp = false;
      bSawSortOp = false;
      continue;
    }

    if ((CK_PROJECT == e_main)
        && (bSawAggOp || bSawFilterOp || bSawIndexJoinOp || bSawProjectOp || bSawSortOp) ) {
      //if (CK_SCAN != e_prev && CK_PROJECT != e_prev)
        vAddCombined(pCI, p_main, i_Main);
      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = true;
      bSawScanOp = false;
      bSawSortOp = false;
      continue;
    }


    if ((CK_GBYAGG == e_main || CK_ORDAGG == e_main) &&
        (bSawAggOp || bSawIndexJoinOp || bSawProjectOp)) {
      if (CK_SCAN != e_prev)
        vAddCombined(pCI, p_main, i_Main);
      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawScanOp = false;
      bSawSortOp = false;
      continue;
    }

    if (CK_SORT == e_main) {
      if (bSawAggOp || bSawIndexJoinOp || bSawProjectOp || bSawSortOp) {
        vAddCombined(pCI, p_main, i_Main);
      }

      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawScanOp = false;
      bSawSortOp = true;
      continue;
    }

    if (CK_SCAN == e_main) {
      bSawScanOp = true;
    }


    // if filter then continue?
    if (CK_FILTER == e_main || CK_HAVING == e_main) {

      bSawFilterOp = true;
      if (CK_FILTER == e_main && ! bSawAggOp)
       continue;

      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawScanOp = false;
      bSawSortOp = true;
      continue;

    }

    if (bSawAggOp || bSawIndexJoinOp) {
      vAddCombined(pCI, p_main, i_Main);
      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawScanOp = false;
      bSawSortOp = false;
      continue;
    }

    if (CK_GBYAGG == e_main|| CK_ORDAGG == e_main)
      bSawAggOp = true;
    if (CK_PROJECT == e_main || CK_AGGREG == e_main)
      bSawProjectOp = true;
  }

  e_prev = e_main;

  i8 i_End = C_UNKNOWN;
  if (CK_SCAN != e_prev && CK_PROJECT != e_prev)
    vAddCombined(pCI, p_main, i_End);
  else
  if (bSawScanOp && bSawProjectOp)
    vAddCombined(pCI, p_main, i_End);
  else
  if (bSawFilterOp && bSawProjectOp)
    vAddCombined(pCI, p_main, i_End);
  else
  if (bSawProjectOp)
    vAddCombined(pCI,p_main, i_End);
  vAddNext(pCI, p_main, i_End,""_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vReinstate_stages_v2(CardIter* pCI)
{
  _static_

  CardUsage* pStager = new CardUsage;
  RASSERT(0 == pStager->nSaw());

  i8 nStages = 0;
  //SawOpCards* pSawOpCards = new SawOpCards;
  eAL e_level_running = Card::e_stageTopo_To_aggLevel(YG_ALL_PARTITIONS);

  for (i8 i_Main = 1; i_Main < pCI->G.pDeck->nChildCards(); i_Main++) {

    Card_OP* p_card_OP = (Card_OP*)(pCI->G.pDeck->pGetChild(i_Main));

    RASSERT(Card_OP::bIsOp(p_card_OP));

    if (pStager->bAddCombined(p_card_OP))
      vAddCombined(pCI, p_card_OP, i_Main);

    if (pStager->bAddNext(p_card_OP))
      vAddNext(pCI, p_card_OP, i_Main, ""_Stz);

    if (pStager->bAddStage(p_card_OP)) {
      vAddStage(pCI, p_card_OP, i_Main, e_level_running, nStages);
      pStager->vClearAll();
    }

   //pSawOpCards->vSaw(p_card_OP);

    pStager->vSaw(p_card_OP);
  }

  //delete pSawOpCards;

  delete pStager;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vAddCombined(CardIter* pCI, Card* p_main, i8& ri_main)
{
  _static_
  Card_Combined* p_combined = new Card_Combined;
  if (C_UNKNOWN == ri_main) {
    pCI->G.pDeck->vAppend(p_combined);
    return;
  }
  pCI->G.pDeck->vInsertBefore(p_combined, p_main);
  ri_main++;
  RASSERT(pCI->G.pDeck->pGetChild(ri_main) == p_main);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vAddNext(CardIter* pCI, Card* p_main, i8& ri_main, Stz* szNextValue)
{
  _static_

  Card_Cast* p_next = new Card_Cast;
  if (*szNextValue->azChar()) {
    eSF eSymbol = Symbol::eEvalSymbol(szNextValue);
    RASSERT(SF_STG_CAST == eSymbol);
    p_next->SYM(SF_STG_CAST, szNextValue);
  }
  if (C_UNKNOWN == ri_main) {
    pCI->G.pDeck->vAppend(p_next);
    return;
  }
  pCI->G.pDeck->vInsertBefore(p_next, p_main);
  ri_main++;
  RASSERT(pCI->G.pDeck->pGetChild(ri_main) == p_main);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vAddStage(CardIter* pCI, Card* p_main, i8& ri_main, eAL e_level_running, i8& nStages)
{
  _static_

  Card_Stage *p_stage = new Card_Stage;
  p_stage->SYM(SF_STG_NO,++nStages);
  eYG eStageTopo = Card::e_aggLevel_To_stageTopo(e_level_running);
  Card_Topology *p_topology = new Card_Topology(eStageTopo);
  p_stage->vAppend(p_topology);
  pCI->G.pDeck->vInsertBefore(p_stage, p_main);
  ri_main++;
  RASSERT(pCI->G.pDeck->pGetChild(ri_main) == p_main);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_notHave(CK_LEVEL,CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
