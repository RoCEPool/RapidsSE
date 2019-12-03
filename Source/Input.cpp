/*============================================================================*/
/* Input.cpp -- Input Processor                                               */
/*============================================================================*/

#include "TokenStream.h"
#include "AM.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Input* Commands::_pInput(void* aAddr)
{
  Input* pInput = reinterpret_cast<Input*>(aAddr);
  
  RASSERT_TYPE(pInput,CN_INPUT);
  
  return pInput;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Input::operator new(size_t)
{
  Object* pObject = HeapMem::pAllocate(CN_INPUT);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Input::operator delete(void* aAddr)
{
  Input* pInput = reinterpret_cast<Input*>(aAddr);
  
  HeapMem::vFree(pInput);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Input::Input(void)
: Object(CN_INPUT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  

  m_pTokeniser = new Tokeniser;
  
  m_pTokeniser->vMalloc(false);
  m_pTokeniser->vPush();
  
  //if (gxArguments.m_bPrompt)
     m_pTokeniser->vUseStdin();
  
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vNormal(true);

    
  _checkInitialised_(CN_INPUT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Input::~Input(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  delete m_pTokeniser;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* potentially execute a command file                                         */
/*----------------------------------------------------------------------------*/
bool Input::bExecuteFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (! bTestFile(pTkn(0)->szText()))
    return false;
  
  bool b = bRunFile();
  //ASSERT(b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set file to run                                                            */
/*----------------------------------------------------------------------------*/
bool Input::bRunFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
//  if ( ! bTokenCheck(1) )
//    if (! pTkn(1)->bSame(";"_Stz))
//      return false;
  
  vPushFile(pTkn(0)->szText());
  m_pTokeniser->vLineMode(true);
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void Input::vPushFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  s256 sPath;
  
  sPath = aFileName;
  
  m_pTokeniser->vPush();
  
  bool b = m_pTokeniser->bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* check for a specific number of tokens                                      */
/*----------------------------------------------------------------------------*/
bool Input::bTokenCheck(i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (pTkn(nTokens)->bHasText()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool Input::bBlankLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (0 != pTkn(0)->iLength())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void Input::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  vGetTokens();
    
  // if empty line just return
  if ( ! pTkn(0)->bHasText() )
    return;
  
  // if its a command file just return
  if ( ! pTkn(0)->szText()->bSame("/"_Stz) && bTestFile(pTkn(0)->szText()))
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
/* get command tokens from input                                              */
/*----------------------------------------------------------------------------*/
void Input::vGetTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  while (true) {
    
    pTknVec()->vReset();
    
    for (;;) {
      
      Token* pToken = pTkn(pTknVec()->nTokens());
      
      eTN eToken = m_pTokeniser->eNextToken((char*)pToken->szText());
      
      if (TN_NONE == eToken)
        break;
      
      pToken->vToken(eToken);
      
      //      vTrace("vGetCommand : token %lld ]%s[\n"_Stz,
      //             pTknVec()->nTokens(),
      //             pTkn(pTknVec()->nTokens())->aText());
      
      pTknVec()->vIncTokens();
      ASSERT(pTknVec()->nTokens() < C_MAX_TOKENS);
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
bool Input::bTestFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (File::bExists(aFileName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* Input::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn = (*pTknVec)[i_Token];
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* Input::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  return m_pTokeniser->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* tokenize string                                                            */
/*----------------------------------------------------------------------------*/
void Input::vTokenise(Stz* aCommand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  s48K sCommand(aCommand);
  
  m_pTokeniser->vPush();
  m_pTokeniser->vUseString(sCommand);
  
  vGetCommand();
  m_pTokeniser->bPop();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Input);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
