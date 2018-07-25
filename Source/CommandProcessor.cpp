/*============================================================================*/
/* CommandProcessor.cpp - Command Processor                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern Arguments   gxArguments;
extern AttachMap*  gpAttachMap;
extern JobCatalog  gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CommandProcessor* Interface::_pCommandProcessor(void* aAddr)
{
  CommandProcessor* pCommandProcessor = reinterpret_cast<CommandProcessor*>(aAddr);
  
  RASSERT( !pCommandProcessor || CN_COMMANDPROCESSOR == pCommandProcessor->eClass() );
  
  return pCommandProcessor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* CommandProcessor::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Object* pObject = Heap::pAllocate(CN_COMMANDPROCESSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void CommandProcessor::operator delete(void* aAddr)
{
  CommandProcessor* pCommandProcessor = reinterpret_cast<CommandProcessor*>(aAddr);
  
  Heap::vFree(pCommandProcessor);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CommandProcessor::CommandProcessor(void)
: Object(CN_COMMANDPROCESSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  m_sPrompt.vSprint("[N%lld %lld] se %s%c%s> "_Stz,
                    gpAttachMap->m_pGrid->iThisNode(),
                    Process::iGetPid(),
                    RAPIDSSE_VERSION_STRING,
#ifdef _DEBUG
                    'd',
#else
                    'r',
#endif
                    gxArguments.m_bPromptR35 ? " R35"_Stz : ""_Stz
                   );

  m_pTokeniser = new Tokeniser;
  
  m_pTokeniser->vMalloc(false);
  m_pTokeniser->vPush();
  
  if (gxArguments.m_bCommand) {
    m_pTokeniser->vUseString(gxArguments.m_sCommand);
  }
  else if (gxArguments.m_bPromptR35 || gxArguments.m_bPrompt || gxArguments.m_bScript) {
    m_pTokeniser->vPrompt(m_sPrompt);
    m_pTokeniser->vUseStdin();
  }
  
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vNormal(true);

  
  m_pJobClass = nullptr;
  m_pJob = nullptr;

  m_pPrompt = nullptr;
  m_pSEitf = nullptr;
  
  _checkInitialised_(CN_COMMANDPROCESSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CommandProcessor::~CommandProcessor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  delete m_pTokeniser;
}


/*----------------------------------------------------------------------------*/
/* potentially execute a command file                                         */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bExecuteFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  if (! bTestFile(pTkn(0)->aText()))
    return false;
  
  bool b = bRunFile();
  ASSERT(b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set file to run                                                            */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bRunFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  if ( ! bTokenCheck(1) )
    return false;
  
  vPushFile(pTkn(0)->aText());
  m_pTokeniser->vLineMode(true);
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vPushFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  s256 sPath;
  
  sPath = aFileName;
  
  m_pTokeniser->vPush();
  
  bool b = m_pTokeniser->bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* check for a specific number of tokens                                      */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bTokenCheck(i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  if (pTkn(nTokens)->bHasText()) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}






/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bBlankLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  if (0 != pTkn(0)->iLength())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vCollectStatement(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(pTkn(0)->bHasText());
  
  TokenVec* pTokenVec = pTknVec();
  
  // have we already seen the ';'
  i8 i_Token = m_pTokeniser->iFindToken(";"_Stz,pTokenVec,C_TOKENS);
  
  if (i_Token >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Stz,m_xTokenVec,C_TOKENS);
    return;
  }
  
  i_Token = m_pTokeniser->iCountTokens(pTokenVec,C_TOKENS);
  
  // switch to terminal mode
  m_pTokeniser->vLineMode(false);
  
  s32 sContinuation("  > "_Stz);
  m_pTokeniser->vPrompt(sContinuation);
  
  // get tokens delimited by ';'
  for ( ; i_Token < C_TOKENS; i_Token++) {
    
    Token* pToken = pTkn(i_Token);
    eTN eToken = m_pTokeniser->eNextToken((char*)pToken->aText());
    ASSERT(TN_NONE != eToken);
    pToken->vToken(eToken);
    
    // look for terminal
    if (TN_OTHER == eToken && pToken->bSame(";"_Stz))
      break;
  }
  
  // switch back to line mode
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Command : "_Stz,m_xTokenVec,C_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* dispatch job into SE                                                       */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bDispatchJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  Timer xTimer;
  
  xTimer.vStart();
  
  pTknVec()->vSetTokens();
  
  //! make this part of verbose??
  //m_pCommandProcessor->m_pTokeniser->vDumpTokens("Command : "_Stz,m_xTokenVec,C_TOKENS);
  
  bool bResult = RapidsSE_DispatchJob(m_pJob,m_pSEitf);
  
  if (pTknVec()->bDetachedRequest())
    return bResult;
  
  xTimer.vUpdate();
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* end a job                                                                  */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vEndJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  // call the job post function (includes deleting the job)
  m_pJobClass->vEndJob(m_pSEitf,m_pJob);
  
  m_pJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt to find job class for command                                      */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bFindJobClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  m_pJobClass =
    gxJobCatalog.pFindJob(pTkn(0)->aText(), pTkn(1)->aText());
  
  return m_pJobClass ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  if (gxArguments.m_bCommand)
    vGetCommandArgument();
  else {
    vGetCommandInput();
    
    // if empty line just return
    if ( ! pTkn(0)->bHasText() )
      return;
    
    // if its a command file just return
    if (bTestFile(pTkn(0)->aText()))
      return;
    
    // collect till ;
    vCollectStatement();
    
    // stip off ;
    pTknVec()->vStripSemicolon();
    
    // stip off &
    pTknVec()->vStripAmpersand();
    
    // set no of tokens in token vector
    pTknVec()->vSetTokens();
  }
}

/*----------------------------------------------------------------------------*/
/* get command from arguments                                                 */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vGetCommandArgument(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  pTknVec()->vReset();
  
  while (TN_NONE !=
         m_pTokeniser->eNextToken((char*)pTkn(pTknVec()->nTokens())->aText())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Stz,i_Token,pTkn(i_Token));
    
    pTknVec()->vIncTokens();
    ASSERT(pTknVec()->nTokens() < C_TOKENS);
  }
}

/*----------------------------------------------------------------------------*/
/* get command from input                                                     */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vGetCommandInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  while (true) {
    
    pTknVec()->vReset();
    pTknVec2()->vReset();
    
    for (;;) {
      
      Token* pToken = pTkn(pTknVec()->nTokens());
      
      eTN eToken = m_pTokeniser->eNextToken((char*)pToken->aText());
      
      if (TN_NONE == eToken)
        break;
      
      pToken->vToken(eToken);
      
      //      vTrace("vGetCommand : token %lld ]%s[\n"_Stz,
      //             pTknVec()->nTokens(),
      //             pTkn(pTknVec()->nTokens())->aText());
      
      pTknVec()->vIncTokens();
      ASSERT(pTknVec()->nTokens() < C_TOKENS);
    }
    
    // end of file? if so, pop and retry
    if (! pTknVec()->nTokens() && m_pTokeniser->bEOF()) {
      bool b = m_pTokeniser->bPop();
      ASSERT(b);
      continue;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* potentially execute a job                                                  */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vNewJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  m_pJob = m_pJobClass->pNewJob(m_pSEitf);
  
  m_pJob->m_bDetached = pTknVec()->bDetachedRequest();
}

/*----------------------------------------------------------------------------*/
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bTestFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  File xFile;
  
  if (xFile.bExists(aFileName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* CommandProcessor::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn = (*pTknVec)[i_Token];
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* CommandProcessor::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  return &m_xTokenVec;
}

/*----------------------------------------------------------------------------*/
/* get token vector 2                                                         */
/*----------------------------------------------------------------------------*/
TokenVec* CommandProcessor::pTknVec2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  return &m_xTokenVec2;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* CommandProcessor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
