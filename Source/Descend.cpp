/*============================================================================*/
/* Descend.cpp -- Token Vector Descender                                      */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Descend,CN_DESCEND)
__aNewFunction(Descend,CN_DESCEND)
__vDeleteFunction(Descend,CN_DESCEND)
__aFooterFunction(Descend)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Descend::Descend(TokenVec* pArgVec, Stmt* pStmt)
: Object(CN_DESCEND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_nDescend = 0;
  for (i8 i_Dispatch = 0; i_Dispatch < C_MAX_DESCENDDSP; i_Dispatch++) {
    m_xaDescend[i_Dispatch].m_sName   = C_NO_NAME;
    m_xaDescend[i_Dispatch].m_pFunc   = nullptr;
    m_xaDescend[i_Dispatch].m_bSubQry = false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_iNesting = 0;
  
  m_pTokenVec = new TokenVec;
  
  m_pTokenVec->vDuplicateVector(pArgVec);
  m_pTokenVec->vCurrToken(0);
  
  m_pStmt = pStmt;
    
  _checkInitialised_(CN_DESCEND);
  
  vInit();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Descend::~Descend(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  delete m_pTokenVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current token                                                          */
/*----------------------------------------------------------------------------*/
i8 Descend::iCurrToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  i8 iCurr = m_pTokenVec->iCurrToken();
  
  return iCurr;
}

/*----------------------------------------------------------------------------*/
/* get current token                                                          */
/*----------------------------------------------------------------------------*/
Token* Descend::pCurrToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  Token* pCurr = m_pTokenVec->pCurrToken();
  
  return pCurr;
}

/*----------------------------------------------------------------------------*/
/* set to and get first token                                                 */
/*----------------------------------------------------------------------------*/
Token* Descend::pFirstToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  vFirstToken();
  
  Token* pFirst = m_pTokenVec->pCurrToken();
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* set to first token                                                         */
/*----------------------------------------------------------------------------*/
void Descend::vFirstToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  m_pTokenVec->vCurrToken(0);
}

/*----------------------------------------------------------------------------*/
/* set to and get last token                                                  */
/*----------------------------------------------------------------------------*/
Token* Descend::pLastToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  vLastToken();
  
  Token* pLast = m_pTokenVec->pCurrToken();
  
  return pLast;
}

/*----------------------------------------------------------------------------*/
/* set to last token                                                          */
/*----------------------------------------------------------------------------*/
void Descend::vLastToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  i8 nTokens = m_pTokenVec->nTokens();
  
  m_pTokenVec->vCurrToken(nTokens - 1);
}

/*----------------------------------------------------------------------------*/
/* more tokens?                                                               */
/*----------------------------------------------------------------------------*/
bool Descend::bMoreTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  i8 nTokens = m_pTokenVec->nTokens();
  i8 i_CurTkn = m_pTokenVec->iCurrToken();
  
  if (i_CurTkn < nTokens)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* decrement current nesting level                                            */
/*----------------------------------------------------------------------------*/
void Descend::vNestDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_iNesting--;
}

/*----------------------------------------------------------------------------*/
/* increment current nesting level                                            */
/*----------------------------------------------------------------------------*/
void Descend::vNestInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_iNesting++;
}

/*----------------------------------------------------------------------------*/
/* get current nesting level                                                  */
/*----------------------------------------------------------------------------*/
i8 Descend::iNesting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  return m_iNesting;
}

/*----------------------------------------------------------------------------*/
/* set to and get next token                                                  */
/*----------------------------------------------------------------------------*/
Token* Descend::pNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  Token* pNext = m_pTokenVec->pCurrToken();
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* set to next token                                                          */
/*----------------------------------------------------------------------------*/
void Descend::vNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_pTokenVec->vNextToken();
}

/*----------------------------------------------------------------------------*/
/* set to and get next token                                                  */
/*----------------------------------------------------------------------------*/
Token* Descend::pPrevToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  Token* pPrev = m_pTokenVec->pPrevToken();
  
  return pPrev;
}

/*----------------------------------------------------------------------------*/
/* set to next token                                                          */
/*----------------------------------------------------------------------------*/
void Descend::vPrevToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_pTokenVec->vPrevToken();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call descn func                                                            */
/*----------------------------------------------------------------------------*/
bool Descend::bCallFunc(Clause_SubQry* pSubQry)
{
  Token* pCurTkn = m_pTokenVec->pCurrToken();
  
  Stz* szChars = pCurrToken()->szChars();
  
  i8 i_Dsp = iFindDescend(szChars);
  
  ASSERT(C_UNKNOWN != i_Dsp);
  bool b = (*m_xaDescend[i_Dsp].m_pFunc)(this,pSubQry);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is current token dispatcable?                                              */
/*----------------------------------------------------------------------------*/
bool Descend::bIsDescendParse(bool& bSubQry_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  Token* pCurTkn = m_pTokenVec->pCurrToken();
  
  Stz* szChars = pCurrToken()->szChars();
  
  i8 i_Dsp = iFindDescend(szChars);

  if (C_UNKNOWN == i_Dsp) {
    bSubQry_ = false;
    return false;
  }
  else {
    bSubQry_ = m_xaDescend[i_Dsp].m_bSubQry;
    return true;
  }
}

/*----------------------------------------------------------------------------*/
/* dispatch init                                                              */
/*----------------------------------------------------------------------------*/
i8 Descend::iFindDescend(Stz* szChars)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  Token* pCurTkn = m_pTokenVec->pCurrToken();
  
  for (i8 i_Dsp = 0; i_Dsp < m_nDescend; i_Dsp++)
    if (m_xaDescend[i_Dsp].m_sName.bSame(pCurrToken()->szChars()))
      return i_Dsp;
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get statement                                                              */
/*----------------------------------------------------------------------------*/
Stmt* Descend::pStmt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  return m_pStmt;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dispatch init                                                              */
/*----------------------------------------------------------------------------*/
void Descend::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_nDescend = 0;
  
  m_xaDescend[m_nDescend].m_sName = C_CLAUSE_FROM;
  m_xaDescend[m_nDescend].m_pFunc = Clause_From::bParse;
  m_nDescend++;

  m_xaDescend[m_nDescend].m_sName = C_CLAUSE_GROUP;
  m_xaDescend[m_nDescend].m_pFunc = Clause_GroupBy::bParse;
  m_nDescend++;

  m_xaDescend[m_nDescend].m_sName = C_CLAUSE_LIMIT;
  m_xaDescend[m_nDescend].m_pFunc = Clause_Limit::bParse;
  m_nDescend++;

  m_xaDescend[m_nDescend].m_sName = C_CLAUSE_ORDER;
  m_xaDescend[m_nDescend].m_pFunc = Clause_OrderBy::bParse;
  m_nDescend++;

  m_xaDescend[m_nDescend].m_sName = C_CLAUSE_SELECT;
  m_xaDescend[m_nDescend].m_pFunc = Clause_Select::bParse;
  m_xaDescend[m_nDescend].m_bSubQry = true;
  m_nDescend++;

  m_xaDescend[m_nDescend].m_sName = C_CLAUSE_WHERE;
  m_xaDescend[m_nDescend].m_pFunc = Clause_Where::bParse;
  m_nDescend++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
