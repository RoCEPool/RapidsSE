/*============================================================================*/
/* PipelineSpec.cpp -- pipeline Specification                                 */
/*============================================================================*/

#include "BASE.h"

#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "PipelineSpec.hpp"
#include "Dynamo.hpp"
#include "Job.hpp"
#include "Grid.hpp"
#include "TagConstants.h"

#include "SP_PipelineSpec.hpp"

static bool gbPipelineDebug = false;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,PipelineSpec,CN_PIPELINESPEC)
__aNewFunction(PipelineSpec,CN_PIPELINESPEC)
__vDeleteFunction(PipelineSpec,CN_PIPELINESPEC)
__aFooterFunction(PipelineSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipelineSpec::PipelineSpec(Stz*          szPipelineNm,
                           PipelineSpec* p_ParentPipelineSpec,
                           TagSet*       p_JobTags,
                           Scanner*      p_Scanner)
: ListNL_t<StageSpec>(CN_PIPELINESPEC)
, LifeGuard_t<PipelineSpec>(this,CN_PIPELINESPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  m_sPipelineNm = szPipelineNm;
  
  m_p_ParentPipelineSpec = p_ParentPipelineSpec;
  
  // m_oNestedPipeline
  
  m_p_Scanner = p_Scanner;
  m_p_JobTags = p_JobTags;

  _checkInitialised_(CN_PIPELINESPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipelineSpec::~PipelineSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  while (StageSpec* pStageSpec = pPop())
    pStageSpec->vGuardedDelete(CN_STAGESPEC,&pStageSpec,
                               CN_PIPELINESPEC,this,(Stz*)__func__);

  vClear();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* recursively compile a new pipeline                                         */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCompile(PipelineSpec* p_ParentPipelineSpec,
                            TagSet*       p_JobTags,
                            Scanner*      p_Scanner,
                            PipelineSpec*& rp_PipelineSpec)
{
  _static_
  
  if (! (p_Scanner->pTkn()->bSame("PIPELINE"_Stz) ||
         p_Scanner->pTkn()->bSame("PIPELINELOCAL"_Stz))
      )
    return false;
  p_Scanner->vTokenAccept();
  
  Stz* szPipelineNm = p_Scanner->pTkn()->szChars();
  rp_PipelineSpec =
    new PipelineSpec(szPipelineNm,
                     p_ParentPipelineSpec,
                     p_JobTags,
                     p_Scanner);
  p_Scanner->vTokenAccept();
  
  if (szPipelineNm->bSame("nestTest"_Stz))
    gbPipelineDebug = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! rp_PipelineSpec->bPass1_collect())
    return false;
   
  if (! rp_PipelineSpec->bPass2_staging())
    return false;
  
//  if (gbPipelineDebug) {
//    SP_PipelineSpec spPipelineSpec(rp_PipelineSpec);
//    vTrace("\n\nPipelineSpec:\n%s\n"_Stz,spPipelineSpec.sz());
//    vDebugPoint();
//  }
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add stage spec                                                             */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vAddStageSpec(StageSpec* pStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  vAppend(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* recursively compute depth of this pipeline spec                            */
/*----------------------------------------------------------------------------*/
i8 PipelineSpec::iDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (! m_p_ParentPipelineSpec)
    return 0;
  
  return 1 + m_p_ParentPipelineSpec->iDepth();
}

/*----------------------------------------------------------------------------*/
/* find stage matching given name                                             */
/*----------------------------------------------------------------------------*/
StageSpec* PipelineSpec::pFindStageSpec(Stz* szFindNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  i8 nStageSpecs = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStageSpecs; i_Stage++) {
    StageSpec* pStageSpec = pGet(i_Stage);
    
    if (pStageSpec->szStageNm()->bSame(szFindNm)) {
      RASSERT(pStageSpec->i_StageSpec() == i_Stage);
      return pStageSpec;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find nested pipeline matching given name                                   */
/*----------------------------------------------------------------------------*/
PipelineSpec* PipelineSpec::pFindNestedPipelineSpec(Stz* szFindNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  i8 nNested = m_oNestedPipelineSpecs.nItems();
  for (i8 i_Nested = 0; i_Nested < nNested; i_Nested++) {
    PipelineSpec* pPipelineSpec = m_oNestedPipelineSpecs.pGet(i_Nested);
    
    if (pPipelineSpec->szPipelineNm()->bSame(szFindNm)) {
      return pPipelineSpec;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get number of nested pipelines                                             */
/*----------------------------------------------------------------------------*/
i8 PipelineSpec::nNestedPipelineSpecs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  i8 nSpecs = m_oNestedPipelineSpecs.nItems();
    
  return nSpecs;
}

/*----------------------------------------------------------------------------*/
/* get nested pipeline spec                                                   */
/*----------------------------------------------------------------------------*/
PipelineSpec* PipelineSpec::pNestedPipelineSpec(i8 i_NestedPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  PipelineSpec* pPipelineSpec = m_oNestedPipelineSpecs.pGet(i_NestedPipeline);
    
  return pPipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* get parent pipeline spec                                                   */
/*----------------------------------------------------------------------------*/
PipelineSpec* PipelineSpec::pParentPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
      
  return m_p_ParentPipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* get pipeline name                                                          */
/*----------------------------------------------------------------------------*/
Stz* PipelineSpec::szPipelineNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_sPipelineNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* PipelineSpec::pStageSpec(i8 i_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  StageSpec* pStageSpec = pGet(i_Stage);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
      
  if (! (pTkn()->bSame("TAG"_Stz) ||
         pTkn()->bSame("BEGIN"_Stz))
      )
    return false;
  
  while (pTkn()->bSame("TAG"_Stz))
    if (! bCollectTag())
      return false;
  
  if (! bCollectBeginEnd())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one tag                                                            */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (! pTkn()->bSame("TAG"_Stz))
    return false;
  vTokenAccept();
  
  s32 sTagNm = pTkn()->szChars();
  vTokenAccept();

  if (! pTkn()->bSame("="_Stz))
    return false;
  vTokenAccept();

  sSTMT sTagValue = pTkn()->szChars();
  vTokenAccept();

  while (! pTkn()->bSame("ENDTAG"_Stz)) {
    vTokenAccept();
  }
  vTokenAccept();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  m_p_JobTags->vInsert(sTagNm,sTagValue);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect a BEGIN { STAGE ... } END sequence                                 */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectBeginEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (! pTkn()->bSame("BEGIN"_Stz))
    return false;
  vTokenAccept();
    
  while (pTkn()->bSame("STAGE"_Stz))
    if (! bCollectStage())
      return false;
  
  while (pTkn()->bSame("PIPELINE"_Stz))
    if (! bCollectPipeline())
      return false;
  
  if (! pTkn()->bSame("END"_Stz) )
    return false;
  vTokenAccept();

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect stage                                                              */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
    
  if (! pTkn()->bSame("STAGE"_Stz))
    return false;
  vTokenAccept();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  StageSpec* p_StageSpec = new StageSpec(pTkn()->szChars(),this);
  vTokenAccept();

  this->vAddStageSpec(p_StageSpec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  if (! bCollectTopology(p_StageSpec))
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (bTokenMaybeOp(pTkn()))
    bCollectOpSequence(p_StageSpec);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect topology                                                           */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectTopology(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (! pTkn()->bSame("ON"_Stz))
    return false;
  vTokenAccept();
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :
  
  if (pTkn()->bSame("ALL"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAll(p_StageSpec);
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
bool PipelineSpec::bCollectOpSequence(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  while (bTokenMaybeOp(pTkn())) {

    if (! bCollectOp(p_StageSpec))
      return false;
    
    if (! pTkn()->bSame("|"_Stz))
      break;
    vTokenAccept();
  }
   
  return true;
}
/*----------------------------------------------------------------------------*/
/* collect one operator (and optionally its arguments)                        */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectOp(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  OpSpec* pOpSpec = new OpSpec(pTkn()->szChars(),p_StageSpec);
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

  if (pTkn()->bSame("RIGHT"_Stz)) {
    pOpSpec->vAppendRight(true);
    vTokenAccept();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! bCollectOpArgs(p_StageSpec,pOpSpec))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect operator arguments                                                 */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectOpArgs(StageSpec* p_StageSpec, OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  UNUSED(p_StageSpec);
  
  for (i8 i_Arg = 0; /*none*/; i_Arg++) {
    
    if (! bTokenMaybeOpArg(pTkn()))
      break;
    
    s32 sArgName;
    sArgName.vSprint("<arg_%lld>"_Stz,i_Arg);
    
    sSTMT sArgValue(pTkn()->szChars());
    
    pOpSpec->vArgTagAdd(sArgName,sArgValue,TN_STRING);
    
    vTokenAccept();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect nested pipeline                                                    */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  if (! (pTkn()->bSame("PIPELINE"_Stz)) )
    return false;

  
  PipelineSpec* p_PipelineSpecNested;
  
  if (!  PipelineSpec::bCompile(this,
                                m_p_JobTags,
                                m_p_Scanner,
                                p_PipelineSpecNested))
    return false;

  m_oNestedPipelineSpecs.vAppend(p_PipelineSpecNested);
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* recognise topology: ON ALL _^ [ NODES | PARTITIONS ]                       */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAll(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
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
/* recognise topology: ON ALL NODE _^                                         */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAllNodes(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :
  
  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    
    p_StageSpec->vTopology(PO_ALL_NODES);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ALL PARTITIONS _^                                   */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAllPartitions(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :
  
  if (pTkn()->bSame("ON"_Stz)) {
    vTokenAccept();
    if (pTkn()->bSame("ORIGIN"_Stz)) {
      
      vTokenAccept();
      if (pTkn()->bSame(":"_Stz)) {
        vTokenAccept();

        p_StageSpec->vTopology(PO_ALL_PARTITIONS_ON_ORIGIN);
        return true;
      }
    }
    else
      return false;
  }
  
  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    p_StageSpec->vTopology(PO_ALL_PARTITIONS);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON NODE <nodeNo> _^                                    */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnNode(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  s32 sLocus(pTkn()->szChars());
  vTokenAccept();
  
  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    p_StageSpec->vTopology(PO_SPECIFIC_NODE);
    i8 i_Node = sLocus.szStz()->iToI8();
    if ( i_Node > AM::pGrid()->nKnownNodes() )
      return false;
    p_StageSpec->vSpecificNode(i_Node);
    return true;
  }
  
  return false;
  
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ORIGIN _^                                           */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnOrigin(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    
    p_StageSpec->vTopology(PO_ORIGIN);
    return true;
  }
  else {
    s32 sLocus(pTkn()->szChars());
    vTokenAccept();
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON PARTITION <tPtnNo> _^                               */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnPartition(StageSpec* p_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  s32 sLocus(pTkn()->szChars());
  vTokenAccept();
  
  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    p_StageSpec->vTopology(PO_SPECIFIC_PARTITION);
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
bool PipelineSpec::bTokenMaybeOp(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (   pTkn->bSame("|"_Stz)
      || pTkn->bSame("END"_Stz)
      || pTkn->bSame("PIPELINE"_Stz)
      || pTkn->bSame("STAGE"_Stz)
      )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* is token a potential operator argument?                                    */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bTokenMaybeOpArg(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (   pTkn->bSame("|"_Stz)
      || pTkn->bSame("END"_Stz)
      || pTkn->bSame("PIPELINE"_Stz)
      || pTkn->bSame("STAGE"_Stz)
      )
    return false;

  return true;
}


/*----------------------------------------------------------------------------*/
/*                           Pass 2 : Staging                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pass 2 staging                                                             */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bPass2_staging(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
    
  // get each stageSpec of pipelineSpec
  i8 nStageSpecs = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStageSpecs; i_Stage++) {
    StageSpec* pStageSpec = this->pGet(i_Stage);
    RASSERT(pStageSpec->i_StageSpec() == i_Stage);

    // get each opSpec of stageSpec
    i8 nOpSpecs = pStageSpec->nItems();
    for (i8 i_Op = 0; i_Op < nOpSpecs; i_Op++) {
      OpSpec* pOpSpec = pStageSpec->pGet(i_Op);
      RASSERT(pOpSpec->iOpSpec() == i_Op);
      
      Stz* szName = pOpSpec->szOpNm();
      
      // 0) is the name a special case?
      if (szName->bSame("next"_Stz)) {
        StageSpec* pStageSpecNext = this->pGet(i_Stage + 1);
        if (! pStageSpecNext)
          return false;
        vNameIsStage(pStageSpec,pOpSpec,pStageSpecNext);
        continue;
      }
      
      if (szName->bSame("first"_Stz)) {
        StageSpec* pStageSpecFirst = this->pGet(0);
        if (! pStageSpecFirst)
          return false;
        vNameIsStage(pStageSpec,pOpSpec,pStageSpecFirst);
        continue;
      }
      
      if (szName->bSame("last"_Stz)) {
        StageSpec* pStageSpecLast = this->pGet(nOpSpecs - 1);
        if (! pStageSpecLast)
          return false;
        vNameIsStage(pStageSpec,pOpSpec,pStageSpecLast);
        continue;
      }
      
      // 1) is the name in the opSpec actually a stage in this pipeline?
      StageSpec* pStageSpecSend = this->pFindStageSpec(szName);
      if (pStageSpecSend) {
        vNameIsStage(pStageSpec,pOpSpec,pStageSpecSend);
        continue;
      }
      
      //
      // 2) is the name in the opSpec actually a nested pipeline?
      //
      PipelineSpec* pPipelineSpecCalled = this->pFindNestedPipelineSpec(szName);
      if (pPipelineSpecCalled) {
        vNameIsPipeline(pStageSpec,pOpSpec,pPipelineSpecCalled);
        continue;
      }
      
      // 3) the name in the opSpec is a known op
      vNameIsOp(pStageSpec,pOpSpec);

    } // each op
    
  } // each stage
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* add <sends> arg to consume op                                              */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vArgAdd_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  i8 iSends = C_UNKNOWN;
  
  switch (eCrossTopo) {
    case PX_ALL_NODES_TO_ORIGIN:
      iSends = AM::pGrid()->nKnownNodes();
      break;
      
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
      iSends = AM::pGrid()->nKnownNodes();
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      iSends = AM::PTNMAP::nPtnsPerNode();
      break;
      
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iSends = AM::PTNMAP::nPtnsPerNode();
      break;
     
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
      iSends = AM::PTNMAP::nPtnsInGrid();
      break;
      
    case PX_ORIGIN_TO_ORIGIN:
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_ALL_NODES:
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
      iSends = 1;
      break;
      
    default:
      RASSERT(0);
  }
  
  pConsumeOpSpec->vArgTagAdd(C_TAG_SENDS,iSends,TN_INTEGER);
}

/*----------------------------------------------------------------------------*/
/* update the <sends> arg of consume op                                       */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vArgUpdate_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  i8 iIncSends = C_UNKNOWN;

  switch (eCrossTopo) {
    case PX_ALL_NODES_TO_ORIGIN:
      iIncSends = AM::pGrid()->nKnownNodes();
      break;

    case PX_ALL_NODES_TO_ALL_PARTITIONS:
      iIncSends = AM::pGrid()->nKnownNodes();
      break;

    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      iIncSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iIncSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
      iIncSends = AM::PTNMAP::nPtnsInGrid();
      break;

    case PX_ORIGIN_TO_ORIGIN:
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_ALL_NODES:
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
      iIncSends = 1;
      break;

    default:
      RASSERT(0);
  }

  TagSet* pOpArgTags = pConsumeOpSpec->pArgTags();
  i8 iOldSends = pOpArgTags->iValue(C_TAG_SENDS);

  pOpArgTags->vReplace(C_TAG_SENDS,iOldSends+iIncSends);
}

/*----------------------------------------------------------------------------*/
/* morph the name of the opSpec to call ????                                  */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vMorphToCall(OpSpec* pOpSpec, PipelineSpec* pPipelineSpecCalled)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  UNUSED(pPipelineSpecCalled);
  
  s32 sName = pOpSpec->szOpNm();

  pOpSpec->vOpNm("call"_Stz);

  pOpSpec->vArgTagAdd("<nested>"_Stz,sName,TN_STRING);
}

/*----------------------------------------------------------------------------*/
/* morph the stageNm of the opSpec to send <sendKind>                         */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vMorphToSend(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_NextStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  pOpSpec->vOpNm("send"_Stz);
  
  pOpSpec->vCrossTopo(eCrossTopo,i_NextStage,0);
}

/*----------------------------------------------------------------------------*/
/* name is a known operator                                                   */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vNameIsOp(StageSpec* pStageSpec, OpSpec* pOpSpec_isOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  i8 i_Op = pOpSpec_isOp->iOpSpec();
  i8 i_Stage = pStageSpec->i_StageSpec();
  ePX eCrossTopo = StageSpec::eCrossTopo(pStageSpec, pStageSpec);
    
  pOpSpec_isOp->vCrossTopo(eCrossTopo,i_Stage,i_Op+1);

}

/*----------------------------------------------------------------------------*/
/* name is a known pipeline                                                   */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vNameIsPipeline(StageSpec*    pStageSpec,
                                   OpSpec*       pOpSpec,
                                   PipelineSpec* pPipelineSpecCalled)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  UNUSED(pStageSpec);
  
  vMorphToCall(pOpSpec,pPipelineSpecCalled);
}

/*----------------------------------------------------------------------------*/
/* name is a known stage                                                      */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vNameIsStage(StageSpec* pStageSpec,
                                OpSpec*    pOpSpec_isStage,
                                StageSpec* pNextStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  // is there a send already? This would be the case if one stage is
  // piped into multiple stage e.g a unionAll
  OpSpec* pOpSpecSend = pStageSpec->pFindOpSpec("send"_Stz);
  
  if (! pOpSpecSend)
    
    // spec for op send does not yet exist in stage spec
    vStageFirstSend(pStageSpec,pOpSpec_isStage,pNextStageSpec);
  
  else
  
    // spec for op send already exists in stage spec
    vStageSubsequentSend(pStageSpec,pOpSpec_isStage,pNextStageSpec,pOpSpecSend);

}

/*----------------------------------------------------------------------------*/
/* add a consumeOp at the beginning of given stage spec                       */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vStageConsumeOpInsert(StageSpec* pStageSpec, ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  OpSpec* pConsumeOpSpec = new OpSpec("consumeOp"_Stz,pStageSpec);
  
  pStageSpec->vAdd(pConsumeOpSpec);
  
  
  vArgAdd_sends(pConsumeOpSpec,eCrossTopo);
}

/*----------------------------------------------------------------------------*/
/* first send to stage                                                        */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vStageFirstSend(StageSpec* pStageSpec, OpSpec* pOpSpec_isStage,
                             StageSpec* pNextStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  i8 i_Op = pOpSpec_isStage->iOpSpec();
  i8 i_NextStage = pNextStageSpec->i_StageSpec();

  ePX eCrossTopo = StageSpec::eCrossTopo(pStageSpec, pNextStageSpec);

  // SPECIAL CASE (to discuss with Alka)
  if (eCrossTopo == PX_ORIGIN_TO_ORIGIN && i_NextStage == 0) {
    // add parameter to the prev sink op
    OpSpec *pPrevOpSpec = pStageSpec->pGet(i_Op - 1);
    pPrevOpSpec->vCrossTopo(eCrossTopo, i_NextStage, 0);

    // remove the last (stage) OpSpec from the OpSpecVec
    RASSERT(i_Op == pStageSpec->nItems() - 1);
    pStageSpec->vRemove(pOpSpec_isStage);
    //delete pOpSpec_isStage;
    pOpSpec_isStage->vGuardedDelete(CN_OPSPEC,&pOpSpec_isStage,
                                    CN_PIPELINESPEC,this,(Stz*)__func__);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // change the op to a send
  vMorphToSend(pOpSpec_isStage,eCrossTopo,i_NextStage);
  
  // send op has <sendkind> argument
  pOpSpec_isStage->vArgTagAdd(C_TAG_SENDKIND,(i8)pOpSpec_isStage->eSendKind(),TN_INTEGER);
  
  // is there a consumeOp already in the destination stage ?
  OpSpec* pConsumeOpSpec = pNextStageSpec->pFindOpSpec("consumeOp"_Stz);
  if (pConsumeOpSpec) {

    // found a consume op
    // implies that there are 2 stages piping into this same stage
    // eg in unionAll
    // 2 scan stages piping into the output stage
    // add to the consume params
    vArgUpdate_sends(pConsumeOpSpec, eCrossTopo);
  }
  else {
    vStageConsumeOpInsert(pNextStageSpec,eCrossTopo);
  }

}

/*----------------------------------------------------------------------------*/
/* second or subsequent send to stage                                         */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vStageSubsequentSend(StageSpec* pStageSpec, OpSpec* pOpSpec,
                                  StageSpec* pNextStageSpec, OpSpec* pOpSpecSend)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  //i8 i_Op = pOpSpec->iOpSpec();
  i8 i_NextStage = pNextStageSpec->i_StageSpec();

  ePX eCrossTopo = StageSpec::eCrossTopo(pStageSpec, pNextStageSpec);

  // we already have a send, so just remove the stage
  pStageSpec->vRemove(pOpSpec);
  pOpSpec->vGuardedDelete(CN_OPSPEC,&pOpSpec,
                          CN_PIPELINESPEC,this,(Stz*)__func__);

  pOpSpecSend->vOtherStage(i_NextStage);
  pOpSpecSend->vOtherOp(0);

  // insert a consume op at the beginning of the stage
  vStageConsumeOpInsert(pNextStageSpec,eCrossTopo);
}


/*----------------------------------------------------------------------------*/
/*                            Token Scanning                                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
Token* PipelineSpec::pTkn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_p_Scanner->pTkn();
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
TokenVec* PipelineSpec::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_p_Scanner->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vTokenAccept(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  m_p_Scanner->vTokenAccept();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
