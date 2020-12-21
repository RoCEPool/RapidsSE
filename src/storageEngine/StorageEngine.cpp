/*============================================================================*/
/* StorageEngine.cpp - StorageEngine namespace functions                      */
/*============================================================================*/

#include "StorageEngine.h"

extern Args       gxArgs;
extern SizeHeap   gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* check assumptions                                                          */
/*----------------------------------------------------------------------------*/
void StorageEngine::vCheckAllAssumptions(void)
{
  Arguments::vCheckAssumptions();
  Chunks::vCheckAssumptions();
  Expressions::vCheckAssumptions();
  ControlData::vCheckAssumptions();
  DataBuffers::vCheckAssumptions();
  Resources::vCheckAssumptions();
  Interface::vCheckAssumptions();
  LexicalAnalysis::vCheckAssumptions();
  MetaData::vCheckAssumptions();
  Metrics::vCheckAssumptions();
  Parser::vCheckAssumptions();
  Plans::vCheckAssumptions();
  Resources::vCheckAssumptions();
  SenVM::vCheckAssumptions();
  Sessions::vCheckAssumptions();
  Steps::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
  Tasking::vCheckAssumptions();
  Transactions::vCheckAssumptions();
  Utilities::vCheckAssumptions();
}

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void StorageEngine::vCheckAssumptions(void)
{
  ALWAYS(sizeof(AttachMap) < C_SIZE_POOL_BUFFER);
}

/*----------------------------------------------------------------------------*/
/* set signal Scheduler                                                        */
/*----------------------------------------------------------------------------*/
void StorageEngine::vSetSignalHandlers(void)
{
  // Segmentation Violation
  signal(SIGSEGV, Engine::vSignalHandler);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
