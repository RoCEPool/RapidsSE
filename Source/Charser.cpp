/*============================================================================*/
/* Charser.cpp -- read input as chars, removing line and block comments       */
/*============================================================================*/

#include "LexicalAnalysis.h"

extern Args gxArgs;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(LexicalAnalysis,Charser,CN_CHARSER)
__aNewFunction(Charser,CN_CHARSER)
__vDeleteFunction(Charser,CN_CHARSER)
__aFooterFunction(Charser)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Charser::Charser(Charser* pNext)
: Object(CN_CHARSER)
, m_oRawFile(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  //m_bMalloc set in new
  m_pNext   = pNext;
  m_iDepth  = m_pNext ? m_pNext->iDepth() + 1 : 0;
  
  d_iDebugMode = false;
  
  m_bUseClosed = true;
  m_bUseEnded  = true;
  vResetCharser();
  
  _checkInitialised_(CN_CHARSER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Charser::~Charser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  if (m_oRawFile.bIsOpen())
    m_oRawFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* dump charser                                                               */
/*----------------------------------------------------------------------------*/
void Charser::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Charser* pCharser = _pCharser(pObject);
  
  vTrace("depth %lld next %p\n"_Stz,
         pCharser->m_iDepth,
         pCharser->m_pNext);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insepct                                                                    */
/*----------------------------------------------------------------------------*/
void Charser::vInspect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  vJot("Charser[%lld] (L%lld C%lld) : "
         "Source %s Path %s %s %s %s %s %s %s : "
         "Loaded %lld %c %c\n"_Stz,
         
         m_iDepth,
         m_iLineRaw,
         m_iColRaw,
         
         aIS[m_eSource],
         m_sPath.szStz(),
         m_bSingleLineCmd  ? "SingleLine"  : "MultiLine",
         m_bUseBegun       ? "UseBegun"    : "Not-UseBegun",
         m_bUseEnded       ? "UseEnded"    : "Not-UseEnded",
         m_bUseOpened      ? "UseOpened"   : "Not-UseOpened",
         m_bUseClosed      ? "UseClosed"   : "Not-UseClosed",
         m_bSeenEOF        ? "SeenEOF"     : "Not-SeenEOF",

         m_nLoaded,
         m_xRegOne.m_cChar ? m_xRegOne.m_cChar : ' ',
         m_xRegTwo.m_cChar ? m_xRegOne.m_cChar : ' '
         );
}

/*----------------------------------------------------------------------------*/
/* get line number of raw char                                                */
/*----------------------------------------------------------------------------*/
i8 Charser::iLineRaw(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_iLineRaw;
}

/*----------------------------------------------------------------------------*/
/* get column number of raw char                                              */
/*----------------------------------------------------------------------------*/
i8 Charser::iLineCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_iColRaw;
}

/*----------------------------------------------------------------------------*/
/* get path                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Charser::szPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_sPath;
}

// Options /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* get next annotated char                                                    */
/*----------------------------------------------------------------------------*/
void Charser::vGetAnnChar(AnnChar& rxAnnChar, bool bStripComments)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);

  // debug
  
  if (IS_FILE == m_eSource) {
    Correctness::vDebugPoint();
    if (m_bSingleLineCmd)
      Correctness::vDebugPoint();
    else
      Correctness::vDebugPoint();
  }
  else if (IS_STDIN == m_eSource) {
    Correctness::vDebugPoint();
    if (m_bSingleLineCmd)
      Correctness::vDebugPoint();
    else
      Correctness::vDebugPoint();
  }
  else if (IS_STRING == m_eSource) {
    Correctness::vDebugPoint();
    if (m_bSingleLineCmd)
      Correctness::vDebugPoint();
    else
      Correctness::vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
/* put char back into stream                                                  */
/*----------------------------------------------------------------------------*/
void Charser::vUngetAnnChar(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  vRewindRegs(rxAnnChar);
}

/*----------------------------------------------------------------------------*/
/* have we seen an EOF?                                                       */
/*----------------------------------------------------------------------------*/
bool Charser::bSeenEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_bSeenEOF;
}

/*----------------------------------------------------------------------------*/
/* single line mode?                                                          */
/*----------------------------------------------------------------------------*/
bool Charser::bSingleLineCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_bSingleLineCmd;
}

/*----------------------------------------------------------------------------*/
/* stdin?                                                                     */
/*----------------------------------------------------------------------------*/
bool Charser::bIsStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  if (IS_STDIN != m_eSource)
    return false;

  bool bIsStdin = m_oRawFile.bIsStdin();

  return bIsStdin;
}

/*----------------------------------------------------------------------------*/
/* set debug mode                                                             */
/*----------------------------------------------------------------------------*/
void Charser::vDebugMode(i8 iDebugMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  d_iDebugMode = iDebugMode;
}


// Options /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* complete reset after fork                                                  */
/*----------------------------------------------------------------------------*/
void Charser::vForkReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  m_bUseEnded = true;
  
  vResetCharser();
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Charser::vSingleLineCmd(bool bSingleLineCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  m_bSingleLineCmd = bSingleLineCmd;
}

/*----------------------------------------------------------------------------*/
/* usage of this charser begins                                               */
/*----------------------------------------------------------------------------*/
void Charser::vUseBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(! m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT(! m_bUseClosed);

  m_bUseBegun = true;
  
  //if (AM::bDeveloper())
  //  vInspect();
}

/*----------------------------------------------------------------------------*/
/* has usage begun?                                                           */
/*----------------------------------------------------------------------------*/
bool Charser::bUseBegun(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_bUseBegun;
}

/*----------------------------------------------------------------------------*/
/* close usage of source                                                      */
/*----------------------------------------------------------------------------*/
void Charser::vUseClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT(m_bUseOpened);
  ASSERT(! m_bUseClosed);

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
  
  m_bUseClosed = true;
}

/*----------------------------------------------------------------------------*/
/* end usage of source                                                        */
/*----------------------------------------------------------------------------*/
void Charser::vUseEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT((m_bUseOpened && m_bUseClosed) || (! m_bUseOpened && ! m_bUseClosed));

  m_bUseEnded = true;
  
  vResetCharser();
}

/*----------------------------------------------------------------------------*/
/* use given file for charsing                                                */
/*----------------------------------------------------------------------------*/
bool Charser::bUseFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE == m_eSource);
  
  m_eSource = IS_FILE;
  m_bRead = true;
  
  m_sPath.vSprint("file: %s"_Stz,szPath);
  
  m_bUseOpened = m_oRawFile.bOpenRead(szPath);
   
  return m_bUseOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin for charsing                                                     */
/*----------------------------------------------------------------------------*/
void Charser::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE == m_eSource);
  
  m_oRawFile.vUseStdin();
  m_eSource = IS_STDIN;
  m_bRead = true;
  
  m_sPath = "stdin"_Stz;
}

/*----------------------------------------------------------------------------*/
/* use given string for charsing                                              */
/*----------------------------------------------------------------------------*/
void Charser::vUseString(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE == m_eSource);
  
  m_sRawBuff = szString;
  m_eSource = IS_STRING;
  m_bSeenEOF = true;
  
  m_sPath = "string"_Stz;
}



// charser /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* get next charser in stack                                                  */
/*----------------------------------------------------------------------------*/
Charser* Charser::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next charser in stack                                                  */
/*----------------------------------------------------------------------------*/
void Charser::vNext(Charser* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* depth of this charser                                                      */
/*----------------------------------------------------------------------------*/
i8 Charser::iDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_iDepth;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* reset this charser                                                         */
/*----------------------------------------------------------------------------*/
void Charser::vResetCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT((m_bUseOpened && m_bUseClosed) || (! m_bUseOpened && !m_bUseClosed));
  ASSERT(m_bUseEnded);

  // source definition
  m_eSource    = IS_NONE;
  m_sPath      = C_NO_NAME;
  m_bStripComments = true;
  
  // sources
  m_oRawFile.vReset();
  m_sRawBuff   = ""_Stz;
  m_azRawBuff  = m_sRawBuff.azChar();
  m_acRawChar  = m_sRawBuff.azChar();
  m_iLineRaw   = 0;
  m_iColRaw    = 0;

  // uses
  m_bUseBegun  = false;
  m_bUseClosed = false;
  m_bUseEnded  = false;
  m_bUseOpened = false;
  
  // registers
  m_bSingleLineCmd  = false;
  m_nLoaded    = 0;
  m_xRegOne.vReset();
  m_xRegTwo.vReset();
  m_eComment   = CS_NONE;
  m_bRead      = false;
  m_bSeenEOF   = false;
}


/*----------------------------------------------------------------------------*/
/*-- Character Processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear character registers                                                  */
/*----------------------------------------------------------------------------*/
void Charser::vClearRegs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  
  m_nLoaded = 0;
  m_xRegOne.vReset();
  m_xRegTwo.vReset();
}

/*----------------------------------------------------------------------------*/
/* consume the character in the first register                                */
/*----------------------------------------------------------------------------*/
void Charser::vConsumeFirst(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(m_nLoaded);

  rxAnnChar = m_xRegOne;
  m_xRegOne = m_xRegTwo;
  
  m_xRegTwo.vReset();
  
  m_nLoaded--;
}

/*----------------------------------------------------------------------------*/
/* discard the character in the first register                                */
/*----------------------------------------------------------------------------*/
void Charser::vDiscardFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(m_nLoaded);
  
  m_xRegOne.m_cChar  = m_xRegTwo.m_cChar;
  
  m_xRegTwo.vReset();

  m_nLoaded--;
}

/*----------------------------------------------------------------------------*/
/* load both the first and second registers from raw input stream             */
/*----------------------------------------------------------------------------*/
void Charser::vLoadBoth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(! m_nLoaded);
  
  vNextRawChar(m_xRegOne);
  
  if (m_xRegOne.m_cChar) {
    
    m_nLoaded++;
    
    vNextRawChar(m_xRegTwo);
    
    if (m_xRegTwo.m_cChar)
      m_nLoaded++;
  }
}

/*----------------------------------------------------------------------------*/
/* load first register from raw input stream                                  */
/*----------------------------------------------------------------------------*/
void Charser::vLoadFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(! m_nLoaded);
  
  vNextRawChar(m_xRegOne);
  
  if (m_xRegOne.m_cChar)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* load first (& maybe) second register from raw input stream, strip comments */
/*----------------------------------------------------------------------------*/
void Charser::vLoadRegs(bool bStripComments)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
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
/* load second register from raw input stream                                 */
/*----------------------------------------------------------------------------*/
void Charser::vLoadSecond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(1 == m_nLoaded);
  ASSERT(m_xRegOne.m_cChar);
  
  vNextRawChar(m_xRegTwo);
  
  if (m_xRegTwo.m_cChar)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* rewind registers by one character                                          */
/*----------------------------------------------------------------------------*/
void Charser::vRewindRegs(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(m_nLoaded < 2);
  ASSERT('\0' == m_xRegTwo.m_cChar);
  
  m_xRegTwo = m_xRegOne;
  m_xRegOne = rxAnnChar;
  
  m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* strip a possible line or block comment                                     */
/*----------------------------------------------------------------------------*/
void Charser::vStripComment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  
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
bool Charser::bBlockCommentEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
    
  if ('*' == m_xRegOne.m_cChar && '/' == m_xRegTwo.m_cChar)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a block comment                                                       */
/*----------------------------------------------------------------------------*/
void Charser::vBlockCommentSkip(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
    
  vClearRegs();
  
  vLoadBoth();
  
  while (! bBlockCommentEnd()) {
    
    if (m_nLoaded)
      vDiscardFirst();
    
    if (0 == m_nLoaded)
      vLoadFirst();
    else
      vLoadSecond();
    
    if (0 == m_nLoaded && bSeenEOF())
      break;
  }
  
  vClearRegs();
}

/*----------------------------------------------------------------------------*/
/* test for start of block comment                                            */
/*----------------------------------------------------------------------------*/
bool Charser::bBlockCommentStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  if ('/' == m_xRegOne.m_cChar && '*' == m_xRegTwo.m_cChar)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test for possible start of comment                                         */
/*----------------------------------------------------------------------------*/
bool Charser::bPossibleComment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
    
  if ('/' == m_xRegOne.m_cChar) {
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
eCS Charser::eCommentStyle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  if (bLineCommentStart())
    return CS_LINE;
  
  if (bBlockCommentStart())
    return CS_BLOCK;
  
  return CS_NONE;
}

/*----------------------------------------------------------------------------*/
/* test for end of line comment                                               */
/*----------------------------------------------------------------------------*/
bool Charser::bLineCommentEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  if ('\n' == m_xRegOne.m_cChar)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a line comment                                                        */
/*----------------------------------------------------------------------------*/
void Charser::vLineCommentSkip(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  vClearRegs();
  
  vLoadFirst();
  
  while (! bLineCommentEnd()) {
    vDiscardFirst();
    vLoadFirst();
  }
  
  ASSERT('\n' == m_xRegOne.m_cChar || '\0' == m_xRegOne.m_cChar);
}

/*----------------------------------------------------------------------------*/
/* test for start of line comment                                             */
/*----------------------------------------------------------------------------*/
bool Charser::bLineCommentStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  if ('/' == m_xRegOne.m_cChar && '/' == m_xRegTwo.m_cChar)
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
void Charser::vNextRawChar(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  
  switch (m_eSource) {
      
    case IS_FILE:
      vNextRawCharFromFile(rxAnnChar);
      
      
      if (! rxAnnChar.m_cChar) {
        
      }
      
      
      ASSERT( (rxAnnChar.m_cChar) ||
              (! rxAnnChar.m_cChar && (m_bSeenEOF || m_bSingleLineCmd)) );
      break;
      
    case IS_STDIN:
      vNextRawCharFromStdin(rxAnnChar);
      ASSERT(! m_bSeenEOF);
      break;
      
    case IS_STRING:
      vNextRawCharFromString(rxAnnChar);
      ASSERT(m_bSeenEOF);
      break;
      
    default:
      rxAnnChar.vReset();
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid m_eSource %lld"_Stz,m_eSource);
  }
  
  if (d_iDebugMode) {
    vJot("    AnnChar (L%lld C%lld) : %c\n"_Stz,
         rxAnnChar.m_iLine,
         rxAnnChar.m_iCol,
         rxAnnChar.m_cChar);
  }
}

/*----------------------------------------------------------------------------*/
/* get next raw character from line-buffered source file                      */
/*----------------------------------------------------------------------------*/
void Charser::vNextRawCharFromFile(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);

  vNextCharFromRawBuff(rxAnnChar);
  
  if (rxAnnChar.m_cChar)
    return ;
  
  if (m_bSeenEOF) {
    rxAnnChar.m_cChar = '\0';
    return;
  }
  
  ASSERT(m_oRawFile.bIsOpen());
  
  if (! m_bSingleLineCmd) {
    vReadFileIntoRawBuff();
    vNextCharFromRawBuff(rxAnnChar);
    return;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_acRawChar = m_sRawBuff.azChar();
    *m_acRawChar = '\0';
    return;
  }
  
  m_bRead = false;
  vReadFileIntoRawBuff();
  vNextCharFromRawBuff(rxAnnChar);
  
  ASSERT( (rxAnnChar.m_cChar) || (!rxAnnChar.m_cChar && m_bSeenEOF) );
}

/*----------------------------------------------------------------------------*/
/* get next raw character from line-buffered stdin                            */
/*----------------------------------------------------------------------------*/
void Charser::vNextRawCharFromStdin(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT(! m_bSeenEOF);
    
  vNextCharFromRawBuff(rxAnnChar);
  
  if (rxAnnChar.m_cChar) {
    ASSERT(! m_bSeenEOF);
    return;
  }
  
  ASSERT(m_oRawFile.bIsStdin());
  ASSERT(m_oRawFile.bIsOpen());
  
  if (! m_bSingleLineCmd) {
    vReadStdinIntoRawBuff();
    vNextCharFromRawBuff(rxAnnChar);
    ASSERT(! m_bSeenEOF);
    return;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_acRawChar = m_sRawBuff.azChar();
    *m_acRawChar = '\0';
    rxAnnChar.m_cChar = '\0';
    ASSERT(! m_bSeenEOF);
    return ;
  }
  
  m_bRead = false;
  vReadStdinIntoRawBuff();
  vNextCharFromRawBuff(rxAnnChar);
  
  ASSERT(! m_bSeenEOF);
}

/*----------------------------------------------------------------------------*/
/* get next raw character from buffered string                                */
/*----------------------------------------------------------------------------*/
void Charser::vNextRawCharFromString(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT(m_bSeenEOF);
    
  vNextCharFromRawBuff(rxAnnChar);
  
  if (rxAnnChar.m_cChar)
    return;
  
  ASSERT(m_bSeenEOF);
  ASSERT('\0' == rxAnnChar.m_cChar);
}

/*----------------------------------------------------------------------------*/
/* get next annotated character from raw buffer                               */
/*----------------------------------------------------------------------------*/
void Charser::vNextCharFromRawBuff(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);

  rxAnnChar.m_cChar = *m_acRawChar++;
  
  rxAnnChar.m_iLine = m_iLineRaw;
  rxAnnChar.m_iCol  = ++m_iColRaw;
}

/*----------------------------------------------------------------------------*/
/* read raw line from input file                                              */
/*----------------------------------------------------------------------------*/
void Charser::vReadFileIntoRawBuff(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT(m_bUseOpened);

  ASSERT(! m_bSeenEOF);

  char cInput;
  
  char* acRawLineCrs = m_sRawBuff.azChar();
  
  do {
    
    cInput = m_oRawFile.cGetChar();
    
    *acRawLineCrs++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
    
  } while (cInput);
  
  ASSERT(! m_bSeenEOF);
  if (! cInput)
    m_bSeenEOF = true;
  
  *acRawLineCrs++ = '\0';
  
  m_iLineRaw++;
  m_iColRaw = 0;
  
  m_acRawChar = m_sRawBuff.azChar();
}

/*----------------------------------------------------------------------------*/
/* read raw line from stdin                                                   */
/*----------------------------------------------------------------------------*/
void Charser::vReadStdinIntoRawBuff(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT(! m_bSeenEOF);
  
  char* acRawLine = m_sRawBuff.azChar();
      
  for (;;) {
    
    char cInput = m_oRawFile.cGetChar();
    
    *acRawLine++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
  }
  
  *acRawLine++ = '\0';
  
  m_iLineRaw++;
  m_iColRaw = 0;

  m_acRawChar = m_sRawBuff.azChar();
  
  ASSERT(! m_bSeenEOF);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
