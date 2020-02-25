/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "DynamoCompiler.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(DynamoCompiler,Dynamo,CN_DYNAMO)
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
  Tokeniser::vTokenise(m_pTknVec, sCmd);
  
  m_pTknVec->vCutToken(m_pTknVec->nTokens());
  
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
  
  delete m_pPipelineSpec;
  
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
        case PO_ORIGIN:             return PX_ALL_PARTITIONS_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ALL_PARTITIONS_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ALL_PARTITIONS_TO_SPECIFIC_PARTITION;
        default:
          ASSERT(0);
      }
    case PO_ORIGIN:
      switch(pToStageSpec->eTopology()) {
        case PO_ALL_NODES:          return PX_ORIGIN_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ORIGIN_TO_ALL_PARTITIONS;
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

    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szTerm());
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
  
  s4K sPipe("pipeline "_Stz);
  pStageSpec->vToPipeStr(sPipe.szStz());
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("Restated stage 0: ]]%s[[\n"_Stz,sPipe.szStz());
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
#endif
  
  Tokeniser::vTokenise(m_pTknVec,sPipe);
  
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
  
  if (m_pTkn->bSame("pipeline"_Stz)) {
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
  
  ASSERT(m_pTkn->bSame("pipeline"_Stz));
  vAcceptToken();
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("        pipeline=%s\n"_Stz,m_pTkn->szTerm());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pPipelineSpec = new PipelineSpec(m_pTkn->szTerm());
  
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
  
  s32 sTagNm = m_pTkn->szTerm();
  vAcceptToken();

  if (! m_pTkn->bSame("="_Stz))
    return false;
  vAcceptToken();

  s4K sTagValue = m_pTkn->szTerm();
  vAcceptToken();

  while (! m_pTkn->bSame("endTag"_Stz)) {
    vAcceptToken();
  }
  vAcceptToken();

//#ifdef WITH_TRACE_DYNAMO
  vTrace("Tag recognised: %s %s\n"_Stz,sTagNm.szStz(),sTagValue.szStz());
//#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EnameSet* pEnameSet = m_p_Job->pEnameSet();
  
  pEnameSet->vAdd(sTagNm,sTagValue);
  
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
  vTrace("        stage=%s\n"_Stz,m_pTkn->szTerm());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pStageSpec = new StageSpec(m_pTkn->szTerm());
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
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all nodes :

    if (m_pTkn->bSame("nodes"_Stz)) {
      vAcceptToken();
      
      if (m_pTkn->bSame(":"_Stz)) {
        vAcceptToken();
        
#ifdef WITH_TRACE_DYNAMO
        vTrace("Topology Phrase recognised >on all nodes :<\n"_Stz);
#endif
        m_pStageSpec->vTopology(PO_ALL_NODES);
        return true;
      }
    }
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all partitions :
    
    else if (m_pTkn->bSame("partitions"_Stz)) {
      vAcceptToken();
      
      if (m_pTkn->bSame(":"_Stz)) {
        vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
        vTrace("Topology Phrase recognised >on all partitions :<\n"_Stz);
#endif
        m_pStageSpec->vTopology(PO_ALL_PARTITIONS);
        return true;
      }
    }
    else
      return false;
  }
 
  // -- -- -- -- -- -- -- -- -- -- --
  // on node <nodeNm> :
  
  else if (m_pTkn->bSame("node"_Stz)) {
    vAcceptToken();
    
    s32 sLocus(m_pTkn->szTerm());
    vAcceptToken();

    if (m_pTkn->bSame(":"_Stz)) {
      vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
      vTrace("Topology Phrase recognised >on node %s :<\n"_Stz,sLocus.szStz());
#endif
      m_pStageSpec->vTopology(PO_SPECIFIC_NODE);
      i8 i_Nde = sLocus.szStz()->iToI8();
      if ( i_Nde > AM::pGrid()->nConnectedNodes() )
        return false;
      m_pStageSpec->vNode(i_Nde);
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on partition <partitionNo> :
  
  else if (m_pTkn->bSame("partition"_Stz)) {
    vAcceptToken();
    
    s32 sLocus(m_pTkn->szTerm());
    vAcceptToken();
    
    if (m_pTkn->bSame(":"_Stz)) {
      vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
      vTrace("Topology Phrase recognised >on partition %s :<\n"_Stz,sLocus.szStz());
#endif
      m_pStageSpec->vTopology(PO_SPECIFIC_PARTITION);
      i8 i_Ptn = sLocus.szStz()->iToI8();
      if (i_Ptn >= AM::pGrid()->nPartitions())
        return false;
      
      m_pStageSpec->vPartition(i_Ptn);
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  else if (m_pTkn->bSame("origin"_Stz)) {
    vAcceptToken();
    
    if (m_pTkn->bSame(":"_Stz)) {
      vAcceptToken();
      
#ifdef WITH_TRACE_DYNAMO
      vTrace("Topology Phrase recognised >on origin :<\n"_Stz);
#endif
      m_pStageSpec->vTopology(PO_ORIGIN);
      return true;
    }
    else {
      
      s32 sLocus(m_pTkn->szTerm());
      vAcceptToken();
    }
  }
  else
    return false;
  
  // -- -- -- -- -- -- -- -- -- -- --
  // :
  
  if (m_pTkn->bSame(":"_Stz)) {
    vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
    vTrace("Topology Phrase recognised :<\n"_Stz);
#endif
    m_pStageSpec->vTopology(PO_NONE);
    return true;
  }

  // -- -- -- -- -- -- -- -- -- -- --
#ifdef WITH_TRACE_DYNAMO
  vTrace("Topology Phrase not recognised\n"_Stz);
#endif
  
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
  vTrace("        op=%s\n"_Stz,m_pTkn->szTerm());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  OpSpec* pOpSpec = new OpSpec(m_pTkn->szTerm(),m_pStageSpec);
  m_pStageSpec->vAddOpSpec(pOpSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();
  
  if (m_pTkn->bSame("Robin"_Stz)) {
    pOpSpec->vXmitKind(XK_ROBIN);
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
    
    s256 sArgValue ( m_pTkn->szTerm() );
    
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
  vTrace("Next %03lld: %s\n"_Stz,m_i_Tkn,m_pTkn->szTerm());
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
  vTrace("Begin %03lld: %s\n"_Stz,i_Tkn,m_pTkn->szTerm());
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
/*                           Phase2 : Staging                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* phase2 staging                                                             */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPhase2_staging(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // get the stageSpecVec from the pipeline
  StageSpecVec* pStageSpecVec = m_pPipelineSpec->pStageSpecVec();
  
  for (i8 i_Stage = 0; i_Stage < pStageSpecVec->nItems(); i_Stage++) {
    
    // get the stageSpec
    StageSpec* pStageSpec = pStageSpecVec->pGet(i_Stage);
    
    // get the OpSpecVec from the stageSpec
    OpSpecVec* pOpSpecVec = pStageSpec->pOpSpecVec();
    i8 nOpSpecs = pOpSpecVec->nItems();
    
    for (i8 i_Op = 0; i_Op < nOpSpecs; i_Op++) {
      
      OpSpec* pOpSpec = pOpSpecVec->pGet(i_Op);
      
      i8 i_FoundStage = C_UNKNOWN;
      StageSpec* pStageSpecFound = m_pPipelineSpec->pFindStageSpec(pOpSpec->szName(), i_FoundStage);
      
      if (pStageSpecFound) {
        ePX eTopologyX = eTopoloyX(pStageSpec, pStageSpecFound);
        
#ifdef WITH_TRACE_DYNAMO
        vTrace("  op %s is a stage %lld %s\n"_Stz,pOpSpec->szName(), i_FoundStage, aPX[eTopologyX]);
#endif
        
        if (eTopologyX == PX_ORIGIN_TO_ORIGIN && i_FoundStage == 0) {
          // add parameter to the prev sink op
          OpSpec* pPrevOpSpec = pOpSpecVec->pGet(i_Op-1);
          vReplaceTopologyParams(pPrevOpSpec, eTopologyX, i_FoundStage, 0);

          // remove the last (stage) OpSpec from the OpSpecVec
          RASSERT(i_Op == pOpSpecVec->nItems()-1);
          pOpSpecVec->vRemove(pOpSpec);
          delete pOpSpec;
        }
        else {
          pOpSpec->vAddEnamePair("<XmitKind>"_Stz,(i8)pOpSpec->eXmitKind(),TN_INTEGER);
          vReplaceWithXmit(pOpSpec, eTopologyX, i_FoundStage);
          pStageSpecFound->vPrependConsumeOp(eTopologyX, i_FoundStage, i_Op+1);
        }
      }
      else {
        ePX eTopologyX = eTopoloyX(pStageSpec, pStageSpec);
        vAddTopologyParams(pOpSpec, eTopologyX, i_Stage, i_Op+1);
      }
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* replace the <stageNm> with xmit <nodeNo>                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vAddTopologyParams(OpSpec* pOpSpec, ePX eTopologyX, i8 i_Stage, i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // arg-0 is the cross topology of the xmit op
  pOpSpec->vAddTopologyEnamePair("<topology>"_Stz,(i8)eTopologyX,TN_INTEGER);
  
  // arg-1 is the next stage for the xmit
  pOpSpec->vAddTopologyEnamePair("<nextStage>"_Stz,(i8)i_Stage,TN_INTEGER);
  
  // arg-2 is the op of the next stage for the xmit
  pOpSpec->vAddTopologyEnamePair("<nextOp>"_Stz,(i8)i_Op,TN_INTEGER);
  //pOpSpec->pEnameSet()->vPrint();
}

/*----------------------------------------------------------------------------*/
/* replace the <stageNm> with xmit <nodeNo>                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vReplaceTopologyParams(OpSpec* pOpSpec, ePX eTopologyX, i8 i_Stage, i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  // arg-0 is the cross topology of the xmit op
  pOpSpec->vReplaceTopologyEnamePair("<topology>"_Stz,(i8)eTopologyX,TN_INTEGER);
  
  // arg-1 is the next stage for the xmit
  pOpSpec->vReplaceTopologyEnamePair("<nextStage>"_Stz,(i8)i_Stage,TN_INTEGER);
  
  // arg-2 is the op of the next stage for the xmit
  pOpSpec->vReplaceTopologyEnamePair("<nextOp>"_Stz,(i8)i_Op,TN_INTEGER);
  //pOpSpec->pEnameSet()->vPrint();
}

/*----------------------------------------------------------------------------*/
/* replace the <stageNm> with xmit <nodeNo>                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vReplaceWithXmit(OpSpec* pOpSpec, ePX eTopologyX, i8 i_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  pOpSpec->vName("xmitOp"_Stz);
  
  vAddTopologyParams(pOpSpec, eTopologyX, i_Stage, 0);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
