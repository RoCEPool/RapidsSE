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
  CompactBuffers::vCheckAssumptions();
  Expressions::vCheckAssumptions();
  ControlData::vCheckAssumptions();
  DataBuffers::vCheckAssumptions();
  DynamoCompiler::vCheckAssumptions();
  Entries::vCheckAssumptions();
  Facts::vCheckAssumptions();
  Resources::vCheckAssumptions();
  Grids::vCheckAssumptions();
  Indexing::vCheckAssumptions();
  InfoData::vCheckAssumptions();
  Interface::vCheckAssumptions();
  Jobs::vCheckAssumptions();
  LexicalAnalysis::vCheckAssumptions();
  Load::vCheckAssumptions();
  MetaData::vCheckAssumptions();
  Metrics::vCheckAssumptions();
  Operands::vCheckAssumptions();
  Parser::vCheckAssumptions();
  Partitioning::vCheckAssumptions();
  Pipelines::vCheckAssumptions();
  PipelineSpecs::vCheckAssumptions();
  Plans::vCheckAssumptions();
  //Prompt::vCheckAssumptions();
  Resources::vCheckAssumptions();
  SenVM::vCheckAssumptions();
  Sessions::vCheckAssumptions();
  SortBuffers::vCheckAssumptions();
  SqlValues::vCheckAssumptions();
  Steps::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
  Tasking::vCheckAssumptions();
  Transactions::vCheckAssumptions();
  Utilities::vCheckAssumptions();

  // third party??

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
