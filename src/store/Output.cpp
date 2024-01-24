/*============================================================================*/
/* Output.cpp -- Output                                                       */
/*============================================================================*/

#include "BASE.h"

#include "Marker.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"

#include "Grid.hpp"
#include "Output.hpp"

#include "Job.hpp"
#include "MY.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Output,CN_OUTPUT)
__aNewFunction(Output,CN_OUTPUT)
__vDeleteFunction(Output,CN_OUTPUT)
__aFooterFunction(Output)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Output::Output(void)
: Object(CN_OUTPUT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);

  m_pOutputQueue      = nullptr;
  m_pStreamDef_result = nullptr;
  m_bToExit           = false;
  m_bHasError         = false;
  m_bLoadResult       = false;

  m_bFileOut          = false;
  m_pFileOut          = nullptr;
  //m_sFilePath

  _checkInitialised_(CN_OUTPUT);
  
  m_pOutputQueue = new Queue(CN_QUEUE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Output::~Output(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  if (m_pOutputQueue) {
    
    while (Operand* pOperand = _pOperand(m_pOutputQueue->pPop()))
      AM::vDeleteOperand(pOperand,nullptr);

    RASSERT(! m_pOutputQueue->nItems());
    delete m_pOutputQueue;
    m_pOutputQueue = nullptr;
  }
  
  if (m_pStreamDef_result)
    delete m_pStreamDef_result;
  m_pStreamDef_result = nullptr;

}

/*----------------------------------------------------------------------------*/
/* -- Public ---------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assert empty output queue                                                  */
/*----------------------------------------------------------------------------*/
void Output::vAssertEmptyQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  RASSERT(! m_pOutputQueue->nItems());
  
  RASSERT(! m_bToExit);
}

/*----------------------------------------------------------------------------*/
/* begin procssing queue for one command                                      */
/*----------------------------------------------------------------------------*/
void Output::vBeginProcOneCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
 if (!m_bFileOut)
   return;
    
  vTrace("<fileOut> = %s\n"_Stz,m_sFilePath.szStz());
      
  m_pFileOut = new File(this);
    
  if (! m_pFileOut->bOpenWrite(m_sFilePath.szStz())) {
      
    vTrace("Output: unable to open file `%s` for writing\n"_Stz,
           m_sFilePath.szStz());
      
    delete m_pFileOut;
    m_pFileOut = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* drain the result/output queue                                              */
/*----------------------------------------------------------------------------*/
void Output::vDrainBufferQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);

  while (! m_bToExit ) {
    
    Operand* pOperand = pPopBufferQueue_blocking();
    
    if (! pOperand ) {
      if ( ! AM::PROCESS::bSeRunning() ) {
        Stz *sNodeName = AM::pGrid()->pThisNode()->szNodeNm();
        vTrace("\nerror: %s; %s; %s; \n"_Stz,
               sNodeName, aEC2[EC_NODE_LOSS], aErrorMsg[EC_NODE_LOSS]);
        m_bHasError = true;
        m_bToExit = true;
        return;
      }

      AM::vNapUsec(1000,CN_OUTPUT);
      continue;
    }

    if (pOperand->bIsMarker()) {
      Marker* pMarker = _pMarker(pOperand);
      if (pMarker->eOpMarker() == OM_EXIT)
        m_bToExit = true;
    }

    AM::vDeleteOperand(pOperand,nullptr);
  }
}

/*----------------------------------------------------------------------------*/
/* end processing queue for one command                                       */
/*----------------------------------------------------------------------------*/
void Output::vEndProcOneCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  if (m_pFileOut) {
    if (m_pFileOut->bIsOpen())
      m_pFileOut->vClose();
    delete m_pFileOut;
    m_pFileOut = nullptr;
  }

}

/*----------------------------------------------------------------------------*/
/* set bFileOut                                                               */
/*----------------------------------------------------------------------------*/
void Output::vFileOut(bool bFileOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  m_bFileOut = bFileOut;
}

/*----------------------------------------------------------------------------*/
/* set sFilePath                                                              */
/*----------------------------------------------------------------------------*/
void Output::vFilePath(Stz* szFilePath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  m_sFilePath = szFilePath;
}

/*----------------------------------------------------------------------------*/
/* pipe to out buffer queue                                                   */
/*----------------------------------------------------------------------------*/
void Output::vPipeToThis(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);

  RASSERT( ! BufferId::bIsBufferId(pOperand));

  // The output queue can potentially fill up with buffers and eat up a lot of memory
  // This can trigger paging so to prevent these buffers from being paged out, we need
  // to lock them in the hashmap. Unfortunately, the Task can quickly reach the max locks
  // it can hold. Hence, we use a flag called m_bOutputQueue saved as a member
  // in DataBuffer to prevent paging out, when queuing to the output.
  if(DataBuffer::bIsDataBuffer(pOperand->eGetClassNo())){
    DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
    pDataBuffer->vOutputQueued(true);
    AM::pHashMap()->vDropKeyLock(pDataBuffer->oGetBufferId().m_pvAsAddress);
  }
  IGNORE_RETURN(m_pOutputQueue->iAppend(pOperand));
}

/*----------------------------------------------------------------------------*/
/* process the result/output queue                                            */
/*----------------------------------------------------------------------------*/
void Output::vProcessBufferQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  Operand* pOperand;
  
  while ( (pOperand = pPopBufferQueue_blocking()) ) {

    if (! bProcessOperand(pOperand))
      break;

  }
  
  vDrainBufferQueue();
}

/*----------------------------------------------------------------------------*/
/* print result                                                               */
/*----------------------------------------------------------------------------*/
void Output::vPrintResult(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);

  if (m_bLoadResult) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // success / error
    if (*m_ixRowsInvalid ||
        *m_ixRowsSkipped)
      vTrace("error: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             *m_ixRowsProcessed -
             *m_ixRowsSkipped,
             *m_ixRowsInvalid,
             *m_ixRowsSkipped);
    else
      vTrace("success: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             *m_ixRowsProcessed -
             *m_ixRowsSkipped,
             *m_ixRowsInvalid,
             *m_ixRowsSkipped);
  }
  else {
//    Task* pTask = MY::pTask();
//    if (! pTask->bIsStart())
    
//    if (PsxThd::bIsSeThread())
//      vTrace("IS SeThread\n"_Stz);
//    else
//      vTrace("NOT SeThread\n"_Stz);
    
//    if (AM::SYSTEM::bReady())
//      vTrace("SYSTEM READY\n"_Stz);
//    else
//      vTrace("NOT READY\n"_Stz);

    if (AM::SYSTEM::bReady()) // || ! AM::SHOW::bBoot())
      vTrace("success: %lld rows fetched\n"_Stz,
             *m_ixRowsProcessed);
  }

  m_ixRowsProcessed = 0;
  m_ixRowsInvalid   = 0;
  m_ixRowsSkipped   = 0;
  m_bLoadResult     = false;
}

/*----------------------------------------------------------------------------*/
/* -- Private --------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pop next operand from result queue                                         */
/*----------------------------------------------------------------------------*/
Operand* Output::pPopBufferQueue_blocking(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
      
  while (AM::PROCESS::bSeRunning()) {

    Operand* pOperand = _pOperand(m_pOutputQueue->pPop());
    if (pOperand)
      return pOperand;
    
    AM::vNapUsec(1000,CN_OUTPUT);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* process an operand from the result/output queue                            */
/*----------------------------------------------------------------------------*/
bool Output::bProcessOperand(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  bool bContinue = true;

  //SP_DataBuffer spDataBuffer;
    
  eCN eOperandClass = pOperand->eGetClassNo();

  //vTrace("OPERAND: %s\n"_Stz, aCN[eOperandClass]);

  switch (eOperandClass) {

    // -- -- -- -- -- --

    case CN_CHILDEXIT: {
      AM::vDeleteOperand(pOperand,nullptr);
      m_bToExit = true;
      if ( !m_bHasError )
        vPrintResult();
      m_bHasError = false;
      bContinue = false;
      goto L_EXIT;
      break;
    }

    // -- -- -- -- -- --
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
      m_ixRowsProcessed += pCrwBuffer->nRows();
      vTrace("\n"_Stz);
      pCrwBuffer->vPrintRows(m_pStreamDef_result);
      AM::vDeleteOperand(pOperand,nullptr);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
      m_ixRowsProcessed += pCsvBuffer->nRecords();
      vTrace("\n"_Stz);
      pCsvBuffer->vPrintRecords();
      AM::vDeleteOperand(pOperand,nullptr);
      break;
    }

    // -- -- -- -- -- --
      
//    case CN_ENTBUFFER: {
//      EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
//      m_ixRowsProcessed += pEntBuffer->nItems();
//      vTrace("\n"_Stz);
//      pEntBuffer->vPrintRefdRows();
//      AM::vDeleteOperand(pOperand,nullptr);
//      break;
//    }
      
    // -- -- -- -- -- --

    case CN_MARKER: {

      Marker *pMarker = _pMarker(pOperand);
			if (pMarker->eOpMarker() == OM_LOWMEMORY) {
        if (!m_bHasError) {
          vTrace("RowsProcessed: %lld\n"_Stz, *m_ixRowsProcessed);
          vTrace("\nerror: low memory\n"_Stz);
          m_bHasError = true;

          m_ixRowsProcessed = 0;
          m_ixRowsInvalid   = 0;
          m_ixRowsSkipped   = 0;
        }
      } else if (pMarker->eOpMarker() == OM_CHILDEXIT) {
        m_bToExit = true;
        if (!m_bHasError)
          vPrintResult();
        m_bHasError = false;
        bContinue = false;
        goto L_EXIT;
        break;
      } else if (pMarker->eOpMarker() == OM_EXIT) {
        m_bToExit = true;
        if ( !m_bHasError )
          vPrintResult();
        m_bHasError = false;
        bContinue = false;
        goto L_EXIT;
        break;
      }
      else if (pMarker->eOpMarker() == OM_INTERNALERROR) {
        if (!m_bHasError) {
          vTrace("RowsProcessed: %lld\n"_Stz, *m_ixRowsProcessed);
          vTrace("\nerror: internal error\n"_Stz);
          m_bHasError = true;

          m_ixRowsProcessed = 0;
          m_ixRowsInvalid   = 0;
          m_ixRowsSkipped   = 0;
        }
      }
      else {
        RASSERT(0);
      }
      break;
    }

    // -- -- -- -- -- --

//    case CN_EXITOPERAND: {
//      AM::vDeleteOperand(pOperand);
//      m_bToExit = true;
//      if ( !m_bHasError )
//        vPrintResult();
//      m_bHasError = false;
//      bContinue = false;
//      goto L_EXIT;
//      break;
//    }
      // -- -- -- -- -- --

    case CN_PDS: {
      Pds* pPdsRowBuffer = _pPds(pOperand);

      if (pPdsRowBuffer->eBufferType() == BT_PROMPTOUT) {
        Row *pInfoRow = pPdsRowBuffer->pFirstRow();
        Stz *szPromptLine = pInfoRow->szColVal_ofs(0);

        vTrace("%s\n"_Stz, szPromptLine);
      }
      else if (pPdsRowBuffer->eBufferType() == BT_AFFECTEDROWS) {
        m_bLoadResult = true;
        Row *pInfoRow = pPdsRowBuffer->pFirstRow();

        i8 iRowsProcessed = pInfoRow->iColVal_int(0);
        i8 iRowsInvalid = pInfoRow->iColVal_int(1);
        i8 iRowsSkipped = pInfoRow->iColVal_int(2);

        m_ixRowsProcessed += iRowsProcessed;
        m_ixRowsInvalid += iRowsInvalid;
        m_ixRowsSkipped += iRowsSkipped;
      }
      else if (pPdsRowBuffer->eBufferType() == BT_EXECERRORS) {
        if ( ! m_bHasError) {
          Row *pInfoRow = pPdsRowBuffer->pFirstRow();

          Stz *sNodeName = pInfoRow->szColVal_ofs(0);
          eEC eErrorCode = (eEC) pInfoRow->iColVal_int(1);
          Stz *szErrorText = pInfoRow->szColVal_ofs(2);

          vTrace("\nerror: %s; %s; %s; \n"_Stz,
                 sNodeName, aEC2[eErrorCode], szErrorText);

          m_ixRowsProcessed = 0;
          m_ixRowsInvalid   = 0;
          m_ixRowsSkipped   = 0;

          m_bHasError = true;
        }
      }
      else if (pPdsRowBuffer->eBufferType() == BT_COMPILEERRORS) {
        if ( ! m_bHasError) {
          Row *pInfoRow = pPdsRowBuffer->pFirstRow();

          Stz *sNodeName = pInfoRow->szColVal_ofs(0);
          eEC eErrorCode = (eEC) pInfoRow->iColVal_int(1);
          i8 iLineNo = pInfoRow->iColVal_int(2);
          i8 iColumnNo = pInfoRow->iColVal_int(3);
          Stz *szErrorText = pInfoRow->szColVal_ofs(4);

          vTrace("\nerror: %s; %s; Line: %lld; Column: %lld; %s; \n"_Stz,
                 sNodeName, aEC2[eErrorCode], iLineNo, iColumnNo, szErrorText);
          m_bHasError = true;
        }
      }
      else {
        m_ixRowsProcessed += pPdsRowBuffer->nItems();
        vTrace("\n"_Stz);
        pPdsRowBuffer->vPrintRows(nullptr, nullptr);
      }
      AM::vDeleteOperand(pOperand, nullptr);
      break;
    }

    // -- -- -- -- -- --

    case CN_STREAMDEF: {
      if ( ! m_pStreamDef_result ) {
        m_pStreamDef_result = _pStreamDef(pOperand);
        
        // print the metadata
        if (AM::bMetadata()) {
          s4K sMetadata;
          m_pStreamDef_result->vToMetadata(sMetadata);
          vTrace("\n%s\n"_Stz, sMetadata.szStz());
        }
      }
      break;
    }
      
    // -- -- -- -- -- --
      
    default:
      RASSERT(0);
      break;
  }
    
L_EXIT:

  return bContinue;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/