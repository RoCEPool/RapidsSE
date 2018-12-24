/*============================================================================*/
/* Parser.cpp -- input processor, stripping out line and block comments      */
/*============================================================================*/

#include "Parsing.h"

#include "StorageEngine.h"

#include "Arguments.hpp"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Parser* Parser::_pParser(void* aAddr)
{
  Parser* pParser = reinterpret_cast<Parser*>(aAddr);
  
  RASSERT_TYPE(pParser,CN_PARSER);
  
  return pParser;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Parser::operator new(size_t, bool bMalloc)
{
  Parser* pParser;
  
  if (bMalloc)
    pParser = reinterpret_cast<Parser*>(MemLib::aMallocBlock(sizeof(Parser)));
  else
    pParser = reinterpret_cast<Parser*>(HeapMem::pAllocate(CN_PARSER));
  
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
    MemLib::vFreeBlock(pParser,sizeof(Parser));
  else
    HeapMem::vFree(pParser);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Parser::Parser(Parser* pNext)
: Object(CN_PARSER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  //m_bMalloc set in new
  m_pNext   = pNext;
  m_iDepth  = 0;
  m_sPrompt = "Parser> "_Stz;
  
  vResetParser();
  
  _checkInitialised_(CN_PARSER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Parser::~Parser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  if (m_xFile.bIsOpen())
    m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* dump parser                                                                */
/*----------------------------------------------------------------------------*/
void Parser::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Parser* pParser = _pParser(pObject);
  
  vTrace("%s depth %lld next %p\n"_Stz,
         pParser->m_bMalloc == true ? "malloc"_Stz : "pool"_Stz,
         pParser->m_iDepth,
         pParser->m_pNext);
}

/*----------------------------------------------------------------------------*/
/*-- Options -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set user prompt                                                            */
/*----------------------------------------------------------------------------*/
void Parser::vPrompt(StrVal& sPrompt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  m_sPrompt = sPrompt;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Parser::vLineMode(bool bLineMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  m_bLineMode = bLineMode;
}

/*----------------------------------------------------------------------------*/
/*-- Use What ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* use given file for parsing                                                 */
/*----------------------------------------------------------------------------*/
bool Parser::bUseFile(StrVal& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vResetParser();
  
  m_xFile.vPath(sPath);
  m_eSource = IS_FILE;
  m_bRead = true;
  
  bool bOpened = m_xFile.bOpen(sPath,C_OPEN_READ);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin for parsing                                                      */
/*----------------------------------------------------------------------------*/
void Parser::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vResetParser();
  
  m_xFile.vUseStdin();
  m_eSource = IS_STDIN;
  m_bRead = true;
}

/*----------------------------------------------------------------------------*/
/* use given string for parsing                                               */
/*----------------------------------------------------------------------------*/
void Parser::vUseString(StrVal& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
/* get next parser in stack                                                   */
/*----------------------------------------------------------------------------*/
Parser* Parser::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next parser in stack                                                   */
/*----------------------------------------------------------------------------*/
void Parser::vNext(Parser* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* depth of this parser                                                       */
/*----------------------------------------------------------------------------*/
i8 Parser::iDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/*-- Parsing -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next parsed char                                                       */
/*----------------------------------------------------------------------------*/
char Parser::cGet(bool bStripComments)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  // do we have any chars in the regs?
  if (! m_nLoaded)
    vLoadRegisters(bStripComments);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vRewindRegisters(cParsed);
}

/*----------------------------------------------------------------------------*/
/* have we seen an EOF?                                                       */
/*----------------------------------------------------------------------------*/
bool Parser::bEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  return m_bEOF;
}

/*----------------------------------------------------------------------------*/
/* linemode?                                                                  */
/*----------------------------------------------------------------------------*/
bool Parser::bLineMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  return m_bLineMode;
}

/*----------------------------------------------------------------------------*/
/* stdin?                                                                     */
/*----------------------------------------------------------------------------*/
bool Parser::bStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  //ASSERT(! m_xFile.bIsOpen());
  
  // reset input sources
  m_eSource   = IS_NONE;
  m_xFile.vReset();
  m_bLineMode = false;
  m_sRawLine  = ""_Stz;
  m_aRawLine  = m_sRawLine.aChar();
  m_pRawChar  = m_sRawLine.aChar();
 
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  m_nLoaded = 0;
  m_cFirst  = '\0';
  m_cSecond = '\0';
}

/*----------------------------------------------------------------------------*/
/* consume the character in the first register                                */
/*----------------------------------------------------------------------------*/
char Parser::cConsumeFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(! m_nLoaded);
  
  m_cFirst = cFromRawInput();
  
  if (m_cFirst)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* load first (and maybe) second register from input stream, strip comments   */
/*----------------------------------------------------------------------------*/
void Parser::vLoadRegisters(bool bStripComments)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(0 == m_nLoaded);
  
  while (! m_nLoaded) {
    
    vLoadFirst();
    
    if (! m_nLoaded)
      break;
    
    if (! bStripComments || ! bPossibleComment())
      continue;               // this may leave a char in second register e.g. /t
    
    vStripComment();
  }
}

/*----------------------------------------------------------------------------*/
/* load second register from input stream                                     */
/*----------------------------------------------------------------------------*/
void Parser::vLoadSecond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vClearRegisters();
  
  vLoadBoth();
  
  while (! bBlockCommentEnd()) {
    
    if (m_nLoaded)
      vDiscardFirst();
    
    if (0 == m_nLoaded)
      vLoadFirst();
    else
      vLoadSecond();
    
    if (0 == m_nLoaded && bEOF())
      break;
  }
  
  vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* test for start of block comment                                            */
/*----------------------------------------------------------------------------*/
bool Parser::bBlockCommentStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
      THROW("Invalid m_eSource %lld"_Stz,m_eSource);
  }
  
  return cRaw;
}

/*----------------------------------------------------------------------------*/
/* get next raw character from source file                                    */
/*----------------------------------------------------------------------------*/
char Parser::cFromRawFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
    m_pRawChar = m_sRawLine.aChar();
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
    m_pRawChar = m_sRawLine.aChar();
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(! m_bEOF);

  char cInput;
  
  m_pRawChar = m_sRawLine.aChar();
  
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
  
  m_pRawChar = m_sRawLine.aChar();
}

/*----------------------------------------------------------------------------*/
/* read raw line from stdin                                                   */
/*----------------------------------------------------------------------------*/
void Parser::vReadLineStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(! m_bEOF);
  
  m_pRawChar = m_sRawLine.aChar();
  
  if (gpAttachMap->m_bTracepoint)
    PosixThread::vSleepMilli(300);
  
  bool bLowMem = gpBufferPool->bLowMemory();
  
  if ( ! gxArguments.m_bScript ) {
    vInfo("%s%s%s> "_Stz,
          m_sPrompt.aChar(),
          gpAttachMap->m_bBootError  ? " *BOOTERROR* " : "",
          bLowMem                    ? " *LOWMEM* "    : ""
          );
    fflush(stdin);
  }
  
  for (;;) {
    
    char cInput = m_xFile.cGetChar();
    
    *m_pRawChar++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
  }
  
  *m_pRawChar++ = '\0';
  
  m_pRawChar = m_sRawLine.aChar();
  
  ASSERT(! m_bEOF);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Parser::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
