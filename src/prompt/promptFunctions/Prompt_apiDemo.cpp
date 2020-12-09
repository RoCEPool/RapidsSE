/*============================================================================*/
/* Prompt_apiDemo.cpp -- Ascii Helper                                           */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_apiDemo (
  PF_APIDEMO,           // prompt function
  "Prompt_apiDemo"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "apiDemo"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_apiDemo,      // prompt function
  true,                 // enabled

  true,                 // visible
  "apiDemo"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_apiDemo(SePrompt* pPrompt)
{
  vTrace("Api Demo\n"_Stz);
  
  void* aSession = nullptr;
  Reader* pReader = nullptr;
  CsvBuffer* pCsvBuffer = nullptr;
  Operand* pOperand = nullptr;
  
  bool b;
  char* azInto;
  i8 nBytesRead;

  eEC eError;
  eCN eClassNo;
  eOM eOpMarker;

  if (pPrompt->pInpTknVec()->nItems() > 1 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("get"_Stz))
    goto L_Get;

                                            
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  pReader = new Reader;
  b = pReader->bOpenRead("/Users/Shared/tpch/SF0/lineitem_10k.csv"_Stz);
  if (! b) {
    vTrace("Unable to open input file\n"_Stz);
    goto L_Cleanup;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::aAllocSession\n"_Stz);
  aSession = RapidsSE::aAllocSession();
  if (! aSession) {
    vTrace("  RapidsSE::aAllocSession error: unable to allocate session\n"_Stz);
    goto L_Cleanup;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::ePrepareJob\n"_Stz);
  eError = RapidsSE::ePrepareJob(aSession,(char*)"apiInsert lineitem");
  if (eError) {
    vTrace("  RapidsSE::ePrepareJob error: %s\n"_Stz,aEC[eError]);
    goto L_Cleanup;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vSubmitJob\n"_Stz);
  RapidsSE::vSubmitJob(aSession);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
    
  for (;;) {
        
    pOperand = RapidsSE::pPopOperand(aSession,&eClassNo,&eOpMarker);
    if (! pOperand)
      break;
    
    vTrace("  Operand %s %s\n"_Stz,aCN[eClassNo],aOM[eOpMarker]);
    
    if (OM_EXITMARKER == eOpMarker) {
      AM::vDeleteOperand(pOperand);
      break;
    }
    
    if (OM_DATAPULSE == eOpMarker) {
     
      // push DataPulse Marker
      RapidsSE::vPushOperand(aSession,(void**)&pOperand);

      for (;;) {
        
//        pCsvBuffer = new CsvBuffer;
//        azInto = (char*)pCsvBuffer->ayDataArea();
//        nBytesRead = pReader->nReadLines((Stz*)azInto,512);
//
//        if (! nBytesRead) {
//          delete pCsvBuffer;
//          pCsvBuffer = nullptr;
//          break;
//        }
//
//        pCsvBuffer->vTake(nBytesRead + 1);
      //  vTrace("CsvBuffer data:\n"_Stz);
      //  vTrace("%s"_Stz,azInto);
      //  vTrace("End CsvBuffer\n"_Stz);
        
        pCsvBuffer = pReader->pReadCsvBuffer();
        if (! pCsvBuffer)
          break;

        // push csv data
        RapidsSE::vPushOperand(aSession,(void**)&pCsvBuffer);
      }
      
      // push Eos Marker
      pOperand = new Marker(OM_EOSMARKER);
      RapidsSE::vPushOperand(aSession,(void**)&pOperand);

      continue;
    }

    b = RapidsSE::bProcessOperand(aSession,(void**)&pOperand);
    if (! b)
      break;
  }

  vTrace("  RapidsSE::vWaitJob\n"_Stz);
  RapidsSE::vWaitJob(aSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vDeleteJob\n"_Stz);
  RapidsSE::vDeleteJob(aSession);

  goto L_Cleanup;

  
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_Get:
  vTrace("  RapidsSE::aAllocSession\n"_Stz);
  aSession = RapidsSE::aAllocSession();
  if (! aSession) {
    vTrace("  RapidsSE::aAllocSession error: unable to allocate session\n"_Stz);
    goto L_Cleanup;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::ePrepareJob\n"_Stz);
  eError = RapidsSE::ePrepareJob(aSession,(char*)"tableScan lineitem");
  if (eError) {
    vTrace("  RapidsSE::ePrepareJob error: %s\n"_Stz,aEC[eError]);
    goto L_Cleanup;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vExecuteJob\n"_Stz);
  RapidsSE::vExecuteJob(aSession);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
      
  for (;;) {
    
    pOperand = RapidsSE::pPopOperand(aSession,&eClassNo,&eOpMarker);
    if (! pOperand)
      break;
    
    b = RapidsSE::bProcessOperand(aSession,(void**)&pOperand);
    if (! b)
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vDeleteJob\n"_Stz);
  RapidsSE::vDeleteJob(aSession);

  goto L_Cleanup;

  
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

L_Cleanup:
  if (aSession)
    RapidsSE::vFreeSession(aSession);
  if (pCsvBuffer)
    delete pCsvBuffer;
  if (pReader)
    delete pReader;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
