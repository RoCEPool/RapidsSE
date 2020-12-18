/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "Plans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Plans,Dynamo,CN_DYNAMO)
__aNewFunction(Dynamo,CN_DYNAMO)
__vDeleteFunction(Dynamo,CN_DYNAMO)
__aFooterFunction(Dynamo)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dynamo::Dynamo(Stz* sCmd, Job* p_Job)
: Object(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenize the pipeline
  
  m_p_Job = p_Job;
  
  m_pTknVec = new TokenVec;
  m_pTknVec->vTokenise(sCmd);
  
  //m_pTknVec->vRemoveToken(m_pTknVec->nTokens());
  
  m_pPipelineSpec = nullptr;
  m_pStageSpec   = nullptr;
  
  m_i_Tkn        = 0;
  m_pTkn         = nullptr;
  
  _checkInitialised_(CN_DYNAMO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamo::~Dynamo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // 2020 Mar 01: RJH move this into destructor of pipeline
  //delete m_pPipelineSpec;
  
  m_pTknVec->vSetAllTokens();
  delete m_pTknVec;
  //delete m_pStageSpec;
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get cross topology between two stages                                      */
/*----------------------------------------------------------------------------*/
ePX Dynamo::eTopoloyX(StageSpec*  pFromStageSpec, StageSpec* pToStageSpec)
{
  _static_
  
  switch (pFromStageSpec->eTopology()) {
    case PO_ALL_NODES:
      switch(pToStageSpec->eTopology()) {
        case PO_ALL_NODES:          return PX_ALL_NODES_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ALL_NODES_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:     ASSERT(0);
        case PO_ORIGIN:             return PX_ALL_NODES_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ALL_NODES_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ALL_NODES_TO_SPECIFIC_PARTITION;
        default:
          ASSERT(0);
      }
    case PO_ALL_PARTITIONS:
      switch(pToStageSpec->eTopology()) {
        case PO_ALL_NODES:          return PX_ALL_PARTITIONS_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:     ASSERT(0);
        case PO_ORIGIN:             return PX_ALL_PARTITIONS_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ALL_PARTITIONS_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ALL_PARTITIONS_TO_SPECIFIC_PARTITION;
        default:
          ASSERT(0);
      }
    case PO_ALL_PARTITIONS_ON_ORIGIN:
      switch(pToStageSpec->eTopology()) {
        case PO_ALL_NODES:          ASSERT(0);
        case PO_ALL_PARTITIONS:     return PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:     return PX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
        case PO_ORIGIN:             ASSERT(0);
        case PO_SPECIFIC_NODE:      ASSERT(0);
        case PO_SPECIFIC_PARTITION: ASSERT(0);
        default:
          ASSERT(0);
      }
    case PO_ORIGIN:
      switch(pToStageSpec->eTopology()) {
        case PO_ALL_NODES:          return PX_ORIGIN_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ORIGIN_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN: return PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN;
        case PO_ORIGIN:             return PX_ORIGIN_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ORIGIN_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ORIGIN_TO_SPECIFIC_PARTITION;
        default:
          ASSERT(0);
      }
    case PO_SPECIFIC_NODE:
      switch(pToStageSpec->eTopology()) {
        case PO_ALL_NODES:          return PX_SPECIFIC_NODE_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_SPECIFIC_NODE_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:     ASSERT(0);
        case PO_ORIGIN:             return PX_SPECIFIC_NODE_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_SPECIFIC_NODE_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_SPECIFIC_NODE_TO_SPECIFIC_PARTITION;
        default:
          ASSERT(0);
      }
    case PO_SPECIFIC_PARTITION:
      switch(pToStageSpec->eTopology()) {
        case PO_ALL_NODES:          return PX_SPECIFIC_PARTITION_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_SPECIFIC_PARTITION_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:     ASSERT(0);
        case PO_ORIGIN:             return PX_SPECIFIC_PARTITION_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_SPECIFIC_PARTITION_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION;
        default:
          ASSERT(0);
      }
    default:
      ASSERT(0);
  }
  return PX_NONE;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile the supplied pipeline                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  bool bResult = false;
  
#ifdef WITH_TRACE_DYNAMO
  i8 nTokens = m_pTknVec->nTokens();
  
  for (i8 i_Tkn = 0; i_Tkn < nTokens; ) {
    Token* pTkn = m_pTknVec->pGet(i_Tkn++);

    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szChars());
  }
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
#endif
  
  bResult = bPhase1_collect();
  if (! bResult)
    return false;
 
#ifdef WITH_TRACE_DYNAMO
  m_pPipelineSpec->vPrint();
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
#endif
  
  bResult = bPhase2_staging();
  if (! bResult)
    return false;

#ifdef WITH_TRACE_DYNAMO
  m_pPipelineSpec->vPrint();
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
#endif
  
#ifdef WITH_TRACE_DYNAMO
  m_pPipelineSpec->vPrint();
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get the pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
PipelineSpec* Dynamo::pPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_pPipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* find stage matching given name                                             */
/*----------------------------------------------------------------------------*/
void Dynamo::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  StageSpec* pStageSpec = m_pPipelineSpec->pStageSpec(0);
  
  sSTMT sPipe("pipeline "_Stz);
  pStageSpec->vToPipeStr(sPipe.szStz());
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("Restated stage 0: ]]%s[[\n"_Stz,sPipe.szStz());
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
#endif
  
  m_pTknVec->vTokenise(sPipe);
  
  SP_TknVecStr spTknVecStr;
  vTrace("TknVecStr ]]%s[[\n"_Stz,spTknVecStr.sz(m_pTknVec));
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                           Phase 1 : Collect                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* phase 1: collect the plan                                                  */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPhase1_collect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  bool bResult = true;
  
  vBeginToken(0);
  
  if (m_pTkn->bSame("pipeline"_Stz) || m_pTkn->bSame("pipelineLocal"_Stz)) {
    bResult = bCollectName();
    if (! bResult)
      return false;
  }

  while (m_pTkn->bSame("tag"_Stz)) {
    bResult = bCollectTag();
    if (! bResult)
      return false;
  }
  
  if (m_pTkn->bSame("begin"_Stz)) {
    bResult = bCollectBegin();
    if (! bResult)
      return false;
  }
  else {
    return false;
  }
  
  if ( ! m_pTkn->bSame("stage"_Stz)) {
    return false;
  }
  
  while (m_pTkn->bSame("stage"_Stz)) {
    bResult = bCollectStage();
    if ( ! bResult )
      return false;
  }
  
  bResult = bCollectEnd();

  return bResult;
}

/*----------------------------------------------------------------------------*/
/* compile begin                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  ASSERT(m_pTkn->bSame("begin"_Stz));
  vAcceptToken();
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("Begin recognised\n"_Stz);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile name phrase                                                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  ASSERT(m_pTkn->bSame("pipeline"_Stz) || m_pTkn->bSame("pipelineLocal"_Stz));
  vAcceptToken();
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("        pipeline=%s\n"_Stz,m_pTkn->szChars());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pPipelineSpec = new PipelineSpec(m_pTkn->szChars(),m_p_Job);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();
 
#ifdef WITH_TRACE_DYNAMO
  vTrace("Name Clause recognised\n"_Stz);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile tag                                                                */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  ASSERT(m_pTkn->bSame("tag"_Stz));
  vAcceptToken();
  
  s32 sTagNm = m_pTkn->szChars();
  vAcceptToken();

  if (! m_pTkn->bSame("="_Stz))
    return false;
  vAcceptToken();

  sSTMT sTagValue = m_pTkn->szChars();
  vAcceptToken();

  while (! m_pTkn->bSame("endTag"_Stz)) {
    vAcceptToken();
  }
  vAcceptToken();

//#ifdef WITH_TRACE_DYNAMO
  if (AM::bVerbose())
    vTrace("Tag recognised: %s %s\n"_Stz,sTagNm.szStz(),sTagValue.szStz());
//#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TagSet* pJobTags = m_p_Job->pJobTags();
  
  pJobTags->vInsert(sTagNm,sTagValue);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile stage                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  ASSERT(m_pTkn->bSame("stage"_Stz));
  vAcceptToken();
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("        stage=%s\n"_Stz,m_pTkn->szChars());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_StageSpec = m_pPipelineSpec->nItems();
  m_pStageSpec = new StageSpec(m_pTkn->szChars(),i_StageSpec,m_pPipelineSpec);
  m_pPipelineSpec->vAddStageSpec(m_pStageSpec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();
  
  bool bResult = bCollectTopology();
  if ( ! bResult )
    return false;
  
  if (bIsOperator(m_pTkn))
    bCollectPipe();
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("Stage Clause recognised\n"_Stz);
#endif
//
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile topology                                                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  if (m_pTkn->bSame("on"_Stz))
    vAcceptToken();
  else
    return false;
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :
  
  if (m_pTkn->bSame("all"_Stz)) {
    vAcceptToken();
    return bParseOnAll();
  }
 
  // -- -- -- -- -- -- -- -- -- -- --
  // on node <nodeNm> :
  
  else if (m_pTkn->bSame("node"_Stz)) {
    vAcceptToken();
    return bParseOnNode();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on partition <partitionNo> :
  
  else if (m_pTkn->bSame("partition"_Stz)) {
    vAcceptToken();
    return bParseOnPartition();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  else if (m_pTkn->bSame("origin"_Stz)) {
    vAcceptToken();
    return bParseOnOrigin();
  }
  else {

    // -- -- -- -- -- -- -- -- -- -- --
  #ifdef WITH_TRACE_DYNAMO
    vTrace("Topology Phrase not recognised\n"_Stz);
  #endif
    
    return false;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* compile pipe                                                               */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectPipe(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  for (;;) {
    
    if (! bIsOperator(m_pTkn))
      break;

    bCollectOp();
    
    if (! m_pTkn->bSame("|"_Stz))
      break;
    
    vAcceptToken();
  }
 
#ifdef WITH_TRACE_DYNAMO
  vTrace("Pipe Phrase recognised\n"_Stz);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile op                                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

#ifdef WITH_TRACE_DYNAMO
  vTrace("        op=%s\n"_Stz,m_pTkn->szChars());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_OpSpec = m_pStageSpec->nItems();
  OpSpec* pOpSpec = new OpSpec(m_pTkn->szChars(),i_OpSpec,m_pStageSpec);
  m_pStageSpec->vAddOpSpec(pOpSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();
  
  if (m_pTkn->bSame("Robin"_Stz)) {
    pOpSpec->vXmitKind(XK_ROBIN);
    vAcceptToken();
  }
  else if (m_pTkn->bSame("localRobin"_Stz)) {
    pOpSpec->vXmitKind(XK_LOCALROBIN);
    vAcceptToken();
  }
  else if (m_pTkn->bSame("Hashed"_Stz)) {
    pOpSpec->vXmitKind(XK_HASHED);
    vAcceptToken();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_Arg = 0;
  for (;;) {
    
    if (m_pTkn->bSame("|"_Stz) || m_pTkn->bSame("stage"_Stz)
                               || m_pTkn->bSame("end"_Stz)) {
      break;
    }
    
    // add an argument
    
    s32 sArgName;
    sArgName.vSprint("<arg_%lld>"_Stz,i_Arg++);
    
    sSTMT sArgValue ( m_pTkn->szChars() );
    
    pOpSpec->vAddEnamePair(sArgName,sArgValue,TN_STRING);
    
    vAcceptToken();
  }

#ifdef WITH_TRACE_DYNAMO
  vTrace("Op Phrase recognised\n"_Stz);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile end                                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  ASSERT(m_pTkn->bSame("end"_Stz));
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("End recognised\n"_Stz);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void Dynamo::vAcceptToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

  ++ m_i_Tkn;
  
  m_pTkn = m_pTknVec->pGet(m_i_Tkn);
#ifdef WITH_TRACE_DYNAMO
  vTrace("Next %03lld: %s\n"_Stz,m_i_Tkn,m_pTkn->szChars());
#endif
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void Dynamo::vBeginToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  m_i_Tkn = i_Tkn;
  m_pTkn  = m_pTknVec->pGet(m_i_Tkn);
  
#ifdef WITH_TRACE_DYNAMO
  //vTrace("Begin %03lld: %s\n"_Stz,i_Tkn,m_pTkn->szChars());
#endif
}

/*----------------------------------------------------------------------------*/
/* is token a known operator?                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bIsOperator(Token* pTkn)
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
/* parse topology: on all                                                     */
/*----------------------------------------------------------------------------*/
bool Dynamo::bParseOnAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :
  
  if (m_pTkn->bSame("nodes"_Stz)) {
    vAcceptToken();
    return bParseOnAllNodes();
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :
  
  else if (m_pTkn->bSame("partitions"_Stz)) {
    vAcceptToken();
    return bParseOnAllPartitions();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* parse topology: on all nodes                                               */
/*----------------------------------------------------------------------------*/
bool Dynamo::bParseOnAllNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :
  
  if (m_pTkn->bSame(":"_Stz)) {
    vAcceptToken();
    
#ifdef WITH_TRACE_DYNAMO
    vTrace("Topology Phrase recognised >on all nodes :<\n"_Stz);
#endif
    m_pStageSpec->vTopology(PO_ALL_NODES);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse topology: on all partitions                                          */
/*----------------------------------------------------------------------------*/
bool Dynamo::bParseOnAllPartitions(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :
  
  if (m_pTkn->bSame("on"_Stz)) {
    vAcceptToken();
    if (m_pTkn->bSame("origin"_Stz)) {
      
      vAcceptToken();
      if (m_pTkn->bSame(":"_Stz)) {
        vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
        vTrace("Topology Phrase recognised >on all partitions to origin :<\n"_Stz);
#endif
        m_pStageSpec->vTopology(PO_ALL_PARTITIONS_ON_ORIGIN);
        return true;
      }
    }
    else
      return false;
  }
  
  if (m_pTkn->bSame(":"_Stz)) {
    vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
    vTrace("Topology Phrase recognised >on all partitions :<\n"_Stz);
#endif
    m_pStageSpec->vTopology(PO_ALL_PARTITIONS);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse topology: on node                                                    */
/*----------------------------------------------------------------------------*/
bool Dynamo::bParseOnNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  s32 sLocus(m_pTkn->szChars());
  vAcceptToken();
  
  if (m_pTkn->bSame(":"_Stz)) {
    vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
    vTrace("Topology Phrase recognised >on node %s :<\n"_Stz,sLocus.szStz());
#endif
    m_pStageSpec->vTopology(PO_SPECIFIC_NODE);
    i8 i_Node = sLocus.szStz()->iToI8();
    if ( i_Node > AM::pGrid()->nConnectedNodes() )
      return false;
    m_pStageSpec->vSpecificNode(i_Node);
    return true;
  }
  
  return false;
  
}


/*----------------------------------------------------------------------------*/
/* parse topology: on partition <tPtnNo>                                      */
/*----------------------------------------------------------------------------*/
bool Dynamo::bParseOnPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  s32 sLocus(m_pTkn->szChars());
  vAcceptToken();
  
  if (m_pTkn->bSame(":"_Stz)) {
    vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
    vTrace("Topology Phrase recognised >on partition %s :<\n"_Stz,sLocus.szStz());
#endif
    m_pStageSpec->vTopology(PO_SPECIFIC_PARTITION);
    tPN tPtnNo = sLocus.szStz()->iToI8();
    if (tPtnNo >= AM::nPtnsInGrid())
      return false;
    
    m_pStageSpec->vSpecificPtnNo(tPtnNo);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse topology: on origin                                                  */
/*----------------------------------------------------------------------------*/
bool Dynamo::bParseOnOrigin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  if (m_pTkn->bSame(":"_Stz)) {
    vAcceptToken();
    
#ifdef WITH_TRACE_DYNAMO
    vTrace("Topology Phrase recognised >on origin :<\n"_Stz);
#endif
    m_pStageSpec->vTopology(PO_ORIGIN);
    return true;
  }
  else {
    s32 sLocus(m_pTkn->szChars());
    vAcceptToken();
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*                           Phase2 : Staging                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* phase2 staging                                                             */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPhase2_staging(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  for (i8 i_Stage = 0; i_Stage < m_pPipelineSpec->nItems(); i_Stage++) {
    
    // get the stageSpec
    StageSpec* pStageSpec = m_pPipelineSpec->pGet(i_Stage);
    
    // get the OpSpecVec from the stageSpec
    //OpSpecVec* pOpSpecVec = pStageSpec->pOpSpecVec();
    i8 nOpSpecs = pStageSpec->nItems();
    
    for (i8 i_Op = 0; i_Op < nOpSpecs; i_Op++) {
      
      OpSpec* pOpSpec = pStageSpec->pGet(i_Op);
      
      i8 i_FoundStage = C_UNKNOWN;
      StageSpec* pStageSpecFound = m_pPipelineSpec->pFindStageSpec(pOpSpec->szOpNm(), i_FoundStage);
      
      if (pStageSpecFound) {
        ePX eCrossTopo = eTopoloyX(pStageSpec, pStageSpecFound);
        
#ifdef WITH_TRACE_DYNAMO
        vTrace("  op %s is a stage %lld %s\n"_Stz,
               pOpSpec->szOpNm(),
               i_FoundStage,
               aPX[eCrossTopo]);
#endif
        
        if (eCrossTopo == PX_ORIGIN_TO_ORIGIN && i_FoundStage == 0) {
          // add parameter to the prev sink op
          OpSpec* pPrevOpSpec = pStageSpec->pGet(i_Op-1);
          vReplaceTopologyParams(pPrevOpSpec, eCrossTopo, i_FoundStage, 0);

          // remove the last (stage) OpSpec from the OpSpecVec
          RASSERT(i_Op == pStageSpec->nItems()-1);
          pStageSpec->vRemove(pOpSpec);
          delete pOpSpec;
        }
        else {
          pOpSpec->vAddEnamePair(C_TAG_XMITKIND,(i8)pOpSpec->eXmitKind(),TN_INTEGER);
          vReplaceWithXmit(pOpSpec, eCrossTopo, i_FoundStage);
          OpSpec* pConsumeOpSpec = pStageSpecFound->pPrependConsumeOp(eCrossTopo, i_FoundStage, i_Op+1);
          vAddConsumeParams(pConsumeOpSpec, eCrossTopo);

        }
      }
      else {
        ePX eCrossTopo = eTopoloyX(pStageSpec, pStageSpec);
        vAddTopologyParams(pOpSpec, eCrossTopo, i_Stage, i_Op+1);
      }
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* replace the <stageNm> with xmit <nodeNo>                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vAddTopologyParams(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_Stage, i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // arg-0 is the cross topology of the xmit op
  pOpSpec->vAddTopologyEnamePair(C_TAG_TOPOLOGY,(i8)eCrossTopo,TN_INTEGER);
  
  // arg-1 is the next stage for the xmit
  pOpSpec->vAddTopologyEnamePair(C_TAG_NEXTSTAGE,(i8)i_Stage,TN_INTEGER);
  
  // arg-2 is the op of the next stage for the xmit
  pOpSpec->vAddTopologyEnamePair(C_TAG_NEXTOP,(i8)i_Op,TN_INTEGER);
  //pOpSpec->pTagSet()->vPrint();
}

/*----------------------------------------------------------------------------*/
/* add the Xmits parameter                                                    */
/*----------------------------------------------------------------------------*/
void Dynamo::vAddConsumeParams(OpSpec* pOpSpec, ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  i8 iXmits = C_UNKNOWN;
  
  // add argument to the consumeOp depending on the topology
  switch (eCrossTopo) {
    case PX_ALL_NODES_TO_ORIGIN:
      iXmits = AM::pGrid()->nConnectedNodes();
      break;
      
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
      iXmits = AM::pGrid()->nConnectedNodes();
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      iXmits = AM::nPtnsPerTable();
      break;
      
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iXmits = AM::nPtnsPerTable();
      break;
     
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
      iXmits = AM::nPtnsInGrid();
      break;
      
    case PX_ORIGIN_TO_ORIGIN:
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_ALL_NODES:
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
      iXmits = 1;
      break;
      
    default:
      RASSERT(0);
  }
  
  // arg-0 is the cross topology of the xmit op
  pOpSpec->vAddEnamePair(C_TAG_XMITS,iXmits,TN_INTEGER);
}

/*----------------------------------------------------------------------------*/
/* replace the <stageNm> with xmit <nodeNo>                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vReplaceTopologyParams(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_Stage, i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // arg-0 is the cross topology of the xmit op
  pOpSpec->vReplaceTopologyEnamePair(C_TAG_TOPOLOGY,(i8)eCrossTopo,TN_INTEGER);
  
  // arg-1 is the next stage for the xmit
  pOpSpec->vReplaceTopologyEnamePair(C_TAG_NEXTSTAGE,(i8)i_Stage,TN_INTEGER);
  
  // arg-2 is the op of the next stage for the xmit
  pOpSpec->vReplaceTopologyEnamePair(C_TAG_NEXTOP,(i8)i_Op,TN_INTEGER);
  //pOpSpec->pTagSet()->vPrint();
}

/*----------------------------------------------------------------------------*/
/* replace the <stageNm> with xmit <nodeNo>                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vReplaceWithXmit(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  pOpSpec->vOpNm("xmit"_Stz);
  
  vAddTopologyParams(pOpSpec, eCrossTopo, i_Stage, 0);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
