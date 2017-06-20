/*============================================================================*/
/* TokenSet.hpp -- set of tokens                                              */
/*============================================================================*/

#include "Parsing.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TokenSet* TokenSet::_pTokenSet(void* aAddr)
{
  TokenSet* pTokenSet = reinterpret_cast<TokenSet*>(aAddr);
  
  RASSERT( !pTokenSet || CN_TOKENSET == pTokenSet->eClass() );
  
  return pTokenSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TokenSet::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: TokenSet class cannot be new'd"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TokenSet::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: TokenSet class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TokenSet::TokenSet(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++)
    m_pItem[i_Token] = new Token;
  
  vReset();
  
  _checkInitialised_(CN_TOKENISER);
  
  _postcondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TokenSet::~TokenSet(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add a token                                                                */
/*----------------------------------------------------------------------------*/
void TokenSet::vAddToken(Str* aStr)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  m_pItem[m_nItems++]->vCopy(aStr);
  
}

/*----------------------------------------------------------------------------*/
/* get number of tokens                                                       */
/*----------------------------------------------------------------------------*/
i8 TokenSet::nTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* inc number of tokens                                                       */
/*----------------------------------------------------------------------------*/
void TokenSet::vIncTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void TokenSet::vReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++)
    m_pItem[i_Token]->vReset();
}

/*----------------------------------------------------------------------------*/
/* set the number of tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenSet::vSetTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
   // get count of words in pWords
  i8 i_Token;
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (m_pItem[i_Token]->bSame(""_Str))
      break;
  }
  
  m_nItems = i_Token;
}

/*----------------------------------------------------------------------------*/
/* strip off ';'                                                              */
/*----------------------------------------------------------------------------*/
void TokenSet::vStripSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
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
Footer* TokenSet::aFooter(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
