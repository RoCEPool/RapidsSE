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
  
  TokenVec* pInpTknVec = new TokenVec;
  TokenVec* pLitTknVec = new TokenVec;
  TagSet    oExtTagSet;

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
  Session* pSession;
  Job* pOriginatingJob;

  if (pPrompt->pInpTknVec()->nItems() > 1 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("put"_Stz))
    goto L_Put;

  
  

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pInpTknVec->vTokeniseC("tableScan lineitem"_Stz);
  pInpTknVec->vDumpVec(0);

  oExtTagSet.vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);
  oExtTagSet.vReplace(C_TAG_TRACEOP,1);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::pAllocSession\n"_Stz);
  pSession = Api::pAllocSession(SK_EXTERNAL);
  if (! pSession) {
    vTrace("  Api::pAllocSession error: unable to allocate session\n"_Stz);
    goto L_Cleanup;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::ePrepareJob\n"_Stz);
  eError = Api::ePrepareJob(pSession,
                            pInpTknVec,
                            pLitTknVec,
                            &oExtTagSet);
  if (eError) {
    vTrace("  Api::ePrepareJob error: %s\n"_Stz,aEC[eError]);
    goto L_End;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vExecuteJob\n"_Stz);
  Api::vExecuteJob(pSession);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);

  pOriginatingJob = pSession->pJob();
  
  if (pOriginatingJob->bIsPipeline()) {
    //pSession->m_pSsnJob->m_xResult.vProcessBufferQueue();
    
    for (;;) {
      
      Operand* pOperand = pSession->m_pSsnJob->m_xResult.pPopBufferQueue_blocking();
      
      if (! pOperand)
        break;
      
      bool b = pSession->m_pSsnJob->m_xResult.bProcessOperand(pOperand);
      
      if (! b)
        break;
    }

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vDeleteJob\n"_Stz);
  Api::vDeleteJob(pSession);   

  goto L_End;
  
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_Put:
  pInpTknVec->vTokeniseC("apiInsert lineitem"_Stz);
  pInpTknVec->vDumpVec(0);

  oExtTagSet.vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);
  oExtTagSet.vReplace(C_TAG_TRACEOP,1);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//pFile = new File(nullptr);
//
//  b = pFile->bOpenRead("/Users/Shared/tpch/SF0/lineitem_10.csv"_Stz);
//  if (! b) {
//    vTrace("Unable to open input file\n"_Stz);
//    goto L_Cleanup;
//  }
  
  
  pReader = new Reader;
  b = pReader->bOpenRead("/Users/Shared/tpch/SF0/lineitem_10k.csv"_Stz);
  if (! b) {
    vTrace("Unable to open input file\n"_Stz);
    goto L_Cleanup;
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  



  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::pAllocSession\n"_Stz);
  pSession = Api::pAllocSession(SK_EXTERNAL);
  if (! pSession) {
    vTrace("  Api::pAllocSession error: unable to allocate session\n"_Stz);
    goto L_Cleanup;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::ePrepareJob\n"_Stz);
  eError = Api::ePrepareJob(pSession,
                            pInpTknVec,
                            pLitTknVec,
                            &oExtTagSet);
  if (eError) {
    vTrace("  Api::ePrepareJob error: %s\n"_Stz,aEC[eError]);
    goto L_End;
  }
  
  pOriginatingJob = pSession->pJob();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vSubmitJob\n"_Stz);
  //Api::vExecuteJob(pOriginatingJob);
  Api::vSubmitJob(pSession);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
  if (pOriginatingJob->bIsPipeline()) {
    //pSession->m_pSsnJob->m_xResult.vProcessBufferQueue();
    
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

  vTrace("  Api::vWaitJob\n"_Stz);
  Api::vWaitJob(pSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vDeleteJob\n"_Stz);
  Api::vDeleteJob(pSession);

  goto L_End;

  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_End:
  vTrace("  Api::vFreeSession\n"_Stz);
  Api::vFreeSession(pSession);

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

  delete pInpTknVec;
  delete pLitTknVec;

  return true;
}








/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_apiDemo_first(SePrompt* pPrompt)
{
  vTrace("Api Demo\n"_Stz);
  
  TokenVec* pInpTknVec = new TokenVec;
  TokenVec* pLitTknVec = new TokenVec;
  TagSet    oExtTagSet;

  File* pFile = nullptr;
  CsvBuffer* pCsvBuffer = nullptr;
  Marker* pPulsedMarker = nullptr;
  Marker* pDataMarker = nullptr;
  Marker* pEosMarker = nullptr;

  bool b;
  char* azInto;
  i8 nBytesRead;
  i8 nBytesToRead;

  eEC eError;
  Session* pSession;
  Job* pOriginatingJob;

  if (pPrompt->pInpTknVec()->nItems() > 1)
    goto L_Insert;

  
  

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pInpTknVec->vTokeniseC("tableScan bolTbl"_Stz);
  pInpTknVec->vDumpVec(0);

  oExtTagSet.vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);
  oExtTagSet.vReplace(C_TAG_TRACEOP,1);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::pAllocSession\n"_Stz);
  pSession = Api::pAllocSession(SK_EXTERNAL);
  if (! pSession) {
    vTrace("  Api::pAllocSession error: unable to allocate session\n"_Stz);
    goto L_Cleanup;
  }
  
  pOriginatingJob = pSession->pJob();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::ePrepareJob\n"_Stz);
  eError = Api::ePrepareJob(pSession,
                            pInpTknVec,
                            pLitTknVec,
                            &oExtTagSet);
  if (eError) {
    vTrace("  Api::ePrepareJob error: %s\n"_Stz,aEC[eError]);
    goto L_End;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vExecuteJob\n"_Stz);
  Api::vExecuteJob(pSession);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
  if (pOriginatingJob->bIsPipeline()) {
    //pSession->m_pSsnJob->m_xResult.vProcessBufferQueue();
    
    for (;;) {
      
      Operand* pOperand = pSession->m_pSsnJob->m_xResult.pPopBufferQueue_blocking();
      
      if (! pOperand)
        break;
      
      bool b = pSession->m_pSsnJob->m_xResult.bProcessOperand(pOperand);
      
      if (! b)
        break;
    }

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vDeleteJob\n"_Stz);
  Api::vDeleteJob(pSession);

  goto L_End;
  
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_Insert:
  pInpTknVec->vTokeniseC("apiInsert lineitem"_Stz);
  pInpTknVec->vDumpVec(0);

  oExtTagSet.vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);
  oExtTagSet.vReplace(C_TAG_TRACEOP,1);

  pFile = new File(nullptr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  b = pFile->bOpenRead("/Users/Shared/tpch/SF0/lineitem_10.csv"_Stz);
  if (! b) {
    vTrace("Unable to open input file\n"_Stz);
    goto L_Cleanup;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  nBytesToRead = C_512KB;
  
  pCsvBuffer = new CsvBuffer;
  azInto = (char*)pCsvBuffer->ayDataArea();
  nBytesRead = pFile->nReadBytes(azInto,nBytesToRead);
  pCsvBuffer->vTake(nBytesRead + 1);
  azInto[nBytesRead] = '\0';

  vTrace("CsvBuffer data:\n"_Stz);
  vTrace("%s"_Stz,azInto);
  vTrace("End CsvBuffer\n"_Stz);


  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::pAllocSession\n"_Stz);
  pSession = Api::pAllocSession(SK_EXTERNAL);
  if (! pSession) {
    vTrace("  Api::pAllocSession error: unable to allocate session\n"_Stz);
    goto L_Cleanup;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::ePrepareJob\n"_Stz);
  eError = Api::ePrepareJob(pSession,
                            pInpTknVec,
                            pLitTknVec,
                            &oExtTagSet);
  if (eError) {
    vTrace("  Api::ePrepareJob error: %s\n"_Stz,aEC[eError]);
    goto L_End;
  }
  
  pOriginatingJob = pSession->pJob();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vSubmitJob\n"_Stz);
  //Api::vExecuteJob(pOriginatingJob);
  Api::vSubmitJob(pSession);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
  if (pOriginatingJob->bIsPipeline()) {
    //pSession->m_pSsnJob->m_xResult.vProcessBufferQueue();
    
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
        
        // push csv data
        pInputOp->vOutboundOperand(pCsvBuffer);
        pCsvBuffer = nullptr;
        while (pInputOp->pOutboundOperand())
          AM::vNapUsec(100,CN_SEPROMPT);

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

  vTrace("  Api::vWaitJob\n"_Stz);
  Api::vWaitJob(pSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vDeleteJob\n"_Stz);
  Api::vDeleteJob(pSession);

  goto L_End;

  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_End:
  vTrace("  Api::vFreeSession\n"_Stz);
  Api::vFreeSession(pSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_Cleanup:
  if (pFile) {
    pFile->vClose();
    delete pFile;
  }
  if (pCsvBuffer) {
    delete pCsvBuffer;
  }
  
  delete pInpTknVec;
  delete pLitTknVec;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
