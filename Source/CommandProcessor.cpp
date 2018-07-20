/*============================================================================*/
/* CommandProcessor.cpp - Command Processor                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern AttachMap*    gpAttachMap;
extern Arguments     gxArguments;

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
