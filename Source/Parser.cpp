/*============================================================================*/
/* Parser.cpp -- input processor, stripping out line and block comments      */
/*============================================================================*/

#include "Common.h"
#include "StorageEngine.h"

extern Heap* gpMediumHeap;

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Parser::operator new(size_t, bool bMalloc)
{
  Parser* pParser;
  
  if (bMalloc)
    pParser = reinterpret_cast<Parser*>(malloc(sizeof(Parser)));
  else
    pParser = reinterpret_cast<Parser*>(gpMediumHeap->pAllocObject());
  
  pParser->m_bMalloc = bMalloc;
  
  return pParser;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Parser::operator delete(void* pParserAddress)
{
  Parser* pParser = reinterpret_cast<Parser*>(pParserAddress);
  
  if (pParser->m_bMalloc)
    free(pParser);
  else
    gpMediumHeap->vFreeObject((Object*)pParser);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Parser::Parser(void)
: cn(CN_PARSER)
{
  //m_bMalloc set in new
  m_iDepth = 0;
  m_bEOF   = false;
  m_pChain = nullptr;
  
  vResetParser();
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Parser::~Parser(void)
{
  if (m_xFile.bIsOpen())
    m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* end parsing                                                                */
/*----------------------------------------------------------------------------*/
void Parser::vEndParsing(void)
{
  switch (m_eSource) {

    case IS_FILE:
      ASSERT( m_xFile.bIsOpen() );
      m_xFile.vClose();
      break;
      
    case IS_STDIN:
      break;
    
    case IS_STRING:
      break;
    
    default: ;
  }

  vResetParser();
}

/*----------------------------------------------------------------------------*/
/* set user prompt                                                            */
/*----------------------------------------------------------------------------*/
void Parser::vPrompt(s& sPrompt)
{
  m_sPrompt = sPrompt;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Parser::vLineMode(bool bLineMode)
{
  m_bLineMode = bLineMode;
  m_bRead   = bLineMode ? true : false;
}

/*----------------------------------------------------------------------------*/
/* use given file for parsing                                                 */
/*----------------------------------------------------------------------------*/
bool Parser::bUseFile(s& sPath)
{
  vResetParser();
  
  m_xFile.vPath(sPath);
  m_eSource = IS_FILE;
  m_bRead = true;
  
  bool bOpened = m_xFile.bOpen(C_OPEN_READ);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin for parsing                                                      */
/*----------------------------------------------------------------------------*/
void Parser::vUseStdin(void)
{
  vResetParser();
  
  m_xFile.vSet(stdin,"stdin",C_OPEN_READ);
  m_eSource = IS_STDIN;
  m_bRead = true;
}

/*----------------------------------------------------------------------------*/
/* use given string for parsing                                               */
/*----------------------------------------------------------------------------*/
void Parser::vUseString(s& sString)
{
  vResetParser();
  
  m_sRawLine = sString;
  m_eSource = IS_STRING;
  m_bEOF = true;
}

/*----------------------------------------------------------------------------*/
/* get next parsed char                                                       */
/*----------------------------------------------------------------------------*/
char Parser::cGet(void)
{
  // do we have any chars in the regs?
  if (! m_nLoaded)
    vLoadRegisters();
  
  if (! m_nLoaded)
    return '\0';
  
  // consume the first char reg
  char cParsedChar = cConsumeFirst();
  
  // return the consumed char
  return cParsedChar;
}

/*----------------------------------------------------------------------------*/
/* put char back into parsing stream                                          */
/*----------------------------------------------------------------------------*/
void Parser::vUnget(char cParsed)
{
  vRewindRegisters(cParsed);
}

/*----------------------------------------------------------------------------*/
/* pop parsing stack                                                          */
/*----------------------------------------------------------------------------*/
bool Parser::bPop(void)
{
  if (! m_pChain)
    return false;
  
  Parser* pCopy = m_pChain;
  
  Memory::vCopy(this,pCopy,sizeof(Parser));
  
  pCopy->vZero();
  
  delete pCopy;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push copy of this parser onto stack                                        */
/*----------------------------------------------------------------------------*/
void Parser::vPush(void)
{
  Parser* pCopy = new(m_bMalloc) Parser;
  
  Memory::vCopy(pCopy,this,sizeof(Parser));
  
  m_pChain = pCopy;
  m_iDepth++;
  
  m_eSource = IS_NONE;
  m_xFile.vZero();
  m_sRawLine = "";
  m_pRawInput = m_sRawLine.a();
  m_bLineMode = false;
  m_bRead = false;
  m_nLoaded = 0;
  m_cFirst  = 0;
  m_cSecond  = 0;
  m_eComment = CT_NONE;
  m_bEOF = false;
}

/*----------------------------------------------------------------------------*/
/* depth of this parser                                                       */
/*----------------------------------------------------------------------------*/
i8 Parser::iDepth(void)
{
  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/* have we seen an EOF?                                                       */
/*----------------------------------------------------------------------------*/
bool Parser::bEOF(void)
{
  return m_bEOF;
}

/*----------------------------------------------------------------------------*/
/* zero this parser                                                           */
/*----------------------------------------------------------------------------*/
void Parser::vZero(void)
{
  m_xFile.vZero();
}


// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* set depth of this parser                                                   */
/*----------------------------------------------------------------------------*/
void Parser::vDepth(i8 iDepth)
{
  m_iDepth = iDepth;
}

/*----------------------------------------------------------------------------*/
/* reset this parser                                                          */
/*----------------------------------------------------------------------------*/
void Parser::vResetParser(void)
{
  ASSERT(! m_xFile.bIsOpen());
  
  m_eSource   = IS_NONE;
  m_xFile.vReset();
  m_sRawLine  = "";
  m_aRawLine  = m_sRawLine.a();
  m_pRawInput = m_sRawLine.a();
  
  m_bRead   = false;
  
  m_nLoaded   = 0;
  m_cFirst    = '\0';
  m_cSecond   = '\0';
  m_eComment  = CT_NONE;
}


/*----------------------------------------------------------------------------*/
/*-- Character Processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear char registers                                                       */
/*----------------------------------------------------------------------------*/
void Parser::vClearRegisters(void)
{
  m_nLoaded = 0;
  m_cFirst  = '\0';
  m_cSecond = '\0';
}

/*----------------------------------------------------------------------------*/
/* consume the char in the first register                                     */
/*----------------------------------------------------------------------------*/
char Parser::cConsumeFirst(void)
{
  ASSERT(m_nLoaded);
  
  char cWasFirst = m_cFirst;
  
  m_cFirst  = m_cSecond;
  m_cSecond = '\0';
  
  m_nLoaded--;
  
  return cWasFirst;
}

/*----------------------------------------------------------------------------*/
/* discard the char in the first register                                     */
/*----------------------------------------------------------------------------*/
void Parser::vDiscardFirst(void)
{
  ASSERT(m_nLoaded);
  
  m_cFirst  = m_cSecond;
  m_cSecond = '\0';
  
  m_nLoaded--;
}

/*----------------------------------------------------------------------------*/
/* load both the first and second registers from raw input stream             */
/*----------------------------------------------------------------------------*/
void Parser::vLoadBoth(void)
{
  ASSERT(! m_nLoaded);
  
  m_cFirst = cFromRawInput();
  
  if (m_cFirst) {
    
    m_nLoaded++;
    
    m_cSecond = cFromRawInput();
    
    if (m_cSecond)
      m_nLoaded++;
  }
}

/*----------------------------------------------------------------------------*/
/* load first register from input stream                                      */
/*----------------------------------------------------------------------------*/
void Parser::vLoadFirst(void)
{
  ASSERT(! m_nLoaded);
  
  m_cFirst = cFromRawInput();
  
  if (m_cFirst)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* load first (and maybe) second register from input stream, strip comments   */
/*----------------------------------------------------------------------------*/
void Parser::vLoadRegisters(void)
{
  ASSERT(0 == m_nLoaded);
  
  while (! m_nLoaded) {
    
    vLoadFirst();
    
    if (! m_nLoaded)
      break;
    
    if (! bPossibleComment())  // this may leave a char in second register
      continue;                // e.g. /t
    
    vStripComment();
  }
}

/*----------------------------------------------------------------------------*/
/* load second register from input stream                                     */
/*----------------------------------------------------------------------------*/
void Parser::vLoadSecond(void)
{
  ASSERT(1 == m_nLoaded);
  ASSERT(m_cFirst);
  
  m_cSecond = cFromRawInput();
  
  if (m_cSecond)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* rewind registers by one character                                          */
/*----------------------------------------------------------------------------*/
void Parser::vRewindRegisters(char cParsed)
{
  ASSERT(m_nLoaded < 2);
  ASSERT('\0' == m_cSecond);
  
  m_cSecond = m_cFirst;
  m_cFirst  = cParsed;
  
  m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* strip a possible line or block comment                                     */
/*----------------------------------------------------------------------------*/
void Parser::vStripComment(void)
{
  switch (eCommentType()) {
      
    case CT_BLOCK:
      vBlockCommentSkip();
      vRewindRegisters(' ');
      break;
      
    case CT_LINE:
      vLineCommentSkip();
      vRewindRegisters(' ');
      break;
      
    default:
      break;
  }
}


/*----------------------------------------------------------------------------*/
/* -- Comment Processing -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test for end of block comment                                              */
/*----------------------------------------------------------------------------*/
bool Parser::bBlockCommentEnd(void)
{
  if ('*' == m_cFirst && '/' == m_cSecond)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a block comment                                                       */
/*----------------------------------------------------------------------------*/
void Parser::vBlockCommentSkip(void)
{
  vClearRegisters();
  
  vLoadBoth();
  
  while (! bBlockCommentEnd()) {
    vDiscardFirst();
    vLoadSecond();
  }
  
  vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* test for start of block comment                                            */
/*----------------------------------------------------------------------------*/
bool Parser::bBlockCommentStart(void)
{
  if ('/' == m_cFirst && '*' == m_cSecond)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test for possible start of comment                                         */
/*----------------------------------------------------------------------------*/
bool Parser::bPossibleComment(void)
{
  ASSERT(m_nLoaded);
  
  if ('/' == m_cFirst) {
    if (1 == m_nLoaded)
      vLoadSecond();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* test for start of comment                                                  */
/*----------------------------------------------------------------------------*/
Parser::eCT Parser::eCommentType(void)
{
  if (bLineCommentStart())
    return CT_LINE;
  
  if (bBlockCommentStart())
    return CT_BLOCK;
  
  return CT_NONE;
}

/*----------------------------------------------------------------------------*/
/* test for end of line comment                                               */
/*----------------------------------------------------------------------------*/
bool Parser::bLineCommentEnd(void)
{
  if ('\n' == m_cFirst)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a line comment                                                        */
/*----------------------------------------------------------------------------*/
void Parser::vLineCommentSkip(void)
{
  vClearRegisters();
  
  vLoadFirst();
  
  while (! bLineCommentEnd()) {
    vDiscardFirst();
    vLoadFirst();
  }
  
  ASSERT('\n' == m_cFirst || '\0' == m_cFirst);
}

/*----------------------------------------------------------------------------*/
/* test for start of line comment                                             */
/*----------------------------------------------------------------------------*/
bool Parser::bLineCommentStart(void)
{
  if ('/' == m_cFirst && '/' == m_cSecond)
    return true;
  else
    return false;
}


/*----------------------------------------------------------------------------*/
/* -- File Reading -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next char from input source                                            */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawInput(void)
{
  char c;

  switch (m_eSource) {
      
    case IS_FILE:
      c = cFromRawFile();
      break;
      
    case IS_STDIN:
      c = cFromRawStdin();
      break;
      
    case IS_STRING:
      c = cFromRawString();
      break;
      
    default:
      ASSERT(0);
  }
  
  return c;
}

/*----------------------------------------------------------------------------*/
/* get next raw char from given file                                          */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawFile(void)
{
  char c;
  
  c = *m_pRawInput++;
  
  if (c)
    return c;
  
  if (m_bEOF) {
    c = '\0';
    return c;
  }
  
  ASSERT(m_xFile.bIsOpen());
  
  if (! m_bLineMode) {
    vReadLineFile();
    c = *m_pRawInput++;
    return c;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_pRawInput = m_sRawLine.a();
    *m_pRawInput = '\0';
    c = '\0';
    return c;
  }
  
  m_bRead = false;
  vReadLineFile();
  c = *m_pRawInput++;
  
  return c;
}

/*----------------------------------------------------------------------------*/
/* get next raw char from stdin                                               */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawStdin(void)
{
  char c;
  
  c = *m_pRawInput++;
  
  if (c)
    return c;
  
  ASSERT(m_xFile.bIsStdin());
  ASSERT(m_xFile.bIsOpen());
  
  if (! m_bLineMode) {
    vReadLineStdin();
    c = *m_pRawInput++;
    return c;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_pRawInput = m_sRawLine.a();
    *m_pRawInput = '\0';
    c = '\0';
    return c;
  }
  
  m_bRead = false;
  vReadLineStdin();
  c = *m_pRawInput++;
  
  return c;
}

/*----------------------------------------------------------------------------*/
/* get next char from input source                                            */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawString(void)
{
  char c;
  
  c = *m_pRawInput++;
  
  if (c)
    return c;
  
  c = '\0';
  
  ASSERT(m_bEOF);
  
  return c;
}

/*----------------------------------------------------------------------------*/
/* read line from input file                                                  */
/*----------------------------------------------------------------------------*/
void Parser::vReadLineFile(void)
{
  ASSERT(! m_bEOF);

  char c;
  
  char* pCharInLine = m_sRawLine.a();
  
  do {
    
    c = m_xFile.cGetChar();
    
    *pCharInLine++ = c;
    
    if (C_END_OF_LINE == c) {
      break;
    }
    
  } while (c);
  
  ASSERT(! m_bEOF);
  if (! c)
    m_bEOF = true;
  
  *pCharInLine++ = '\0';
  
  m_pRawInput = m_sRawLine.a();
}

/*----------------------------------------------------------------------------*/
/* read line from stdin                                                       */
/*----------------------------------------------------------------------------*/
void Parser::vReadLineStdin(void)
{
  char c;
  
  char* pCharInLine = m_sRawLine.a();
  
  printf("%s",m_sPrompt.a());
  fflush(stdin);
  
  for (;;) {
    
    c = m_xFile.cGetChar();
    
    *pCharInLine++ = c;
    
    if (C_END_OF_LINE == c) {
      break;
    }
    
  }
  
  *pCharInLine++ = '\0';
  
  m_pRawInput = m_sRawLine.a();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/