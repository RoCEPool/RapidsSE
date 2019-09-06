/*============================================================================*/
/* Dynamo.cpp -- Dynamo (dynamic plan compiler)                               */
/*============================================================================*/

#include "Dynamics.h"

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
  
  m_pDynamicSpec = nullptr;
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
  
  delete m_pDynamicSpec;
  delete m_pStageSpec;
  delete m_pOpSpec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile entire plan                                                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  i8 nTokens = m_p_TknVec->nTokens();

  for (i8 i_Tkn = 0; i_Tkn < nTokens; ) {
    Token* m_pTkn = m_p_TknVec->pGet(i_Tkn++);
    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,m_pTkn->szText());
  }
  
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
  
  bool bResult = true;
  
  vBeginToken(1);
  
  if (m_pTkn->bSame("plan"_Stz))
    bResult = bCompileName();

  if (m_pTkn->bSame("begin"_Stz))
    bResult = bCompileBegin();
  
  while (m_pTkn->bSame("stage"_Stz))
    bResult = bCompileStage();

  bResult = bCompileEnd();
  
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);

  m_pDynamicSpec->vPrint();
  
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* compile name phrase                                                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  ASSERT(m_pTkn->bSame("plan"_Stz));
  vAcceptToken();

  vTrace("        plan=%s\n"_Stz,m_pTkn->szText());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDynamicSpec = new DynamicSpec(m_pTkn->szText());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();
  
  vTrace("Name Clause recognised\n"_Stz);

  return true;
}

/*----------------------------------------------------------------------------*/
/* compile end                                                                */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  ASSERT(m_pTkn->bSame("begin"_Stz));
  vAcceptToken();

  vTrace("Begin recognised\n"_Stz);
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile stage                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  delete m_pStageSpec; //! TEMP
  
  ASSERT(m_pTkn->bSame("stage"_Stz));
  vAcceptToken();
  
  vTrace("        stage=%s\n"_Stz,m_pTkn->szText());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pStageSpec = new StageSpec(m_pTkn->szText());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();
  
  if (m_pTkn->bSame("on"_Stz))
    bCompileTopology();
  
  if (bIsOperator(m_pTkn))
    bCompilePipe();

  vTrace("Stage Clause recognised\n"_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile topology                                                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  ASSERT(m_pTkn->bSame("on"_Stz));
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
        vTrace("Topology Phrase recognised >on all nodes :<\n"_Stz);
        m_pStageSpec->vTopology(PO_ALL_NODES);
        return true;
      }
    }
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all partitions :
    
    if (m_pTkn->bSame("partitions"_Stz)) {
      vAcceptToken();
      
      if (m_pTkn->bSame(":"_Stz)) {
        vTrace("Topology Phrase recognised >on all partitions :<\n"_Stz);
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
      vTrace("Topology Phrase recognised >on node %s :<\n"_Stz,sLocus.aStz());
      m_pStageSpec->vTopology(PO_SPECIFIC_NODE);
      m_pStageSpec->vLocus(sLocus.aStz());
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
      vTrace("Topology Phrase recognised >on partition %s :<\n"_Stz,sLocus.aStz());
      m_pStageSpec->vTopology(PO_SPECIFIC_PARTITION);
      m_pStageSpec->vLocus(sLocus.aStz());
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  if (m_pTkn->bSame("origin"_Stz)) {
    vAcceptToken();
    
    if (m_pTkn->bSame(":"_Stz)) {
      vTrace("Topology Phrase recognised >on origin :<\n"_Stz);
      m_pStageSpec->vTopology(PO_ORIGIN);
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  vTrace("Topology Phrase not recognised\n"_Stz);
  return false;
}

/*----------------------------------------------------------------------------*/
/* compile pipe                                                               */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompilePipe(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  for (;;) {
    
    if (! bIsOperator(m_pTkn))
      break;

    bCompileOp();
    
    if (! m_pTkn->bSame("|"_Stz))
      break;
    
    vAcceptToken();
  }
  
  vTrace("Pipe Phrase recognised\n"_Stz);
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile op                                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  delete m_pOpSpec; //! TEMP
  
  vTrace("        op=%s\n"_Stz,m_pTkn->szText());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pOpSpec = new OpSpec(m_pTkn->szText());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAcceptToken();

  for (;;) {
    
    if (m_pTkn->bSame("|"_Stz) || m_pTkn->bSame("stage"_Stz)
                               || m_pTkn->bSame("end"_Stz)) {
      break;
    }
    
    vAcceptToken();
  }

 vTrace("Op Phrase recognised\n"_Stz);
 return true;
}

/*----------------------------------------------------------------------------*/
/* compile end                                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  ASSERT(m_pTkn->bSame("end"_Stz));

  vTrace("End recognised\n"_Stz);
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
  vTrace("Next %03lld: %s\n"_Stz,m_i_Tkn,m_pTkn->szText());
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void Dynamo::vBeginToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  m_i_Tkn = i_Tkn;
  m_pTkn  = m_p_TknVec->pGet(m_i_Tkn);
  
  vTrace("Begin %03lld: %s\n"_Stz,i_Tkn,m_pTkn->szText());
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
