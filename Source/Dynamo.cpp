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
    Token* pTkn = m_p_TknVec->pGet(i_Tkn++);
    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szText());
  }
  
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
  
  bool bResult = true;
  i8 i_Tkn = 2;
  
  Token* pTkn = pGetToken(i_Tkn);
  
  if (pTkn->bSame("name"_Stz)) {
    bResult = bCompileName(i_Tkn);
    pTkn = pGetToken(i_Tkn);
  }

  while (pTkn->bSame("stage"_Stz)) {
    bResult = bCompileStage(i_Tkn);
    pTkn = pGetToken(i_Tkn);
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* compile name phrase                                                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileName(i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Token* pTkn = pGetToken(i_Tkn);
  ASSERT(pTkn->bSame("name"_Stz));
  vAcceptToken(i_Tkn);

  pTkn = m_p_TknVec->pGet(i_Tkn);
  vTrace("        name=%s\n"_Stz,pTkn->szText());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDynamicSpec = new DynamicSpec(pTkn->szText());
  vAcceptToken(i_Tkn);

  return true;
}

/*----------------------------------------------------------------------------*/
/* compile stage                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileStage(i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  delete m_pStageSpec; //! TEMP
  
  Token* pTkn = pGetToken(i_Tkn);
  ASSERT(pTkn->bSame("stage"_Stz));
 
  vAcceptToken(i_Tkn);
  pTkn = pGetToken(i_Tkn);
  vTrace("        stage=%s\n"_Stz,pTkn->szText());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pStageSpec = new StageSpec(pTkn->szText());
  vAcceptToken(i_Tkn);
  
  pTkn = pGetToken(i_Tkn);
  if (pTkn->bSame("on"_Stz))
    bCompileTopology(i_Tkn);
  
  pTkn = pGetToken(i_Tkn);
  if (bIsOperator(pTkn))
    bCompilePipe(i_Tkn);

  return true;
}

/*----------------------------------------------------------------------------*/
/* compile topology                                                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileTopology(i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Token* pTkn = pGetToken(i_Tkn);
  ASSERT(pTkn->bSame("on"_Stz));
  vAcceptToken(i_Tkn);
  
  pTkn = pGetToken(i_Tkn);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :
  
  if (pTkn->bSame("all"_Stz)) {
    vAcceptToken(i_Tkn);
    
    pTkn = pGetToken(i_Tkn);
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all nodes :

    if (pTkn->bSame("nodes"_Stz)) {
      vAcceptToken(i_Tkn);
      
      pTkn = pGetToken(i_Tkn);
      if (pTkn->bSame(":"_Stz)) {
        vTrace("recognised >on all nodes :<"_Stz);
        m_pStageSpec->vTopology(PO_ALL_NODES);
        return true;
      }
    }
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all partitions :
    
    if (pTkn->bSame("partitions"_Stz)) {
      vAcceptToken(i_Tkn);
      
      pTkn = pGetToken(i_Tkn);
      if (pTkn->bSame(":"_Stz)) {
        vTrace("recognised >on all partitions :<"_Stz);
        m_pStageSpec->vTopology(PO_ALL_PARTITIONS);
        return true;
      }
    }
  }
 
  // -- -- -- -- -- -- -- -- -- -- --
  // on node <nodeNm> :
  
  if (pTkn->bSame("node"_Stz)) {
    vAcceptToken(i_Tkn);
    
    pTkn = pGetToken(i_Tkn);
    s32 sLocus(pTkn->szText());
    vAcceptToken(i_Tkn);
    pTkn = m_p_TknVec->pGet(++ i_Tkn);
    
    if (pTkn->bSame(":"_Stz)) {
      vTrace("recognised >on node %s :<"_Stz,sLocus.aStz());
      m_pStageSpec->vTopology(PO_SPECIFIC_NODE);
      m_pStageSpec->vLocus(sLocus.aStz());
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  if (pTkn->bSame("origin"_Stz)) {
    vAcceptToken(i_Tkn);
    
    pTkn = pGetToken(i_Tkn);
    
    if (pTkn->bSame(":"_Stz)) {
      vTrace("recognised >on origin :<"_Stz);
      m_pStageSpec->vTopology(PO_ORIGIN);
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  return false;
}

/*----------------------------------------------------------------------------*/
/* compile pipe                                                               */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompilePipe(i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  for (;;) {
    
    Token* pTkn = pGetToken(i_Tkn);
    
    if (! bIsOperator(pTkn))
      break;

    bCompileOp(i_Tkn);
    
    pTkn = pGetToken(i_Tkn);
    if (! pTkn->bSame("|"_Stz))
      break;
    
    vAcceptToken(i_Tkn);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compile op                                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileOp(i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  delete m_pOpSpec; //! TEMP
  
  Token* pTkn = pGetToken(i_Tkn);
  
  vTrace("        op=%s\n"_Stz,pTkn->szText());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pOpSpec = new OpSpec(pTkn->szText());
  vAcceptToken(i_Tkn);

  for (;;) {
    
    pTkn = pGetToken(i_Tkn);
    
    if (pTkn->bSame("|"_Stz) || pTkn->bSame("stage"_Stz)
                             || pTkn->bSame("end"_Stz)) {
      break;
    }
    
    vAcceptToken(i_Tkn);
  }

  return true;
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
/* is token a known operator?                                                 */
/*----------------------------------------------------------------------------*/
void Dynamo::vAcceptToken(i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);

  ++ i_Tkn;
}

/*----------------------------------------------------------------------------*/
/* is token a known operator?                                                 */
/*----------------------------------------------------------------------------*/
Token* Dynamo::pGetToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Token* pTkn = m_p_TknVec->pGet(i_Tkn);
  
  vTrace("%03lld: %s\n"_Stz,i_Tkn,pTkn->szText());
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Dynamo)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
