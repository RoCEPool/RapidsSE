/*============================================================================*/
/* Command.cpp - Command Processor                                   */
/*============================================================================*/

#include "Commands.h"
#include "StorageEngine.h"

extern Arguments   gxArguments;
extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Command* Commands::_pCommand(void* aAddr)
{
  Command* pCommand = reinterpret_cast<Command*>(aAddr);
  
  RASSERT_TYPE(pCommand,CN_COMMAND);
  
  return pCommand;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Command::operator new(size_t)
{
  Object* pObject = HeapMem::pAllocate(CN_COMMAND);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Command::operator delete(void* aAddr)
{
  Command* pCommand = reinterpret_cast<Command*>(aAddr);
  
  HeapMem::vFree(pCommand);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Command::Command(void)
: Object(CN_COMMAND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  

  m_pTokeniser = new Tokeniser;
  
  m_pTokeniser->vMalloc(false);
  m_pTokeniser->vPush();
  
  //if (gxArguments.m_bPrompt)
     m_pTokeniser->vUseStdin();
  
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vNormal(true);

    
  _checkInitialised_(CN_COMMAND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Command::~Command(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  delete m_pTokeniser;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* potentially execute a command file                                         */
/*----------------------------------------------------------------------------*/
bool Command::bExecuteFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  if (! bTestFile(pTkn(0)->aText()))
    return false;
  
  bool b = bRunFile();
  //ASSERT(b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set file to run                                                            */
/*----------------------------------------------------------------------------*/
bool Command::bRunFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
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
void Command::vPushFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  s256 sPath;
  
  sPath = aFileName;
  
  m_pTokeniser->vPush();
  
  bool b = m_pTokeniser->bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* check for a specific number of tokens                                      */
/*----------------------------------------------------------------------------*/
bool Command::bTokenCheck(i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  if (pTkn(nTokens)->bHasText()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool Command::bBlankLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  if (0 != pTkn(0)->iLength())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void Command::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
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
void Command::vGetCommandArgument(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  s48K sCommand(gxArguments.m_sCommand);
  
  m_pTokeniser->vUseString(sCommand);
  
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
void Command::vGetCommandInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
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
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool Command::bTestFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  if (File::bExists(aFileName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* Command::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn = (*pTknVec)[i_Token];
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* Command::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  return m_pTokeniser->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* tokenize string                                                            */
/*----------------------------------------------------------------------------*/
void Command::vTokenise(Stz* aCommand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMAND,_FOOTER_);
  
  s48K sCommand(aCommand);
  
  m_pTokeniser->vPush();
  m_pTokeniser->vUseString(sCommand);
  
  vGetCommand();
  m_pTokeniser->bPop();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Command);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
