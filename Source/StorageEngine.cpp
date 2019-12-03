/*============================================================================*/
/* StorageEngine.cpp - StorageEngine namespace functions                      */
/*============================================================================*/

#include "StorageEngine.h"

extern Arguments  gxArguments;
extern eHT        geWhichHeap[];
extern i8         giClassSize[];
extern pFINALIZE *gpFinalize[];

/*----------------------------------------------------------------------------*/
/* check assumptions                                                          */
/*----------------------------------------------------------------------------*/
void StorageEngine::vCheckAllAssumptions(void)
{
  Alignment::vCheckAssumptions();
  Assembler::vCheckAssumptions();
  Base::vCheckAssumptions();
  Chunks::vCheckAssumptions();
  ClassData::vCheckAssumptions();
  ControlData::vCheckAssumptions();
  Correctness::vCheckAssumptions();
  Enums::vCheckAssumptions();
  Esses::vCheckAssumptions();
  Focals::vCheckAssumptions();
  Gadgets::vCheckAssumptions();
  Host::vCheckAssumptions();
  Library::vCheckAssumptions();
  Locking::vCheckAssumptions();
  MetaData::vCheckAssumptions();
  Persistence::vCheckAssumptions();
  Pipelines::vCheckAssumptions();
  Pool::vCheckAssumptions();
  Registers::vCheckAssumptions();
  Resources::vCheckAssumptions();
  SenVM::vCheckAssumptions();
  Streams::vCheckAssumptions();
  Strings::vCheckAssumptions();
  Structures::vCheckAssumptions();
  TokenStream::vCheckAssumptions();
  Transactions::vCheckAssumptions();

  // third party??

  CompilerStream::vCheckAssumptions();
  Transform::vCheckAssumptions();
  ScanSprint::vCheckAssumptions();
  Rows::vCheckAssumptions();
  Indexes::vCheckAssumptions();
  Partitioning::vCheckAssumptions();
  Grids::vCheckAssumptions();
  Sessions::vCheckAssumptions();
  Transactions::vCheckAssumptions();
  Plans::vCheckAssumptions();
  Operators::vCheckAssumptions();
  CompilerOperators::vCheckAssumptions();
  DataOperators::vCheckAssumptions();
  Jobs::vCheckAssumptions();
  Tasks::vCheckAssumptions();
  Threads::vCheckAssumptions();
  Scheduling::vCheckAssumptions();
  Interface::vCheckAssumptions();
  Services::vCheckAssumptions();
  Utilities::vCheckAssumptions();
  Steps::vCheckAssumptions();
  Prompt::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
  
  
}

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void StorageEngine::vCheckAssumptions(void)
{
  ASSERT(sizeof(AttachMap) < C_SIZE_POOL_BUFFER);
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
