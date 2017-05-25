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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  for (i8 iToken = 0; iToken < C_TOKENS; iToken++)
    m_xTokenAry[iToken] = (Str*)&m_xTokenArea.aArea()[C_TOKEN_SPACE * iToken];
  
  vReset();
  
  _checkInitialised_(CN_TOKENISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TokenSet::~TokenSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* [] operator                                                                */
/*----------------------------------------------------------------------------*/
Str*& TokenSet::operator [](i8 iTokenNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  return m_xTokenAry[iTokenNo];
}

/*----------------------------------------------------------------------------*/
/* get number of tokens                                                       */
/*----------------------------------------------------------------------------*/
i8 TokenSet::nTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  return m_nTokens;
}

/*----------------------------------------------------------------------------*/
/* inc number of tokens                                                       */
/*----------------------------------------------------------------------------*/
void TokenSet::vIncTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  m_nTokens++;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void TokenSet::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET,_FOOTER_);
  
  m_nTokens = 0;
  
  for (i8 iToken = 0; iToken < C_TOKENS; iToken++)
    m_xTokenAry[iToken]->vCopy(""_Str);
}

/*----------------------------------------------------------------------------*/
/* set the number of tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenSet::vSetTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET,_FOOTER_);
  
   // get count of words in pWords
  for (m_nTokens = 0; m_nTokens < C_TOKENS; m_nTokens++) {
    if (m_xTokenAry[m_nTokens]->bEqual(""_Str))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TokenSet::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSET);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
