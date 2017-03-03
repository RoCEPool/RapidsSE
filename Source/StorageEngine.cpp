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
  Threading::vCheckAssumptions();
  Debugging::vCheckAssumptions();
  Magic::vCheckAssumptions();
  Datums::vCheckAssumptions();
  Values::vCheckAssumptions();
  Common::vCheckAssumptions();
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
  // No of entries in eCN , pCN and gpDump should match
  i8 iEntriesECN = CN_ENUMS;
  i8 iEntriesPCN = 0;
  i8 iEntriesGPDUMP = 0;
  i8 iEntriesGCLASSSIZE = 0;
  
  bool bFoundEnum = false;
  
  // find number of entries in pCN
  for (i8 i = 0; i <= CN_ENUMS; i++) {
    if (String::bEqualTo(pCN[i], "ENUMS")) {
      bFoundEnum = true;
      break;
    }
    iEntriesPCN++;
  }
  
  if (!bFoundEnum)
    vFatal("Entry for CN_ENUM not found in pCN\n");
    
  if (iEntriesECN != iEntriesPCN) {
    vFatal("\n!!!ALERT!!! No of entries in pCN (%lld) do not match the no in eCN (%lld)\n\n",
           iEntriesPCN, iEntriesECN);
  }
  
  bFoundEnum = false;
  // find number of entries in gpDump
  for (i8 i = 0; i <= CN_ENUMS; i++) {
    if (gpDump[i] == &Dump::vEnums) {
      bFoundEnum = true;
      break;
    }
    iEntriesGPDUMP++;
  }
  
  if (!bFoundEnum)
    vFatal("Entry for CN_ENUM not found in gpDump\n");
    
  if (iEntriesECN != iEntriesGPDUMP) {
    vFatal("\n!!!ALERT!!! No of entries in gpDump (%lld) do not match the no in eCN (%lld)\n\n",
           iEntriesECN, iEntriesGPDUMP);
  }

  bFoundEnum = false;
  // find number of entries in giClassSize
  for (i8 i = 0; i <= CN_ENUMS; i++) {
    if (giClassSize[i] == -1) {
      bFoundEnum = true;
      break;
    }
    iEntriesGCLASSSIZE++;
  }
  
  if (!bFoundEnum)
    vFatal("Entry for CN_ENUM not found in giClassSize\n");
  
  if (iEntriesECN != iEntriesGCLASSSIZE) {
    vFatal("\n!!!ALERT!!! No of entries in giClassSize (%lld) do not match the no in eCN (%lld)\n\n",
           iEntriesGCLASSSIZE, iEntriesECN);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
