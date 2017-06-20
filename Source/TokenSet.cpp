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
: Object(CN_TOKENSET)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++)
    m_xTokenAry[i_Token] = (Str*)&m_xTokenArea.aArea()[C_TOKEN_SPACE * i_Token];
  
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
/* [] operator                                                                */
/*----------------------------------------------------------------------------*/
Str*& TokenSet::operator [](i8 i_TokenNo)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  return m_xTokenAry[i_TokenNo];
}

/*----------------------------------------------------------------------------*/
/* add a token                                                                */
/*----------------------------------------------------------------------------*/
void TokenSet::vAddToken(Str* aToken)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  i8 i_Token = m_xTokenAry.nItems();
  
  m_xTokenAry[i_Token]->vCopy(aToken);
  
  m_xTokenAry.vItems(++i_Token);
}

/*----------------------------------------------------------------------------*/
/* get number of tokens                                                       */
/*----------------------------------------------------------------------------*/
i8 TokenSet::nTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  i8 nTokens = m_xTokenAry.nItems();
 
  return nTokens;
}

/*----------------------------------------------------------------------------*/
/* inc number of tokens                                                       */
/*----------------------------------------------------------------------------*/
void TokenSet::vIncTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  i8 nTokens = m_xTokenAry.nItems();
  
  m_xTokenAry.vItems(++nTokens);
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void TokenSet::vReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  m_xTokenAry.vItems(0);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++)
    m_xTokenAry[i_Token]->vCopy(""_Str);
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
    if (m_xTokenAry[i_Token]->bEqual(""_Str))
      break;
  }
  m_xTokenAry.vItems(i_Token);
}

/*----------------------------------------------------------------------------*/
/* strip off ';'                                                              */
/*----------------------------------------------------------------------------*/
void TokenSet::vStripSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (m_xTokenAry[i_Token]->bEqual(";"_Str)) {
      *m_xTokenAry[i_Token]->s() = '\0';
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
