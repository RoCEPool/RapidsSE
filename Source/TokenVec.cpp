/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/

#include "Parsing.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TokenVec* TokenVec::_pTokenSet(void* aAddr)
{
  TokenVec* pTokenSet = reinterpret_cast<TokenVec*>(aAddr);
  
  RASSERT( !pTokenSet || CN_TOKENVEC == pTokenSet->eClass() );
  
  return pTokenSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TokenVec::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: TokenVec class cannot be new'd"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TokenVec::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: TokenVec class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TokenVec::TokenVec(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++)
    m_pItem[i_Token] = new Token;
  
  vReset();
  
  _checkInitialised_(CN_TOKENISER);
  
  _postcondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TokenVec::~TokenVec(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add a token                                                                */
/*----------------------------------------------------------------------------*/
void TokenVec::vAddToken(Str* aStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_pItem[m_nItems++]->vCopy(aStr);
  
}

/*----------------------------------------------------------------------------*/
/* get number of tokens                                                       */
/*----------------------------------------------------------------------------*/
i8 TokenVec::nTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* inc number of tokens                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vIncTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++)
    m_pItem[i_Token]->vReset();
}

/*----------------------------------------------------------------------------*/
/* set the number of tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vSetTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
   // get count of words in pWords
  i8 i_Token;
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if ( ! m_pItem[i_Token]->bHasText() )
      break;
  }
  
  m_nItems = i_Token;
}

/*----------------------------------------------------------------------------*/
/* strip off ';'                                                              */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (m_pItem[i_Token]->bSame(";"_Str)) {
      m_pItem[i_Token]->vReset();
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TokenVec::aFooter(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
