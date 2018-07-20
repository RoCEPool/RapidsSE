/*============================================================================*/
/* CommandProcessor.cpp - Command Processor                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

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
  
  m_pTokeniser = new Tokeniser;

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
