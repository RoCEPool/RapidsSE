/*============================================================================*/
/* Input.cpp -- Input : collect input stream into tokens                      */
/*============================================================================*/

#include "BASE.h"

#include "SP_Token.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Input,CN_INPUT)
__aNewFunction(Input,CN_INPUT)
__vDeleteFunction(Input,CN_INPUT)
__aFooterFunction(Input)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Input::Input(Input* pNext)
: Object(CN_INPUT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pNext   = pNext;
  m_iDepth  = m_pNext ? m_pNext->iDepth() + 1 : 0;
  
  m_pCharser = new Charser(nullptr);
  
  m_eTknForm = UF_NONE;
  
  m_xAnnChar.vReset();
      
  m_pInpTknVec  = new TokenVec;
  m_pCurrTkn = nullptr;
  m_pInpTagSet  = new TagSet;

  m_pCharser_spares = nullptr;

  m_bRevertToCpp = false;

  // debugging aids
  d_iDebugMode               = false;
  
  d_i_Division               = 0;
  d_nCollectOneToken         = 0;
  d_nCollectVecFirstLine     = 0;
  d_nCollectVecTillSemicolon = 0;
  d_nReadCommand             = 0;
  d_nUseBegin                = 0;

  _checkInitialised_(CN_INPUT);
  
  
  for (i8 i_Charser = 0; i_Charser < 3; i_Charser++)
    m_pCharser_spares = new Charser(m_pCharser_spares);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Input::~Input(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  while (m_pCharser)
    RASSERT(bPopCharser());
  
  while (m_pCharser_spares)
    m_pCharser_spares = m_pCharser_spares->pDeleteCharser();

  RASSERT(! m_pNext);

  delete m_pInpTagSet;
  delete m_pInpTknVec;
}

/*----------------------------------------------------------------------------*/
/* dump tokeniser                                                             */
/*----------------------------------------------------------------------------*/
void Input::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  _static_
  UNUSED(eClass);
  UNUSED(aTab);

  Input* pInput = _pInput(pObject);
  
  vTrace("parser %p\n"_Stz,
         pInput->m_pCharser);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a charser from spares                                             */
/*----------------------------------------------------------------------------*/
void Input::vAllocCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
      
  if (m_pCharser_spares) {
    
    Charser* pCharser_spare = m_pCharser_spares;
    m_pCharser_spares = m_pCharser_spares->pNext();

    pCharser_spare->vNext(m_pCharser);
    m_pCharser = pCharser_spare;
  }
  else {
    m_pCharser = new Charser(m_pCharser);
  }
}

/*----------------------------------------------------------------------------*/
/* get chars of current token                                                 */
/*----------------------------------------------------------------------------*/
Stz* Input::szChars(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  Token* pCurrToken = m_pInpTknVec->pCurrToken();
  
  return pCurrToken->szChars();
}

/*----------------------------------------------------------------------------*/
/* set debug mode                                                             */
/*----------------------------------------------------------------------------*/
void Input::vDebugMode(i8 iDebugMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  d_iDebugMode = iDebugMode;
  
  if (d_iDebugMode > 2)
    m_pCharser->vDebugMode(d_iDebugMode);
  else
    m_pCharser->vDebugMode(0);
}

/*----------------------------------------------------------------------------*/
/* delete input                                                               */
/*----------------------------------------------------------------------------*/
Input* Input::pDeleteInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Input* pNext = m_pNext;
  
  m_pNext = nullptr;
  
  delete this;
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* get depth of charser                                                       */
/*----------------------------------------------------------------------------*/
i8 Input::iDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/* set prompt division                                                        */
/*----------------------------------------------------------------------------*/
void Input::vDivision(Stz* szDivision)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vDivision(szDivision);
}

/*----------------------------------------------------------------------------*/
/* get prompt division                                                        */
/*----------------------------------------------------------------------------*/
Stz* Input::szDivision(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Stz* szDivision = m_pCharser->szDivision();
  
  return szDivision;
}

/*----------------------------------------------------------------------------*/
/* dump the tokeniser                                                         */
/*----------------------------------------------------------------------------*/
void Input::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("Input::vDump\n"_Stz);
  vTrace("----------------\n"_Stz);

  Token* pCurTkn = m_pInpTknVec->pCurrToken();
  
  vTrace("Current Token: '%s' : %s @%lld,%lld\n"_Stz,
         pCurTkn->szChars(),
         aTN[pCurTkn->eToken()],
         pCurTkn->iLineNo(),
         pCurTkn->iColNo()
         );
  
  vTrace("Charser TypedStack:\n"_Stz);
         
  Charser* pCharser = m_pCharser;
  while (pCharser) {
    
    vTrace("[%lld] %s @%lld,%lld\n"_Stz,
           pCharser->iDepth(),
           pCharser->szPath(),
           pCharser->iSrcLine(),
           pCharser->iSrcCol()
           );
    
    pCharser = pCharser->pNext();
  }
  
  vTrace("----------------\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* hard reset after fork                                                      */
/*----------------------------------------------------------------------------*/
void Input::vForkReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vForkReset();
  
  m_eTknForm = UF_SEN_TOKENS;
}

/*----------------------------------------------------------------------------*/
/* collecting cpp tokens?                                                     */
/*----------------------------------------------------------------------------*/
bool Input::bFormCppTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  if (UF_CPP_TOKENS == m_eTknForm)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* set cpp tokens                                                             */
/*----------------------------------------------------------------------------*/
void Input::vFormCppTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_eTknForm = UF_CPP_TOKENS;
}

/*----------------------------------------------------------------------------*/
/* collecting sen tokens?                                                     */
/*----------------------------------------------------------------------------*/
bool Input::bFormSenTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  if (UF_SEN_TOKENS == m_eTknForm)
    return true;

  return false;

}

/*----------------------------------------------------------------------------*/
/* set sen tokens                                                             */
/*----------------------------------------------------------------------------*/
void Input::vFormSenTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
    
  m_eTknForm = UF_SEN_TOKENS;
}

/*----------------------------------------------------------------------------*/
/* allocate a charser from spares                                             */
/*----------------------------------------------------------------------------*/
void Input::vFreeCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Charser* pCharser = m_pCharser->pNext();
  
  m_pCharser->vReset();
  m_pCharser->vNext(m_pCharser_spares);
  m_pCharser_spares = m_pCharser;

  m_pCharser = pCharser;
}

/*----------------------------------------------------------------------------*/
/* get input tag set                                                          */
/*----------------------------------------------------------------------------*/
TagSet* Input::pInpTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  return m_pInpTagSet;
}

/*----------------------------------------------------------------------------*/
/* get input token vector                                                     */
/*----------------------------------------------------------------------------*/
TokenVec* Input::pInpTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  return m_pInpTknVec;
}

/*----------------------------------------------------------------------------*/
/* inspection                                                                 */
/*----------------------------------------------------------------------------*/
void Input::vInspect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
//  vTrace("Input : %s\n"_Stz,
//         aUF[m_eTknForm]
//         );

  if (! m_pCurrTkn) {
    vJot1('r',"Input #UseBegin.%lld #ReadCommand.%lld #CollectToken.%lld : "
         "]>NoToken<[\n"_Stz,
           d_nUseBegin,
           d_nReadCommand,
           d_nCollectOneToken
           );
    m_pCharser->vInspect();
    return;
  }
  
  SP_Token spTkn;

  vJot1('r',"Input #UseBegin.%lld #ReadCommand.%lld #CollectToken.%lld : "
       "%s\n"_Stz,
         d_nUseBegin,
         d_nReadCommand,
         d_nCollectOneToken,
         spTkn.sz(m_pCurrTkn, m_pInpTknVec->iCurrToken())
         );

  m_pCharser->vInspect();
}

/*----------------------------------------------------------------------------*/
/* get stdin?                                                                 */
/*----------------------------------------------------------------------------*/
bool Input::bIsStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bStdin = m_pCharser->bIsStdin();
  
  return bStdin;
}

/*----------------------------------------------------------------------------*/
/* get charser path                                                           */
/*----------------------------------------------------------------------------*/
Stz* Input::szPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Stz* szSource = m_pCharser->szPath();
  
  return szSource;
}

/*----------------------------------------------------------------------------*/
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool Input::bPopCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (! m_pCharser)
    return false;
  
//  m_pCharser = m_pCharser->pDeleteCharser();
//
  vFreeCharser();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void Input::vPushCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  vAllocCharser();
  
  m_pCharser->vUseBegin();
    
  m_pCharser->vDebugMode(d_iDebugMode);
}

/*----------------------------------------------------------------------------*/
/* get raw input                                                              */
/*----------------------------------------------------------------------------*/
Stz* Input::szRawInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Stz* szRaw = m_pCharser->szRawInput();
  
  return szRaw;
}

/*----------------------------------------------------------------------------*/
/* raw input clear                                                            */
/*----------------------------------------------------------------------------*/
void Input::vRawInputClear(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vRawInputClear();
}

/*----------------------------------------------------------------------------*/
/* read next command from input stream                                        */
/*----------------------------------------------------------------------------*/
bool Input::bReadCommand(eEC& eInputError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
    
  d_nReadCommand++;
  
  ASSERT(m_pCharser->bUseBegun());

  if (d_iDebugMode) {
    d_i_Division = iJotDivision('>',"Input"_Stz,(Stz*)__func__,""_Stz);
   }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first line so we can test if its a command file to be executed
  
  //m_pCharser->vRawInputClear();
  
  vSingleLineCmd(true);
    
  if (d_iDebugMode)
    m_pInpTknVec->vDumpVec();
  
  if (! bCollectVecFirstLine(eInputError))
    goto L_Exit;

  //vTrace(" : >>>%s<<<\n"_Stz,m_pCharser->szRawInput());

  if (d_iDebugMode)
    m_pInpTknVec->vDumpVec();

  m_pInpTknVec->vIntegrityCheck();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for empty file

  if (0 == m_pInpTknVec->nTokens()) {
    if (d_iDebugMode)
      vJot1('r',"empty line return"_Stz);
    goto L_Exit;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for a command file
  
  // if its a command file just return
  if ( ! m_pInpTknVec->pGet(0)->bSame("/"_Stz) &&
      File::bExists(m_pInpTknVec->pGet(0)->szChars())) {
    if (d_iDebugMode)
      vJot1('r',"is cmd file return"_Stz);
    goto L_Exit;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // have we already seen the ';'
  
  if (m_pInpTknVec->bHasSemicolon()) {
    m_pInpTknVec->vRemoveSemicolon();
    m_pInpTknVec->vIntegrityCheck();
    m_pInpTknVec->vListTokens();
    goto L_Exit;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read more until we encounter a ';'
  
  // switch to multi-line command
  vSingleLineCmd(false);
  
  // read more till ;
  if (d_iDebugMode)
    m_pInpTknVec->vDumpVec();
  
  if (! bCollectVecTillSemicolon(eInputError))
    goto L_Exit;

  //vTrace(" : %s\n"_Stz,m_pCharser->szRawInput());
  
  if (d_iDebugMode)
    m_pInpTknVec->vDumpVec();

  m_pInpTknVec->vIntegrityCheck();
  if (d_iDebugMode) {
    vJot1('r',"collected till semicolon"_Stz);
    m_pInpTknVec->vListTokens();
  }

  // switch back to single line command
  vSingleLineCmd(true);
  
  // stip off ;
  m_pInpTknVec->vStripSemicolon();
  m_pInpTknVec->vIntegrityCheck();

  // stip off &
  //m_pTknVec->vStripAmpersand();
  //m_pTknVec->vIntegrityCheck();

  m_pInpTknVec->vIntegrityCheck();
  
  
L_Exit:

  vTestRevertToCpp();


  if (d_iDebugMode) {
    i8 i = iJotSummary(':',(Stz*)__func__,"TokenVec:"_Stz);
    UNUSED(i);

    m_pInpTknVec->vListTokens();
    vJot2('<',(Stz*)__func__,""_Stz);
  }

  //m_pInpTknVec->vDumpVec(0);

  if (eInputError)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* read till semicolon                                                        */
/*----------------------------------------------------------------------------*/
bool Input::bReadTillSemicolon(eEC& eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  vCursorReset();

  for (;;) {
    if (! bCursorAdvance(eError))
      return true;
    vCollectOneToken();
    
    // look for terminal
    if (m_pCurrTkn->bSame(";"_Stz))
      break;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* parse next token                                                           */
/*----------------------------------------------------------------------------*/
bool Input::bReadToken(eEC& eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  
  vCursorReset();

  if (! bCursorAdvance(eError))
    return false;

  vCollectOneToken();
  
//  if (m_pCurrTkn)
//    m_pCurrTkn->vShow();

  return true;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Input::vReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_INPUT, _FOOTER_);

  m_pInpTknVec->vSoftReset();
  m_pCurrTkn = nullptr;
  m_eTknForm = UF_NONE;
  m_pInpTagSet->vReset();

  while (m_pCharser) {
    m_pCharser->vResetCharser();
   RASSERT(bPopCharser());
  }

  vPushCharser();
  m_pCharser->vResetCharser();

  vUseStdin();
  vFormCppTokens();
  vUseBegin();
}

/*----------------------------------------------------------------------------*/
/* set prompt section                                                         */
/*----------------------------------------------------------------------------*/
void Input::vSection(Stz* szSection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vSection(szSection);
}

/*----------------------------------------------------------------------------*/
/* set prompt section                                                         */
/*----------------------------------------------------------------------------*/
Stz* Input::szSection(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Stz* szSection = m_pCharser->szSection();
  
  return szSection;
}

/*----------------------------------------------------------------------------*/
/* seen EOF?                                                                  */
/*----------------------------------------------------------------------------*/
bool Input::bSeenEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bEOF = m_pCharser->bSeenEOF();
  
  return bEOF;
}

/*----------------------------------------------------------------------------*/
/* set single line mode                                                       */
/*----------------------------------------------------------------------------*/
void Input::vSingleLineCmd(bool bLineMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vSingleLineCmd(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* get SignleLine mode?                                                       */
/*----------------------------------------------------------------------------*/
bool Input::bSingleLineCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bLineMode = m_pCharser->bSingleLineCmd();
  
  return bLineMode;
}

/*----------------------------------------------------------------------------*/
/* get token type                                                             */
/*----------------------------------------------------------------------------*/
eTN Input::eToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Token* pCurrToken = m_pInpTknVec->pCurrToken();
  
  eTN eToken = pCurrToken->eToken();
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* begin use of charser source                                                */
/*----------------------------------------------------------------------------*/
void Input::vUseBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  d_nUseBegin++;
  
  m_pCharser->vUseBegin();
  
  //vInspect();
}

/*----------------------------------------------------------------------------*/
/* close use of charser source                                                */
/*----------------------------------------------------------------------------*/
void Input::vUseClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUseClose();
}

/*----------------------------------------------------------------------------*/
/* end use of source                                                          */
/*----------------------------------------------------------------------------*/
void Input::vUseEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUseEnd();
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Input::bUseFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bOpened = m_pCharser->bUseFile(szPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void Input::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Input::vUseString(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUseString(szString);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


//
// collecting tokens ///////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* collect next token                                                         */
/*----------------------------------------------------------------------------*/
void Input::vCollectOneToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  d_nCollectOneToken++;
  
  ASSERT(m_pCharser->bUseBegun());
  ASSERT(UF_CPP_TOKENS == m_eTknForm || UF_SEN_TOKENS == m_eTknForm);

  if (d_iDebugMode) {
    vJot2('>',(Stz*)__func__,""_Stz);
  }

  if (bFormCppTokens())
    vCollectCpp();
  else
    vCollectSen();

  if (d_iDebugMode) {
    SP_Token spTkn;
    vJot2('<',(Stz*)__func__,spTkn.sz(m_pCurrTkn,
                                                            m_pInpTknVec->iCurrToken()));
  }
}

/*----------------------------------------------------------------------------*/
/* collect one line line of tokens from input stream                          */
/*----------------------------------------------------------------------------*/
bool Input::bCollectVecFirstLine(eEC& eInputError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
   d_nCollectVecFirstLine++;
  
   if (d_iDebugMode) {
     i8 i = iJotSection('>',(Stz*)__func__,""_Stz);
     UNUSED(i);

     //vInspect();
   }

  ASSERT(m_pCharser->bUseBegun());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pInpTknVec->vSoftReset();
  
  for (;;) {

    if (! bCursorAdvance(eInputError))
      return false;
    
    if (d_iDebugMode)
      m_pInpTknVec->vDumpVec();

    vCollectOneToken();

    if (d_iDebugMode)
      m_pInpTknVec->vDumpVec();

    if (m_pCurrTkn->bInvalidToken()) {
      eInputError = EC_INVALID_TOKEN;
      return false;
    }

    if (m_pCurrTkn->bIsNull()) {
      if (d_iDebugMode)
        vJot1('r',"null token detected"_Stz);
      vCursorRewind();
      if (d_iDebugMode)
        vJot1('r',"cursor rewound"_Stz);
      break;
    }
  }

  if (! m_pInpTknVec->nTokens() && bSeenEOF()) {
    bool b = bPopCharser();
    UNUSED(b);

    ASSERT(b);
  }
  
  if (d_iDebugMode) {
    vJot1(':',"TokenVec:"_Stz);
    m_pInpTknVec->vListTokens();
    vJot2('<',(Stz*)__func__,""_Stz);
  }

  return true;
 }

/*----------------------------------------------------------------------------*/
/* collect vector of tokens from input stream till ';'                        */
/*----------------------------------------------------------------------------*/
bool Input::bCollectVecTillSemicolon(eEC& eInputError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  d_nCollectVecTillSemicolon++;
  
  if (d_iDebugMode) {
    i8 i = iJotSection('>',(Stz*)__func__,""_Stz);
    UNUSED(i);
    vInspect();
  }

  ASSERT(m_pCharser->bUseBegun());
  
  // we should have a first token (e.g. create or drop)
  //ASSERT(m_pTknVec->pGet(0)->bHasChars());
  
  // we should not have seen ';'
  ASSERT( ! m_pInpTknVec->bHasSemicolon());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  if (d_iDebugMode)
    m_pInpTknVec->vDumpVec();
  
  // get tokens delimited by ';'
  for (;;) {

    if (! bCursorAdvance(eInputError))
      return false;
    
    vCollectOneToken();

    if (m_pCurrTkn->bInvalidToken()) {
      eInputError = EC_INVALID_TOKEN;
      return false;
    }

    if (TN_NULL == m_pCurrTkn->eToken())
      m_pInpTknVec->vDumpVec();

    ASSERT(TN_NULL != m_pCurrTkn->eToken());
    
    // look for terminal
    if (m_pCurrTkn->bIsSemicolon())
      break;
  }

  ASSERT(m_pInpTknVec->bHasSemicolon());
  
  if (d_iDebugMode) {
    vJot1(':',"TokenVec:"_Stz);
    m_pInpTknVec->vListTokens();
    vJot2('<',(Stz*)__func__,""_Stz);
  }

  return true;
}

// collect cpp tokens //////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* collect normal token                                                       */
/*----------------------------------------------------------------------------*/
void Input::vCollectCpp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(UF_CPP_TOKENS == m_eTknForm);

  char* azTermCrs = m_pCurrTkn->azChars();
  UNUSED(azTermCrs);
  
  m_pCurrTkn->vReset();

  vGetAnnCharStripped();
  
  vSkipWhitespace();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vTokenCollectBegin();
  // Note: We are removing vTokenCollectBegin(), which would get a pointer into
  // the TokenVecs byte array so it could write the token string directly into it.
  // While this would be quite performant, it means that the byte array must be
  // big enough for all supported inputs so as not to overflow.
  // Instead we will collect it into a string on the stack and then copy it to
  // the TokenVecs byte array when we append the token.
  s1M sTokenStr;
  m_pCurrTkn->vLineNo(m_xAnnChar.m_iLine);
  m_pCurrTkn->vColNo(m_xAnnChar.m_iCol);
  // The implementation below will use the string pointer from here, which points
  // to the stack string. At the end of this we will copy the stack string into
  // the TokenVec.
  m_pCurrTkn->vChars(sTokenStr.szStz());

  // end of input?
  if (bHitEOF()) {
    m_pCurrTkn->vSetNull();
  }

  // collect integer literal?
  else
  if (bStartCppNumber()) {
    vCollectCppNumber();
  }
  
  // collect double quoted string ?
  else
  if (bStartCppString()) {
    vCollectCppString();
  }

  // collect single quoted literal?
  else
  if (bStartCppLiteral()) {
    vCollectCppLiteral();
  }

    // collect name token?
  else
  if (bStartCppName()) {
    vCollectCppName();
  }
  
  // collect other
  else {
    vCollectCppOther();
  }
  
  //vTokenCollectEnd();
  // Copy the string that we've collected on the stack into the TokenVec bytespace.
  ASSERT(m_pInpTknVec->pGet(m_pInpTknVec->nTokens() - 1) == m_pCurrTkn); // always the last token
  m_pInpTknVec->szCopyStrForLastToken(m_pCurrTkn, sTokenStr.szStz());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
  // cross check valid normal tokens
  switch (m_pCurrTkn->eToken()) {
     
    // the expected tokens
    case TN_BOOLEAN:
    case TN_DECIMAL:
    case TN_FLOAT:
    case TN_INTEGER:
    case TN_INVALID:
    case TN_LITERAL:
    case TN_NAME:
    case TN_NULL:
    case TN_STRING:
    case TN_OTHER:
      break;
     
    // unexpected tokens
    case TN_TAG:
      NEVER_REACH;
     
    default:
      NEVER_REACH;
   }

   // test for collecting a pipeline and switch to sen tokens
  vTestSwitchToSen();

}

/*----------------------------------------------------------------------------*/
/* collect normal cpp single quoted literal                                    */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppLiteral(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  ASSERT(bStartSenString());

  // skip the leading '
  vGetAnnCharStripped();

  // collect the characters within the string
  while ( ! bEndCppLiteral() ) {

    if (m_xAnnChar.bIsNewLine()) {
      *z = '\0';
      m_pCurrTkn->vToken(TN_INVALID);
      return;
    }

    if (m_xAnnChar.bIsBackSlash()) {
      vGetAnnCharStripped();
      // translate \n into new line
      if ('n' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\n';
      else if ('r' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\r';
    }

    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }

  // terminate the string literal
  *z = '\0';

  m_pCurrTkn->vToken(TN_LITERAL);
}
/*----------------------------------------------------------------------------*/
/* collect normal cpp name                                                    */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  ASSERT(bStartCppName());
    
  // collect the characters of the name
  while ( StrLib::bIsNameBodyChar(m_xAnnChar.m_cChar) ) {
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vUngetAnnCharStripped();
  
  // terminate the name
  *z = '\0';
  
  m_pCurrTkn->vToken(TN_NAME);
}

/*----------------------------------------------------------------------------*/
/* collect normal cpp number literal (integer, decimal, float)                */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppNumber(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  i8 iNoDigits = 0;
  
  // we have seen the start of a number
  ASSERT(bStartCppNumber());
  
  // - start parsing an integer

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect initial digits
  while (m_xAnnChar.bIsDigit()) {
    *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float of form 123e4 (no decimal point)
  if (m_xAnnChar.bIsE()) {
    *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();

    if (m_xAnnChar.bIsSign()) {
      *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();
    }

    while (m_xAnnChar.bIsDigit()) {
      *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();
    }

    vUngetAnnCharStripped();

    // terminate the float literal
    *z = '\0';
    m_pCurrTkn->vToken(TN_FLOAT);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 0x hex form of integer
  if (1 == iNoDigits && m_xAnnChar.bIsX()) {
    
    // collect the 'x'
    *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();
    
    // collect the hex digits
    while (m_xAnnChar.bIsHex()) {
      *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();
    }

    vUngetAnnCharStripped();

    // terminate the integer literal
    *z = '\0';
    m_pCurrTkn->vToken(TN_INTEGER);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // . decimal number?
  if (m_xAnnChar.bIsDot()) {

    // collect the '.'
    *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();

    // test for ..
    if (m_xAnnChar.bIsDot()) {
      *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();

      vUngetAnnCharStripped();

      // terminate the other literal
      *z = '\0';
      m_pCurrTkn->vToken(TN_OTHER);
      return;
    }
    
    // collect digits after decimal
    while (m_xAnnChar.bIsDigit()) {
      *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();
    }

    // an 'e' introduces a float (scientific notation)
    if (m_xAnnChar.bIsE()) {

      // collect the 'e'
      *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();

      // collect optional sign of e
      if (m_xAnnChar.bIsSign()) {
        *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();
      }

      // collect e digits
      while (m_xAnnChar.bIsDigit()) {
        *z++ = m_xAnnChar.m_cChar; iNoDigits++; vGetAnnCharStripped();
      }

      vUngetAnnCharStripped();

      // terminate the float literal
      *z = '\0';
      m_pCurrTkn->vToken(TN_FLOAT);
      return;
    }

    vUngetAnnCharStripped();
    
    // terminate the decimal literal
    *z = '\0';
    m_pCurrTkn->vToken(TN_DECIMAL);
    
    return;
  }
  
  // push back the unconsumed character
  vUngetAnnCharStripped();
    
  // terminate the integer literal
  *z = '\0';
  
  m_pCurrTkn->vToken(TN_INTEGER);
}

/*----------------------------------------------------------------------------*/
/* collect other cpp token                                                    */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  ASSERT(bStartCppOther());
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather ]] as a token
  if (m_xAnnChar.bIsCloseBracket()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsCloseBracket()) {
      *z++ = ']';
      *z++ = ']';
      *z = '\0';
      m_pCurrTkn->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as ]
      m_xAnnChar.m_cChar = ']';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather :: as a string
  if (m_xAnnChar.bIsColon()) {

    vGetAnnCharStripped();

    if (m_xAnnChar.bIsColon()) {
      *z++ = ':';
      *z++ = ':';
      *z = '\0';
      m_pCurrTkn->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();

      // set last single character as :
      m_xAnnChar.m_cChar = ':';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather .. as a string
  if (m_xAnnChar.bIsDot()) {
    
    vGetAnnCharStripped();
    
    if (m_xAnnChar.bIsDot()) {
      *z++ = '.';
      *z++ = '.';
      *z = '\0';
      m_pCurrTkn->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as .
      m_xAnnChar.m_cChar = '.';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather == as a token
  if (m_xAnnChar.bIsEqual()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsEqual()) {
      *z++ = '=';
      *z++ = '=';
      *z = '\0';
      m_pCurrTkn->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as =
      m_xAnnChar.m_cChar = '=';
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather >= as a token
  if (m_xAnnChar.bIsGreaterThan()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsEqual()) {
      *z++ = '>';
      *z++ = '=';
      *z = '\0';
      m_pCurrTkn->vToken(TN_OTHER);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as >
      m_xAnnChar.m_cChar = '>';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather <= as a token
  if (m_xAnnChar.bIsLessThan()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsEqual()) {
      *z++ = '<';
      *z++ = '=';
      *z = '\0';
      m_pCurrTkn->vToken(TN_OTHER);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as <
      m_xAnnChar.m_cChar = '<';
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather <> as a token
  if (m_xAnnChar.bIsLessThan()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsGreaterThan()) {
      *z++ = '<';
      *z++ = '>';
      *z = '\0';
      m_pCurrTkn->vToken(TN_OTHER);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();

      // set last single character as <
      m_xAnnChar.m_cChar = '<';
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather != as a token
  if (m_xAnnChar.bIsExclamation()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsEqual()) {
      *z++ = '!';
      *z++ = '=';
      *z = '\0';
      m_pCurrTkn->vToken(TN_OTHER);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();

      // set last single character as !
      m_xAnnChar.m_cChar = '!';
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather [[ as a token
  if (m_xAnnChar.bIsOpenBracket()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsOpenBracket()) {
      *z++ = '[';
      *z++ = '[';
      *z = '\0';
      m_pCurrTkn->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as [
      m_xAnnChar.m_cChar = '[';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather || as a string
  if (m_xAnnChar.bIsPipe()) {
    
    vGetAnnCharStripped();
    
    if (m_xAnnChar.bIsPipe()) {
      *z++ = '|';
      *z++ = '|';
      *z = '\0';
      m_pCurrTkn->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as |
      m_xAnnChar.m_cChar = '|';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather *| as a string
  if (m_xAnnChar.bIsAsterisk()) {
    
    vGetAnnCharStripped();
    
    if (m_xAnnChar.bIsPipe()) {
      *z++ = '*';
      *z++ = '|';
      *z = '\0';
      m_pCurrTkn->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as *
      m_xAnnChar.m_cChar = '*';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather |# as a string
  if (m_xAnnChar.bIsPipe()) {
    
    vGetAnnCharStripped();
    
    if (m_xAnnChar.bIsHash()) {
      *z++ = '|';
      *z++ = '#';
      *z = '\0';
      m_pCurrTkn->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as |
      m_xAnnChar.m_cChar = '|';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect a single character
  *z++ = m_xAnnChar.m_cChar;
  
  // terminate the other token
  *z = '\0';
  m_pCurrTkn->vToken(TN_OTHER);
}

/*----------------------------------------------------------------------------*/
/* collect normal cpp string literal                                          */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  ASSERT(bStartSenString());
    
  // skip the leading '"'
  vGetAnnCharStripped();
  
  // collect the characters within the string
  while ( ! bEndCppString() ) {

//    if (m_xAnnChar.bIsNewLine()) {
//      *z = '\0';
//      m_pCurrTkn->vToken(TN_INVALID);
//      return;
//    }

    if (m_xAnnChar.bIsBackSlash()) {
      vGetAnnCharStripped();
      // translate \n into new line
      if ('n' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\n';
      else if ('r' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\r';
    }
    
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // terminate the string literal
  *z = '\0';
  
  m_pCurrTkn->vToken(TN_STRING);
}

//
// collect sen tokens //////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* collect Sen token                                                          */
/*----------------------------------------------------------------------------*/
void Input::vCollectSen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(UF_SEN_TOKENS == m_eTknForm);
  
  char* azTermCrs = m_pCurrTkn->azChars();
  UNUSED(azTermCrs);

  m_pCurrTkn->vReset();

  vGetAnnCharStripped();
  
  vSkipWhitespace();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vTokenCollectBegin();
  // Note: We are removing vTokenCollectBegin(), which would get a pointer into
  // the TokenVecs byte array so it could write the token string directly into it.
  // While this would be quite performant, it means that the byte array must be
  // big enough for all supported inputs so as not to overflow.
  // Instead we will collect it into a string on the stack and then copy it to
  // the TokenVecs byte array when we append the token.
  s1M sTokenStr;
  m_pCurrTkn->vLineNo(m_xAnnChar.m_iLine);
  m_pCurrTkn->vColNo(m_xAnnChar.m_iCol);
  // The implementation below will use the string pointer from here, which points
  // to the stack string.
  m_pCurrTkn->vChars(sTokenStr.szStz());

  // end of input?
  if (bHitEOF()) {
    m_pCurrTkn->vSetNull();
  }

  // collect integer literal?
  else
  if (bStartSenInteger()) {
    vCollectSenNumber();
  }
  
  // collect string literal?
  else
  if (bStartSenString()) {
    vCollectSenString();
  }

  // collect sen tag?
  else
  if (bStartSenTag()) {
    vCollectSenTag();
  }
  
  // collect name token
  else {
    vCollectSenName();
  }
  
  //vTokenCollectEnd();
  // Copy the string that we've collected on the stack into the TokenVec bytespace.
  m_pInpTknVec->szCopyStrForLastToken(m_pCurrTkn, sTokenStr.szStz());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // cross check valid SEN tokens
  switch (m_pCurrTkn->eToken()) {
      
    // the expected tokens
    case TN_BOOLEAN:
    case TN_DECIMAL:
    case TN_FLOAT:
    case TN_INTEGER:
    case TN_NAME:
    case TN_TAG:
//      if (m_pCurrTkn->szChars()->bSame(""_Stz)) {
//        vInspect();
//        BASE::vDebugPoint();
//      }
      break;
      
    case TN_NULL:
    case TN_STRING:
      break;

    // unexpected tokens
    case TN_OTHER:
      NEVER_REACH;
      
    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* collect Sen name                                                           */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  //ASSERT(bStartSenName());
  
  // collect the characters of the name
  while ( m_xAnnChar.m_cChar && !m_xAnnChar.bIsWhite() && !bStartSenString() ) {
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vUngetAnnCharStripped();
  
  // terminate a namelit
  *z = '\0';

  if (StrLib::bIsBoolean(m_pCurrTkn->azChars()))
    m_pCurrTkn->vToken(TN_BOOLEAN);
  else
    m_pCurrTkn->vToken(TN_NAME);
}

/*----------------------------------------------------------------------------*/
/* collect Sen number (integer, decimal, float)                               */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenNumber(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  i8 nIntegerDigits  = 0;
//  i8 nFractionDigits = 0;
  i8 nExponentDigits = 0;

  // we have seen a minus or a digit
  ASSERT(bStartSenInteger());
  
  // by default we are collectin an integer
  m_pCurrTkn->vToken(TN_INTEGER);

  // - start parsing an integer
  
  // collect leading sign
  if (m_xAnnChar.bIsSign()) {
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // collect initial digits of an integer
  while (m_xAnnChar.bIsDigit()) {
    *z++ = m_xAnnChar.m_cChar;
    nIntegerDigits++;
    vGetAnnCharStripped();
  }
  *z = '\0';

  // 0x hex number?
  if (1 == nIntegerDigits && m_xAnnChar.bIsX()) {

    // add the 'x'
    *z++ = m_xAnnChar.m_cChar;
    
    // collect the hex digits
    vGetAnnCharStripped();
    while (m_xAnnChar.bIsHex()) {
      *z++ = m_xAnnChar.m_cChar;
      nIntegerDigits++;
      vGetAnnCharStripped();
    }
    *z = '\0';
  }
  
  
  // - handle minus only
  if (! nIntegerDigits && ! m_xAnnChar.bIsDot()) {
    vCollectSenName();
    return;
  }

  // . handle dot only
  if ( ! nIntegerDigits && m_xAnnChar.bIsDot()) {
    AnnChar xDot = m_xAnnChar;

    // collect next char
    vGetAnnCharStripped();

    if ( ! m_xAnnChar.bIsDigit() ) {
      vUngetAnnCharStripped();
      m_xAnnChar = xDot;
      vCollectSenName();
      return;
    }
    vUngetAnnCharStripped();
    m_xAnnChar = xDot;
  }

  // dot indicating fraction digits of decimal number follow
  if (m_xAnnChar.bIsDot()) {

    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();

    // collect digits after decimal
    while (m_xAnnChar.bIsDigit()) {
      *z++ = m_xAnnChar.m_cChar;
//      nFractionDigits++;
      vGetAnnCharStripped();
    }


    // terminate the decimal literal
    *z = '\0';
    
    m_pCurrTkn->vToken(TN_DECIMAL);
  }
  
  
  // an e|E indicating a float number ?
  if ('e' == m_xAnnChar.m_cChar || 'E' == m_xAnnChar.m_cChar) {
    
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();

    // collect optional sign after E
    if (m_xAnnChar.bIsSign()) {
      *z++ = m_xAnnChar.m_cChar;
      vGetAnnCharStripped();
    }


    // collect digits after E
    while (m_xAnnChar.bIsDigit()) {
      *z++ = m_xAnnChar.m_cChar;
      nExponentDigits++;
      vGetAnnCharStripped();
    }

    // terminate the float literal
    *z = '\0';
    
    if (! nExponentDigits) {
      vCollectSenName();
      return;
    }
    
    m_pCurrTkn->vToken(TN_FLOAT);
    
    //m_pCurrTkn->vShow();
  }

  
  
  // - handle the case where we have a name like 1_ugh
  
  // just a number?
  if (m_xAnnChar.bIsNull() || m_xAnnChar.bIsWhite() || bStartSenString() ) {
    
    // push back the unconsumed char
    vUngetAnnCharStripped();
    
    // terminate
    *z = '\0';
    return;
  }
  
  // its actually a name; complete parsing name
  vCollectSenName();
}

/*----------------------------------------------------------------------------*/
/* collect Sen string                                                         */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  ASSERT(bStartSenString());
    
  char cStartQuote = m_xAnnChar.m_cChar;
  
  // skip the leading '"' or '\''
  vGetAnnChar();
  
  // collect the chars in the string
  //while ( ! bEndSenString() ) {
  while ( m_xAnnChar.m_cChar != cStartQuote ) {
  
    //vTrace(">>%c<<\n"_Stz,m_xAnnChar.m_cChar);
        
    if (m_xAnnChar.bIsBackSlash()) {
      vGetAnnChar();
      // translate \n into new line
      if ('n' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\n';
    }    
    
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnChar();
  }
 
  // terminate the strlit
  *z = '\0';

  m_pCurrTkn->vToken(TN_STRING);
}

/*----------------------------------------------------------------------------*/
/* collect Sen tag                                                            */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::azNullTerminator(m_pCurrTkn->azChars());
  char *z = azChars;

  ASSERT(bStartSenTag());
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! m_xAnnChar.m_cChar || m_xAnnChar.bIsWhite() || bStartSenString() ) {
    // push back the unconsumed char
    vUngetAnnCharStripped();
    
    // terminate a namelit
    *z = '\0';

    m_pCurrTkn->vToken(TN_NAME);
    
    return;
  }
     
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect the characters of the name
  
  while ( m_xAnnChar.m_cChar && !m_xAnnChar.bIsWhite() && !bStartSenString() ) {
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vUngetAnnCharStripped();
  
  // terminate a namelit
  *z = '\0';

  m_pCurrTkn->vToken(TN_TAG);
}


//
// character processing ////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void Input::vSkipWhitespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  while (bHaveWhitespace())
    vGetAnnCharStripped();
}

/*----------------------------------------------------------------------------*/
/* have whitespace                                                            */
/*----------------------------------------------------------------------------*/
bool Input::bHaveWhitespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  // do we have a current parsed character, which is a white char?
  if (m_xAnnChar.m_cChar) {
    if (StrLib::bIsWhite(m_xAnnChar.m_cChar))
      return true;
    else
      return false;
  }

  // - no current parsed character
  
  // a '\0' in line mode indicates end of line; not considered whitespace
  if (bSingleLineCmd())
    return false;
  
  // a '\0' at eof is the eof marker; not considered whitespace
  if (bSeenEOF())
    return false;

  // a '\0' is otherwise whitespace
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal number                                            */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppNumber(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (StrLib::bIsDigit(m_xAnnChar.m_cChar))
    return true;
  if (StrLib::bIsDot(m_xAnnChar.m_cChar))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of quoted literal string                                    */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppLiteral(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  if (m_xAnnChar.bIsSingleQuote())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if ('$' == m_xAnnChar.m_cChar)
    return true;
  
  if (StrLib::bIsNameStartChar(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal other                                             */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (! StrLib::bIsNameBodyChar(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of quoted literal string                                      */
/*----------------------------------------------------------------------------*/
bool Input::bEndCppLiteral(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  if (m_xAnnChar.bIsSingleQuote())
    return true;

  //if (bFormCppTokens() && m_xAnnChar.bIsNewLine())
  //  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Input::bEndCppString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;

  //if (bFormCppTokens() && m_xAnnChar.bIsNewLine())
  //  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen integer                                              */
/*----------------------------------------------------------------------------*/
bool Input::bStartSenInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsMinus() || StrLib::bIsDigit(m_xAnnChar.m_cChar) || m_xAnnChar.bIsDot())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen name                                                 */
/*----------------------------------------------------------------------------*/
bool Input::bStartSenName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (bStartSenInteger() || bStartSenString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen string                                               */
/*----------------------------------------------------------------------------*/
bool Input::bStartSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;
  
// 2020-Feb-26 RJH: I commented this out as I don't think we have 'string'
//                  in sen literals
  if (m_xAnnChar.bIsSingleQuote())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen tag                                                  */
/*----------------------------------------------------------------------------*/
bool Input::bStartSenTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  //if (m_xAnnChar.bIsTilde())
  if (m_xAnnChar.bIsLessThan())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of Sen string                                                 */
/*----------------------------------------------------------------------------*/
bool Input::bEndSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;

// 2020-Feb-26 RJH: I commented this out as I don't think we have 'string'
//                  in sen literals
  if (m_xAnnChar.bIsSingleQuote())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get next character from parser input                                       */
/*----------------------------------------------------------------------------*/
void Input::vGetAnnChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vGetAnnChar(m_xAnnChar,false);
  m_xAnnChar.m_cChar = m_xAnnChar.m_cChar;
}

/*----------------------------------------------------------------------------*/
/* get next input character (stripped of comments)                            */
/*----------------------------------------------------------------------------*/
void Input::vGetAnnCharStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vGetAnnChar(m_xAnnChar,true);
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Input::vUngetAnnCharStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUngetAnnChar(m_xAnnChar);
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool Input::bHitEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsNull())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* reset the token vector and current token cursor pair                       */
/*----------------------------------------------------------------------------*/
bool Input::bCursorAdvance(eEC& eError)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_INPUT, _FOOTER_);
  UNUSED(eError);

  m_pInpTknVec->vAdvanceCurr();
  m_pCurrTkn = m_pInpTknVec->pCurrToken();

  return true;
}

/*----------------------------------------------------------------------------*/
/* reset the token vector and current token cursor pair                       */
/*----------------------------------------------------------------------------*/
void Input::vCursorReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pInpTknVec->vSoftReset();
  m_pCurrTkn = nullptr;
}

/*----------------------------------------------------------------------------*/
/* reset the current token vector and move back to prior token                */
/*----------------------------------------------------------------------------*/
void Input::vCursorRewind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Token* pTkn = m_pCurrTkn;
  
  m_pInpTknVec->vRewindCurr();
  
  pTkn->vReset();
    
  m_pCurrTkn = m_pInpTknVec->pCurrToken();
}

/*----------------------------------------------------------------------------*/
/* revert to cpp tokens                                                       */
/*----------------------------------------------------------------------------*/
void Input::vTestRevertToCpp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  if (m_bRevertToCpp) {
    m_bRevertToCpp = false;
    vFormCppTokens();
  }
}

/*----------------------------------------------------------------------------*/
/* switch to cpp tokens                                                       */
/*----------------------------------------------------------------------------*/
void Input::vTestSwitchToSen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  if (m_pInpTknVec->pFirstToken() == m_pCurrTkn && m_pCurrTkn->bIsPipeline()) {
    m_bRevertToCpp = true;
    vFormSenTokens();
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
