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

__pCastFunction_Card(STORE, Card_Stage, CK_o_STAGE)
__aNewFunction(Card_Stage,CN_CARD)
__vDeleteFunction(Card_Stage,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stage::Card_Stage(void)
: Card_CONSTRUCTION(CK_o_STAGE, 1)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vReinstate_stages(CardIter* pCI)
{
  _static_


  vReinstate_stages_v0(pCI);
//  vReinstate_stages_v1(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vReinstate_stages_v0(CardIter* pCI)
{
  _static_

  i8 nStages = 0;

  Card_Stage* p_stage = new Card_Stage;
  p_stage->SYM(SF_STG_NM,++nStages);
  Card_Topology* p_topology = new Card_Topology(YG_ALL_PARTITIONS);
  p_stage->vAppendChild(p_topology);
  pCI->E.p_deck->vPushChild(p_stage);

  eAL e_level_running = Card::e_stageTopo_To_aggLevel(YG_ALL_PARTITIONS);

  bool bSawAggOp = false;
  bool bSawFilterOp = false;
  bool bSawIndexJoinOp = false;
  bool bSawProjectOp = false;
  bool bSawSortOp = false;

  pCI->vClearRegisters();
  pCI->vBeginEach();
  while (pCI->bEachOp(CK_o_FOLD))
    pCI->vRemoveDeleteMain(CK_o_FOLD);
  pCI->vClearRegisters();
  pCI->vBeginEach();
  while (pCI->bEachOp(CK_o_FLAT))
    pCI->vRemoveDeleteMain(CK_o_FLAT);

  Card* p_main = p_stage;
  eCK   e_main = p_main->eCard();

  for (i8 i_Main = 1; i_Main < pCI->E.p_deck->nChildCards(); i_Main++) {


    p_main = pCI->E.p_deck->pGetChild(i_Main);
    e_main = p_main->eCard();



    eAL e_level_main = (eAL) p_main->pSub_level()->pSymbol()->Y_iGet(P_Level);

    // change of level
    if (e_level_main != e_level_running) {
      if (e_level_running == AL_ORIGIN &&
          e_level_main == AL_PARTITION)
        vAddNext(pCI, p_main, i_Main,"broadcast"_Stz);
      else
        vAddNext(pCI, p_main, i_Main,""_Stz);
      e_level_running = e_level_main;
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawFilterOp = false;
      if (CK_o_GBYAGG == e_main || CK_o_AGGREG == e_main || CK_o_ORDAGG == e_main)
        bSawAggOp = true;
      else
        bSawAggOp = false;
      if (CK_o_PROJECT == e_main)
        bSawProjectOp = true;
      else
        bSawProjectOp = false;
      continue;
    }

    if (CK_o_CROSSJOIN == e_main || CK_o_INDEXJOIN == e_main
        || CK_o_KEYEXISTS == e_main
        || CK_o_KEYNOTEXISTS == e_main) {
      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = true;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if ((CK_o_AGGREG == e_main)
        && (bSawAggOp || bSawFilterOp || bSawIndexJoinOp || bSawProjectOp || bSawSortOp) ) {
       vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = true;
      bSawSortOp = false;
      continue;
    }

    if ((CK_o_PROJECT == e_main)
        && (bSawAggOp || bSawFilterOp || bSawIndexJoinOp || bSawProjectOp || bSawSortOp) ) {
      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = true;
      bSawSortOp = false;
      continue;
    }


    if ((CK_o_GBYAGG == e_main || CK_o_ORDAGG == e_main) &&
        (bSawAggOp || bSawIndexJoinOp || bSawProjectOp)) {
       vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if (CK_o_SORT == e_main) {

      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = true;
      continue;
    }


    // if filter then continue?
    if (CK_o_FILTER == e_main || CK_o_HAVING == e_main) {

      bSawFilterOp = true;
      if (CK_o_FILTER == e_main && ! bSawAggOp)
        continue;

      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = true;
      continue;

    }

    if (bSawAggOp || bSawIndexJoinOp) {
      vAddNext(pCI, p_main, i_Main,""_Stz);
      vAddStage(pCI, p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawFilterOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if (CK_o_GBYAGG == e_main || CK_o_ORDAGG == e_main)
      bSawAggOp = true;
    if (CK_o_PROJECT == e_main || CK_o_AGGREG == e_main)
      bSawProjectOp = true;
  }

  i8 i_End = C_UNKNOWN;
  vAddNext(pCI, p_main, i_End,""_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vReinstate_stages_v1(CardIter* pCI)
{
  _static_

  CardUsage* pStager = new CardUsage;
  RASSERT(0 == pStager->nSaw());

  i8 nStages = 0;
  //SawOpCards* pSawOpCards = new SawOpCards;
  eAL e_level_running = Card::e_stageTopo_To_aggLevel(YG_ALL_PARTITIONS);

  for (i8 i_Main = 1; i_Main < pCI->E.p_deck->nChildCards(); i_Main++) {

    Card_OP* p_card_OP = (Card_OP*)(pCI->E.p_deck->pGetChild(i_Main));

    RASSERT(Card_OP::bIsOp(p_card_OP));

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
void Card_Stage::vAddNext(CardIter* pCI, Card* p_main, i8& ri_main, Stz* szNextValue)
{
  _static_

  Card_Cast* p_next = new Card_Cast;
  if (*szNextValue->azChar()) {
    eSF eSymbol = Symbol::eEval_facet(szNextValue);
    RASSERT(SF_STG_CAST == eSymbol);
    p_next->SYM(SF_STG_CAST, szNextValue);
  }
  if (C_UNKNOWN == ri_main) {
    pCI->E.p_deck->vAppendChild(p_next);
    return;
  }
  pCI->E.p_deck->vInsertChildBefore(p_next, p_main);
  ri_main++;
  RASSERT(pCI->E.p_deck->pGetChild(ri_main) == p_main);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stage::vAddStage(CardIter* pCI, Card* p_main, i8& ri_main, eAL e_level_running, i8& nStages)
{
  _static_

  Card_Stage *p_stage = new Card_Stage;
  p_stage->SYM(SF_STG_NM,++nStages);
  eYG eStageTopo = Card::e_aggLevel_To_stageTopo(e_level_running);
  Card_Topology *p_topology = new Card_Topology(eStageTopo);
  p_stage->vAppendChild(p_topology);
  pCI->E.p_deck->vInsertChildBefore(p_stage, p_main);
  ri_main++;
  RASSERT(pCI->E.p_deck->pGetChild(ri_main) == p_main);

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

  vValidate_child_notBe(CK_c_LEVEL, CK_NONE);

  RASSERT(m_xSymbol.bSingleFaceted() && m_xSymbol.bHasFacet(SF_STG_NM));

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
