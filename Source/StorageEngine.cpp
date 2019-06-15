/*============================================================================*/
/* StorageEngine.cpp - StorageEngine namespace functions                      */
/*============================================================================*/

#include "StorageEngine.h"

extern Arguments gxArguments;
extern eHT geWhichHeap[];
extern i8 giClassSize[];

/*----------------------------------------------------------------------------*/
/* check assumptions                                                          */
/*----------------------------------------------------------------------------*/
void StorageEngine::vCheckAllAssumptions(void)
{
  // this is in reverse order of the spaces in the project
  Base::vCheckAssumptions();
  Debugging::vCheckAssumptions();
  // third party??
  Correctness::vCheckAssumptions();
  Transform::vCheckAssumptions();
  Values::vCheckAssumptions();
  Assembler::vCheckAssumptions();
  Pool::vCheckAssumptions();
  Persistence::vCheckAssumptions();
  Structures::vCheckAssumptions();
  Chunks::vCheckAssumptions();
  Measurement::vCheckAssumptions();
  Maps::vCheckAssumptions();
  Transactions::vCheckAssumptions();
  Commands::vCheckAssumptions();
  Forth::vCheckAssumptions();
  MetaData::vCheckAssumptions();
  Data::vCheckAssumptions();
  Indexes::vCheckAssumptions();
  PushDown::vCheckAssumptions();
  Plans::vCheckAssumptions();
  Jobs::vCheckAssumptions();
  Utilities::vCheckAssumptions();
  Steps::vCheckAssumptions();
  Tasks::vCheckAssumptions();
  Threads::vCheckAssumptions();
  Grids::vCheckAssumptions();
  Interface::vCheckAssumptions();
  Prompt::vCheckAssumptions();
  Axioms::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
}

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void StorageEngine::vCheckAssumptions(void)
{
  
  ASSERT(sizeof(AttachMap) < C_SIZE_POOL_BUFFER);
  
  // No of entries in eCN , aCN and gpDump should match
  i8 iEntriesECN = CN_ENUMS;
  i8 iEntriesPCN = 0;
  i8 iEntriesGPDUMP = 0;
  i8 iEntriesGCLASSSIZE = 0;
  i8 iEntriesGPFINALIZE = 0;
  
  bool bFoundEnum = false;
  
  // find number of entries in aCN
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (aCN[i_Class]->bEqual("ENUMS"_Stz)) {
      bFoundEnum = true;
      break;
    }
    iEntriesPCN++;
  }
  
  if (!bFoundEnum)
    vFatal("Enum for CN_ENUM not found in aCN\n"_Stz);
    
  if (iEntriesECN != iEntriesPCN) {
    vFatal("\n!!!ALERT!!! No of entries in aCN (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesPCN, iEntriesECN);
  }
  
  bFoundEnum = false;
  // find number of entries in gpDump
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (gpDump[i_Class] == &Dump::vEnums) {
      bFoundEnum = true;
      break;
    }
    iEntriesGPDUMP++;
  }
  
  if (!bFoundEnum)
    vFatal("Enum for CN_ENUM not found in gpDump\n"_Stz);
    
  if (iEntriesECN != iEntriesGPDUMP) {
    vFatal("\n!!!ALERT!!! No of entries in gpDump (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesECN, iEntriesGPDUMP);
  }

  bFoundEnum = false;
  // find number of entries in giClassSize
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (giClassSize[i_Class] == -1) {
      bFoundEnum = true;
      break;
    }
    iEntriesGCLASSSIZE++;
  }
  
  if (!bFoundEnum)
    vFatal("Enum for CN_ENUM not found in giClassSize\n"_Stz);
  
  if (iEntriesECN != iEntriesGCLASSSIZE) {
    vFatal("\n!!!ALERT!!! No of entries in giClassSize (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesGCLASSSIZE, iEntriesECN);
  }
  
  bFoundEnum = false;
  // find number of entries in gpDump
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (gpFinalize[i_Class] == &Finalize::vEnums) {
      bFoundEnum = true;
      break;
    }
    iEntriesGPFINALIZE++;
  }
  
  if (!bFoundEnum)
    vFatal("Enum for CN_ENUM not found in gpDump\n"_Stz);
  
  if (iEntriesECN != iEntriesGPDUMP) {
    vFatal("\n!!!ALERT!!! No of entries in gpDump (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesECN, iEntriesGPDUMP);
  }

  
  // check sizes are less than pool block size
  for (i8 iClass = 0; iClass <= CN_ENUMS; iClass++) {
  
    eCN eClass = (eCN)iClass;
    i8  iClassSize = giClassSize[eClass];
    
    switch (geWhichHeap[eClass]) {
        
      case HT_INVALID:
        continue;
       
      case HT_PERSISTENT_SMALL:
      case HT_TRANSIENT_SMALL:
        assert(iClassSize < C_SIZE_SMALL_OBJECT);
        continue;
        
      case HT_PERSISTENT_MEDIUM:
      case HT_TRANSIENT_MEDIUM:
        assert(iClassSize < C_SIZE_MEDIUM_OBJECT);
        continue;
        
      case HT_PERSISTENT_LARGE:
      case HT_TRANSIENT_LARGE:
        assert(iClassSize < C_SIZE_LARGE_OBJECT);
        continue;
        
      case HT_PERSISTENT_JUMBO:
      case HT_TRANSIENT_JUMBO:
        assert(iClassSize < C_SIZE_JUMBO_OBJECT);
        continue;
        
      default:
        assert(0);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* set signal handlers                                                        */
/*----------------------------------------------------------------------------*/
void StorageEngine::vSetSignalHandlers(void)
{
  // Segmentation Violation
  signal(SIGSEGV, Engine::vSignalHandler);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
