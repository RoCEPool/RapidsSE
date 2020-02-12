/*============================================================================*/
/* Parser.cpp -- input processor, stripping out line and block comments       */
/*============================================================================*/

#include "TokenStream.h"


extern Args gxArgs;

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
, m_oRawFile(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  //m_bMalloc set in new
  m_pNext   = pNext;
  m_iDepth  = 0;
  
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
  
  if (m_oRawFile.bIsOpen())
    m_oRawFile.vClose();
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
bool Parser::bUseFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vResetParser();
  
  m_eSource = IS_FILE;
  m_bRead = true;
  
  bool bOpened = m_oRawFile.bOpenRead(szPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin for parsing                                                      */
/*----------------------------------------------------------------------------*/
void Parser::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vResetParser();
  
  m_oRawFile.vUseStdin();
  m_eSource = IS_STDIN;
  m_bRead = true;
}

/*----------------------------------------------------------------------------*/
/* use given string for parsing                                               */
/*----------------------------------------------------------------------------*/
void Parser::vUseString(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vResetParser();
  
  m_sRawLine = szString;
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
      ASSERT( m_oRawFile.bIsOpen() );
      m_oRawFile.vClose();
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
/*-- Commands Stack -----------------------------------------------------------*/
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
/*-- Commands -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next parsed char                                                       */
/*----------------------------------------------------------------------------*/
char Parser::cGet(bool bStripComments)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  AnnChar xAnnChar;
  
  // do we have any chars in the regs?
  if (! m_nLoaded)
    vLoadRegs(bStripComments);
  
  if (! m_nLoaded)
    return '\0';
  
  // consume the first char reg
  vConsumeFirst(xAnnChar);
  
  // return the consumed char
  return xAnnChar.m_cChar;
}

/*----------------------------------------------------------------------------*/
/* put char back into parsing stream                                          */
/*----------------------------------------------------------------------------*/
void Parser::vUnget(char cParsed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vRewindRegs(cParsed);
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

  bool bIsStdin = m_oRawFile.bIsStdin();

  return bIsStdin;
}


// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* reset this parser                                                          */
/*----------------------------------------------------------------------------*/
void Parser::vResetParser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  //ASSERT(! m_oRawFile.bIsOpen());
  
  // reset input sources
  m_eSource    = IS_NONE;
  m_oRawFile.vReset();
  m_bLineMode  = false;
  m_sRawLine   = ""_Stz;
  m_azRawLine  = m_sRawLine.azChar();
  m_acRawChar  = m_sRawLine.azChar();
  m_iLineRaw   = 0;
  m_iColRaw    = 0;

  m_nLoaded    = 0;
  m_xAnnOne.vReset();
  m_xAnnTwo.vReset();
  m_eComment   = CS_NONE;
  m_bRead      = false;
  m_bEOF       = false;
}


/*----------------------------------------------------------------------------*/
/*-- Character Processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear character registers                                                  */
/*----------------------------------------------------------------------------*/
void Parser::vClearRegs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  m_nLoaded = 0;
  m_xAnnOne.m_cChar  = '\0';
  m_xAnnTwo.m_cChar = '\0';
}

/*----------------------------------------------------------------------------*/
/* consume the character in the first register                                */
/*----------------------------------------------------------------------------*/
void Parser::vConsumeFirst(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(m_nLoaded);

  rxAnnChar = m_xAnnOne;
  
  m_xAnnOne = m_xAnnTwo;
  
  m_xAnnTwo.m_cChar = '\0';
  
  m_nLoaded--;
}

/*----------------------------------------------------------------------------*/
/* discard the character in the first register                                */
/*----------------------------------------------------------------------------*/
void Parser::vDiscardFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(m_nLoaded);
  
  m_xAnnOne.m_cChar  = m_xAnnTwo.m_cChar;
  m_xAnnTwo.m_cChar = '\0';
  
  m_nLoaded--;
}

/*----------------------------------------------------------------------------*/
/* load both the first and second registers from raw input stream             */
/*----------------------------------------------------------------------------*/
void Parser::vLoadBoth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(! m_nLoaded);
  
  vRawCharFromInput(m_xAnnOne);
  
  if (m_xAnnOne.m_cChar) {
    
    m_nLoaded++;
    
    vRawCharFromInput(m_xAnnTwo);
    
    if (m_xAnnTwo.m_cChar)
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
  
  vRawCharFromInput(m_xAnnOne);
  
  if (m_xAnnOne.m_cChar)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* load first (and maybe) second register from input stream, strip comments   */
/*----------------------------------------------------------------------------*/
void Parser::vLoadRegs(bool bStripComments)
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
  ASSERT(m_xAnnOne.m_cChar);
  
  vRawCharFromInput(m_xAnnTwo);
  
  if (m_xAnnTwo.m_cChar)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* rewind registers by one character                                          */
/*----------------------------------------------------------------------------*/
void Parser::vRewindRegs(char cParsed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(m_nLoaded < 2);
  ASSERT('\0' == m_xAnnTwo.m_cChar);
  
  m_xAnnTwo.m_cChar = m_xAnnOne.m_cChar;
  m_xAnnOne.m_cChar = cParsed;
  
  m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* strip a possible line or block comment                                     */
/*----------------------------------------------------------------------------*/
void Parser::vStripComment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  switch (eCommentStyle()) {
      
    case CS_BLOCK:
      vBlockCommentSkip();
      vRewindRegs(' ');
      break;
      
    case CS_LINE:
      vLineCommentSkip();
      vRewindRegs(' ');
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
  
  if ('*' == m_xAnnOne.m_cChar && '/' == m_xAnnTwo.m_cChar)
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
  
  vClearRegs();
  
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
  
  vClearRegs();
}

/*----------------------------------------------------------------------------*/
/* test for start of block comment                                            */
/*----------------------------------------------------------------------------*/
bool Parser::bBlockCommentStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  if ('/' == m_xAnnOne.m_cChar && '*' == m_xAnnTwo.m_cChar)
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
  
  if ('/' == m_xAnnOne.m_cChar) {
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
eCS Parser::eCommentStyle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  if (bLineCommentStart())
    return CS_LINE;
  
  if (bBlockCommentStart())
    return CS_BLOCK;
  
  return CS_NONE;
}

/*----------------------------------------------------------------------------*/
/* test for end of line comment                                               */
/*----------------------------------------------------------------------------*/
bool Parser::bLineCommentEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  if ('\n' == m_xAnnOne.m_cChar)
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
  
  vClearRegs();
  
  vLoadFirst();
  
  while (! bLineCommentEnd()) {
    vDiscardFirst();
    vLoadFirst();
  }
  
  ASSERT('\n' == m_xAnnOne.m_cChar || '\0' == m_xAnnOne.m_cChar);
}

/*----------------------------------------------------------------------------*/
/* test for start of line comment                                             */
/*----------------------------------------------------------------------------*/
bool Parser::bLineCommentStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  if ('/' == m_xAnnOne.m_cChar && '/' == m_xAnnTwo.m_cChar)
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
void Parser::vRawCharFromInput(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  switch (m_eSource) {
      
    case IS_FILE:
      vRawCharFromFile(rxAnnChar);
      ASSERT( (rxAnnChar.m_cChar) ||
              (! rxAnnChar.m_cChar && (m_bEOF || m_bLineMode)) );
      break;
      
    case IS_STDIN:
      vRawCharFromStdin(rxAnnChar);
      ASSERT(! m_bEOF);
      break;
      
    case IS_STRING:
      vRawCharFromString(rxAnnChar);
      ASSERT(m_bEOF);
      break;
      
    default:
      rxAnnChar.m_cChar = '\0';
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid m_eSource %lld"_Stz,m_eSource);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get next raw character from line-buffered source file                      */
/*----------------------------------------------------------------------------*/
void Parser::vRawCharFromFile(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  vNextCharFromRawLine(rxAnnChar);
  
  if (rxAnnChar.m_cChar)
    return ;
  
  if (m_bEOF) {
    rxAnnChar.m_cChar = '\0';
    return;
  }
  
  ASSERT(m_oRawFile.bIsOpen());
  
  if (! m_bLineMode) {
    vReadRawLineFile();
    vNextCharFromRawLine(rxAnnChar);
    return;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_acRawChar = m_sRawLine.azChar();
    *m_acRawChar = '\0';
    return;
  }
  
  m_bRead = false;
  vReadRawLineFile();
  vNextCharFromRawLine(rxAnnChar);
  
  ASSERT( (rxAnnChar.m_cChar) || (!rxAnnChar.m_cChar && m_bEOF) );
}

/*----------------------------------------------------------------------------*/
/* get next raw character from line-buffered stdin                            */
/*----------------------------------------------------------------------------*/
void Parser::vRawCharFromStdin(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(! m_bEOF);
    
  vNextCharFromRawLine(rxAnnChar);
  
  if (rxAnnChar.m_cChar) {
    ASSERT(! m_bEOF);
    return;
  }
  
  ASSERT(m_oRawFile.bIsStdin());
  ASSERT(m_oRawFile.bIsOpen());
  
  if (! m_bLineMode) {
    vReadRawLineStdin();
    vNextCharFromRawLine(rxAnnChar);
    ASSERT(! m_bEOF);
    return;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_acRawChar = m_sRawLine.azChar();
    *m_acRawChar = '\0';
    rxAnnChar.m_cChar = '\0';
    ASSERT(! m_bEOF);
    return ;
  }
  
  m_bRead = false;
  vReadRawLineStdin();
  vNextCharFromRawLine(rxAnnChar);
  
  ASSERT(! m_bEOF);
}

/*----------------------------------------------------------------------------*/
/* get next raw character from buffered string                                */
/*----------------------------------------------------------------------------*/
void Parser::vRawCharFromString(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(m_bEOF);
    
  vNextCharFromRawLine(rxAnnChar);
  
  if (rxAnnChar.m_cChar)
    return;
  
  ASSERT(m_bEOF);
  ASSERT('\0' == rxAnnChar.m_cChar);
}

/*----------------------------------------------------------------------------*/
/* get next raw character                                                     */
/*----------------------------------------------------------------------------*/
void Parser::vNextCharFromRawLine(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  rxAnnChar.m_cChar = *m_acRawChar++;
}

/*----------------------------------------------------------------------------*/
/* read raw line from input file                                              */
/*----------------------------------------------------------------------------*/
void Parser::vReadRawLineFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(! m_bEOF);

  char cInput;
  
  char* acRawLine = m_sRawLine.azChar();
  
  do {
    
    cInput = m_oRawFile.cGetChar();
    
    *acRawLine++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
    
  } while (cInput);
  
  ASSERT(! m_bEOF);
  if (! cInput)
    m_bEOF = true;
  
  *acRawLine++ = '\0';
  
  m_acRawChar = m_sRawLine.azChar();
}

/*----------------------------------------------------------------------------*/
/* read raw line from stdin                                                   */
/*----------------------------------------------------------------------------*/
void Parser::vReadRawLineStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARSER,_FOOTER_);
  
  ASSERT(! m_bEOF);
  
  char* acRawLine = m_sRawLine.azChar();
    
//  bool bHasStarted = AM::bHasStarted();
//
//  bool bLowMem = gpBufferPool->bLowMemory();
//  
//  if (! gxArgs.m_bSethScript) {
//    
//    if (! AM::bDeveloper()) {
//      vInfo("%s "_Stz,m_sPrompt.azChar());
//    }
//    else {
//      vInfo("%s%s%s%s %s> "_Stz,
//            m_sPrompt.azChar(),
//            AM::bBootError()      ? " *BOOTERROR* " : "",
//            bLowMem               ? " *LOWMEM* "    : "",
//  #ifdef WITH_ENTRY_EXIT
//            AM::pStepTiming()     ? " *STEPTIMING*" : "",
//  #else
//            "",
//  #endif
//            bHasStarted           ? "has" : "not"
//
//            );
//    }
//    fflush(stdin);
//  }
  
  for (;;) {
    
    char cInput = m_oRawFile.cGetChar();
    
    *acRawLine++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
  }
  
  *acRawLine++ = '\0';
  
  m_acRawChar = m_sRawLine.azChar();
  
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
