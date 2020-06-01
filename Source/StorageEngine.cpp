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
  Alignment::vCheckAssumptions();
  Arguments::vCheckAssumptions();
  Assembler::vCheckAssumptions();
  Base::vCheckAssumptions();
  Chunks::vCheckAssumptions();
  ClassData::vCheckAssumptions();
  CompactBuffers::vCheckAssumptions();
  CompilerStream::vCheckAssumptions();
  ControlData::vCheckAssumptions();
  Correctness::vCheckAssumptions();
  DataBuffers::vCheckAssumptions();
  DynamoCompiler::vCheckAssumptions();
  Entries::vCheckAssumptions();
  Enums::vCheckAssumptions();
  Esses::vCheckAssumptions();
  Focals::vCheckAssumptions();
  Gadgets::vCheckAssumptions();
  GoldenGate::vCheckAssumptions();
  Grids::vCheckAssumptions();
  Host::vCheckAssumptions();
  Indexing::vCheckAssumptions();
  InfoData::vCheckAssumptions();
  Interface::vCheckAssumptions();
  Jobs::vCheckAssumptions();
  LexicalAnalysis::vCheckAssumptions();
  Library::vCheckAssumptions();
  Locking::vCheckAssumptions();
  MetaData::vCheckAssumptions();
  Metrics::vCheckAssumptions();
  Operands::vCheckAssumptions();
  Partitioning::vCheckAssumptions();
  Persistence::vCheckAssumptions();
  Pipelines::vCheckAssumptions();
  PipelineSpecs::vCheckAssumptions();
  Plans::vCheckAssumptions();
  Pool::vCheckAssumptions();
  //Prompt::vCheckAssumptions();
   Reflection::vCheckAssumptions();
  Resources::vCheckAssumptions();
  SenVM::vCheckAssumptions();
  Sessions::vCheckAssumptions();
  SharedMemory::vCheckAssumptions();
  SortBuffers::vCheckAssumptions();
  Steps::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
  Strings::vCheckAssumptions();
  Structures::vCheckAssumptions();
  Tasks::vCheckAssumptions();
  Threads::vCheckAssumptions();
  Transactions::vCheckAssumptions();
  UpperLoad::vCheckAssumptions();
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
