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
  CompactBuffers::vCheckAssumptions();
  CompilerStream::vCheckAssumptions();
  ControlData::vCheckAssumptions();
  Correctness::vCheckAssumptions();
  DataBuffers::vCheckAssumptions();
  Entries::vCheckAssumptions();
  Enums::vCheckAssumptions();
  Esses::vCheckAssumptions();
  Focals::vCheckAssumptions();
  Gadgets::vCheckAssumptions();
  Grids::vCheckAssumptions();
  Host::vCheckAssumptions();
  IndexTrees::vCheckAssumptions();
  Interface::vCheckAssumptions();
  Jobs::vCheckAssumptions();
  Library::vCheckAssumptions();
  Locking::vCheckAssumptions();
  MetaData::vCheckAssumptions();
  Operators::vCheckAssumptions();
  Partitioning::vCheckAssumptions();
  Persistence::vCheckAssumptions();
  Pipelines::vCheckAssumptions();
  Plans::vCheckAssumptions();
  Pool::vCheckAssumptions();
  Prompt::vCheckAssumptions();
  Registers::vCheckAssumptions();
  Resources::vCheckAssumptions();
  Scheduling::vCheckAssumptions();
  SenVM::vCheckAssumptions();
  Services::vCheckAssumptions();
  Sessions::vCheckAssumptions();
  SortBuffers::vCheckAssumptions();
  Steps::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
  Operands::vCheckAssumptions();
  Strings::vCheckAssumptions();
  Structures::vCheckAssumptions();
  Tasks::vCheckAssumptions();
  Threads::vCheckAssumptions();
  TokenStream::vCheckAssumptions();
  Transactions::vCheckAssumptions();
  Utilities::vCheckAssumptions();

  // third party??

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
