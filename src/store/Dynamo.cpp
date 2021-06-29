/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "Grid.hpp"
#include "OpSpec.hpp"
#include "PipelineSpec.hpp"
#include "Pipeline.hpp"
#include "Dynamo.hpp"
#include "Job.hpp"
#include "StageSpec.hpp"

#include "TagConstants.h"

#include "SP_TknVecStr.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Dynamo,CN_DYNAMO)
__aNewFunction(Dynamo,CN_DYNAMO)
__vDeleteFunction(Dynamo,CN_DYNAMO)
__aFooterFunction(Dynamo)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dynamo::Dynamo(Stz* szPipelineStmt, Job* p_PipelineJob)
: Object(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  m_p_PipelineJob = p_PipelineJob;
  
  m_pTknVec = new TokenVec(szPipelineStmt);
  m_i_Tkn   = 0;
  m_pTkn    = nullptr;
  
  m_p_PipelineSpec = nullptr;
  m_p_StageSpec    = nullptr;
  
  _checkInitialised_(CN_DYNAMO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamo::~Dynamo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  m_pTknVec->vSetAllTokens();
  delete m_pTknVec;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* translate token vector into a pipeline statement                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bToPipelineStmt(String* pAsPipelineStmt,
                             TokenVec* pFromTokenVec)
{
  _static_
  
  i8 nTkns = pFromTokenVec->nTokens();
  
  Stz* szOpNm;
  Stz* szOpArg;
  
  char cStageStart   = 'A';
  char cStageCurrent = 'A';

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pAsPipelineStmt->vCopy(
    "pipeline unnamed\n"
    "begin\n"
    ""_Stz);

  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on origin :\n"
    "    start\n"
    "    | periodic once\n"
    "    | stageB\n"
    ""_Stz,
  cStageStart);

  ++ cStageCurrent;
  
  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on all partitions :\n"
    ""_Stz,
    cStageCurrent);
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Tkn = 0; i_Tkn < nTkns; /*none*/) {

    szOpNm = pFromTokenVec->pGet(i_Tkn++)->szChars();

    // -- -- -- -- -- -- -- -- -- --
    
    if (szOpNm->bSame("scan"_Stz)) {
      
      szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();

      pAsPipelineStmt->vCatSprint("    scan %s\n"_Stz,szOpArg);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
      }
      
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("filter"_Stz)) {
      
      pAsPipelineStmt->vCatSprint("    | filter 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint(" %s"_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint(" <>\"\n"_Stz);
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("project"_Stz)) {
      
      pAsPipelineStmt->vCatSprint("    | project 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint("%s "_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint("\"\n"_Stz);
            
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("aggreg"_Stz)) {
      
      pAsPipelineStmt->vCatSprint(
        "    | stage%c\n"
        ""_Stz,
        cStageCurrent + 1);

      ++ cStageCurrent;
      
      pAsPipelineStmt->vCatSprint(
        "  stage stage%c on origin :\n"
        ""_Stz,
        cStageCurrent);
      
      pAsPipelineStmt->vCatSprint("    aggreg 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint("%s "_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint("\"\n"_Stz);
            
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else {
      
      return false;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pAsPipelineStmt->vCatSprint(
    "    | stage%c\n"
    ""_Stz,
    cStageCurrent + 1);
  
  ++ cStageCurrent;
  
  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on origin :\n"
    "    output\n"
    "    | sink\n"
    "    | stage%c\n"
    ""_Stz,
    cStageCurrent, cStageStart);

  pAsPipelineStmt->vConcat(
    "end ;\n"
    ""_Stz);
  
  vTrace("bToPipelineStmt : \n%s\n"_Stz,pAsPipelineStmt->szStz());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get cross topology between two stages                                      */
/*----------------------------------------------------------------------------*/
ePX Dynamo::eCrossTopo(StageSpec*  pFromStageSpec, StageSpec* pToStageSpec)
{
  _static_
    
  ePO eFromTopo = pFromStageSpec->eTopology();
  ePO eToTopo   = pToStageSpec->eTopology();
  
  switch (eFromTopo) {
          
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all nodes to ...
    case PO_ALL_NODES:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_ALL_NODES_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ALL_NODES_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  RASSERT(0);
        case PO_ORIGIN:             return PX_ALL_NODES_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ALL_NODES_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ALL_NODES_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all partitions to ...
    case PO_ALL_PARTITIONS:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_ALL_PARTITIONS_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  RASSERT(0);
        case PO_ORIGIN:             return PX_ALL_PARTITIONS_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ALL_PARTITIONS_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ALL_PARTITIONS_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all partitions on origin to ...
    case PO_ALL_PARTITIONS_ON_ORIGIN:
      switch(eToTopo) {
        case PO_ALL_NODES:          RASSERT(0);
        case PO_ALL_PARTITIONS:     return PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  return PX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
        case PO_ORIGIN:             RASSERT(0);
        case PO_SPECIFIC_NODE:      RASSERT(0);
        case PO_SPECIFIC_PARTITION: RASSERT(0);
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from origin to ...
    case PO_ORIGIN:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_ORIGIN_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ORIGIN_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN: return PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN;
        case PO_ORIGIN:             return PX_ORIGIN_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ORIGIN_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ORIGIN_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from specific node to ...
    case PO_SPECIFIC_NODE:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_SPECIFIC_NODE_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_SPECIFIC_NODE_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  RASSERT(0);
        case PO_ORIGIN:             return PX_SPECIFIC_NODE_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_SPECIFIC_NODE_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_SPECIFIC_NODE_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from specific partition to ...
    case PO_SPECIFIC_PARTITION:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_SPECIFIC_PARTITION_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_SPECIFIC_PARTITION_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  RASSERT(0);
        case PO_ORIGIN:             return PX_SPECIFIC_PARTITION_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_SPECIFIC_PARTITION_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ugh
    default:
      RASSERT(0);
  }
  
  return PX_NONE;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile pipeline statement into pipeline spec                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  bool bResult = false;
    
  bResult = bPass1_collect();
  if (! bResult)
    return false;
   
  bResult = bPass2_staging();
  if (! bResult)
    return false;
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* get the pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
PipelineSpec* Dynamo::pPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_p_PipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* show the pipeline                                                          */
/*----------------------------------------------------------------------------*/
void Dynamo::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  StageSpec* pStageSpec = m_p_PipelineSpec->pStageSpec(0);
  
  sSTMT sPipe("pipeline "_Stz);
  pStageSpec->vToPipeStr(sPipe.szStz());
    
  m_pTknVec->vTokenise(sPipe);
  
  SP_TknVecStr spTknVecStr;
  vTrace("TknVecStr ]]%s[[\n"_Stz,spTknVecStr.sz(m_pTknVec));
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
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
bool Dynamo::bPass1_collect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  bool bResult = true;
  
  vTokenBegin(0);
  
  if (m_pTkn->bSame("PIPELINE"_Stz) || m_pTkn->bSame("PIPELINELOCAL"_Stz)) {
    bResult = bCollectName();
    if (! bResult)
      return false;
  }

  while (m_pTkn->bSame("TAG"_Stz)) {
    bResult = bCollectTag();
    if (! bResult)
      return false;
  }
  
  if (m_pTkn->bSame("BEGIN"_Stz)) {
    bResult = bCollectBegin();
    if (! bResult)
      return false;
  }
  else {
    return false;
  }
  
  if ( ! m_pTkn->bSame("STAGE"_Stz)) {
    return false;
  }
  
  while (m_pTkn->bSame("STAGE"_Stz)) {
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
bool Dynamo::bCollectBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  RASSERT(m_pTkn->bSame("BEGIN"_Stz));
  vTokenAccept();
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect the pipeline END                                                   */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  if ( ! m_pTkn->bSame("END"_Stz) )
    return false;
    
  return true;
}
/*----------------------------------------------------------------------------*/
/* collect PIPELINE & the name of the pipeline                                */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  RASSERT(m_pTkn->bSame("PIPELINE"_Stz) || m_pTkn->bSame("PIPELINELOCAL"_Stz));
  vTokenAccept();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_p_PipelineSpec = new PipelineSpec(m_pTkn->szChars(),this);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTokenAccept();
   
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one operator (and optionally its arguments)                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_OpSpec = m_p_StageSpec->nItems();
  OpSpec* pOpSpec = new OpSpec(m_pTkn->szChars(),i_OpSpec,m_p_StageSpec);
  m_p_StageSpec->vAddOpSpec(pOpSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTokenAccept();
  
  if (m_pTkn->bSame("ROBIN"_Stz)) {
    pOpSpec->vSendKind(XK_ROBIN);
    vTokenAccept();
  }
  else if (m_pTkn->bSame("LOCALROBIN"_Stz)) {
    pOpSpec->vSendKind(XK_LOCALROBIN);
    vTokenAccept();
  }
  else if (m_pTkn->bSame("HASHED"_Stz)) {
    pOpSpec->vSendKind(XK_HASHED);
    vTokenAccept();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (m_pTkn->bSame("RIGHT"_Stz)) {
    pOpSpec->vAppendRight(true);
    vTokenAccept();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! bCollectOpArgs(pOpSpec))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect operator arguments                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectOpArgs(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

  i8 i_Arg = 0;
  for (;;) {
    
    if (m_pTkn->bSame("|"_Stz)     ||
        m_pTkn->bSame("STAGE"_Stz) ||
        m_pTkn->bSame("END"_Stz)) {
      break;
    }
    
    // add an argument
    
    s32 sArgName;
    sArgName.vSprint("<arg_%lld>"_Stz,i_Arg++);
    
    sSTMT sArgValue ( m_pTkn->szChars() );
    
    pOpSpec->vArgTagAdd(sArgName,sArgValue,TN_STRING);
    
    vTokenAccept();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect operator-pipe sequence                                             */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectOpSequence(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  for (;;) {
    
    if (! bTokenIsOp(m_pTkn))
      break;

    bCollectOp();
    
    if (! m_pTkn->bSame("|"_Stz))
      break;
    
    vTokenAccept();
  }
   
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect stage                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  RASSERT(m_pTkn->bSame("STAGE"_Stz));
  vTokenAccept();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_StageSpec = m_p_PipelineSpec->nItems();
  m_p_StageSpec = new StageSpec(m_pTkn->szChars(),i_StageSpec,m_p_PipelineSpec);
  m_p_PipelineSpec->vAddStageSpec(m_p_StageSpec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTokenAccept();
  
  bool bResult = bCollectTopology();
  if ( ! bResult )
    return false;
  
  if (bTokenIsOp(m_pTkn))
    bCollectOpSequence();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one tag                                                            */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  RASSERT(m_pTkn->bSame("TAG"_Stz));
  vTokenAccept();
  
  s32 sTagNm = m_pTkn->szChars();
  vTokenAccept();

  if (! m_pTkn->bSame("="_Stz))
    return false;
  vTokenAccept();

  sSTMT sTagValue = m_pTkn->szChars();
  vTokenAccept();

  while (! m_pTkn->bSame("ENDTAG"_Stz)) {
    vTokenAccept();
  }
  vTokenAccept();

  if (AM::bVerbose())
    vTrace("Tag recognised: %s %s\n"_Stz,sTagNm.szStz(),sTagValue.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TagSet* pJobTags = m_p_PipelineJob->pJobTags();
  
  pJobTags->vInsert(sTagNm,sTagValue);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect topology                                                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  if (m_pTkn->bSame("ON"_Stz))
    vTokenAccept();
  else
    return false;
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :
  
  if (m_pTkn->bSame("ALL"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAll();
  }
 
  // -- -- -- -- -- -- -- -- -- -- --
  // on node <nodeNm> :
  
  else if (m_pTkn->bSame("NODE"_Stz)) {
    vTokenAccept();
    return bRecogniseOnNode();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on partition <partitionNo> :
  
  else if (m_pTkn->bSame("PARTITION"_Stz)) {
    vTokenAccept();
    return bRecogniseOnPartition();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  else if (m_pTkn->bSame("ORIGIN"_Stz)) {
    vTokenAccept();
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
bool Dynamo::bRecogniseOnAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :
  
  if (m_pTkn->bSame("NODES"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAllNodes();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :
  
  else if (m_pTkn->bSame("PARTITIONS"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAllPartitions();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ALL NODE _^                                         */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnAllNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :
  
  if (m_pTkn->bSame(":"_Stz)) {
    vTokenAccept();
    
    m_p_StageSpec->vTopology(PO_ALL_NODES);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ALL PARTITIONS _^                                   */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnAllPartitions(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :
  
  if (m_pTkn->bSame("ON"_Stz)) {
    vTokenAccept();
    if (m_pTkn->bSame("ORIGIN"_Stz)) {
      
      vTokenAccept();
      if (m_pTkn->bSame(":"_Stz)) {
        vTokenAccept();

        m_p_StageSpec->vTopology(PO_ALL_PARTITIONS_ON_ORIGIN);
        return true;
      }
    }
    else
      return false;
  }
  
  if (m_pTkn->bSame(":"_Stz)) {
    vTokenAccept();
    m_p_StageSpec->vTopology(PO_ALL_PARTITIONS);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON NODE <nodeNo> _^                                    */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  s32 sLocus(m_pTkn->szChars());
  vTokenAccept();
  
  if (m_pTkn->bSame(":"_Stz)) {
    vTokenAccept();
    m_p_StageSpec->vTopology(PO_SPECIFIC_NODE);
    i8 i_Node = sLocus.szStz()->iToI8();
    if ( i_Node > AM::pGrid()->nConnectedNodes() )
      return false;
    m_p_StageSpec->vSpecificNode(i_Node);
    return true;
  }
  
  return false;
  
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON PARTITION <tPtnNo> _^                               */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  s32 sLocus(m_pTkn->szChars());
  vTokenAccept();
  
  if (m_pTkn->bSame(":"_Stz)) {
    vTokenAccept();
    m_p_StageSpec->vTopology(PO_SPECIFIC_PARTITION);
    tPN tPtnNo = sLocus.szStz()->iToI8();
    if (tPtnNo >= AM::nPtnsInGrid())
      return false;
    
    m_p_StageSpec->vSpecificPtnNo(tPtnNo);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise topology: ON ORIGIN _^                                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnOrigin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  if (m_pTkn->bSame(":"_Stz)) {
    vTokenAccept();
    
    m_p_StageSpec->vTopology(PO_ORIGIN);
    return true;
  }
  else {
    s32 sLocus(m_pTkn->szChars());
    vTokenAccept();
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void Dynamo::vTokenAccept(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

  ++ m_i_Tkn;
  
  m_pTkn = m_pTknVec->pGet(m_i_Tkn);

}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void Dynamo::vTokenBegin(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  m_i_Tkn = i_Tkn;
  m_pTkn  = m_pTknVec->pGet(m_i_Tkn);
  
}

/*----------------------------------------------------------------------------*/
/* is token a known operator?                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bTokenIsOp(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
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
bool Dynamo::bPass2_staging(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  // get each stageSpec of pipelineSpec
  i8 nStageSpecs = m_p_PipelineSpec->nItems();
  for (i8 i_Stage = 0; i_Stage < nStageSpecs; i_Stage++) {
    StageSpec* pStageSpec = m_p_PipelineSpec->pGet(i_Stage);
    RASSERT(pStageSpec->i_StageSpec() == i_Stage);

    // get each opSpec of stageSpec
    i8 nOpSpecs = pStageSpec->nItems();
    for (i8 i_Op = 0; i_Op < nOpSpecs; i_Op++) {
      OpSpec* pOpSpec = pStageSpec->pGet(i_Op);
      RASSERT(pOpSpec->i_OpSpec() == i_Op);
      
      Stz* szName = pOpSpec->szOpNm();
      
      // 1) is the name in the opSpec actually a stage in this pipeline?
      StageSpec* pStageSpecDst = m_p_PipelineSpec->pFindStageSpec(szName);
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
void Dynamo::vArgAdd_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
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
void Dynamo::vArgUpdate_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

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
void Dynamo::vMorphToSend(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_NextStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  pOpSpec->vOpNm("send"_Stz);
  
  vTopoTagsAdd(pOpSpec, eCrossTopo, i_NextStage, 0);
}

/*----------------------------------------------------------------------------*/
/* name is a known operator                                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vNameIsOp(StageSpec* pStageSpec, OpSpec* pOpSpec_isOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  i8 i_Op = pOpSpec_isOp->i_OpSpec();
  i8 i_Stage = pStageSpec->i_StageSpec();
  ePX eCrossTopo = Dynamo::eCrossTopo(pStageSpec, pStageSpec);
  
  vTopoTagsAdd(pOpSpec_isOp,eCrossTopo,i_Stage,i_Op+1);
}

/*----------------------------------------------------------------------------*/
/* name is a known pipeline                                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vNameIsPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  NOT_IMPLEMENTED_YET;
}

/*----------------------------------------------------------------------------*/
/* name is a known stage                                                      */
/*----------------------------------------------------------------------------*/
void Dynamo::vNameIsStage(StageSpec* pStageSpec, OpSpec* pOpSpec_isStage,
                          StageSpec* pNextStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
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
void Dynamo::vStageConsumeOpInsert(StageSpec* pStageSpec, ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  OpSpec* pConsumeOpSpec = new OpSpec("consumeOp"_Stz,0,pStageSpec);
  
  pStageSpec->vInsert(pConsumeOpSpec);
  
  pStageSpec->vRenumberOpSpecs();
  
  vArgAdd_sends(pConsumeOpSpec,eCrossTopo);
}

/*----------------------------------------------------------------------------*/
/* first send to stage                                                        */
/*----------------------------------------------------------------------------*/
void Dynamo::vStageFirstSend(StageSpec* pStageSpec, OpSpec* pOpSpec_isStage,
                             StageSpec* pNextStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  i8 i_Op = pOpSpec_isStage->i_OpSpec();
  i8 i_NextStage = pNextStageSpec->i_StageSpec();

  ePX eCrossTopo = Dynamo::eCrossTopo(pStageSpec, pNextStageSpec);

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
void Dynamo::vStageSubsequentSend(StageSpec* pStageSpec, OpSpec* pOpSpec,
                                  StageSpec* pNextStageSpec, OpSpec* pOpSpecSend)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  i8 i_Op = pOpSpec->i_OpSpec();
  i8 i_NextStage = pNextStageSpec->i_StageSpec();

  ePX eCrossTopo = Dynamo::eCrossTopo(pStageSpec, pNextStageSpec);

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
void Dynamo::vTopoTagsAdd(OpSpec* pOpSpec,
                          ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
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
void Dynamo::vTopoTagsUpdate(OpSpec* pOpSpec,
                             ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

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
