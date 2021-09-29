/*============================================================================*/
/* Scanner.cpp -- Token Scanner                                               */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Scanner,CN_SCANNER)
__aNewFunction(Scanner,CN_SCANNER)
__vDeleteFunction(Scanner,CN_SCANNER)
__aFooterFunction(Scanner)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Scanner::Scanner(Stz* szStatement)
: Object(CN_SCANNER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANNER,_FOOTER_);
      
  m_pTknVec = new TokenVec(szStatement);
  m_i_Tkn   = 0;
  m_p_Tkn   = nullptr;
  
  _checkInitialised_(CN_SCANNER);
  
  vTokenBegin(0);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCANNER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Scanner::~Scanner(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANNER,_FOOTER_);
    
  m_pTknVec->vSetAllTokens();
  delete m_pTknVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current token                                                          */
/*----------------------------------------------------------------------------*/
Token* Scanner::pTkn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANNER,_FOOTER_);
  
  return m_p_Tkn;
}

/*----------------------------------------------------------------------------*/
/* get token by subscript                                                     */
/*----------------------------------------------------------------------------*/
Token* Scanner::pTkn(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANNER,_FOOTER_);
  
  Token* pToken = m_pTknVec->pGet(i_Tkn);
  
  return pToken;
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
TokenVec* Scanner::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANNER,_FOOTER_);
  
  return m_pTknVec;
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void Scanner::vTokenAccept(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANNER,_FOOTER_);

  ++ m_i_Tkn;
  
  m_p_Tkn = m_pTknVec->pGet(m_i_Tkn);
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void Scanner::vTokenBegin(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANNER,_FOOTER_);
  
  m_i_Tkn = i_Tkn;
  m_p_Tkn  = m_pTknVec->pGet(m_i_Tkn);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
