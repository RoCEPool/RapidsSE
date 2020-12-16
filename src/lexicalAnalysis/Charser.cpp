/*============================================================================*/
/* Charser.cpp -- read input as chars, removing line and block comments       */
/*============================================================================*/

#include "LexicalAnalysis.h"

#include "File.hpp" //?

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
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  //m_bMalloc set in new
  m_pNext   = pNext;
  m_iDepth  = m_pNext ? m_pNext->iDepth() + 1 : 0;
  
  d_iDebugMode = false;
  
  m_bUseOpened = true;
  m_bUseBegun  = true;
  
  m_bUseClosed = true;
  m_bUseEnded  = true;
  
  m_pSrcFile = new File(nullptr);
  
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
  
  if (m_pSrcFile->bIsOpen())
    m_pSrcFile->vClose();
  
  delete m_pSrcFile;
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
/* get prompt division                                                        */
/*----------------------------------------------------------------------------*/
Stz* Charser::szDivision(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_sDivision;
}

/*----------------------------------------------------------------------------*/
/* set prompt division                                                        */
/*----------------------------------------------------------------------------*/
void Charser::vDivision(Stz* szDivision)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  m_sDivision = szDivision;
}

/*----------------------------------------------------------------------------*/
/* get next annotated char                                                    */
/*----------------------------------------------------------------------------*/
void Charser::vGetAnnChar(AnnChar& rxAnnChar, bool bStrip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);

  if (d_iDebugMode) {
    SP_Charser spCharser;

    vJot2('>',(Stz*)__func__,"Charser[%lld] %s %s"_Stz,
          m_iDepth,aIS[m_eSource],m_sPath.szStz());
    vJot2(':',"Charser"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
  
  // debug

//  if (IS_FILE == m_eSource) {
//    Resources::vDebugPoint();
//    if (m_bSingleLineCmd)
//      Resources::vDebugPoint();
//    else
//      Resources::vDebugPoint();
//  }
//  else if (IS_STDIN == m_eSource) {
//    Resources::vDebugPoint();
//    if (m_bSingleLineCmd)
//      Resources::vDebugPoint();
//    else
//      Resources::vDebugPoint();
//  }
//  else if (IS_STRING == m_eSource) {
//    Resources::vDebugPoint();
//    if (m_bSingleLineCmd)
//      Resources::vDebugPoint();
//    else
//      Resources::vDebugPoint();
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // do we have any chars in the regs?
  if (! m_nLoaded)
    vLoadRegs(bStrip);
  
  if (! m_nLoaded) {
    rxAnnChar.vReset();
    if (d_iDebugMode) {
      vJot1('r',"no more AnnChar"_Stz);
      SP_AnnChar spAnnChar;
      vJot2(':',"AnnChar:(null)"_Stz,"%s"_Stz,spAnnChar.sz(&rxAnnChar));
      vJot2('<',(Stz*)__func__,""_Stz);
    }
    return;
  }
  
  // consume the first char reg
  vConsumeFirst(rxAnnChar);
  
  if (d_iDebugMode) {
    SP_AnnChar spAnnChar;
    vJot1('r',"got next AnnChar"_Stz);
    vJot2(':',"AnnChar:(some)"_Stz,"%s"_Stz,spAnnChar.sz(&rxAnnChar));
    vJot2('<',(Stz*)__func__,""_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* put char back into stream                                                  */
/*----------------------------------------------------------------------------*/
void Charser::vUngetAnnChar(AnnChar& rxAnnChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  vRewindRegs(rxAnnChar);
}

//
// Source //////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* is stdin?                                                                  */
/*----------------------------------------------------------------------------*/
bool Charser::bIsStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  if (IS_STDIN != m_eSource)
    return false;

  bool bIsStdin = m_pSrcFile->bIsStdin();

  return bIsStdin;
}

/*----------------------------------------------------------------------------*/
/* get path                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Charser::szPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_sPath;
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
/* get column number of source                                                */
/*----------------------------------------------------------------------------*/
i8 Charser::iSrcCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_iSrcCol;
}

/*----------------------------------------------------------------------------*/
/* get line number of source                                                  */
/*----------------------------------------------------------------------------*/
i8 Charser::iSrcLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_iSrcLine;
}

/*----------------------------------------------------------------------------*/
/* get prompt section                                                         */
/*----------------------------------------------------------------------------*/
Stz* Charser::szSection(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_sSection;
}

/*----------------------------------------------------------------------------*/
/* set prompt section                                                         */
/*----------------------------------------------------------------------------*/
void Charser::vSection(Stz* szSection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  m_sSection = szSection;
}


//
// Control /////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* set debug mode                                                             */
/*----------------------------------------------------------------------------*/
void Charser::vDebugMode(i8 iDebugMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  d_iDebugMode = iDebugMode;
}

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
/* insepct                                                                    */
/*----------------------------------------------------------------------------*/
void Charser::vInspect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  SP_String spStr;
  
  vJot1('r',"Charser[%lld] (L%lld C%lld) : "
         "Source %s Path %s %s %s %s %s %s %s : "
         "Loaded %lld %c %c [>%s<]\n"_Stz,
         
         m_iDepth,
         m_iSrcLine,
         m_iSrcCol,
         
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
         m_xRegTwo.m_cChar ? m_xRegOne.m_cChar : ' ',
       
         spStr.szSample(m_sRawBuff, (Stz*)m_acRawChar, -1)
       
         );
}

//
// Mode ////////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* single line mode?                                                          */
/*----------------------------------------------------------------------------*/
bool Charser::bSingleLineCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_bSingleLineCmd;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Charser::vSingleLineCmd(bool bSingleLineCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  m_bSingleLineCmd = bSingleLineCmd;
}

//
// Usage ///////////////////////////////////////////////////////////////////////
//

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
      ASSERT( m_pSrcFile->bIsOpen() );
      m_pSrcFile->vClose();
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
  
  m_bUseOpened = m_pSrcFile->bOpenRead(szPath);
   
  return m_bUseOpened;
}

/*----------------------------------------------------------------------------*/
/* get include                                                                */
/*----------------------------------------------------------------------------*/
Word* Charser::pUseInclude(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_pInclude;
}

/*----------------------------------------------------------------------------*/
/* use is given include group/file                                            */
/*----------------------------------------------------------------------------*/
void Charser::vUseInclude(Word* pInclude)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);

  m_pInclude = pInclude;
}

/*----------------------------------------------------------------------------*/
/* use stdin for charsing                                                     */
/*----------------------------------------------------------------------------*/
void Charser::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE == m_eSource);
  
  m_pSrcFile->vUseStdin();
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

//
// Charser Stack ///////////////////////////////////////////////////////////////
//

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

//
// Debug ///////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* get number loaded of loaded registers                                      */
/*----------------------------------------------------------------------------*/
i8 Charser::nLoaded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_nLoaded;
}

/*----------------------------------------------------------------------------*/
/* get reg one                                                                */
/*----------------------------------------------------------------------------*/
AnnChar* Charser::aRegOne(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return &m_xRegOne;
}

/*----------------------------------------------------------------------------*/
/* get reg two                                                                */
/*----------------------------------------------------------------------------*/
AnnChar* Charser::aRegTwo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return &m_xRegTwo;
}

/*----------------------------------------------------------------------------*/
/* get raw buffer                                                             */
/*----------------------------------------------------------------------------*/
char* Charser::azRawBuff(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_azRawBuff;
}

/*----------------------------------------------------------------------------*/
/* get current pointer into raw buffer                                        */
/*----------------------------------------------------------------------------*/
char* Charser::acRawChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  return m_acRawChar;
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
  m_sDivision  = ""_Stz;
  m_sSection   = ""_Stz;
  m_pInclude   = nullptr;
  
  // source
  m_pSrcFile->vReset();
  m_iSrcLine   = 0;
  m_iSrcCol    = 0;
  
  // raw buffer
  m_sRawBuff   = ""_Stz;
  m_azRawBuff  = m_sRawBuff.azChar();
  m_acRawChar  = m_sRawBuff.azChar();

  // use state
  m_bUseBegun  = false;
  m_bUseClosed = false;
  m_bUseEnded  = false;
  m_bUseOpened = false;
  
  // mode
  m_bSingleLineCmd  = false;

  // registers
  m_nLoaded    = 0;
  m_xRegOne.vReset();
  m_xRegTwo.vReset();
  m_eComment   = CS_NONE;
  m_bRead      = false;
  m_bSeenEOF   = false;
}

//
// Character Processing ////////////////////////////////////////////////////////
//

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

  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"cleared both regs"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }

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
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"consumed R1"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
}

/*----------------------------------------------------------------------------*/
/* discard the character in the first register (its part of a comment)        */
/*----------------------------------------------------------------------------*/
void Charser::vDiscardFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(m_nLoaded);
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"discard R1"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
  
  m_xRegOne.m_cChar  = m_xRegTwo.m_cChar;
  
  m_xRegTwo.vReset();

  m_nLoaded--;
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"discarded R1"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
}

/*----------------------------------------------------------------------------*/
/* load both the first and second registers from raw input stream             */
/*----------------------------------------------------------------------------*/
void Charser::vLoadBoth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(! m_nLoaded);
  
  vLoadRegWithRawChar(m_xRegOne);
  
  if (m_xRegOne.m_cChar) {
    
    m_nLoaded++;
    
    vLoadRegWithRawChar(m_xRegTwo);
    
    if (m_xRegTwo.m_cChar)
      m_nLoaded++;
  }
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"loaded both R1 & R2"_Stz,"{ %s }"_Stz,spCharser.sz(this));
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
  
  vLoadRegWithRawChar(m_xRegOne);
  
  if (m_xRegOne.m_cChar)
    m_nLoaded++;
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"loaded R1"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
}

/*----------------------------------------------------------------------------*/
/* load first (& maybe) second register from raw input stream, strip comments */
/*----------------------------------------------------------------------------*/
void Charser::vLoadRegs(bool bStrip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  ASSERT(0 == m_nLoaded);
  
  while (! m_nLoaded) {
    
    vLoadFirst();
    
    if (! m_nLoaded)
      break;
    
    if (! bStrip || ! bPossibleComment())
      continue;              // this may leave a char in second register e.g. /t
    
    vStripComment();
  }
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"loaded R1 & R2"_Stz,"{ %s }"_Stz,spCharser.sz(this));
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
  
  vLoadRegWithRawChar(m_xRegTwo);
  
  if (m_xRegTwo.m_cChar)
    m_nLoaded++;
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"loaded R2"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
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
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"rewound regs"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
}

//
// Comment Processing //////////////////////////////////////////////////////////
//

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
    
  if (d_iDebugMode) {
    vJot1('r',"skipping block comment"_Stz);
  }
  
  vClearRegs();
  
  vLoadBoth();
  
  while (! bBlockCommentEnd()) {
    
    if (m_nLoaded)
      vDiscardFirst();
    
    if (0 == m_nLoaded)
      vLoadBoth();
    else
      vLoadSecond();
    
    if (0 == m_nLoaded && bSeenEOF())
      break;
  }
  
  vClearRegs();
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"skipped block comment"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
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
  
  if ('\n' == m_xRegOne.m_cChar || '\0' == m_xRegOne.m_cChar)
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
  
  if (d_iDebugMode) {
    vJot1('r',"skipping line comment"_Stz);
  }
  
  vClearRegs();
  
  vLoadFirst();
  
  //char cEndChar = ' ';
  while (! bLineCommentEnd()) {
    
    //cEndChar = m_xRegOne.m_cChar;
    
    vDiscardFirst();
    vLoadFirst();
  }
  
  ASSERT('\n' == m_xRegOne.m_cChar || '\0' == m_xRegOne.m_cChar);
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"skipped line comment"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
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
/* strip a possible line or block comment, replace with a single ' '          */
/*----------------------------------------------------------------------------*/
void Charser::vStripComment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(IS_NONE != m_eSource);
  
  AnnChar xReplaceWith;
  
  xReplaceWith.vReset();
  xReplaceWith.m_cChar = ' ';

  switch (eCommentStyle()) {
      
    case CS_BLOCK:
      vBlockCommentSkip();
      xReplaceWith.m_iLine = m_iSrcLine;
      xReplaceWith.m_iCol  = m_iSrcCol;
      vRewindRegs(xReplaceWith);
      break;
      
    case CS_LINE:
      vLineCommentSkip();
      xReplaceWith.m_iLine = m_iSrcLine;
      xReplaceWith.m_iCol  = m_iSrcCol;
      vRewindRegs(xReplaceWith);
      break;
      
    default:
      break;
  }

  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"stripping comment"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
}

//
// Loading /////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* get next raw character from input source                                   */
/*----------------------------------------------------------------------------*/
void Charser::vLoadRegWithRawChar(AnnChar& rxReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  
  switch (m_eSource) {
      
    case IS_FILE:
      vLoadRegWithRawCharFromFile(rxReg);
      
      
      if (! rxReg.m_cChar) {
        
      }
      
      
      ASSERT( (rxReg.m_cChar) ||
              (! rxReg.m_cChar && (m_bSeenEOF || m_bSingleLineCmd)) );
      break;
      
    case IS_STDIN:
      vLoadRegWithRawCharFromStdin(rxReg);
      ASSERT(! m_bSeenEOF);
      break;
      
    case IS_STRING:
      vLoadRegWithRawCharFromString(rxReg);
      ASSERT(m_bSeenEOF);
      break;
      
    default:
      rxReg.vReset();
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid m_eSource %lld"_Stz,m_eSource);
  }
}

/*----------------------------------------------------------------------------*/
/* get next raw character from line-buffered source file                      */
/*----------------------------------------------------------------------------*/
void Charser::vLoadRegWithRawCharFromFile(AnnChar& rxReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);

  vReadRawCharFromRawBuff(rxReg);
  
  if (rxReg.m_cChar)
    return ;
  
  if (m_bSeenEOF) {
    rxReg.m_cChar = '\0';
    return;
  }
  
  ASSERT(m_pSrcFile->bIsOpen());
  
  if (! m_bSingleLineCmd) {
    vReadFileIntoRawBuff();
    vReadRawCharFromRawBuff(rxReg);
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
  vReadRawCharFromRawBuff(rxReg);
  
  ASSERT( (rxReg.m_cChar) || (!rxReg.m_cChar && m_bSeenEOF) );
}

/*----------------------------------------------------------------------------*/
/* get next raw character from line-buffered stdin                            */
/*----------------------------------------------------------------------------*/
void Charser::vLoadRegWithRawCharFromStdin(AnnChar& rxReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT(! m_bSeenEOF);
    
  vReadRawCharFromRawBuff(rxReg);
  
  if (rxReg.m_cChar) {
    ASSERT(! m_bSeenEOF);
    return;
  }
  
  ASSERT(m_pSrcFile->bIsStdin());
  ASSERT(m_pSrcFile->bIsOpen());
  
  if (! m_bSingleLineCmd) {
    vReadStdinIntoRawBuff();
    vReadRawCharFromRawBuff(rxReg);
    ASSERT(! m_bSeenEOF);
    return;
  }
  
  if (! m_bRead) {
    m_bRead = true;
    m_acRawChar = m_sRawBuff.azChar();
    *m_acRawChar = '\0';
    rxReg.m_cChar = '\0';
    ASSERT(! m_bSeenEOF);
    return ;
  }
  
  m_bRead = false;
  vReadStdinIntoRawBuff();
  vReadRawCharFromRawBuff(rxReg);
  
  ASSERT(! m_bSeenEOF);
}

/*----------------------------------------------------------------------------*/
/* get next raw character from buffered string                                */
/*----------------------------------------------------------------------------*/
void Charser::vLoadRegWithRawCharFromString(AnnChar& rxReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);
  ASSERT(m_bSeenEOF);
    
  vReadRawCharFromRawBuff(rxReg);
  
  if (rxReg.m_cChar)
    return;
  
  ASSERT(m_bSeenEOF);
  ASSERT('\0' == rxReg.m_cChar);
}

//
// Reading /////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* read next raw character from raw buffer                                    */
/*----------------------------------------------------------------------------*/
void Charser::vReadRawCharFromRawBuff(AnnChar& rxReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHARSER,_FOOTER_);
  
  ASSERT(m_bUseBegun);
  ASSERT(! m_bUseEnded);

  rxReg.m_cChar = *m_acRawChar++;
  
  rxReg.m_iLine = m_iSrcLine;
  rxReg.m_iCol  = ++m_iSrcCol;
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
    
    cInput = m_pSrcFile->cGetChar();
    
    *acRawLineCrs++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
    
  } while (cInput);
  
  ASSERT(! m_bSeenEOF);
  if (! cInput)
    m_bSeenEOF = true;
  
  *acRawLineCrs++ = '\0';
  
  m_iSrcLine++;
  m_iSrcCol = 0;
  
  m_acRawChar = m_sRawBuff.azChar();
  
  if (d_iDebugMode) {
    SP_Charser spCharser;
    vJot2('r',"read file into RawBuff"_Stz,"{ %s }"_Stz,spCharser.sz(this));
  }
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
    
    char cInput = m_pSrcFile->cGetChar();
    
    *acRawLine++ = cInput;
    
    if (C_END_OF_LINE == cInput)
      break;
  }
  
  *acRawLine++ = '\0';
  
  m_iSrcLine++;
  m_iSrcCol = 0;

  m_acRawChar = m_sRawBuff.azChar();
  
  ASSERT(! m_bSeenEOF);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
