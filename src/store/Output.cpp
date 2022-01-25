/*============================================================================*/
/* Output.cpp -- Output                                                       */
/*============================================================================*/

#include "BASE.h"

#include "Marker.hpp"

#include "RowBuffer.hpp"

#include "InfoSet.hpp"

#include "StreamSet.hpp"
#include "Session.hpp"

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
Output::Output(Session* pSession)
: Object(CN_OUTPUT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
      
  m_pSession          = pSession;
  m_pOutputQueue      = nullptr;
  m_pStreamDef_result = nullptr;
  m_bExitMarker       = false;
  m_bExitEarlyMarker  = false;
  m_bCmdErr           = false;
  m_bExecErr          = false;
  m_bPrintEnabled     = true;

  m_iOutputState      = 0;
  
  m_pFileOut          = nullptr;

  _checkInitialised_(CN_OUTPUT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Output::~Output(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  if (m_pOutputQueue) {
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
  
  RASSERT(! m_bExitMarker);
}

/*----------------------------------------------------------------------------*/
/* begin procssing queue for one command                                      */
/*----------------------------------------------------------------------------*/
void Output::vBeginProcOneCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  RASSERT(! m_iOutputState);
  m_iOutputState = 1;
  
  TagSet* pSsnTagSet = &m_pSession->m_oSsnTagSet;
  
  if (pSsnTagSet->bHasName("<fileOut>"_Stz)) {
    
    vTrace("Ssn <fileOut> = %s\n"_Stz,pSsnTagSet->szValue("<fileOut>"_Stz));
  }
  
  TagSet* pJobTagSet = &m_pSession->m_oJobTagSet;
  if (pJobTagSet->bHasName("<fileOut>"_Stz)) {
    
    Stz* szFileOutNm = pJobTagSet->szValue("<fileOut>"_Stz);
    vTrace("Job <fileOut> = %s\n"_Stz,szFileOutNm);
    
    m_pFileOut = new File(this);
    
    if (! m_pFileOut->bOpenWrite(szFileOutNm)) {
      
      vTrace("Output: unable to open file `%s` for writing\n"_Stz,szFileOutNm);
      
      delete m_pFileOut;
      m_pFileOut = nullptr;
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* drain the result/output queue                                              */
/*----------------------------------------------------------------------------*/
void Output::vDrainBufferQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  while (! m_bExitMarker   &&
         ! m_pSession->m_pSsnJob->bJobHasError()) {
    
    Operand* pOperand = pPopBufferQueue_blocking();
    
    if (! pOperand) {
      AM::vNapUsec(1000,CN_OUTPUT);
      continue;
    }
    
    eCN eOperandClass = pOperand->eGetClassNo();

    if (eOperandClass == CN_MARKER) {
      Marker *pMarker = _pMarker(pOperand);
      eOM eOpMarker = pMarker->eOpMarker();

      if (eOpMarker == OM_EXITING)
        m_bExitMarker = true;
    }

    AM::vDeleteOperand(pOperand);
  }

}
/*----------------------------------------------------------------------------*/
//void Output::vDrainBufferQueue(Session* pSession)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
//
//  Operand* pOperand;
//
//  while ( (pOperand = pPopBufferQueue_blocking(pSession)) ) {
//    eCN eOperandClass = pOperand->eGetClassNo();
//
//    if (eOperandClass == CN_MARKER) {
//      Marker *pMarker = _pMarker(pOperand);
//      eOM eOpMarker = pMarker->eOpMarker();
//
//      if (eOpMarker == OM_CANCELMARKER)
//        m_bExitMarker = true;
//
//      if (eOpMarker == OM_EXITMARKER)
//        m_bCancelMarker = true;
//
//      AM::vDeleteOperand(pOperand);
//
//      if (eOpMarker == OM_CANCELMARKER ||
//          eOpMarker == OM_EXITMARKER)
//        break;
//    }
//    else {
//      AM::vDeleteOperand(pOperand);
//    }
//  }
//
//}


/*----------------------------------------------------------------------------*/
/* end procssing queue for one command                                        */
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
  
  m_iOutputState = 0;
}

/*----------------------------------------------------------------------------*/
/* process the result/output queue                                            */
/*----------------------------------------------------------------------------*/
void Output::vProcessBufferQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  RASSERT(m_iOutputState);
  
  Operand* pOperand;
  
  while ( (pOperand = pPopBufferQueue_blocking()) ) {

    if (! bProcessOperand(pOperand))
      break;

  }
  
  vDrainBufferQueue();
}

/*----------------------------------------------------------------------------*/
/* get print enable/disabled                                                  */
/*----------------------------------------------------------------------------*/
bool Output::bPrintEnabled(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  return m_bPrintEnabled;
}

/*----------------------------------------------------------------------------*/
/* set print enable/disabled                                                  */
/*----------------------------------------------------------------------------*/
void Output::vPrintEnabled(bool bPrintEnabled)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  m_bPrintEnabled = bPrintEnabled;
}

/*----------------------------------------------------------------------------*/
/* print result                                                               */
/*----------------------------------------------------------------------------*/
void Output::vPrintResult(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);

  if (*m_ixRowsInvalid   ||
      *m_ixRowsSkipped ) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // success / error
    if (*m_ixRowsInvalid   ||
        *m_ixRowsSkipped )
      vTrace("error: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             *m_ixRowsProcessed -
             *m_ixRowsSkipped,
             *m_ixRowsInvalid,
             *m_ixRowsSkipped);
    else
      vTrace("\nsuccess: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
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
      vTrace("\nsuccess: %lld rows fetched\n"_Stz,
             *m_ixRowsProcessed);
  }

  m_ixRowsProcessed = 0;
  m_ixRowsInvalid   = 0;
  m_ixRowsSkipped   = 0;
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
  
  for (;;) {
    
    Operand* pOperand = _pOperand(m_pSession->m_pOutput->m_pOutputQueue->pPop());
    
    if (pOperand)
      return pOperand;
    
    AM::vNapUsec(1000,CN_OUTPUT);
    
    if (m_pSession->m_pSsnJob->bJobHasError())
      return nullptr;
  }
  
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
  
  switch (eOperandClass) {
      
    // -- -- -- -- -- --
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
      m_ixRowsProcessed += pCrwBuffer->nRows();
      if (m_bPrintEnabled) {
        vTrace("\n"_Stz);
        pCrwBuffer->vPrintRows(m_pStreamDef_result);
      }
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
      m_ixRowsProcessed += pCsvBuffer->nRecords();
      if (m_bPrintEnabled) {
        vTrace("\n"_Stz);
        pCsvBuffer->vPrintRecords();
      }
      AM::vDeleteOperand(pOperand);
      break;
    }

    // -- -- -- -- -- --

    case CN_EACHROWBUFFER: {

      EachRowBuffer* pEachRowBuffer = _pEachRowBuffer(pOperand);
//        if (AM::bDeveloper())
//          vTrace("[[RowBuffer %s ; Row=%lld]]\n"_Stz,
//                 spDataBuffer.sz(pRowBuffer),
//                 pRowBuffer->nRows());
      m_ixRowsProcessed += pEachRowBuffer->nRows();
      if (m_bPrintEnabled) {
        vTrace("\n"_Stz);
        pEachRowBuffer->vPrintRows();
      }
      AM::vDeleteOperand(pOperand);
      break;
    }

    // -- -- -- -- -- --
      
    case CN_ENTBUFFER: {
      EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
      m_ixRowsProcessed += pEntBuffer->nItems();
      if (m_bPrintEnabled) {
        vTrace("\n"_Stz);
        pEntBuffer->vPrintRefdRows();
      }
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_INDEXDEF: {
      //vTrace("Pipeline Job: INDEXDEF\n"_Stz);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_INFOSET: {
      InfoSet* pInfoSet = _pInfoSet(pOperand);
      ASSERT( ! pInfoSet->szBufferNm()->bSame(C_NO_NAME) );
      if (pInfoSet->szBufferNm()->bSame(C_TDEF_COMMANDERRORS)) {

        if ( ! m_bCmdErr) {
          Row *pInfoRow = pInfoSet->pFirstRow();

          Stz *sNodeName = pInfoRow->szColVal_ofs(0);
          eEC eErrorCode = (eEC) pInfoRow->iColVal_int(1);
          i8 iLineNo = pInfoRow->iColVal_int(2);
          i8 iColumnNo = pInfoRow->iColVal_int(3);
          Stz *szErrorText = pInfoRow->szColVal_ofs(4);

          vTrace("\nerror: %s; %s; Line: %lld; Column: %lld; %s; \n"_Stz,
                 sNodeName, aEC2[eErrorCode], iLineNo, iColumnNo, szErrorText);
          m_bCmdErr = true;
        }
        else {
          // LOG error
        }
        AM::vDeleteOperand(pOperand);
      }
      else if (pInfoSet->szBufferNm()->bSame(C_TDEF_EXECERRORS)) {

        if ( ! m_bExecErr) {
          Row *pInfoRow = pInfoSet->pFirstRow();

          Stz *sNodeName = pInfoRow->szColVal_ofs(0);
          eEC eErrorCode = (eEC) pInfoRow->iColVal_int(1);
          Stz *szErrorText = pInfoRow->szColVal_ofs(2);

          vTrace("\nerror: %s; %s; %s; \n"_Stz,
                 sNodeName, aEC2[eErrorCode], szErrorText);
          m_bExecErr = true;
        }
        else {
          // LOG error
        }
        AM::vDeleteOperand(pOperand);
      }
      else if (pInfoSet->szBufferNm()->bSame(C_TDEF_LOADRESULT)) {
        
        Row* pInfoRow = pInfoSet->pFirstRow();

        i8 iRowsProcessed = pInfoRow->iColVal_int(0);
        i8 iRowsInvalid   = pInfoRow->iColVal_int(1);
        i8 iRowsSkipped   = pInfoRow->iColVal_int(2);

        m_ixRowsProcessed += iRowsProcessed;
        m_ixRowsInvalid   += iRowsInvalid;
        m_ixRowsSkipped   += iRowsSkipped;

        //vTrace("INFOSET: %lld, %lld, %lld\n"_Stz,
        //       iRowsProcessed,iRowsInvalid,iRowsSkipped);

        AM::vDeleteOperand(pOperand);
      }

      else if (pInfoSet->szBufferNm()->bSame(C_TDEF_PROMPTOUT)) {

        Row* pInfoRow = pInfoSet->pFirstRow();
        Stz *szPromptLine = pInfoRow->szColVal_ofs(0);

        vTrace("%s\n"_Stz,szPromptLine);

        AM::vDeleteOperand(pOperand);
      }
      
      else {
        m_ixRowsProcessed += pInfoSet->nRows();
        if (m_bPrintEnabled) {
          vTrace("\n"_Stz);
          pInfoSet->vPrintRows(m_pStreamDef_result);
        }
        AM::vDeleteOperand(pOperand);
      }
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_MARKER: {
      Marker* pMarker = _pMarker(pOperand);
      
      eOM eOpMarker = pMarker->eOpMarker();

      switch (eOpMarker) {
          
        case OM_CANCELLING: {
          NEVER_REACH
          break;
        }

        case OM_CLEANING: {
          NEVER_REACH
          break;
        }

        case OM_DATAPULSE: {
          //vTrace("Pipeline Job: PULSEMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_DESTROYMARKER: {
          //vTrace("Pipeline Job: DESTROYMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }

        case OM_EACHSCANEOSMARKER: {
          //vTrace("Pipeline Job: EACHSCANEOSMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }

        case OM_EOSMARKER: {
          //vTrace("Pipeline Job: EOSMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_ERRORMARKER: {
          AM::vDeleteOperand(pOperand);
          break;
        }
         
        case OM_ERROREDMARKER: {
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_EXITING: {
          //vTrace("Pipeline Job: EXITMARKER\n"_Stz);
          //if ( ! bHasError )
          //  vTrace("success: %lld rows fetched\n"_Stz, *m_ixRowsProcessed);
          AM::vDeleteOperand(pOperand);
          m_bExitMarker = true;
          if ( !m_bCmdErr && !m_bExecErr )
            vPrintResult();
          m_bCmdErr = false;
          m_bExecErr = false;
          bContinue = false;
          goto L_EXIT;
          break;
        }

        case OM_PULSEDMARKER: {
          //vTrace("Pipeline Job: PULSEDMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }

        case OM_RETURNMARKER: {
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_RETURNEDMARKER: {
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_STARTMARKER: {
          //vTrace("Pipeline Job: STARTMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        default:
          ASSERT(0);
      }
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_ROWBUFFER: {
      RowBuffer* pRowBuffer = _pRowBuffer(pOperand);
//        if (AM::bDeveloper())
//          vTrace("[[RowBuffer %s ; Row=%lld]]\n"_Stz,
//                 spDataBuffer.sz(pRowBuffer),
//                 pRowBuffer->nRows());
      m_ixRowsProcessed += pRowBuffer->nRows();
      if (m_bPrintEnabled) {
        vTrace("\n"_Stz);
        pRowBuffer->vPrintRows(m_pFileOut,nullptr);
      }
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
            
    // -- -- -- -- -- --

    case CN_STREAMDEF: {
      //vTrace("Pipeline Job: STREAMMARKER\n"_Stz);
      if ( ! m_pStreamDef_result ) {
        m_pStreamDef_result = _pStreamDef(pOperand);
        
        // print the metadata
        if (AM::bMetadata()) {
          s4K sMetadata;
          m_pStreamDef_result->vToMetadata(sMetadata.szStz());
          vTrace("\n%s\n"_Stz, sMetadata.szStz());
        }
      }
      break;
    }

    case CN_STREAMDEFRHT: {
      NEVER_REACH;
      break;
    }

    // -- -- -- -- -- --
    case CN_STREAMSET: {
      StreamSet* pStreamSet = _pStreamSet(pOperand);
      ASSERT( ! pStreamSet->szBufferNm()->bSame(C_NO_NAME) );
      m_ixRowsProcessed += pStreamSet->nRows();
      if (m_bPrintEnabled) {
        vTrace("\n"_Stz);
        pStreamSet->vPrintRows(m_pStreamDef_result);
      }
      AM::vDeleteOperand(pOperand);
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
