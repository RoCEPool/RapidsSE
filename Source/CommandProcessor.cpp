/*============================================================================*/
/* CommandProcessor.cpp - Command Processor                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern Arguments   gxArguments;
extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CommandProcessor* Interface::_pCommandProcessor(void* aAddr)
{
  CommandProcessor* pCommandProcessor = reinterpret_cast<CommandProcessor*>(aAddr);
  
  RASSERT_TYPE(pCommandProcessor,CN_COMMANDPROCESSOR);
  
  return pCommandProcessor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* CommandProcessor::operator new(size_t)
{
  Object* pObject = HeapMem::pAllocate(CN_COMMANDPROCESSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void CommandProcessor::operator delete(void* aAddr)
{
  CommandProcessor* pCommandProcessor = reinterpret_cast<CommandProcessor*>(aAddr);
  
  HeapMem::vFree(pCommandProcessor);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CommandProcessor::CommandProcessor(void)
: Object(CN_COMMANDPROCESSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  

  m_pTokeniser = new Tokeniser;
  
  m_pTokeniser->vMalloc(false);
  m_pTokeniser->vPush();
  
  //if (gxArguments.m_bPrompt)
     m_pTokeniser->vUseStdin();
  
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vNormal(true);

  
  m_pJobClass = nullptr;
  m_pJob = nullptr;
  
  _checkInitialised_(CN_COMMANDPROCESSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CommandProcessor::~CommandProcessor(void) noexcept(false)
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
  //ASSERT(b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set file to run                                                            */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bRunFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
//  if ( ! bTokenCheck(1) )
//    if (! pTkn(1)->bSame(";"_Stz))
//      return false;
  
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
/* delete a job                                                               */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vDeleteJob(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  // call the job delete function
  m_pJobClass->vDeleteJob(pSession,m_pJob);
  
  m_pJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt to find job class for command                                      */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bFindJobClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  Stz* aCommand1 = pTkn(0)->aText();
  Stz* aCommand2 = pTkn(1)->aText();
  
  m_pJobClass = JobClass::pFind(aCommand1, aCommand2);
  
  return m_pJobClass ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  if (gxArguments.m_bCommand) {
    vGetCommandArgument();
    return;
  }
  else
    vGetCommandInput();
    
  // if empty line just return
  if ( ! pTkn(0)->bHasText() )
    return;
  
  // if its a command file just return
  if (bTestFile(pTkn(0)->aText()))
    return;
  
  // collect till ;
  m_pTokeniser->vCollectStatement();
  
  // stip off ;
  pTknVec()->vStripSemicolon();
  
  // stip off &
  pTknVec()->vStripAmpersand();
  
  // set no of tokens in token vector
  pTknVec()->vSetTokens();
}

/*----------------------------------------------------------------------------*/
/* get command from arguments                                                 */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vGetCommandArgument(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  m_pTokeniser->vUseString(gxArguments.m_sCommand);
  
  pTknVec()->vReset();
  
  //while (TN_NONE !=
  //       m_pTokeniser->eNextToken((char*)pTkn(pTknVec()->nTokens())->aText())) {
  
  while (true) {
    
    Token* pToken = pTkn(pTknVec()->nTokens());
    
    eTN eToken = m_pTokeniser->eNextToken((char*)pToken->aText());
    
    if (TN_NONE == eToken)
      break;
    
    pToken->vToken(eToken);

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
/* get job                                                                    */
/*----------------------------------------------------------------------------*/
Job* CommandProcessor::pJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  return m_pJob;
}

/*----------------------------------------------------------------------------*/
/* get job class                                                              */
/*----------------------------------------------------------------------------*/
JobClass* CommandProcessor::pJobClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  return m_pJobClass;
}

/*----------------------------------------------------------------------------*/
/* end a job                                                                  */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vPostJob(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  // call the job post function
  m_pJobClass->vPostJob(pSession,m_pJob);
}

/*----------------------------------------------------------------------------*/
/* potentially execute a job                                                  */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vPreJob(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  m_pJob = m_pJobClass->pPreJob(pSession);
}

/*----------------------------------------------------------------------------*/
/* spawn job in SE                                                            */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bSpawnJob(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  Timer xTimer;
  
  xTimer.vStart();
  
  pTknVec()->vSetTokens();
  
  //! make this part of verbose??
  //m_pCommandProcessor->m_pCompiler->m_pTokeniser->vDumpTokens("Command : "_Stz,m_xTokenVec,C_TOKENS);
  
  bool bResult = RapidsSE_bSpawnJob(m_pJob,SS_SEED_REQ,pSession);
  
  if (pTknVec()->bDetachedRequest())
    return bResult;
  
  xTimer.vUpdate();
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool CommandProcessor::bTestFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  if (File::bExists(aFileName))
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
  
  return m_pTokeniser->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* tokenize string                                                            */
/*----------------------------------------------------------------------------*/
void CommandProcessor::vTokenise(Stz* aCommand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPROCESSOR,_FOOTER_);
  
  s4K sCommand(aCommand);
  
  m_pTokeniser->vPush();
  m_pTokeniser->vUseString(sCommand);
  
  vGetCommand();
  m_pTokeniser->bPop();
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
