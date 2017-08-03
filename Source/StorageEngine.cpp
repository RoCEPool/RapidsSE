/*============================================================================*/
/* StorageEngine.cpp - storage engine functions                               */
/*============================================================================*/

#include "StorageEngine.h"

extern eHT geWhichHeap[];

/*----------------------------------------------------------------------------*/
/* check assumptions                                                          */
/*----------------------------------------------------------------------------*/
void StorageEngine::vCheckAllAssumptions(void)
{
  // this is in reverse order of the spaces in the project
  Base::vCheckAssumptions();
  Debugging::vCheckAssumptions();
  CorrectnessAlgebra::vCheckAssumptions();
  Datums::vCheckAssumptions();
  Values::vCheckAssumptions();
  System::vCheckAssumptions();
  Pool::vCheckAssumptions();
  Structures::vCheckAssumptions();
  Parsing::vCheckAssumptions();
  Transactions::vCheckAssumptions();
  Dictionaries::vCheckAssumptions();
  DBobjects::vCheckAssumptions();
  Plans::vCheckAssumptions();
  Tasks::vCheckAssumptions();
  Threads::vCheckAssumptions();
  GridModel::vCheckAssumptions();
  Interface::vCheckAssumptions();
  GridModel::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
}

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void StorageEngine::vCheckAssumptions(void)
{
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
    vFatal("Entry for CN_ENUM not found in aCN\n"_Stz);
    
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
    vFatal("Entry for CN_ENUM not found in gpDump\n"_Stz);
    
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
    vFatal("Entry for CN_ENUM not found in giClassSize\n"_Stz);
  
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
    vFatal("Entry for CN_ENUM not found in gpDump\n"_Stz);
  
  if (iEntriesECN != iEntriesGPDUMP) {
    vFatal("\n!!!ALERT!!! No of entries in gpDump (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesECN, iEntriesGPDUMP);
  }

  
  // check sizes are less than pool block size
  for (i8 iClass = 0; iClass <= CN_ENUMS; iClass++) {
  
    eCN eClass = (eCN)iClass;
    
    switch (geWhichHeap[eClass]) {
        
      case HT_INVALID:
        continue;
        
      case HT_SMALL:
        assert(giClassSize[eClass] < C_SIZE_SMALL_OBJECT);
        continue;
        
      case HT_MEDIUM:
        assert(giClassSize[eClass] < C_SIZE_MEDIUM_OBJECT);
        continue;
        
      case HT_LARGE:
        assert(giClassSize[eClass] < C_SIZE_LARGE_OBJECT);
        continue;
        
      case HT_JUMBO:
        assert(giClassSize[eClass] < C_SIZE_JUMBO_OBJECT);
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
  signal(SIGSEGV, RapidsSE::vSignalHandler);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
