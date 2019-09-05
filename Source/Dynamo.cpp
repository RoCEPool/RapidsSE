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
Dynamo::Dynamo(void)
: Object(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
   _checkInitialised_(CN_DYNAMO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamo::~Dynamo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile entire plan                                                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompile(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  i8 nTokens = pTknVec->nTokens();

  for (i8 i_Tkn = 0; i_Tkn < nTokens; ) {
    Token* pTkn = pTknVec->pGet(i_Tkn++);
    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szText());
  }
  
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);

  bool bResult = true;
  for (i8 i_Tkn = 2; i_Tkn < nTokens && bResult; ) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    
    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szText());
    if (pTkn->bSame("name"_Stz)) {
      bResult = bCompileName(pTknVec,i_Tkn);
      continue;
    }
    if (pTkn->bSame("stage"_Stz)) {
      bResult = bCompileStage(pTknVec,i_Tkn);
      continue;
    }
    if (pTkn->bSame(";"_Stz)) {
      break;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* compile name phrase                                                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileName(TokenVec* pTknVec, i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Token* pTkn = pTknVec->pGet(i_Tkn);
  ASSERT(pTkn->bSame("name"_Stz));
  
  pTkn = pTknVec->pGet(++ i_Tkn);
  vTrace("        name=%s\n"_Stz,pTkn->szText());
  
  ++ i_Tkn;

  return true;
}

/*----------------------------------------------------------------------------*/
/* compile stage                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileStage(TokenVec* pTknVec, i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Token* pTkn = pTknVec->pGet(i_Tkn);
  ASSERT(pTkn->bSame("stage"_Stz));
 
  pTkn = pTknVec->pGet(++ i_Tkn);
  vTrace("        stage=%s\n"_Stz,pTkn->szText());

  pTkn = pTknVec->pGet(++ i_Tkn);
  if (pTkn->bSame("on"_Stz))
    bCompileTopology(pTknVec,i_Tkn);

  pTkn = pTknVec->pGet(++ i_Tkn);
  if (bIsOperator(pTkn))
    bCompilePipe(pTknVec,i_Tkn);

  return true;
}

/*----------------------------------------------------------------------------*/
/* compile topology                                                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompileTopology(TokenVec* pTknVec, i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Token* pTkn = pTknVec->pGet(i_Tkn);
  ASSERT(pTkn->bSame("on"_Stz));
  
  pTkn = pTknVec->pGet(++ i_Tkn);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :
  
  if (pTkn->bSame("all"_Stz)) {
    
    pTkn = pTknVec->pGet(++ i_Tkn);
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all nodes :

    if (pTkn->bSame("nodes"_Stz)) {
      pTkn = pTknVec->pGet(++ i_Tkn);
      if (pTkn->bSame(":"_Stz)) {
        vTrace("recognised >on all nodes :<"_Stz);
        ++ i_Tkn;
        return true;
      }
    }
    
    // -- -- -- -- -- -- -- -- -- -- --
    // on all partitions :
    
    if (pTkn->bSame("partitions"_Stz)) {
      pTkn = pTknVec->pGet(++ i_Tkn);
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
    pTkn = pTknVec->pGet(++ i_Tkn);
    s32 sNodeNm(pTkn->szText());
    pTkn = pTknVec->pGet(++ i_Tkn);
    if (pTkn->bSame(":"_Stz)) {
      vTrace("recognised >on node %s :<"_Stz,sNodeNm.aStz());
      ++ i_Tkn;
      return true;
    }
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :
  
  if (pTkn->bSame("origin"_Stz)) {
    pTkn = pTknVec->pGet(++ i_Tkn);
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
bool Dynamo::bCompilePipe(TokenVec* pTknVec, i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Token* pTkn = pTknVec->pGet(i_Tkn);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* is token a known operator?                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bIsOperator(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Dynamo)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
