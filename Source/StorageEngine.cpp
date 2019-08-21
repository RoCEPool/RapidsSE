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
  // in reverse order of the namespaces in the project
  Base::vCheckAssumptions();
  Assembler::vCheckAssumptions();
  // third party??
  Strings::vCheckAssumptions();
  Correctness::vCheckAssumptions();
  ClassData::vCheckAssumptions();
  Enums::vCheckAssumptions();
  Esses::vCheckAssumptions();
  Host::vCheckAssumptions();
  Locking::vCheckAssumptions();
  Alignment::vCheckAssumptions();
  Pool::vCheckAssumptions();
  Library::vCheckAssumptions();
  Gadgets::vCheckAssumptions();
  Resources::vCheckAssumptions();
  Persistence::vCheckAssumptions();
  Structures::vCheckAssumptions();
  Chunks::vCheckAssumptions();
  Transactions::vCheckAssumptions();
  Commands::vCheckAssumptions();
  Forth::vCheckAssumptions();
  Streams::vCheckAssumptions();
  MetaData::vCheckAssumptions();
  ControlData::vCheckAssumptions();
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
