/*============================================================================*/
/* InputStream.cpp - Input Stream Handler                                     */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* InputParser::operator new(size_t)
{
  vFatal("fatal: InputParser class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void InputParser::operator delete(void*)
{
  vFatal("fatal: InputParser class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InputParser::InputParser(void)
: ObjVal(sizeof(InputParser))
{
  m_pCharred = nullptr;
  m_bMalloc  = false;
  m_bNormal  = false;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InputParser::~InputParser(void)
{
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void InputParser::vPush(void)
{
  m_pCharred->vPush();
}

/*----------------------------------------------------------------------------*/
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool InputParser::bPop(void)
{
  bool bPopped = m_pCharred->bPop();
  
  return bPopped;
}

/*----------------------------------------------------------------------------*/
/* get depth of parser                                                        */
/*----------------------------------------------------------------------------*/
i8 InputParser::iDepth(void)
{
  i8 iParserDepth = 0;
  
  if (m_pCharred)
    iParserDepth = m_pCharred->iDepth();
  
  return iParserDepth;
}

/*----------------------------------------------------------------------------*/
/* set malloc flag                                                            */
/*----------------------------------------------------------------------------*/
void InputParser::vMalloc(bool bMalloc)
{
  m_bMalloc = bMalloc;
}

/*----------------------------------------------------------------------------*/
/* set normal token flag                                                      */
/*----------------------------------------------------------------------------*/
void InputParser::vNormal(bool bNormal)
{
  m_bNormal = bNormal;
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool InputParser::bUseFile(s& sPath)
{
  bool bOpened = m_pCharred->bUseFile(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void InputParser::vUseStdin(void)
{
  m_pCharred->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void InputParser::vUseString(s& sString)
{
  m_pCharred->vUseString(sString);
}

/*----------------------------------------------------------------------------*/
/* parse next literal into given buffer                                       */
/*----------------------------------------------------------------------------*/
eLT InputParser::eNextLiteral(char* wLiteral)
{
  eLT eLiteral;
  
  if (m_bNormal)
    eLiteral = eNextNormal(wLiteral);
  else
    eLiteral = eNextSpecial(wLiteral);
  
  return eLiteral;
}


// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* parse next normal literal into given buffer                                */
/*----------------------------------------------------------------------------*/
eLT InputParser::eNextNormal(char* wLiteral)
{
  *wLiteral = '\0';
  
  eLT eLiteral = eParseNormal(wLiteral);
  
  return eLiteral;
}

/*----------------------------------------------------------------------------*/
/* parse next special literal into given buffer                               */
/*----------------------------------------------------------------------------*/
eLT InputParser::eNextSpecial(char* wLiteral)
{
  *wLiteral = '\0';

  eLT eLiteral = eParseSpecial(wLiteral);
  
  return eLiteral;
}


/*----------------------------------------------------------------------------*/
/*-- token processing --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse normal                                                               */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseNormal(char* wLiteral)
{
  vSkipWhitespace();
  
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal integer                                                       */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseNormalInteger(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal name                                                          */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseNormalName(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal string                                                        */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseNormalString(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse special                                                              */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseSpecial(char* wLiteral)
{
  vSkipWhitespace();
  
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse special integer                                                      */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseSpecialInteger(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse special name                                                         */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseSpecialName(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse special string                                                       */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseSpecialString(char* wLiteral)
{
  return LT_NONE;
}


/*----------------------------------------------------------------------------*/
/*-- character processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void InputParser::vSkipWhitespace(void)
{
  while ( m_cCurrChar && String::bIsWhite(m_cCurrChar) )
    vGetChar();
}

/*----------------------------------------------------------------------------*/
/* test for start of normal integer                                           */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartNormalInteger(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartNormalName(void)
{
  if (bStartNormalInteger() || bStartNormalString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartNormalString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special integer                                          */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartSpecialInteger(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special name                                             */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartSpecialName(void)
{
  if (bStartSpecialInteger() || bStartSpecialString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of special string                                           */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartSpecialString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void InputParser::vGetChar(void)
{
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void InputParser::vUngetChar(void)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/