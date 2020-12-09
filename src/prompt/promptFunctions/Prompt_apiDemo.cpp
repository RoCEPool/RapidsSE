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
/* RapidsSE Api Demo : get rows (table scan example)                          */
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
/* RapidsSE Api Demo : put rows (insert into table example)                   */
/*----------------------------------------------------------------------------*/
void vRapidsSE_ApiDemo_put(void)
{
  void* aSession = nullptr;
  void* aOperand = nullptr;
  eCN eClassNo;
  eOM eOpMarker;

  eEC eError = EC_NONE;
  i8  iErrLine = 0;

  CsvBuffer* pCsvBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // create a csv file reader
  Reader* pReader = new Reader;
  bool b = pReader->bOpenRead("/Users/Shared/tpch/SF0/lineitem_10k.csv"_Stz);
  if (! b) ERROR_HANDLER(EC_NO_SUCH_FILE);

  // allocate an SE session
  eError = RapidsSE::eAllocSession(&aSession);
  if (eError) ERROR_HANDLER(eError);

  // parse command and prepare job
  eError = RapidsSE::ePrepareJob(aSession,(char*)"apiInsert lineitem");
  if (eError) ERROR_HANDLER(eError);

  // submit job
  eError = RapidsSE::eSubmitJob(aSession);
  if (eError) ERROR_HANDLER(eError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push buffers and markers

  for (;;) {
        
    // get next operand
    eError = RapidsSE::ePopOperand(aSession,&aOperand,
                                   &eClassNo,&eOpMarker);
    if (eError)
      break;
        
    // exit marker?
    if (OM_EXITMARKER == eOpMarker) {
      RapidsSE::eDeleteOperand(aSession,&aOperand);
      break;
    }
    
    // push data?
    if (OM_DATAPULSE == eOpMarker) {
     
      // push DataPulse Marker
      eError = RapidsSE::ePushOperand(aSession,&aOperand);
      if (eError) ERROR_HANDLER(eError);

      for (;;) {
                
        // read a bufer of csv data
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

    // default processing for any other operand
    eError = RapidsSE::eProcessOperand(aSession,&aOperand);
    if (eError)
      break;
  }

  // wait for job to complete
  eError = RapidsSE::eWaitJob(aSession);
  if (eError) ERROR_HANDLER(eError);

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
  if (pCsvBuffer)
    delete pCsvBuffer;
  if (pReader)
    delete pReader;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
