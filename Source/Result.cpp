/*============================================================================*/
/* Result.hpp -- Result                                                       */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Result* Jobs::_pResult(void* aAddr)
{
  Result* pResult = reinterpret_cast<Result*>(aAddr);
  
  RASSERT_TYPE(pResult,CN_RESULT);
  
  return pResult;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Result::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_RESULT);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Result::operator delete(void* pAddress)
{
  Result* pResult = reinterpret_cast<Result*>(pAddress);
  
  HeapMem::vFree(pResult);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Result::Result(void)
: Object(CN_RESULT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT);
  
  m_sMetadata          = ""_Stz;
  
  m_eSyntaxError       = EC_NONE;
  //m_xJobResponse
  m_bJobHasError       = false;
  
  m_pOriginResponseVec = nullptr;
  m_bPartHasError      = false;
  
  m_pOriginBufferQueue = nullptr;
  
  _checkInitialised_(CN_RESULT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Result::~Result(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
  
  if ( m_pOriginResponseVec)
    delete m_pOriginResponseVec;
  
  if (m_pOriginBufferQueue)
    delete m_pOriginBufferQueue;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
void Result::vPrint(eRS eResponse)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
  
  // check for syntax errors
  if (m_eSyntaxError != EC_NONE) {
    vTrace("error: %s\n"_Stz, aEC[m_eSyntaxError]);
    return;
  }
  
  switch (eResponse) {
      
    case RS_NONE         : /* do nothing */      break;
    case RS_DATA_ROWS    : RASSERT(0);           break;
    case RS_LOAD_ROWS    : vPrintLoadRows();     break;
    case RS_LOCAL_ROWS   : vPrintLocalRows();    break;
    case RS_LOCAL_ZERO   : vPrintLocalZero();    break;
    case RS_NODE_ROWS    : vPrintNodeRows();     break;
    case RS_QUEUE_ROWS   : vPrintQueueRows();    break;
      
    default:
      vTrace("Invalid response : %s\n"_Stz,aRS[eResponse]);
  }
}

/*----------------------------------------------------------------------------*/
/* print load rows                                                            */
/*----------------------------------------------------------------------------*/
void Result::vPrintLoadRows(void)
{
  i8 iResponses = m_pOriginResponseVec->nItems();
  
  if ( m_bPartHasError ) {
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++)
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pOriginResponseVec->pGet(i_Node)->m_eErrorCode]);
    return;
  }
  
  RASSERT(iResponses == AM::nDefinedNodes());
  
  i8 iRowsProcessed = 0;
  i8 iRowsInvalid   = 0;
  i8 iRowsSkipped   = 0;
  
  for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
    
    Response* pResponse = m_pOriginResponseVec->pGet(i_Node);
    iRowsProcessed += *pResponse->m_ixRowsProcessed;
    iRowsInvalid   += *pResponse->m_ixRowsInvalid;
    iRowsSkipped   += *pResponse->m_ixRowsSkipped;
  }
  if (iRowsInvalid || iRowsSkipped)
    vTrace("error: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
           iRowsProcessed-iRowsSkipped,iRowsInvalid,iRowsSkipped);
  else
    vTrace("success: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
           iRowsProcessed-iRowsSkipped,iRowsInvalid,iRowsSkipped);
  
}

/*----------------------------------------------------------------------------*/
/* print local rows                                                           */
/*----------------------------------------------------------------------------*/
void Result::vPrintLocalRows(void)
{
  if (m_bJobHasError) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::iThisNodeNo(),
           aEC[m_xJobResponse.m_eErrorCode]);
    return;
  }
  
  if (AM::bMetadata() && ! m_sMetadata.szStz()->bEqual(""_Stz))
    vTrace("metadata: %s\n"_Stz, m_sMetadata.szStz());
  
  if (*m_xJobResponse.m_ixRowsProcessed)
    vTrace("%s"_Stz, m_xJobResponse.m_sResponse.azChar());
  
  vTrace("success: %lld rows affected\n"_Stz, *m_xJobResponse.m_ixRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* print node rows                                                            */
/*----------------------------------------------------------------------------*/
void Result::vPrintNodeRows(void)
{
  i8 iResponses = m_pOriginResponseVec->nItems();

  if (m_bPartHasError) {
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pOriginResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    return;
  }
  
  RASSERT(iResponses == AM::pGrid()->nConnectedNodes());
  
  if (AM::bMetadata() && ! m_sMetadata.szStz()->bEqual(""_Stz)) {
    vTrace("metadata: %s\n"_Stz, m_sMetadata.szStz());
  }
  
  i8 iRowsProcessed = 0;
  
  for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
    vTrace("%s\n"_Stz, m_pOriginResponseVec->pGet(i_Node)->m_sResponse.azChar());
    iRowsProcessed += *m_pOriginResponseVec->pGet(i_Node)->m_ixRowsProcessed;
  }
  vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* print local zero rows                                                      */
/*----------------------------------------------------------------------------*/
void Result::vPrintLocalZero(void)
{
  if (m_bPartHasError) {
    vTrace("error: (node%lld) %s\n"_Stz,
           AM::iThisNodeNo(),
           aEC[m_xJobResponse.m_eErrorCode]);
    return;
  }
  
  vTrace("success: 0 rows affected\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print queue rows                                                            */
/*----------------------------------------------------------------------------*/
void Result::vPrintQueueRows(void)
{
  //  if ( m_bHasError ) {
  //    i8 iResponses = m_pOriginResponseVec->nItems();
  //    RASSERT(iResponses == AM::nDefinedNodes());
  //    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
  //      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pOriginResponseVec->pGet(i_Node)->m_eErrorCode]);
  //    }
  //    return;
  //  }
  
  StreamDef* pStreamDef = nullptr;
  bool bHasError = false;
  
  for (;;) {
    
    Operand* pOperand = _pOperand(m_pOriginBufferQueue->pPop());
    
    if (! pOperand) {
      // invalid command ? exit early
      bool bInvalidCommand = false;
      if (m_bPartHasError) {
        i8 iResponses = m_pOriginResponseVec->nItems();

        for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
          if (m_pOriginResponseVec->pGet(i_Node)->m_eErrorCode == EC_INVALID_COMMAND) {
            bInvalidCommand = true;
            break;
          }
        }
      }

      if (bInvalidCommand)
        break;
      AM::vNapUsec(250,CN_NONE);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
        // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pCrwBuffer->nRowCount();
        pCrwBuffer->vPrintRows(pStreamDef);
        AM::vDeleteOperand(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_CSVBUFFER: {
        CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pCsvBuffer->nRecords();
        pCsvBuffer->vPrintRecords();
        AM::vDeleteOperand(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pEntBuffer->nItems();
        pEntBuffer->vPrintRows();
        AM::vDeleteOperand(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pIrwBuffer->nItems();
        pIrwBuffer->vPrintRows(pStreamDef);
        AM::vDeleteOperand(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
      case CN_MARKER: {
        Marker* pMarker = _pMarker(pOperand);
        
        switch (pMarker->eOpMarker()) {
            
          case OM_CANCELMARKER: {
            vTrace("Query cancelled!!! (rows fetched so far %lld)\n"_Stz,
                   *m_xJobResponse.m_ixRowsProcessed);
            AM::vDeleteOperand(pOperand);
            goto L_EXIT;
            break;
          }
            
          case OM_EOSMARKER: {
            //vTrace("Pipeline Job: EOSMARKER\n"_Stz);
            AM::vDeleteOperand(pOperand);
            break;
          }
            
          case OM_ERRORMARKER: {
            AM::vDeleteOperand(pOperand);
            bHasError = true;
            break;
          }
            
          case OM_PULSEMARKER: {
            //vTrace("Pipeline Job: PULSEMARKER\n"_Stz);
            AM::vDeleteOperand(pOperand);
            break;
          }
            
          case OM_STARTMARKER: {
            //vTrace("Pipeline Job: STARTMARKER\n"_Stz);
            AM::vDeleteOperand(pOperand);
            break;
          }
            
          case OM_STOPMARKER: {
            //vTrace("Pipeline Job: STOPMARKER\n"_Stz);
            if ( ! bHasError )
              vTrace("success: %lld rows fetched\n"_Stz, *m_xJobResponse.m_ixRowsProcessed);
            AM::vDeleteOperand(pOperand);
            goto L_EXIT;
            break;
          }
            
          default:
            ASSERT(0);
        }
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pSrtBuffer->nItems();
        pSrtBuffer->vPrintRows(pStreamDef);
        AM::vDeleteOperand(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        //vTrace("Pipeline Job: STREAMMARKER\n"_Stz);
        if ( ! pStreamDef ) {
          pStreamDef = _pStreamDef(pOperand);
          
          // print the metadata
          if (AM::bMetadata()) {
            pStreamDef->vToMetadata(m_sMetadata.szStz());
            vTrace("%s\n"_Stz, m_sMetadata.szStz());
          }
        }
        break;
      }
        
        // -- -- -- -- -- --
        
      default:
        RASSERT(0);
        break;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  
  if (pStreamDef)
    AM::vDeleteOperand(pStreamDef);
  return;
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Result::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
