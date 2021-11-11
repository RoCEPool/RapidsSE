/*============================================================================*/
/* PipelineSpec.cpp -- pipeline Specification                                 */
/*============================================================================*/

#include "BASE.h"

#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "PipelineSpec.hpp"
#include "Dynamo.hpp"
#include "Procedure.hpp"
#include "Job.hpp"
#include "Grid.hpp"
#include "TagConstants.h"

#include "SP_PipelineSpec.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, PipelineSpec, CN_PIPELINESPEC)
__aNewFunction(PipelineSpec, CN_PIPELINESPEC)
__vDeleteFunction(PipelineSpec, CN_PIPELINESPEC)
__aFooterFunction(PipelineSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipelineSpec::PipelineSpec(Stz *szPipelineNm,
                           TagSet *p_JobTags,
                           Scanner *p_Scanner)
    
: ListNL_t<StageSpec>(CN_PIPELINESPEC)
, LifeGuard_t<PipelineSpec>(this, CN_PIPELINESPEC)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  m_sPipelineNm = szPipelineNm;

  m_p_Scanner = p_Scanner;
  m_p_JobTags = p_JobTags;

  _checkInitialised_(CN_PIPELINESPEC);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipelineSpec::~PipelineSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  while (StageSpec *pStageSpec = pPop())
    pStageSpec->vGuardedDelete(CN_STAGESPEC, &pStageSpec,
                               CN_PIPELINESPEC, this, (Stz *) __func__);

  vClear();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pipeline compile                                                           */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCompile(void)
{
  _static_
  
  if (!bPass1_collect()) {
    m_p_Scanner = nullptr;
    return false;
  }

  if (!bPass2_piping()) {
    m_p_Scanner = nullptr;
    return false;
  }

  if (!bPass3_includes()) {
    m_p_Scanner = nullptr;
    return false;
  }

  m_p_Scanner = nullptr;
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add stage spec                                                             */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vAddStageSpec(StageSpec *pStageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  vAppend(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* find stage matching given name                                             */
/*----------------------------------------------------------------------------*/
StageSpec *PipelineSpec::pFindStageSpec(Stz *szFindNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  i8 nStageSpecs = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStageSpecs; i_Stage++) {
    StageSpec *pStageSpec = pGet(i_Stage);

    if (pStageSpec->szStageNm()->bSame(szFindNm)) {
      RASSERT(pStageSpec->i_StageSpec() == i_Stage);
      return pStageSpec;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get pipeline name                                                          */
/*----------------------------------------------------------------------------*/
Stz *PipelineSpec::szPipelineNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return m_sPipelineNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec *PipelineSpec::pStageSpec(i8 i_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  StageSpec *pStageSpec = pGet(i_Stage);

  return pStageSpec;
}


/*----------------------------------------------------------------------------*/
/*                           Pass 1 : Collect                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pass 1: collect the pipeline statement                                     */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bPass1_collect(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (!bCollectPipeline())
    return false;

  if (!(pTkn()->bSame("TAG"_Stz) ||
        pTkn()->bSame("BEGIN"_Stz))
      )
    return false;

  while (pTkn()->bSame("TAG"_Stz))
    if (!bCollectTag())
      return false;

  if (!bCollectBeginEnd())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect pipeline <name>                                                    */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectPipeline(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (! (pTkn()->bSame("PIPELINE"_Stz) ||
         pTkn()->bSame("PIPELINELOCAL"_Stz)))
    return false;

  vTokenAccept();
  vTokenAccept();

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one tag                                                            */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectTag(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (!pTkn()->bSame("TAG"_Stz))
    return false;
  vTokenAccept();

  s32 sTagNm = pTkn()->szChars();
  vTokenAccept();

  if (!pTkn()->bSame("="_Stz))
    return false;
  vTokenAccept();

  sSTMT sTagValue = pTkn()->szChars();
  vTokenAccept();

  while (!pTkn()->bSame("ENDTAG"_Stz)) {
    vTokenAccept();
  }
  vTokenAccept();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_p_JobTags->vInsert(sTagNm, sTagValue);

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect a BEGIN { STAGE ... } END sequence                                 */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectBeginEnd(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (!pTkn()->bSame("BEGIN"_Stz))
    return false;
  vTokenAccept();

  while (pTkn()->bSame("STAGE"_Stz))
    if (!bCollectStage())
      return false;

  if (!pTkn()->bSame("END"_Stz))
    return false;
  vTokenAccept();

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect stage                                                              */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (!pTkn()->bSame("STAGE"_Stz))
    return false;
  vTokenAccept();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz *szStageNm = pTkn()->szChars();
  StageSpec *p_StageSpec = new StageSpec(szStageNm, this);
  vTokenAccept();

  this->vAddStageSpec(p_StageSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (!bCollectTopology(p_StageSpec))
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (bTokenMaybeOp(pTkn()))
    bCollectOpSequence(p_StageSpec);

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect topology                                                           */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectTopology(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (!pTkn()->bSame("ON"_Stz))
    return false;
  vTokenAccept();

  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :

  if (pTkn()->bSame("ALL"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAll(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --
  // on next node :

  else if (pTkn()->bSame("NEXT"_Stz)) {
    vTokenAccept();
    return bRecogniseOnNextNode(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --
  // on node <nodeNm> :

  else if (pTkn()->bSame("NODE"_Stz)) {
    vTokenAccept();
    return bRecogniseOnNode(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --
  // on partition <partitionNo> :

  else if (pTkn()->bSame("PARTITION"_Stz)) {
    vTokenAccept();
    return bRecogniseOnPartition(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :

  else if (pTkn()->bSame("ORIGIN"_Stz)) {
    vTokenAccept();
    return bRecogniseOnOrigin(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --

  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* collect operator-pipe sequence                                             */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectOpSequence(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  while (bTokenMaybeOp(pTkn())) {

    if (!bCollectOp(p_StageSpec))
      return false;

    if (!pTkn()->bSame("|"_Stz))
      break;
    vTokenAccept();
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one operator (and optionally its arguments)                        */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectOp(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  OpSpec *pOpSpec = new OpSpec(pTkn()->szChars(), p_StageSpec);
  vTokenAccept();

  p_StageSpec->vAddOpSpec(pOpSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pTkn()->bSame("BROADCAST"_Stz)) {
    pOpSpec->vSendKind(XK_BROADCAST);
    vTokenAccept();
  }
  else if (pTkn()->bSame("ROBIN"_Stz)) {
    pOpSpec->vSendKind(XK_ROBIN);
    vTokenAccept();
  }
  else if (pTkn()->bSame("LOCALROBIN"_Stz)) {
    pOpSpec->vSendKind(XK_LOCALROBIN);
    vTokenAccept();
  }
  else if (pTkn()->bSame("HASHCAST"_Stz)) {
    pOpSpec->vSendKind(XK_HASHCAST);
    vTokenAccept();
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (!bCollectOpArgs(p_StageSpec, pOpSpec))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect operator arguments                                                 */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectOpArgs(StageSpec *p_StageSpec, OpSpec *pOpSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  UNUSED(p_StageSpec);

  for (i8 i_Arg = 0; /*none*/; i_Arg++) {

    if (!bTokenMaybeOpArg(pTkn()))
      break;

    s32 sArgName;
    sArgName.vSprint("<arg_%lld>"_Stz, i_Arg);

    sSTMT sArgValue(pTkn()->szChars());

    pOpSpec->vArgTagAdd(sArgName, sArgValue, TN_STRING);

    vTokenAccept();
  }

  return true;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON ALL _^ [ NODES | PARTITIONS ]                 */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAll(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :

  if (pTkn()->bSame("NODES"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAllNodes(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :

  else if (pTkn()->bSame("PARTITIONS"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAllPartitions(p_StageSpec);
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON ALL NODE _^                                   */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAllNodes(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :

  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();

    p_StageSpec->vStageTopology(YG_ALL_NODES);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON ALL PARTITIONS _^                             */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAllPartitions(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :

  if (pTkn()->bSame("ON"_Stz)) {
    vTokenAccept();
    if (pTkn()->bSame("ORIGIN"_Stz)) {

      vTokenAccept();
      if (pTkn()->bSame(":"_Stz)) {
        vTokenAccept();

        p_StageSpec->vStageTopology(YG_ALL_PARTITIONS_ON_ORIGIN);
        return true;
      }
    }
    else {
      return false;
    }
  }

  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    p_StageSpec->vStageTopology(YG_ALL_PARTITIONS);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON NEXT NODE ^_                                  */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnNextNode(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);
  
  if (! pTkn()->bSame("NODE"_Stz))
    return false;

  vTokenAccept();

  if (! pTkn()->bSame(":"_Stz))
    return false;

  vTokenAccept();

  p_StageSpec->vStageTopology(YG_NEXT_NODE);

  return true;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON NODE <nodeNo> _^                              */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnNode(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  NEVER_REACH;
  
  s32 sLocus(pTkn()->szChars());
  vTokenAccept();

  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    p_StageSpec->vStageTopology(YG_SPECIFIC_NODE);
    i8 i_Node = sLocus.szStz()->iToI8();
    if (i_Node > AM::pGrid()->nKnownNodes())
      return false;
    p_StageSpec->vSpecificNode(i_Node);
    return true;
  }

  return false;

}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON ORIGIN _^                                     */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnOrigin(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();

    p_StageSpec->vStageTopology(YG_ORIGIN);
    return true;
  }
  else {
    s32 sLocus(pTkn()->szChars());
    vTokenAccept();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON PARTITION <tPtnNo> _^                         */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnPartition(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  s32 sLocus(pTkn()->szChars());
  vTokenAccept();

  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    p_StageSpec->vStageTopology(YG_SPECIFIC_PARTITION);
    i8 tPtnNo = sLocus.szStz()->iToI8();
    if (tPtnNo >= AM::PTNMAP::nPtnsInGrid())
      return false;

    PtnId tPtnId;
    tPtnId = tPtnNo;
    p_StageSpec->vSpecificPtnNo(tPtnId);

    return true;
  }

  return false;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* is token a potential operator?                                             */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bTokenMaybeOp(Token *pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (pTkn->bSame("|"_Stz)
      || pTkn->bSame("END"_Stz)
      || pTkn->bSame("PIPELINE"_Stz)
      || pTkn->bSame("STAGE"_Stz))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is token a potential operator argument?                                    */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bTokenMaybeOpArg(Token *pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  if (pTkn->bSame("|"_Stz)
      || pTkn->bSame("END"_Stz)
      || pTkn->bSame("PIPELINE"_Stz)
      || pTkn->bSame("STAGE"_Stz))
    return false;

  return true;
}


/*----------------------------------------------------------------------------*/
/*                           Pass 2 : Piping                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pass 2 staging                                                             */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bPass2_piping(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  // foreach stageSpec of pipelineSpec
  i8 nStageSpecs = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStageSpecs; i_Stage++) {
    StageSpec *pStageSpec = this->pGet(i_Stage);
    RASSERT(pStageSpec->i_StageSpec() == i_Stage);

    // foreach opSpec of stageSpec
    i8 nOpSpecs = pStageSpec->nItems();
    for (i8 i_Op = 0; i_Op < nOpSpecs; i_Op++) {
      OpSpec *pOpSpec = pStageSpec->pGet(i_Op);
      RASSERT(pOpSpec->iOpSpec() == i_Op);

      Stz *szName = pOpSpec->szOpNm();
      StageSpec *pDstStageSpec = nullptr;

      // can we find a stage matching the name in the OpSepc?

      if (szName->bSame("next"_Stz)) {
        pDstStageSpec = this->pGet(i_Stage + 1);
        if (!pDstStageSpec)
          return false;
      }
      else if (szName->bSame("first"_Stz)) {
        pDstStageSpec = this->pGet(0);
        if (!pDstStageSpec)
          return false;
      }
      else if (szName->bSame("last"_Stz)) {
        pDstStageSpec = this->pGet(nOpSpecs - 1);
        if (!pDstStageSpec)
          return false;
      }
      else {
        pDstStageSpec = this->pFindStageSpec(szName);
      }

      // what did we find?

      if (pDstStageSpec) {
        // name in the opSpec is a stage
        vNameIsStage(pStageSpec, pOpSpec, pDstStageSpec);
      } else {
        // assume the name in the opSpec is an op
        vNameIsOp(pStageSpec, pOpSpec);
      }

    } // foreach op

  } // foreach stage

  return true;
}

/*----------------------------------------------------------------------------*/
/* name is a known stage                                                      */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vNameIsStage(StageSpec *pSrcStageSpec,
                                OpSpec *pSrcOpSpec_nameOfStage,
                                StageSpec *pDstStageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  // is there a send already? This would be the case if one stage is
  // piped into multiple stage e.g a unionAll
  OpSpec *pOpSpecSend = pSrcStageSpec->pFindOpSpec("send"_Stz);

  if (! pOpSpecSend)

    // spec for op send does not yet exist in stage spec
    vFirstSend(pSrcStageSpec, pSrcOpSpec_nameOfStage, pDstStageSpec);

  else

    // spec for op send already exists in stage spec
    vOtherSend(pSrcStageSpec, pSrcOpSpec_nameOfStage,
               pDstStageSpec, pOpSpecSend);
}

/*----------------------------------------------------------------------------*/
/* name is assumed to be an operator                                          */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vNameIsOp(StageSpec *pStageSpec,
                             OpSpec *pOpSpec_nameOfOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  i8 i_Op = pOpSpec_nameOfOp->iOpSpec();
  i8 i_Stage = pStageSpec->i_StageSpec();

  eYO eIntraTopology = StageSpec::eIntraTopoCalc(pStageSpec);

  pOpSpec_nameOfOp->vIntraTopology(eIntraTopology);
  pOpSpec_nameOfOp->vNextStageOp(i_Stage, i_Op + 1);
}


/*----------------------------------------------------------------------------*/
/* first send to stage                                                        */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vFirstSend(StageSpec *pStageSpec,
                              OpSpec *pOpSpec_isStage,
                              StageSpec *pDstStageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);
  
  i8 i_Op = pOpSpec_isStage->iOpSpec();

  i8 i_NextStage = pDstStageSpec->i_StageSpec();

  eYO eIntraTopology;

  eYX eCrossTopology =
    StageSpec::eCrossTopoCalc(pStageSpec, pDstStageSpec, eIntraTopology);

  // SPECIAL CASE (to discuss with Alka)
  if (eCrossTopology == YX_ORIGIN_TO_ORIGIN && i_NextStage == 0) {
    // add parameter to the prev sink op
    OpSpec *pPrevOpSpec = pStageSpec->pGet(i_Op - 1);
    Stz *szStgNm = szStageNm(i_NextStage);
    pPrevOpSpec->vIntraTopology(eIntraTopology);
    pPrevOpSpec->vCrossTopology(eCrossTopology, szStgNm, i_NextStage, 0);

    RASSERT(pStageSpec == pPrevOpSpec->pStageSpec());

    // remove the last (stage) OpSpec from the OpSpecVec
    RASSERT(i_Op == pStageSpec->nItems() - 1);
    pStageSpec->vRemove(pOpSpec_isStage);
    //delete pOpSpec_isStage;
    pOpSpec_isStage->vGuardedDelete(CN_OPSPEC, &pOpSpec_isStage,
                                    CN_PIPELINESPEC, this, (Stz *) __func__);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // change the op to a send
  vMorphToSend(pOpSpec_isStage, eCrossTopology, eIntraTopology, i_NextStage);

  // send op has <sendkind> argument
  pOpSpec_isStage->vArgTagAdd(C_TAG_SENDKIND,
                              (i8) pOpSpec_isStage->eSendKind(), TN_INTEGER);

  // is there a consumeOp already in the destination stage ?
  OpSpec *pConsumeOpSpec = pDstStageSpec->pFindOpSpec("consumeOp"_Stz);
  if (pConsumeOpSpec) {

    // found a consume op
    // implies that there are 2 stages piping into this same stage
    // eg in unionAll
    // 2 scan stages piping into the output stage
    // add to the consume params
    vArgUpdate_sends(pConsumeOpSpec, eCrossTopology);
  }
  else {
    vAddConsumeOp(pDstStageSpec, eCrossTopology);
  }

}

/*----------------------------------------------------------------------------*/
/* morph the stageNm of the opSpec to send <sendKind>                         */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vMorphToSend(OpSpec *pOpSpec,
                                eYX eCrossTopology,
                                eYO eIntraTopology,
                                i8 i_NextStage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  pOpSpec->vOpNm("send"_Stz);

  Stz *szStgNm = szStageNm(i_NextStage);
  pOpSpec->vIntraTopology(eIntraTopology);
  pOpSpec->vCrossTopology(eCrossTopology, szStgNm, i_NextStage, 0);
}

/*----------------------------------------------------------------------------*/
/* second or subsequent send to stage                                         */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vOtherSend(StageSpec *pStageSpec,
                              OpSpec *pOpSpec,
                              StageSpec *pOtherStageSpec,
                              OpSpec *pOtherOpSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  i8 i_OtherStage = pOtherStageSpec->i_StageSpec();

  eYO eIntraTopology;

  eYX eCrossTopology =
    StageSpec::eCrossTopoCalc(pStageSpec, pOtherStageSpec, eIntraTopology);

  // we already have a send, so just remove the stage
  pStageSpec->vRemove(pOpSpec);
  pOpSpec->vGuardedDelete(CN_OPSPEC, &pOpSpec,
                          CN_PIPELINESPEC, this, (Stz *) __func__);

  Stz *szStgNm = szStageNm(i_OtherStage);

  pOtherOpSpec->vOtherStageOp(szStgNm, i_OtherStage, 0);

  // insert a consume op at the beginning of the stage
  vAddConsumeOp(pOtherStageSpec, eCrossTopology);
}


/*----------------------------------------------------------------------------*/
/* add <sends> arg to consume op                                              */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vArgAdd_sends(OpSpec *pConsumeOpSpec, eYX eCrossTopology)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  i8 iSends = C_UNKNOWN;

  switch (eCrossTopology) {
      
    case YX_ALL_NODES_TO_ORIGIN:
      iSends = AM::pGrid()->nKnownNodes();
      break;

    case YX_ALL_NODES_TO_ALL_PARTITIONS:
      iSends = AM::pGrid()->nKnownNodes();
      break;

    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES:
    case YX_ALL_PARTITIONS_TO_ALL_NODES:
      iSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case YX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case YX_ALL_PARTITIONS_TO_ORIGIN:
      iSends = AM::PTNMAP::nPtnsInGrid();
      break;

    case YX_NEXT_NODE_TO_ORIGIN:
    case YX_ORIGIN_TO_ORIGIN:
    case YX_ORIGIN_TO_ALL_PARTITIONS:
    case YX_ORIGIN_TO_ALL_NODES:
    case YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
    case YX_ORIGIN_TO_NEXT_NODE:
    case YX_ORIGIN_TO_SPECIFIC_PARTITION:
    case YX_SPECIFIC_PARTITION_TO_ORIGIN:
      iSends = 1;
      break;

    default:
      RASSERT(0);
  }

  pConsumeOpSpec->vArgTagAdd(C_TAG_SENDS, iSends, TN_INTEGER);
}

/*----------------------------------------------------------------------------*/
/* update the <sends> arg of consume op                                       */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vArgUpdate_sends(OpSpec *pConsumeOpSpec, eYX eCrossTopology)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  i8 iIncSends = C_UNKNOWN;

  switch (eCrossTopology) {
      
    case YX_ALL_NODES_TO_ORIGIN:
      iIncSends = AM::pGrid()->nKnownNodes();
      break;

    case YX_ALL_NODES_TO_ALL_PARTITIONS:
      iIncSends = AM::pGrid()->nKnownNodes();
      break;

    case YX_ALL_PARTITIONS_TO_ALL_NODES:
      iIncSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iIncSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case YX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case YX_ALL_PARTITIONS_TO_ORIGIN:
      iIncSends = AM::PTNMAP::nPtnsInGrid();
      break;

    case YX_ORIGIN_TO_ORIGIN:
    case YX_ORIGIN_TO_ALL_PARTITIONS:
    case YX_ORIGIN_TO_ALL_NODES:
    case YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
    case YX_ORIGIN_TO_SPECIFIC_PARTITION:
    case YX_SPECIFIC_PARTITION_TO_ORIGIN:
      iIncSends = 1;
      break;

    default:
      RASSERT(0);
  }

  TagSet *pOpArgTags = pConsumeOpSpec->pArgTags();
  i8 iOldSends = pOpArgTags->iValue(C_TAG_SENDS);

  pOpArgTags->vReplace(C_TAG_SENDS, iOldSends + iIncSends);
}

/*----------------------------------------------------------------------------*/
/* add a consumeOp at the beginning of given stage spec                       */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vAddConsumeOp(StageSpec *pStageSpec, eYX eCrossTopology)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  OpSpec *pConsumeOpSpec = new OpSpec("consumeOp"_Stz, pStageSpec);

  pStageSpec->vAdd(pConsumeOpSpec);


  vArgAdd_sends(pConsumeOpSpec, eCrossTopology);
}


/*----------------------------------------------------------------------------*/
/* get name of given stage                                                    */
/*----------------------------------------------------------------------------*/
Stz *PipelineSpec::szStageNm(i8 i_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  StageSpec *pStageSpec = pGet(i_Stage);
  Stz *szNm = pStageSpec->szStageNm();

  return szNm;
}

/*----------------------------------------------------------------------------*/
/*                           Pass 3 : Includes                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pass 3: include other pipelines                                            */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bPass3_includes(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  Stz *szIncludePipNm;

  TokenVec *pIncludeTknVec = nullptr;

  if (m_p_JobTags->bHasName("<include_0>"_Stz)) {
    szIncludePipNm = m_p_JobTags->szValue("<include_0>"_Stz);
    if (!m_sPipelineNm.bSame(szIncludePipNm)) {
      vTrace("Has Include 0: %lls\n"_Stz, szIncludePipNm);
      bPass3_include(szIncludePipNm);
    }
  }

  if (m_p_JobTags->bHasName("<include_1>"_Stz)) {
    szIncludePipNm = m_p_JobTags->szValue("<include_1>"_Stz);
    if (!m_sPipelineNm.bSame(szIncludePipNm)) {
      vTrace("Has Include 1: %lls\n"_Stz, szIncludePipNm);
      bPass3_include(szIncludePipNm);
    }
  }

  if (m_p_JobTags->bHasName("<include_2>"_Stz)) {
    szIncludePipNm = m_p_JobTags->szValue("<include_2>"_Stz);
    if (!m_sPipelineNm.bSame(szIncludePipNm)) {
      vTrace("Has Include 2: %lls\n"_Stz, szIncludePipNm);
      bPass3_include(szIncludePipNm);
    }
  }


  return true;
}

/*----------------------------------------------------------------------------*/
/* include stage(s) from other pipeline                                       */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bPass3_include(Stz *szIncludePipNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  sSTMT sPipelineSmt;

  TokenVec *pIncludeTknVec = nullptr;
  Word *pwMatchedProc = nullptr;

  pIncludeTknVec = TokenVec::pTokenise(szIncludePipNm);

  bool b = Procedure::bTranslateToPipeline(sPipelineSmt.pString(),
                                           pIncludeTknVec,
                                           &pwMatchedProc);

  vTrace("Including: %lls\n"_Stz, sPipelineSmt.szStz());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TagSet *pWrdTagSet = pwMatchedProc->pWrdTagSet();
  pWrdTagSet->vDumpSet();

  Dynamo *pDynamo = new Dynamo(sPipelineSmt.szStz(), pWrdTagSet);

  PipelineSpec *pIncludePipelineSpec = pDynamo->pCompile();

  delete pDynamo;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pWrdTagSet->bHasName("<as_first>"_Stz)) {
    bool b = bPass3_includeFirst();
  }
  else if (pWrdTagSet->bHasName("<as_last>"_Stz)) {
    bool b = bPass3_includeLast();
  }
  else {
    NEVER_REACH;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pIncludeTknVec;

  return true;
}

/*----------------------------------------------------------------------------*/
/* include a stage from another pipeline as a first stage                     */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bPass3_includeFirst(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return true;
}

/*----------------------------------------------------------------------------*/
/* include a stage from another pipeline as a last stage                      */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bPass3_includeLast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return true;
}


/*----------------------------------------------------------------------------*/
/*                            Token Scanning                                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
Token *PipelineSpec::pTkn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return m_p_Scanner->pTkn();
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
TokenVec *PipelineSpec::pTknVec(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return m_p_Scanner->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vTokenAccept(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  m_p_Scanner->vTokenAccept();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
