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
  eError = RapidsSE::eAllocSession(&aSession);
  if (eError) {
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
  eError = RapidsSE::eSubmitJob(aSession);
  if (eError) {
    vTrace("  RapidsSE::eSubmitJob error: %s\n"_Stz,aEC[eError]);
    goto L_Cleanup;
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
    
  for (;;) {
        
    eError = RapidsSE::ePopOperand(aSession,(void**)&pOperand,
                                   &eClassNo,&eOpMarker);
    if (eError)
      break;
    
    vTrace("  Operand %s %s\n"_Stz,aCN[eClassNo],aOM[eOpMarker]);
    
    if (OM_EXITMARKER == eOpMarker) {
      AM::vDeleteOperand(pOperand);
      break;
    }
    
    if (OM_DATAPULSE == eOpMarker) {
     
      // push DataPulse Marker
      eError = RapidsSE::ePushOperand(aSession,(void**)&pOperand);
      if (eError) {
        vTrace("  RapidsSE::ePushOperand error: %s\n"_Stz,aEC[eError]);
        goto L_Cleanup;
      }

      for (;;) {
                
        pCsvBuffer = pReader->pReadCsvBuffer();
        if (! pCsvBuffer)
          break;

        // push csv data
        eError = RapidsSE::ePushOperand(aSession,(void**)&pCsvBuffer);
        if (eError) {
          vTrace("  RapidsSE::ePushOperand error: %s\n"_Stz,aEC[eError]);
          goto L_Cleanup;
        }
      }
      
      // push Eos Marker
      pOperand = new Marker(OM_EOSMARKER);
      eError = RapidsSE::ePushOperand(aSession,(void**)&pOperand);
      if (eError) {
        vTrace("  RapidsSE::ePushOperand error: %s\n"_Stz,aEC[eError]);
        goto L_Cleanup;
      }

      continue;
    }

    eError = RapidsSE::eProcessOperand(aSession,(void**)&pOperand);
    if (eError)
      break;
  }

  vTrace("  RapidsSE::vWaitJob\n"_Stz);
  eError = RapidsSE::eWaitJob(aSession);
  if (eError) {
    vTrace("  RapidsSE::eWaitJob error: %s\n"_Stz,aEC[eError]);
    goto L_Cleanup;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vDeleteJob\n"_Stz);
  eError = RapidsSE::eDeleteJob(aSession);
  if (eError) {
    vTrace("  RapidsSE::eDeleteJob error: %s\n"_Stz,aEC[eError]);
    goto L_Cleanup;
  }

  goto L_Cleanup;

  
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_Get:
  vTrace("  RapidsSE::aAllocSession\n"_Stz);
  eError = RapidsSE::eAllocSession(&aSession);
  if (eError) {
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
  eError = RapidsSE::eExecuteJob(aSession);
  if (eError) {
    vTrace("  RapidsSE::ePrepareJob error: %s\n"_Stz,aEC[eError]);
    goto L_Cleanup;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
      
  for (;;) {
    
    eError = RapidsSE::ePopOperand(aSession,(void**)&pOperand,
                                   &eClassNo,&eOpMarker);
    if (eError)
      break;
    
    eError = RapidsSE::eProcessOperand(aSession,(void**)&pOperand);
    if (eError)
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vDeleteJob\n"_Stz);
  eError = RapidsSE::eDeleteJob(aSession);
  if (eError) {
    vTrace("  RapidsSE::eDeleteJob error: %s\n"_Stz,aEC[eError]);
    goto L_Cleanup;
  }

  goto L_Cleanup;

  
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

L_Cleanup:
  if (aSession)
    RapidsSE::eFreeSession(&aSession);
  if (pCsvBuffer)
    delete pCsvBuffer;
  if (pReader)
    delete pReader;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
