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
#include "Response.hpp"
#include "Output.hpp"

#include "Result.hpp"
#include "Job.hpp"

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
  m_bExitMarker       = false;
  m_bCancelMarker     = false;


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
    RASSERT(! m_pOutputQueue->iCount());
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
  
  RASSERT(! m_pOutputQueue->iCount());
  
  RASSERT(! m_bExitMarker);
  RASSERT(! m_bCancelMarker);
}

/*----------------------------------------------------------------------------*/
/* drain the result/output queue                                              */
/*----------------------------------------------------------------------------*/
void Output::vDrainBufferQueue(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);

  while (! m_bExitMarker   &&
         ! m_bCancelMarker &&
         ! pSession->m_pSsnJob->m_xResult.m_xJobResponse.m_bJobHasError) {
    
    Operand* pOperand = pPopBufferQueue_blocking(pSession);
    
    if (! pOperand) {
      AM::vNapUsec(1000,CN_OUTPUT);
      continue;
    }
    
    eCN eOperandClass = pOperand->eGetClassNo();

    if (eOperandClass == CN_MARKER) {
      Marker *pMarker = _pMarker(pOperand);
      eOM eOpMarker = pMarker->eOpMarker();

      if (eOpMarker == OM_CANCELMARKER)
        m_bExitMarker = true;
      else
      if (eOpMarker == OM_EXITMARKER)
        m_bCancelMarker = true;
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
/* process the result/output queue                                            */
/*----------------------------------------------------------------------------*/
void Output::vProcessBufferQueue(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  Operand* pOperand;
  
  while ( (pOperand = pPopBufferQueue_blocking(pSession)) ) {

    if (! bProcessOperand(pSession,pOperand))
      break;

  }
  
  vDrainBufferQueue(pSession);
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
    vTrace("\nsuccess: %lld rows fetched\n"_Stz,
           *m_ixRowsProcessed);
  }

  m_ixRowsProcessed = 0;
  m_ixRowsInvalid   = 0;
  m_ixRowsSkipped   = 0;
}

/*----------------------------------------------------------------------------*/
/* pop next operand from result queue                                         */
/*----------------------------------------------------------------------------*/
Operand* Output::pPopBufferQueue_blocking(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  for (;;) {
    
    //if (pSession->m_pSsnJob->m_xResult.m_bJobHasError) {
      //vDrainBufferQueue(pSession);
      //return nullptr;
    //}
    
    Operand* pOperand = _pOperand(pSession->m_pOutput->m_pOutputQueue->pPop());
    
    if (pOperand)
      return pOperand;
    
    AM::vNapUsec(1000,CN_OUTPUT);
    
    if (pSession->m_pSsnJob->m_xResult.m_xJobResponse.m_bJobHasError)
      return nullptr;
  }
  
}

/*----------------------------------------------------------------------------*/
/* process an operand from the result/output queue                            */
/*----------------------------------------------------------------------------*/
bool Output::bProcessOperand(Session* pSession, Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  

  bool bContinue = true;
  
  //SP_DataBuffer spDataBuffer;
    
  eCN eOperandClass = pOperand->eGetClassNo();
  
  switch (eOperandClass) {
      
    // -- -- -- -- -- --
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
      m_ixRowsProcessed += pCrwBuffer->nRowCount();
      vTrace("\n"_Stz);
      pCrwBuffer->vPrintRows(m_pStreamDef_result);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
      m_ixRowsProcessed += pCsvBuffer->nRecords();
      vTrace("\n"_Stz);
      pCsvBuffer->vPrintRecords();
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_ENTBUFFER: {
      EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
      m_ixRowsProcessed += pEntBuffer->nItems();
      vTrace("\n"_Stz);
      pEntBuffer->vPrintRefdRows();
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
      ASSERT( ! pInfoSet->szInfoSetNm()->bSame(C_NO_NAME) );
      if (pInfoSet->szInfoSetNm()->bSame(C_TDEF_COMMANDERRORS)) {

        Row *pInfoRow = pInfoSet->aFirstRow();

        Stz* sNodeName  = pInfoRow->szColVal_ofs(0);
        eEC eErrorCode  = (eEC)pInfoRow->iColVal_int(1);
        i8 iLineNo      = pInfoRow->iColVal_int(2);
        i8 iColumnNo    = pInfoRow->iColVal_int(3);
        Stz* sErrorText = pInfoRow->szColVal_ofs(4);

        vTrace("%s: Error : %s ; Line: %lld; Column: %lld; %s; \n"_Stz,
               sNodeName, aEC[eErrorCode] , iLineNo, iColumnNo, sErrorText);

        AM::vDeleteOperand(pOperand);
      }
      else if (pInfoSet->szInfoSetNm()->bSame(C_TDEF_EXECERRORS)) {

        Row *pInfoRow = pInfoSet->aFirstRow();

        Stz* sNodeName  = pInfoRow->szColVal_ofs(0);
        eEC eErrorCode  = (eEC)pInfoRow->iColVal_int(1);
        Stz* sErrorText = pInfoRow->szColVal_ofs(2);

        vTrace("%s: Error : %s ; %s; \n"_Stz,
               sNodeName, aEC2[eErrorCode] , sErrorText);

        AM::vDeleteOperand(pOperand);
      }
      else if (pInfoSet->szInfoSetNm()->bSame(C_TDEF_LOADRESULT)) {
        
        Row* pInfoRow = pInfoSet->aFirstRow();

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

      else {
        m_ixRowsProcessed += pInfoSet->nInfoRows();
        vTrace("\n"_Stz);
        pInfoSet->vPrintRows(m_pStreamDef_result);
        AM::vDeleteOperand(pOperand);
      }
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_MARKER: {
      Marker* pMarker = _pMarker(pOperand);
      
      eOM eOpMarker = pMarker->eOpMarker();

      switch (eOpMarker) {
          
        case OM_CANCELMARKER: {
          vTrace("Query cancelled!!! (rows fetched so far %lld)\n"_Stz,
                 *m_ixRowsProcessed);
          AM::vDeleteOperand(pOperand);
          m_bExitMarker = true;
          bContinue = false;
          goto L_EXIT;
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
          
        case OM_EXITMARKER: {
          //vTrace("Pipeline Job: EXITMARKER\n"_Stz);
          //if ( ! bHasError )
          //  vTrace("success: %lld rows fetched\n"_Stz, *m_ixRowsProcessed);
          AM::vDeleteOperand(pOperand);
          m_bExitMarker = true;
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
      vTrace("\n"_Stz);
      pRowBuffer->vPrintRows();
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
      
    // -- -- -- -- -- --
    case CN_STREAMSET: {
      StreamSet* pStreamSet = _pStreamSet(pOperand);
      ASSERT( ! pStreamSet->szInfoSetNm()->bSame(C_NO_NAME) );
      m_ixRowsProcessed += pStreamSet->nInfoRows();
      vTrace("\n"_Stz);
      pStreamSet->vPrintRows(m_pStreamDef_result);
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
