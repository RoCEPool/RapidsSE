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
: Object(CN_PARSER)
{
  //m_bMalloc set in new
  m_pChain  = nullptr;
  m_iDepth  = 0;
  m_sPrompt = "Parser> ";
  
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
/*-- Options -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
}

/*----------------------------------------------------------------------------*/
/*-- Use What ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* end usage of source                                                        */
/*----------------------------------------------------------------------------*/
void Parser::vUseEnd(void)
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
/*-- Parsing Stack -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pop parsing stack                                                          */
/*----------------------------------------------------------------------------*/
bool Parser::bPop(void)
{
  if (! m_pChain)
    return false;
  
  Parser* pCopy = m_pChain;
  
  Memory::vCopy(this,pCopy,sizeof(Parser));
  
  pCopy->m_xFile.vZero();
  
  delete pCopy;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push copy of this parser onto stack, reset this parser                     */
/*----------------------------------------------------------------------------*/
void Parser::vPush(void)
{
  // n.b. don't call vReset.  File may be open
  
  // create a copy of this parser
  Parser* pCopy = new(m_bMalloc) Parser;
  Memory::vCopy(pCopy,this,sizeof(Parser));
  
  // push it onto stack
  m_pChain = pCopy;
  m_iDepth++;
  // m_sPrompt we leave as is
  
  // reset input sources
  m_eSource   = IS_NONE;
  m_xFile.vZero();
  m_bLineMode = false;
  m_sRawLine  = "";
  m_pRawChar  = m_sRawLine.a();
  
  // reset registers
  m_nLoaded   = 0;
  m_cFirst    = 0;
  m_cSecond   = 0;
  m_eComment  = CT_NONE;
  m_bRead     = false;
  m_bEOF      = false;
}

/*----------------------------------------------------------------------------*/
/* depth of this parser                                                       */
/*----------------------------------------------------------------------------*/
i8 Parser::iDepth(void)
{
  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/*-- Parsing -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* have we seen an EOF?                                                       */
/*----------------------------------------------------------------------------*/
bool Parser::bEOF(void)
{
  return m_bEOF;
}

/*----------------------------------------------------------------------------*/
/* linemode?                                                                  */
/*----------------------------------------------------------------------------*/
bool Parser::bLineMode(void)
{
  return m_bLineMode;
}

/*----------------------------------------------------------------------------*/
/* stdin?                                                                     */
/*----------------------------------------------------------------------------*/
bool Parser::bStdin(void)
{
  if (IS_STDIN != m_eSource)
    return false;

  bool bIsStdin = m_xFile.bIsStdin();

  return bIsStdin;
}


// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* reset this parser                                                          */
/*----------------------------------------------------------------------------*/
void Parser::vResetParser(void)
{
  ASSERT(! m_xFile.bIsOpen());
  
  // reset input sources
  m_eSource   = IS_NONE;
  m_xFile.vZero();
  m_bLineMode = false;
  m_sRawLine  = "";
  m_aRawLine  = m_sRawLine.a();
  m_pRawChar  = m_sRawLine.a();
 
  m_nLoaded   = 0;
  m_cFirst    = '\0';
  m_cSecond   = '\0';
  m_eComment  = CT_NONE;
  m_bRead     = false;
  m_bEOF      = false;
}


/*----------------------------------------------------------------------------*/
/*-- Character Processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear character registers                                                  */
/*----------------------------------------------------------------------------*/
void Parser::vClearRegisters(void)
{
  m_nLoaded = 0;
  m_cFirst  = '\0';
  m_cSecond = '\0';
}

/*----------------------------------------------------------------------------*/
/* consume the character in the first register                                */
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
/* discard the character in the first register                                */
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
//  bool bLineMode = m_bLineMode;
//  m_bLineMode = false;
  
  vClearRegisters();
  
  vLoadBoth();
  
  while (! bBlockCommentEnd()) {
    vDiscardFirst();
    vLoadSecond();
  }
  
  vClearRegisters();

//  m_bLineMode = bLineMode;
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
/* -- Reading ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next raw character from input source                                   */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawInput(void)
{
  char cRaw;

  switch (m_eSource) {
      
    case IS_FILE:
      cRaw = cFromRawFile();
      ASSERT( (cRaw) || (!cRaw && (m_bEOF || m_bLineMode)) );
      break;
      
    case IS_STDIN:
      cRaw = cFromRawStdin();
      ASSERT(! m_bEOF);
      break;
      
    case IS_STRING:
      cRaw = cFromRawString();
      ASSERT(m_bEOF);
      break;
      
    default:
      THROW("Invalid m_eSource %lld",m_eSource);
  }
  
  return cRaw;
}

/*----------------------------------------------------------------------------*/
/* get next raw character from source file                                    */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawFile(void)
{
  char cRaw;
  
  cRaw = *m_pRawChar++;
  
  if (cRaw)
    return cRaw;
  
  if (m_bEOF) {
    cRaw = '\0';
    return cRaw;
  }
  
  ASSERT(m_xFile.bIsOpen());
  
  if (! m_bLineMode) {
    vReadLineFile();
    cRaw = *m_pRawChar++;
    return cRaw;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_pRawChar = m_sRawLine.a();
    *m_pRawChar = '\0';
    return '\0';
  }
  
  m_bRead = false;
  vReadLineFile();
  cRaw = *m_pRawChar++;
  
  ASSERT( (cRaw) || (!cRaw && m_bEOF) );
  
  return cRaw;
}

/*----------------------------------------------------------------------------*/
/* get next raw character from stdin                                          */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawStdin(void)
{
  ASSERT(! m_bEOF);
  
  char cRaw;
  
  cRaw = *m_pRawChar++;
  
  if (cRaw) {
    ASSERT(! m_bEOF);
    return cRaw;
  }
  
  ASSERT(m_xFile.bIsStdin());
  ASSERT(m_xFile.bIsOpen());
  
  if (! m_bLineMode) {
    vReadLineStdin();
    cRaw = *m_pRawChar++;
    ASSERT(! m_bEOF);
    return cRaw;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_pRawChar = m_sRawLine.a();
    *m_pRawChar = '\0';
    ASSERT(! m_bEOF);
    return '\0';
  }
  
  m_bRead = false;
  vReadLineStdin();
  cRaw = *m_pRawChar++;
  
  ASSERT(! m_bEOF);
  return cRaw;
}

/*----------------------------------------------------------------------------*/
/* get next raw character from string                                         */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawString(void)
{
  ASSERT(m_bEOF);
  
  char cRaw;
  
  cRaw = *m_pRawChar++;
  
  if (cRaw)
    return cRaw;
  
  ASSERT(m_bEOF);
  
  return '\0';
}

/*----------------------------------------------------------------------------*/
/* read raw line from input file                                              */
/*----------------------------------------------------------------------------*/
void Parser::vReadLineFile(void)
{
  ASSERT(! m_bEOF);

  char cInput;
  
  m_pRawChar = m_sRawLine.a();
  
  do {
    
    cInput = m_xFile.cGetChar();
    
    *m_pRawChar++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
    
  } while (cInput);
  
  ASSERT(! m_bEOF);
  if (! cInput)
    m_bEOF = true;
  
  *m_pRawChar++ = '\0';
  
  m_pRawChar = m_sRawLine.a();
}

/*----------------------------------------------------------------------------*/
/* read raw line from stdin                                                   */
/*----------------------------------------------------------------------------*/
void Parser::vReadLineStdin(void)
{
  ASSERT(! m_bEOF);
  
  m_pRawChar = m_sRawLine.a();
  
  printf("%s",m_sPrompt.a());
  fflush(stdin);
  
  for (;;) {
    
    char cInput = m_xFile.cGetChar();
    
    *m_pRawChar++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
  }
  
  *m_pRawChar++ = '\0';
  
  m_pRawChar = m_sRawLine.a();
  
  ASSERT(! m_bEOF);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/