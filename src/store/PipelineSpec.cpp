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
PipelineSpec::PipelineSpec(Stz* szPipelineNm, Dynamo* p_Dynamo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  m_sPipelineNm = szPipelineNm;
  
  // m_oNestedPipeline
  
  m_p_Dynamo = p_Dynamo;
      
  _checkInitialised_(CN_PIPELINESPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipelineSpec::~PipelineSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
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
  
  vAdd(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* add stage spec                                                             */
/*----------------------------------------------------------------------------*/
Stz* PipelineSpec::szPipelineNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_sPipelineNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* find stage matching given name                                             */
/*----------------------------------------------------------------------------*/
StageSpec* PipelineSpec::pFindStageSpec(Stz* szFindNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    StageSpec* pStageSpec = pGet(i_Stage);
    
    if (pStageSpec->szStageNm()->bSame(szFindNm)) {
      RASSERT(pStageSpec->i_StageSpec() == i_Stage);
      return pStageSpec;
    }
  }
  
  return nullptr;
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print pipeline spec                                                        */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  vTrace("pipeline %s\n"_Stz,m_sPipelineNm.szStz());
  vTrace("begin\n"_Stz);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++)
    pGet(i_Stage)->vPrint();

  vTrace("\n"_Stz);
  vTrace("end\n"_Stz);
}





/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                           Pass 1 : Collect                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pass 1: collect the pipeline statement                                     */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bPass1_collect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
    
  bool bResult = true;
  
  while (m_p_Dynamo->m_pTkn->bSame("TAG"_Stz)) {
    bResult = bCollectTag();
    if (! bResult)
      return false;
  }
  
  if (m_p_Dynamo->m_pTkn->bSame("BEGIN"_Stz)) {
    bResult = bCollectBegin();
    if (! bResult)
      return false;
  }
  else {
    return false;
  }
  
  if ( ! m_p_Dynamo->m_pTkn->bSame("STAGE"_Stz)) {
    return false;
  }
  
  while (m_p_Dynamo->m_pTkn->bSame("STAGE"_Stz)) {
    bResult = bCollectStage();
    if ( ! bResult )
      return false;
  }
  
  bResult = bCollectEnd();

  return bResult;
}

/*----------------------------------------------------------------------------*/
/* collect the pipeline BEGIN                                                 */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  RASSERT(m_p_Dynamo->m_pTkn->bSame("BEGIN"_Stz));
  m_p_Dynamo->vTokenAccept();
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect the pipeline END                                                   */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if ( ! m_p_Dynamo->m_pTkn->bSame("END"_Stz) )
    return false;
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one operator (and optionally its arguments)                        */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_OpSpec = m_p_Dynamo->m_p_StageSpec->nItems();
  OpSpec* pOpSpec = new OpSpec(m_p_Dynamo->m_pTkn->szChars(),i_OpSpec,m_p_Dynamo->m_p_StageSpec);
  m_p_Dynamo->m_p_StageSpec->vAddOpSpec(pOpSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_p_Dynamo->vTokenAccept();
  
  if (m_p_Dynamo->m_pTkn->bSame("ROBIN"_Stz)) {
    pOpSpec->vSendKind(XK_ROBIN);
    m_p_Dynamo->vTokenAccept();
  }
  else if (m_p_Dynamo->m_pTkn->bSame("LOCALROBIN"_Stz)) {
    pOpSpec->vSendKind(XK_LOCALROBIN);
    m_p_Dynamo->vTokenAccept();
  }
  else if (m_p_Dynamo->m_pTkn->bSame("HASHED"_Stz)) {
    pOpSpec->vSendKind(XK_HASHED);
    m_p_Dynamo->vTokenAccept();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (m_p_Dynamo->m_pTkn->bSame("RIGHT"_Stz)) {
    pOpSpec->vAppendRight(true);
    m_p_Dynamo->vTokenAccept();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! bCollectOpArgs(pOpSpec))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect operator arguments                                                 */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectOpArgs(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  i8 i_Arg = 0;
  for (;;) {
    
    if (m_p_Dynamo->m_pTkn->bSame("|"_Stz)     ||
        m_p_Dynamo->m_pTkn->bSame("STAGE"_Stz) ||
        m_p_Dynamo->m_pTkn->bSame("END"_Stz)) {
      break;
    }
    
    // add an argument
    
    s32 sArgName;
    sArgName.vSprint("<arg_%lld>"_Stz,i_Arg++);
    
    sSTMT sArgValue ( m_p_Dynamo->m_pTkn->szChars() );
    
    pOpSpec->vArgTagAdd(sArgName,sArgValue,TN_STRING);
    
    m_p_Dynamo->vTokenAccept();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect operator-pipe sequence                                             */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectOpSequence(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  for (;;) {
    
    if (! bTokenIsOp(m_p_Dynamo->m_pTkn))
      break;

    bCollectOp();
    
    if (! m_p_Dynamo->m_pTkn->bSame("|"_Stz))
      break;
    
    m_p_Dynamo->vTokenAccept();
  }
   
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect stage                                                              */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
    
  RASSERT(m_p_Dynamo->m_pTkn->bSame("STAGE"_Stz));
  m_p_Dynamo->vTokenAccept();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_StageSpec = this->nItems();
  m_p_Dynamo->m_p_StageSpec = new StageSpec(m_p_Dynamo->m_pTkn->szChars(),i_StageSpec,this);
  this->vAddStageSpec(m_p_Dynamo->m_p_StageSpec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_p_Dynamo->vTokenAccept();
  
  bool bResult = bCollectTopology();
  if ( ! bResult )
    return false;
  
  if (bTokenIsOp(m_p_Dynamo->m_pTkn))
    bCollectOpSequence();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one tag                                                            */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  RASSERT(m_p_Dynamo->m_pTkn->bSame("TAG"_Stz));
  m_p_Dynamo->vTokenAccept();
  
  s32 sTagNm = m_p_Dynamo->m_pTkn->szChars();
  m_p_Dynamo->vTokenAccept();

  if (! m_p_Dynamo->m_pTkn->bSame("="_Stz))
    return false;
  m_p_Dynamo->vTokenAccept();

  sSTMT sTagValue = m_p_Dynamo->m_pTkn->szChars();
  m_p_Dynamo->vTokenAccept();

  while (! m_p_Dynamo->m_pTkn->bSame("ENDTAG"_Stz)) {
    m_p_Dynamo->vTokenAccept();
  }
  m_p_Dynamo->vTokenAccept();

  if (AM::bVerbose())
    vTrace("Tag recognised: %s %s\n"_Stz,sTagNm.szStz(),sTagValue.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TagSet* pJobTags = m_p_Dynamo->m_p_PipelineJob->pJobTags();
  
  pJobTags->vInsert(sTagNm,sTagValue);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect topology                                                           */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCollectTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (m_p_Dynamo->m_pTkn->bSame("ON"_Stz))
    m_p_Dynamo->vTokenAccept();
  else
    return false;
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :
  
  if (m_p_Dynamo->m_pTkn->bSame("ALL"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    return bRecogniseOnAll();
  }
 
  // -- -- -- -- -- -- -- -- -- -- --
  // on node <nodeNm> :
  
  else if (m_p_Dynamo->m_pTkn->bSame("NODE"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    return bRecogniseOnNode();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on partition <partitionNo> :
  
  else if (m_p_Dynamo->m_pTkn->bSame("PARTITION"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    return bRecogniseOnPartition();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  else if (m_p_Dynamo->m_pTkn->bSame("ORIGIN"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    return bRecogniseOnOrigin();
  }
  else {

    // -- -- -- -- -- -- -- -- -- -- --
    
    return false;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ALL _^ [ NODES | PARTITIONS ]                       */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :
  
  if (m_p_Dynamo->m_pTkn->bSame("NODES"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    return bRecogniseOnAllNodes();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :
  
  else if (m_p_Dynamo->m_pTkn->bSame("PARTITIONS"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    return bRecogniseOnAllPartitions();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ALL NODE _^                                         */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAllNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :
  
  if (m_p_Dynamo->m_pTkn->bSame(":"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    
    m_p_Dynamo->m_p_StageSpec->vTopology(PO_ALL_NODES);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ALL PARTITIONS _^                                   */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnAllPartitions(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :
  
  if (m_p_Dynamo->m_pTkn->bSame("ON"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    if (m_p_Dynamo->m_pTkn->bSame("ORIGIN"_Stz)) {
      
      m_p_Dynamo->vTokenAccept();
      if (m_p_Dynamo->m_pTkn->bSame(":"_Stz)) {
        m_p_Dynamo->vTokenAccept();

        m_p_Dynamo->m_p_StageSpec->vTopology(PO_ALL_PARTITIONS_ON_ORIGIN);
        return true;
      }
    }
    else
      return false;
  }
  
  if (m_p_Dynamo->m_pTkn->bSame(":"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    m_p_Dynamo->m_p_StageSpec->vTopology(PO_ALL_PARTITIONS);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON NODE <nodeNo> _^                                    */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  s32 sLocus(m_p_Dynamo->m_pTkn->szChars());
  m_p_Dynamo->vTokenAccept();
  
  if (m_p_Dynamo->m_pTkn->bSame(":"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    m_p_Dynamo->m_p_StageSpec->vTopology(PO_SPECIFIC_NODE);
    i8 i_Node = sLocus.szStz()->iToI8();
    if ( i_Node > AM::pGrid()->nConnectedNodes() )
      return false;
    m_p_Dynamo->m_p_StageSpec->vSpecificNode(i_Node);
    return true;
  }
  
  return false;
  
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON PARTITION <tPtnNo> _^                               */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  s32 sLocus(m_p_Dynamo->m_pTkn->szChars());
  m_p_Dynamo->vTokenAccept();
  
  if (m_p_Dynamo->m_pTkn->bSame(":"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    m_p_Dynamo->m_p_StageSpec->vTopology(PO_SPECIFIC_PARTITION);
    tPN tPtnNo = sLocus.szStz()->iToI8();
    if (tPtnNo >= AM::nPtnsInGrid())
      return false;
    
    m_p_Dynamo->m_p_StageSpec->vSpecificPtnNo(tPtnNo);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ORIGIN _^                                           */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bRecogniseOnOrigin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (m_p_Dynamo->m_pTkn->bSame(":"_Stz)) {
    m_p_Dynamo->vTokenAccept();
    
    m_p_Dynamo->m_p_StageSpec->vTopology(PO_ORIGIN);
    return true;
  }
  else {
    s32 sLocus(m_p_Dynamo->m_pTkn->szChars());
    m_p_Dynamo->vTokenAccept();
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is token a known operator?                                                 */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bTokenIsOp(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  if (pTkn->bSame("|"_Stz))
    return false;
  
  if (pTkn->bSame("end"_Stz))
    return false;
  
  if (pTkn->bSame("stage"_Stz))
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
      RASSERT(pOpSpec->i_OpSpec() == i_Op);
      
      Stz* szName = pOpSpec->szOpNm();
      
      // 1) is the name in the opSpec actually a stage in this pipeline?
      StageSpec* pStageSpecDst = this->pFindStageSpec(szName);
      if (pStageSpecDst) {
        vNameIsStage(pStageSpec,pOpSpec,pStageSpecDst);
        continue;
      }
      
      //
      // 2) (add test for pipeline here)
      //
      // ... vNameIsPipeline(...);
      //
      
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
      iSends = AM::pGrid()->nConnectedNodes();
      break;
      
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
      iSends = AM::pGrid()->nConnectedNodes();
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      iSends = AM::nPtnsPerTable();
      break;
      
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iSends = AM::nPtnsPerTable();
      break;
     
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
      iSends = AM::nPtnsInGrid();
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
      iIncSends = AM::pGrid()->nConnectedNodes();
      break;

    case PX_ALL_NODES_TO_ALL_PARTITIONS:
      iIncSends = AM::pGrid()->nConnectedNodes();
      break;

    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      iIncSends = AM::nPtnsPerTable();
      break;

    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iIncSends = AM::nPtnsPerTable();
      break;

    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
      iIncSends = AM::nPtnsInGrid();
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
/* morph the stageNm of the opSpec to send <sendKind>                         */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vMorphToSend(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_NextStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  pOpSpec->vOpNm("send"_Stz);
  
  vTopoTagsAdd(pOpSpec, eCrossTopo, i_NextStage, 0);
}

/*----------------------------------------------------------------------------*/
/* name is a known operator                                                   */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vNameIsOp(StageSpec* pStageSpec, OpSpec* pOpSpec_isOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  i8 i_Op = pOpSpec_isOp->i_OpSpec();
  i8 i_Stage = pStageSpec->i_StageSpec();
  ePX eCrossTopo = StageSpec::eCrossTopo(pStageSpec, pStageSpec);
  
  vTopoTagsAdd(pOpSpec_isOp,eCrossTopo,i_Stage,i_Op+1);
}

/*----------------------------------------------------------------------------*/
/* name is a known pipeline                                                   */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vNameIsPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  NOT_IMPLEMENTED_YET;
}

/*----------------------------------------------------------------------------*/
/* name is a known stage                                                      */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vNameIsStage(StageSpec* pStageSpec, OpSpec* pOpSpec_isStage,
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
/* insert a consumeOp at the beginning of given stage spec                    */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vStageConsumeOpInsert(StageSpec* pStageSpec, ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  OpSpec* pConsumeOpSpec = new OpSpec("consumeOp"_Stz,0,pStageSpec);
  
  pStageSpec->vInsert(pConsumeOpSpec);
  
  pStageSpec->vRenumberOpSpecs();
  
  vArgAdd_sends(pConsumeOpSpec,eCrossTopo);
}

/*----------------------------------------------------------------------------*/
/* first send to stage                                                        */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vStageFirstSend(StageSpec* pStageSpec, OpSpec* pOpSpec_isStage,
                             StageSpec* pNextStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  i8 i_Op = pOpSpec_isStage->i_OpSpec();
  i8 i_NextStage = pNextStageSpec->i_StageSpec();

  ePX eCrossTopo = StageSpec::eCrossTopo(pStageSpec, pNextStageSpec);

  // SPECIAL CASE (to discuss with Alka)
  if (eCrossTopo == PX_ORIGIN_TO_ORIGIN && i_NextStage == 0) {
    // add parameter to the prev sink op
    OpSpec *pPrevOpSpec = pStageSpec->pGet(i_Op - 1);
    vTopoTagsUpdate(pPrevOpSpec, eCrossTopo, i_NextStage, 0);

    // remove the last (stage) OpSpec from the OpSpecVec
    RASSERT(i_Op == pStageSpec->nItems() - 1);
    pStageSpec->vRemove(pOpSpec_isStage);
    delete pOpSpec_isStage;
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
  i8 i_Op = pOpSpec->i_OpSpec();
  i8 i_NextStage = pNextStageSpec->i_StageSpec();

  ePX eCrossTopo = StageSpec::eCrossTopo(pStageSpec, pNextStageSpec);

  TagSet* pTopologyTagSet = pOpSpecSend->pTopoTags();

  ePX eOpSpecCrossTopo = (ePX)pTopologyTagSet->iValue(C_TAG_CROSSTOPO);

  // this needs to checked at compile time
  RASSERT(eCrossTopo == eOpSpecCrossTopo);

  // we already have a send, so just remove the stage
  pStageSpec->vRemove(pOpSpec);
  delete pOpSpec;

  // add otherStage and otherOp tags into the send
  pOpSpecSend->vTopoTagAdd(C_TAG_OTHERSTAGE,(i8)i_NextStage,TN_INTEGER);
  pOpSpecSend->vTopoTagAdd(C_TAG_OTHEROP,(i8)0,TN_INTEGER);


  // insert a consume op at the beginning of the stage
  vStageConsumeOpInsert(pNextStageSpec,eCrossTopo);
}

/*----------------------------------------------------------------------------*/
/* add topo tags for the opSpec                                               */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vTopoTagsAdd(OpSpec* pOpSpec,
                          ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  // cross topology of the send op
  pOpSpec->vTopoTagAdd(C_TAG_CROSSTOPO,(i8)eCrossTopo,TN_INTEGER);
  
  // next stage for the send
  pOpSpec->vTopoTagAdd(C_TAG_NEXTSTAGE,(i8)i_NextStage,TN_INTEGER);
  
  //  op of the next stage for the send
  pOpSpec->vTopoTagAdd(C_TAG_NEXTOP,(i8)i_NextOp,TN_INTEGER);
}

/*----------------------------------------------------------------------------*/
/* update topo tags for the opSpec                                            */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vTopoTagsUpdate(OpSpec* pOpSpec,
                             ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);

  // cross topology of the send op
  pOpSpec->vTopoTagReplace(C_TAG_CROSSTOPO,(i8)eCrossTopo,TN_INTEGER);
  
  // next stage for the send
  pOpSpec->vTopoTagReplace(C_TAG_NEXTSTAGE,(i8)i_NextStage,TN_INTEGER);
  
  // op of the next stage for the send
  pOpSpec->vTopoTagReplace(C_TAG_NEXTOP,(i8)i_NextOp,TN_INTEGER);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
