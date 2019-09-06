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
  
  m_pDynamicSpec = new DynamicSpec;

  bool bResult = true;
  for (i8 i_Tkn = 2; i_Tkn < nTokens && bResult; ) {
    Token* pTkn = m_p_TknVec->pGet(i_Tkn);
    
    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szText());
    if (pTkn->bSame("name"_Stz)) {
      bResult = bCompileName(i_Tkn);
      continue;
    }
    if (pTkn->bSame("stage"_Stz)) {
      bResult = bCompileStage(i_Tkn);
      continue;
    }
    if (pTkn->bSame("end"_Stz)) {
      break;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* compile name phrase                                                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileName(i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Token* pTkn = m_p_TknVec->pGet(i_Tkn);
  ASSERT(pTkn->bSame("name"_Stz));
  
  pTkn = m_p_TknVec->pGet(++ i_Tkn);
  vTrace("        name=%s\n"_Stz,pTkn->szText());
  
  ++ i_Tkn;

  return true;
}

/*----------------------------------------------------------------------------*/
/* compile stage                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileStage(i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  delete m_pStageSpec; //! TEMP
  
  m_pStageSpec = new StageSpec;

  Token* pTkn = m_p_TknVec->pGet(i_Tkn);
  ASSERT(pTkn->bSame("stage"_Stz));
 
  pTkn = m_p_TknVec->pGet(++ i_Tkn);
  vTrace("        stage=%s\n"_Stz,pTkn->szText());

  pTkn = m_p_TknVec->pGet(++ i_Tkn);
  if (pTkn->bSame("on"_Stz))
    bCompileTopology(i_Tkn);

  pTkn = m_p_TknVec->pGet(++ i_Tkn);
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
  
  Token* pTkn = m_p_TknVec->pGet(i_Tkn);
  ASSERT(pTkn->bSame("on"_Stz));
  
  pTkn = m_p_TknVec->pGet(++ i_Tkn);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :
  
  if (pTkn->bSame("all"_Stz)) {
    
    pTkn = m_p_TknVec->pGet(++ i_Tkn);
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all nodes :

    if (pTkn->bSame("nodes"_Stz)) {
      pTkn = m_p_TknVec->pGet(++ i_Tkn);
      if (pTkn->bSame(":"_Stz)) {
        vTrace("recognised >on all nodes :<"_Stz);
        ++ i_Tkn;
        return true;
      }
    }
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all partitions :
    
    if (pTkn->bSame("partitions"_Stz)) {
      pTkn = m_p_TknVec->pGet(++ i_Tkn);
      if (pTkn->bSame(":"_Stz)) {
        vTrace("recognised >on all partitions :<"_Stz);
        ++ i_Tkn;
        return true;
      }
    }
  }
 
  // -- -- -- -- -- -- -- -- -- -- --
  // on node <nodeNm> :
  
  if (pTkn->bSame("node"_Stz)) {
    pTkn = m_p_TknVec->pGet(++ i_Tkn);
    s32 sNodeNm(pTkn->szText());
    pTkn = m_p_TknVec->pGet(++ i_Tkn);
    if (pTkn->bSame(":"_Stz)) {
      vTrace("recognised >on node %s :<"_Stz,sNodeNm.aStz());
      ++ i_Tkn;
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  if (pTkn->bSame("origin"_Stz)) {
    pTkn = m_p_TknVec->pGet(++ i_Tkn);
    if (pTkn->bSame(":"_Stz)) {
      vTrace("recognised >on origin :<"_Stz);
      ++ i_Tkn;
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
  
  delete m_pOpSpec; //! TEMP
  
  m_pOpSpec = new OpSpec;
  
  for (;;) {
    Token* pTkn = m_p_TknVec->pGet(++ i_Tkn);
    
    if (pTkn->bSame("|"_Stz))
      continue;
    
    if (! bIsOperator(pTkn))
      break;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* is token a known operator?                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bIsOperator(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
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
