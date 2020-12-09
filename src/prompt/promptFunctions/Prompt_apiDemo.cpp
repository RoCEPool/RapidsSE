/*============================================================================*/
/* Prompt_apiDemo.cpp -- RapidsSE Api Demo                                    */
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

  if (pPrompt->pInpTknVec()->nItems() > 1 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("get"_Stz))
    
    vRapidsSE_ApiDemo_get();
  
  else
    
    vRapidsSE_ApiDemo_put();
  
  return true;
}

#define ERROR_HANDLER(_error) {\
  eError = _error; \
  iErrLine == __LINE__; \
  goto L_Error; \
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
void vRapidsSE_ApiDemo_get(void)
{
  void* aSession = nullptr;
  void* aOperand = nullptr;
  eCN eClassNo;
  eOM eOpMarker;

  eEC eError = EC_NONE;
  i8  iErrLine = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // allocate an SE session
  eError = RapidsSE::eAllocSession(&aSession);
  if (eError) ERROR_HANDLER(eError);

  // parse command and prepare job
  eError = RapidsSE::ePrepareJob(aSession,(char*)"tableScan lineitem");
  if (eError) ERROR_HANDLER(eError);

  // execute job
  eError = RapidsSE::eExecuteJob(aSession);
  if (eError) ERROR_HANDLER(eError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process results
      
  for (;;) {
    
    // get next operand
    eError = RapidsSE::ePopOperand(aSession,&aOperand,
                                   &eClassNo,&eOpMarker);
    if (eError)
      break;
    
    // process the operand
    eError = RapidsSE::eProcessOperand(aSession,&aOperand);
    if (eError)
      break;
  }
  
  // delete the job
  eError = RapidsSE::eDeleteJob(aSession);
  if (eError) ERROR_HANDLER(eError);

  goto L_CleanUp;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // error handler

L_Error:
  vTrace("  Error : code %lld line %lld : %s\n"_Stz,
         eError,iErrLine,RapidsSE::aErrorText(eError));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // clean up

L_CleanUp:
  if (aSession)
    RapidsSE::eFreeSession(&aSession);
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
void vRapidsSE_ApiDemo_put(void)
{
  vTrace("Api Demo\n"_Stz);
  
  void* aSession = nullptr;
  void* aOperand = nullptr;
  eCN eClassNo;
  eOM eOpMarker;

  eEC eError = EC_NONE;
  i8  iErrLine = 0;

  CsvBuffer* pCsvBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Reader* pReader = new Reader;
  bool b = pReader->bOpenRead("/Users/Shared/tpch/SF0/lineitem_10k.csv"_Stz);
  if (! b) ERROR_HANDLER(EC_NO_SUCH_FILE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::aAllocSession\n"_Stz);
  eError = RapidsSE::eAllocSession(&aSession);
  if (eError) ERROR_HANDLER(eError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::ePrepareJob\n"_Stz);
  eError = RapidsSE::ePrepareJob(aSession,(char*)"apiInsert lineitem");
  if (eError) ERROR_HANDLER(eError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vSubmitJob\n"_Stz);
  eError = RapidsSE::eSubmitJob(aSession);
  if (eError) ERROR_HANDLER(eError);



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
    
  for (;;) {
        
    eError = RapidsSE::ePopOperand(aSession,&aOperand,
                                   &eClassNo,&eOpMarker);
    if (eError)
      break;
    
    vTrace("  Operand %s %s\n"_Stz,aCN[eClassNo],aOM[eOpMarker]);
    
    if (OM_EXITMARKER == eOpMarker) {
      RapidsSE::eDeleteOperand(aSession,&aOperand);
      break;
    }
    
    if (OM_DATAPULSE == eOpMarker) {
     
      // push DataPulse Marker
      eError = RapidsSE::ePushOperand(aSession,&aOperand);
      if (eError) ERROR_HANDLER(eError);

      for (;;) {
                
        pCsvBuffer = pReader->pReadCsvBuffer();
        if (! pCsvBuffer)
          break;

        // push csv data
        eError = RapidsSE::ePushOperand(aSession,(void**)&pCsvBuffer);
        if (eError) ERROR_HANDLER(eError);
      }
      
      // push Eos Marker
      aOperand = new Marker(OM_EOSMARKER);
      eError = RapidsSE::ePushOperand(aSession,&aOperand);
      if (eError) ERROR_HANDLER(eError);
      
      continue;
    }

    eError = RapidsSE::eProcessOperand(aSession,&aOperand);
    if (eError)
      break;
  }

  vTrace("  RapidsSE::vWaitJob\n"_Stz);
  eError = RapidsSE::eWaitJob(aSession);
  if (eError) ERROR_HANDLER(eError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vDeleteJob\n"_Stz);
  eError = RapidsSE::eDeleteJob(aSession);
  if (eError) ERROR_HANDLER(eError);

  goto L_CleanUp;

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

L_Error:
  vTrace("  Error : code %lld line %lld : %s\n"_Stz,
         eError,iErrLine,RapidsSE::aErrorText(eError));

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

L_CleanUp:
  if (aSession)
    RapidsSE::eFreeSession(&aSession);
  if (pCsvBuffer)
    delete pCsvBuffer;
  if (pReader)
    delete pReader;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
