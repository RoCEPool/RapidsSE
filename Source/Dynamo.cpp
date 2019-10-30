/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "Pipelines.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dynamo::operator new(size_t)
{
  Dynamo* pDynamo = reinterpret_cast<Dynamo*>(HeapMem::pAllocate(CN_DYNAMO));
  
  return pDynamo;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Dynamo::operator delete(void* aAddr)
{
  Dynamo* pDynamo = reinterpret_cast<Dynamo*>(aAddr);
  
  HeapMem::vFree(pDynamo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dynamo::Dynamo(TokenVec* pTknVec)
: Object(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  m_p_TknVec = pTknVec;
  
  m_pPipelineSpec = nullptr;
  m_pStageSpec   = nullptr;
  m_pOpSpec      = nullptr;
  
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
  //delete m_pStageSpec;
  delete m_pOpSpec;
}

/*----------------------------------------------------------------------------*/
/* get cross topology between two stages                                      */
/*----------------------------------------------------------------------------*/
ePX Dynamo::eTopoloyX(StageSpec*  pFromStageSpec, StageSpec* pToStageSpec) {
  
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
/* compile entire pipeline                                                    */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  bool bResult = false;
  
#ifdef WITH_TRACE_DYNAMO
  i8 nTokens = m_p_TknVec->nTokens();
  
  for (i8 i_Tkn = 0; i_Tkn < nTokens; ) {
    Token* pTkn = m_p_TknVec->pGet(i_Tkn++);

    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szText());
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
  
//  bResult = bPhase3_flatten();
//  if (! bResult)
//    return false;
//  
//  m_pPipelineSpec->vPrint();
//  
//  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
  
  bResult = bPhase4_opdefn();
  if (! bResult)
    return false;
  
#ifdef WITH_TRACE_DYNAMO
  m_pPipelineSpec->vPrint();
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
#endif
  
  return true;
}

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
  
  if (m_pTkn->bSame("begin"_Stz)) {
    bResult = bCollectBegin();
    if (! bResult)
      return false;
  }
  
  while (m_pTkn->bSame("stage"_Stz))
    bResult = bCollectStage();
  
  bResult = bCollectEnd();

  return bResult;
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
  vTrace("        pipeline=%s\n"_Stz,m_pTkn->szText());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pPipelineSpec = new PipelineSpec(m_pTkn->szText());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();
 
#ifdef WITH_TRACE_DYNAMO
  vTrace("Name Clause recognised\n"_Stz);
#endif
  
  return true;
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
/* compile stage                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  ASSERT(m_pTkn->bSame("stage"_Stz));
  vAcceptToken();
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("        stage=%s\n"_Stz,m_pTkn->szText());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pStageSpec = new StageSpec(m_pTkn->szText());
  m_pPipelineSpec->vAddStageSpec(m_pStageSpec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();
  
  bCollectTopology();
  
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
    
    if (m_pTkn->bSame("partitions"_Stz)) {
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
  }
 
  // -- -- -- -- -- -- -- -- -- -- --
  // on node <nodeNm> :
  
  if (m_pTkn->bSame("node"_Stz)) {
    vAcceptToken();
    
    s32 sLocus(m_pTkn->szText());
    vAcceptToken();

    if (m_pTkn->bSame(":"_Stz)) {
      vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
      vTrace("Topology Phrase recognised >on node %s :<\n"_Stz,sLocus.szStz());
#endif
      m_pStageSpec->vTopology(PO_SPECIFIC_NODE);
      m_pStageSpec->vLocus(sLocus.szStz());
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on partition <nodeNm> :
  
  if (m_pTkn->bSame("partition"_Stz)) {
    vAcceptToken();
    
    s32 sLocus(m_pTkn->szText());
    vAcceptToken();
    
    if (m_pTkn->bSame(":"_Stz)) {
      vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
      vTrace("Topology Phrase recognised >on partition %s :<\n"_Stz,sLocus.szStz());
#endif
      m_pStageSpec->vTopology(PO_SPECIFIC_PARTITION);
      m_pStageSpec->vLocus(sLocus.szStz());
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  if (m_pTkn->bSame("origin"_Stz)) {
    vAcceptToken();
    
    if (m_pTkn->bSame(":"_Stz)) {
      vAcceptToken();
#ifdef WITH_TRACE_DYNAMO
      vTrace("Topology Phrase recognised >on origin :<\n"_Stz);
#endif
      m_pStageSpec->vTopology(PO_ORIGIN);
      return true;
    }
  }

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
  vTrace("        op=%s\n"_Stz,m_pTkn->szText());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pOpSpec = new OpSpec(m_pTkn->szText(),m_pStageSpec);
  m_pStageSpec->vAddOpSpec(m_pOpSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();

  i8 i_Arg = 0;
  for (;;) {
    
    if (m_pTkn->bSame("|"_Stz) || m_pTkn->bSame("stage"_Stz)
                               || m_pTkn->bSame("end"_Stz)) {
      break;
    }
    
    // add an argument
    
    s32 sArgName;
    sArgName.vSprint("<arg_%lld>"_Stz,i_Arg++);
    
    s256 sArgValue ( m_pTkn->szText() );
    
    m_pOpSpec->vAddEnmPair(sArgName,sArgValue,TN_STRING);
    
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Phase2 : staging                                                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPhase2_staging(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  StageSpecVec* pStageSpecVec = m_pPipelineSpec->pStageSpecVec();
  
  for (i8 i_Stage = 0; i_Stage < pStageSpecVec->nItems(); i_Stage++) {
    StageSpec* pStageSpec = pStageSpecVec->pGet(i_Stage);

    // prepend a StartOp to the first stage
    if (i_Stage == 0)
      pStageSpec->vPrependStartOp();
    
    OpSpecVec* pOpSpecVec = pStageSpec->pOpSpecVec();
    for (i8 i_Op = 0; i_Op < pOpSpecVec->nItems(); i_Op++) {
      OpSpec* pOpSpec = pOpSpecVec->pGet(i_Op);
      
#ifdef WITH_TRACE_DYNAMO
      vTrace("  op=%s\n"_Stz,pOpSpec->szName());
#endif
      i8 i_Stage = C_UNKNOWN;
      StageSpec* pStageSpecFound = m_pPipelineSpec->pFindStageSpec(pOpSpec->szName(), i_Stage);
      if (pStageSpecFound) {
        ePX eTopologyX = eTopoloyX(pStageSpec, pStageSpecFound);
        
#ifdef WITH_TRACE_DYNAMO
        vTrace("  op %s is a stage %lld %s\n"_Stz,pOpSpec->szName(), i_Stage, aPX[eTopologyX]);
#endif
        
        vReplaceWithXmit(pOpSpec, eTopologyX, i_Stage);
        pStageSpecFound->vPrependConsumeOp(eTopologyX);
      }
      
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* replace the <stageNm> with xmit <nodeNo>                                   */
/*----------------------------------------------------------------------------*/
void Dynamo::vReplaceWithXmit(OpSpec* pOpSpec, ePX eTopologyX, i8 i_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  pOpSpec->vName("xmit"_Stz);
  
  // arg-0 is the cross topology of the xmit op
  pOpSpec->vAddEnmPair("<arg_0>"_Stz,(i8)eTopologyX,TN_INTEGER);
  
  // arg-1 is the next stage for the xmit
  pOpSpec->vAddEnmPair("<arg_1>"_Stz,(i8)i_Stage,TN_INTEGER);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Phase3 : flatten into one stage                                            */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPhase3_flatten(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  StageSpecVec* pStageSpecVec = m_pPipelineSpec->pStageSpecVec();
  
  StageSpec* pStageSpec_0 = pStageSpecVec->pGet(0);
  for (i8 i_Stage = 1; i_Stage < pStageSpecVec->nItems(); i_Stage++) {
    StageSpec* pStageSpec_n = pStageSpecVec->pGet(i_Stage);
    
    pStageSpec_0->vCopyOps(pStageSpec_n);
  }
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Phase4 : find op defn's                                                    */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPhase4_opdefn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  StageSpecVec* pStageSpecVec = m_pPipelineSpec->pStageSpecVec();
  
  for (i8 i_Stage = 0; i_Stage < pStageSpecVec->nItems(); i_Stage++) {
    StageSpec* pStageSpec = pStageSpecVec->pGet(i_Stage);
    
    OpSpecVec* pOpSpecVec = pStageSpec->pOpSpecVec();
    for (i8 i_Op = 0; i_Op < pOpSpecVec->nItems(); i_Op++) {
      OpSpec* pOpSpec = pOpSpecVec->pGet(i_Op);
      
      OpDefn* aOpDefn = gxOpCatalog.aFindOp(pOpSpec->szName());
      
      //! HOW DO WE HANDLE ERRORS?
      if (! aOpDefn) {
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "Unable to find %s in op catalog"_Stz,pOpSpec->szName());
      }
      
      pOpSpec->vOpDefn(aOpDefn);
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void Dynamo::vAcceptToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

  ++ m_i_Tkn;
  
  m_pTkn = m_p_TknVec->pGet(m_i_Tkn);
#ifdef WITH_TRACE_DYNAMO
  vTrace("Next %03lld: %s\n"_Stz,m_i_Tkn,m_pTkn->szText());
#endif
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void Dynamo::vBeginToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  m_i_Tkn = i_Tkn;
  m_pTkn  = m_p_TknVec->pGet(m_i_Tkn);
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("Begin %03lld: %s\n"_Stz,i_Tkn,m_pTkn->szText());
#endif
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
/* set the pipeline spec                                                       */
/*----------------------------------------------------------------------------*/
void Dynamo::vPipelineSpec(PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  m_pPipelineSpec = pPipelineSpec;
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Dynamo)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
