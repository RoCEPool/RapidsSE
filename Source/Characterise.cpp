/*============================================================================*/
/* Characterise.cpp -- read input as chars, removing line and block comments  */
/*============================================================================*/

#include "LexicalAnalysis.h"


extern Args gxArgs;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Characterise* Characterise::_pParser(void* aAddr)
{
  Characterise* pParser = reinterpret_cast<Characterise*>(aAddr);
  
  RASSERT_TYPE(pParser,CN_CHARACTERISE);
  
  return pParser;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Characterise::operator new(size_t, i8 iViaMalloc)
{
  Characterise* pParser;
  
  if (iViaMalloc)
    pParser = reinterpret_cast<Characterise*>(MemLib::aMallocBlock(sizeof(Characterise)));
  else
    pParser = reinterpret_cast<Characterise*>(HeapMem::pAllocate(CN_CHARACTERISE));
  
  pParser->m_iViaMalloc = iViaMalloc;
  
  return pParser;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Characterise::operator delete(void* pParserAddress)
{
  Characterise* pParser = reinterpret_cast<Characterise*>(pParserAddress);
  
  if (pParser->m_iViaMalloc)
    MemLib::vFreeBlock(pParser,sizeof(Characterise));
  else
    HeapMem::vFree(pParser);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Characterise::Characterise(Characterise* pNext)
: Object(CN_CHARACTERISE)
, m_oRawFile(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  //m_bMalloc set in new
  m_pNext   = pNext;
  m_iDepth  = m_pNext ? m_pNext->iDepth() + 1 : 0;
  m_sSource = C_NO_NAME;
  
  vResetParser();
  
  _checkInitialised_(CN_CHARACTERISE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Characterise::~Characterise(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  if (m_oRawFile.bIsOpen())
    m_oRawFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* dump parser                                                                */
/*----------------------------------------------------------------------------*/
void Characterise::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Characterise* pParser = _pParser(pObject);
  
  vTrace("%s depth %lld next %p\n"_Stz,
         pParser->m_iViaMalloc ? "malloc"_Stz : "pool"_Stz,
         pParser->m_iDepth,
         pParser->m_pNext);
}

/*----------------------------------------------------------------------------*/
/* get source                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Characterise::szSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  return m_sSource;
}

/*----------------------------------------------------------------------------*/
/* get m_iLineRaw                                                             */
/*----------------------------------------------------------------------------*/
i8 Characterise::iLineRaw(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  return m_iLineRaw;
}

/*----------------------------------------------------------------------------*/
/* get m_iColRaw                                                              */
/*----------------------------------------------------------------------------*/
i8 Characterise::iLineCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  return m_iColRaw;
}


/*----------------------------------------------------------------------------*/
/*-- Options -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Characterise::vLineMode(bool bLineMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  m_bLineMode = bLineMode;
}

/*----------------------------------------------------------------------------*/
/*-- Use What ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* use given file for parsing                                                 */
/*----------------------------------------------------------------------------*/
bool Characterise::bUseFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  vResetParser();
  
  m_eSource = IS_FILE;
  m_bRead = true;
  
  m_sSource.vSprint("file: %s"_Stz,szPath);
  
  bool bOpened = m_oRawFile.bOpenRead(szPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin for parsing                                                      */
/*----------------------------------------------------------------------------*/
void Characterise::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  vResetParser();
  
  m_oRawFile.vUseStdin();
  m_eSource = IS_STDIN;
  m_bRead = true;
  
  m_sSource = "stdin"_Stz;
}

/*----------------------------------------------------------------------------*/
/* use given string for parsing                                               */
/*----------------------------------------------------------------------------*/
void Characterise::vUseString(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  vResetParser();
  
  m_sRawLine = szString;
  m_eSource = IS_STRING;
  m_bEOF = true;
  
m_sSource = "string"_Stz;
}

/*----------------------------------------------------------------------------*/
/* end usage of source                                                        */
/*----------------------------------------------------------------------------*/
void Characterise::vUseEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
Characterise* Characterise::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next parser in stack                                                   */
/*----------------------------------------------------------------------------*/
void Characterise::vNext(Characterise* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* depth of this parser                                                       */
/*----------------------------------------------------------------------------*/
i8 Characterise::iDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/*-- Commands -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next parsed char                                                       */
/*----------------------------------------------------------------------------*/
void Characterise::vGet(AnnChar& rxAnnChar, bool bStripComments)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  // do we have any chars in the regs?
  if (! m_nLoaded)
    vLoadRegs(bStripComments);
  
  if (! m_nLoaded) {
    rxAnnChar.vReset();
    return;
  }
  
  // consume the first char reg
  vConsumeFirst(rxAnnChar);
}

/*----------------------------------------------------------------------------*/
/* put char back into parsing stream                                          */
/*----------------------------------------------------------------------------*/
void Characterise::vUnget(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  vRewindRegs(rxAnnChar);
}

/*----------------------------------------------------------------------------*/
/* have we seen an EOF?                                                       */
/*----------------------------------------------------------------------------*/
bool Characterise::bEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  return m_bEOF;
}

/*----------------------------------------------------------------------------*/
/* linemode?                                                                  */
/*----------------------------------------------------------------------------*/
bool Characterise::bLineMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  return m_bLineMode;
}

/*----------------------------------------------------------------------------*/
/* stdin?                                                                     */
/*----------------------------------------------------------------------------*/
bool Characterise::bStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  if (IS_STDIN != m_eSource)
    return false;

  bool bIsStdin = m_oRawFile.bIsStdin();

  return bIsStdin;
}


// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* reset this parser                                                          */
/*----------------------------------------------------------------------------*/
void Characterise::vResetParser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
void Characterise::vClearRegs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  m_nLoaded = 0;
  m_xAnnOne.vReset();
  m_xAnnTwo.vReset();
}

/*----------------------------------------------------------------------------*/
/* consume the character in the first register                                */
/*----------------------------------------------------------------------------*/
void Characterise::vConsumeFirst(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  ASSERT(m_nLoaded);

  rxAnnChar = m_xAnnOne;
  m_xAnnOne = m_xAnnTwo;
  
  m_xAnnTwo.vReset();
  
  m_nLoaded--;
}

/*----------------------------------------------------------------------------*/
/* discard the character in the first register                                */
/*----------------------------------------------------------------------------*/
void Characterise::vDiscardFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  ASSERT(m_nLoaded);
  
  m_xAnnOne.m_cChar  = m_xAnnTwo.m_cChar;
  
  m_xAnnTwo.vReset();

  m_nLoaded--;
}

/*----------------------------------------------------------------------------*/
/* load both the first and second registers from raw input stream             */
/*----------------------------------------------------------------------------*/
void Characterise::vLoadBoth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
void Characterise::vLoadFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  ASSERT(! m_nLoaded);
  
  vRawCharFromInput(m_xAnnOne);
  
  if (m_xAnnOne.m_cChar)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* load first (and maybe) second register from input stream, strip comments   */
/*----------------------------------------------------------------------------*/
void Characterise::vLoadRegs(bool bStripComments)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
void Characterise::vLoadSecond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  ASSERT(1 == m_nLoaded);
  ASSERT(m_xAnnOne.m_cChar);
  
  vRawCharFromInput(m_xAnnTwo);
  
  if (m_xAnnTwo.m_cChar)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* rewind registers by one character                                          */
/*----------------------------------------------------------------------------*/
void Characterise::vRewindRegs(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  ASSERT(m_nLoaded < 2);
  ASSERT('\0' == m_xAnnTwo.m_cChar);
  
  m_xAnnTwo = m_xAnnOne;
  m_xAnnOne = rxAnnChar;
  
  m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* strip a possible line or block comment                                     */
/*----------------------------------------------------------------------------*/
void Characterise::vStripComment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  AnnChar xRewAnnChar;
  
  xRewAnnChar.vReset();
  xRewAnnChar.m_cChar = ' ';

  switch (eCommentStyle()) {
      
    case CS_BLOCK:
      vBlockCommentSkip();
      xRewAnnChar.m_iLine = m_iLineRaw;
      xRewAnnChar.m_iCol  = m_iColRaw;
      vRewindRegs(xRewAnnChar);
      break;
      
    case CS_LINE:
      vLineCommentSkip();
      xRewAnnChar.m_iLine = m_iLineRaw;
      xRewAnnChar.m_iCol  = m_iColRaw;
      vRewindRegs(xRewAnnChar);
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
bool Characterise::bBlockCommentEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  if ('*' == m_xAnnOne.m_cChar && '/' == m_xAnnTwo.m_cChar)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a block comment                                                       */
/*----------------------------------------------------------------------------*/
void Characterise::vBlockCommentSkip(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
bool Characterise::bBlockCommentStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  if ('/' == m_xAnnOne.m_cChar && '*' == m_xAnnTwo.m_cChar)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test for possible start of comment                                         */
/*----------------------------------------------------------------------------*/
bool Characterise::bPossibleComment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
eCS Characterise::eCommentStyle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  if (bLineCommentStart())
    return CS_LINE;
  
  if (bBlockCommentStart())
    return CS_BLOCK;
  
  return CS_NONE;
}

/*----------------------------------------------------------------------------*/
/* test for end of line comment                                               */
/*----------------------------------------------------------------------------*/
bool Characterise::bLineCommentEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  if ('\n' == m_xAnnOne.m_cChar)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a line comment                                                        */
/*----------------------------------------------------------------------------*/
void Characterise::vLineCommentSkip(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
bool Characterise::bLineCommentStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
void Characterise::vRawCharFromInput(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
      rxAnnChar.vReset();
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid m_eSource %lld"_Stz,m_eSource);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get next raw character from line-buffered source file                      */
/*----------------------------------------------------------------------------*/
void Characterise::vRawCharFromFile(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
void Characterise::vRawCharFromStdin(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
void Characterise::vRawCharFromString(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
void Characterise::vNextCharFromRawLine(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  rxAnnChar.m_cChar = *m_acRawChar++;
  
  rxAnnChar.m_iLine = m_iLineRaw;
  rxAnnChar.m_iCol  = ++m_iColRaw;
}

/*----------------------------------------------------------------------------*/
/* read raw line from input file                                              */
/*----------------------------------------------------------------------------*/
void Characterise::vReadRawLineFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
  ASSERT(! m_bEOF);

  char cInput;
  
  char* acRawLineCrs = m_sRawLine.azChar();
  
  do {
    
    cInput = m_oRawFile.cGetChar();
    
    *acRawLineCrs++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
    
  } while (cInput);
  
  ASSERT(! m_bEOF);
  if (! cInput)
    m_bEOF = true;
  
  *acRawLineCrs++ = '\0';
  
  m_iLineRaw++;
  m_iColRaw = 0;
  
  m_acRawChar = m_sRawLine.azChar();
}

/*----------------------------------------------------------------------------*/
/* read raw line from stdin                                                   */
/*----------------------------------------------------------------------------*/
void Characterise::vReadRawLineStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE,_FOOTER_);
  
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
  
  m_iLineRaw++;
  m_iColRaw = 0;

  m_acRawChar = m_sRawLine.azChar();
  
  ASSERT(! m_bEOF);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Characterise::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARACTERISE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
