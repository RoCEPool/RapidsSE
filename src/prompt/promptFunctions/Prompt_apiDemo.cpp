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
  
  File* pFile = nullptr;
  CsvBuffer* pCsvBuffer = nullptr;
  Marker* pPulsedMarker = nullptr;
  Marker* pDataMarker = nullptr;
  Marker* pEosMarker = nullptr;

  Reader* pReader = nullptr;
  
  i8 nBytesToRead = 512; // C_512KB;

  bool b;
  char* azInto;
  i8 nBytesRead;

  eEC eError;
  void* aSession;
  Job* pOriginatingJob;

  if (pPrompt->pInpTknVec()->nItems() > 1 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("put"_Stz))
    goto L_Put;

  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
    goto L_End;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vExecuteJob\n"_Stz);
  RapidsSE::vExecuteJob(aSession);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);

  pOriginatingJob = ((Session*)aSession)->pJob();
  
  if (pOriginatingJob->bIsPipeline()) {
    //aSession->m_pSsnJob->m_xResult.vProcessBufferQueue();
    
    for (;;) {
      
      Operand* pOperand = pOriginatingJob->m_xResult.pPopBufferQueue_blocking();
      
      if (! pOperand)
        break;
      
      bool b = pOriginatingJob->m_xResult.bProcessOperand(pOperand);
      
      if (! b)
        break;
    }

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vDeleteJob\n"_Stz);
  RapidsSE::vDeleteJob(aSession);

  goto L_End;
  
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_Put:
  
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
    goto L_End;
  }
  
  pOriginatingJob = ((Session*)aSession)->pJob();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vSubmitJob\n"_Stz);
  //RapidsSE::vExecuteJob(pOriginatingJob);
  RapidsSE::vSubmitJob(aSession);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
  if (pOriginatingJob->bIsPipeline()) {
    //aSession->m_pSsnJob->m_xResult.vProcessBufferQueue();
    
    for (;;) {
      
      Operand* pOperand = pOriginatingJob->m_xResult.pPopBufferQueue_blocking();
      
      if (! pOperand)
        break;
      
      eCN eClassNo;
      eOM eOpMarker;
      Result::vDecode(pOperand,eClassNo,eOpMarker);
      vTrace("  Operand %s %s\n"_Stz,aCN[eClassNo],aOM[eOpMarker]);

      //bool b = pOriginatingJob->m_xResult.bProcessOperand(pOperand);
      
      if (OM_EXITMARKER == eOpMarker) {
        AM::vDeleteOperand(pOperand);
        break;
      }
      
      if (OM_DATAPULSE == eOpMarker) {
       
        Op* pInputOp = pOriginatingJob->m_pInputOp;
        
        // push DataPulse Marker
        pInputOp->vOutboundOperand(pOperand);
        pOperand = nullptr;
        while (pInputOp->pOutboundOperand())
          AM::vNapUsec(100,CN_SEPROMPT);
        
        for (;;) {
          
          pCsvBuffer = new CsvBuffer;
          azInto = (char*)pCsvBuffer->ayDataArea();
          nBytesRead = pReader->nReadLines((Stz*)azInto,nBytesToRead);
          
          if (! nBytesRead) {
            delete pCsvBuffer;
            pCsvBuffer = nullptr;
            break;
          }
          
          pCsvBuffer->vTake(nBytesRead + 1);
        //  vTrace("CsvBuffer data:\n"_Stz);
        //  vTrace("%s"_Stz,azInto);
        //  vTrace("End CsvBuffer\n"_Stz);
          
          // push csv data
          pInputOp->vOutboundOperand(pCsvBuffer);
          pCsvBuffer = nullptr;
          while (pInputOp->pOutboundOperand())
            AM::vNapUsec(100,CN_SEPROMPT);
        }
        
        // push Eos Marker
        pEosMarker = new Marker(OM_EOSMARKER);
        pInputOp->vOutboundOperand(pEosMarker);
        pEosMarker = nullptr;

        continue;
      }


      bool b = pOriginatingJob->m_xResult.bProcessOperand(pOperand);
      
      if (! b)
        break;
    }

  }

  vTrace("  RapidsSE::vWaitJob\n"_Stz);
  RapidsSE::vWaitJob(aSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  RapidsSE::vDeleteJob\n"_Stz);
  RapidsSE::vDeleteJob(aSession);

  goto L_End;

  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_End:
  vTrace("  RapidsSE::vFreeSession\n"_Stz);
  RapidsSE::vFreeSession(aSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_Cleanup:
  if (pFile) {
    pFile->vClose();
    delete pFile;
  }
  if (pCsvBuffer) {
    delete pCsvBuffer;
  }
  if (pReader) {
    delete pReader;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
