/*============================================================================*/
/* JobResult.cpp -- Job Result                                                */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* JobResult::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: JobResult class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void JobResult::operator delete(void* aAddress)
{

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobResult::JobResult(void)
{
  m_sMetadata = ""_Stz;
  m_bHasError = false;
  
  //m_xResponse
  m_pResponseVec = nullptr;
  m_pBufferQueue = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobResult::~JobResult(void) noexcept(false)
{
  if (m_pResponseVec)
    delete m_pResponseVec;
  delete m_pBufferQueue;
}


/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print errors                                                               */
/*----------------------------------------------------------------------------*/
void JobResult::vAddError(eEC eError)
{
  // get the node id
  i8 iNode = AM::iThisNodeNo();
  
  // create a response vector
  m_pResponseVec = new ResponseVec();
  
  // create a new response
  Response* pResponse = new Response;
  
  m_bHasError = true;
  
  // set node and error in the response
  pResponse->m_iNode           = iNode;
  pResponse->m_eErrorCode      = EC_INVALID_COMMAND;
  
  // add the response to the response vector
  m_pResponseVec->vAdd(pResponse);
}

/*----------------------------------------------------------------------------*/
/* print load rows                                                            */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintLoadRows(void)
{
  i8 iResponses = m_pResponseVec->nItems();
  
  if ( m_bHasError ) {
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++)
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pResponseVec->pGet(i_Node)->m_eErrorCode]);
    return;
  }

  RASSERT(iResponses == AM::nDefinedNodes());
  
  i8 iRowsProcessed = 0;
  i8 iRowsInvalid   = 0;
  i8 iRowsSkipped   = 0;
  
  for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
    
    Response* pResponse = m_pResponseVec->pGet(i_Node);
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
/* print local errors                                                         */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintLocalErrors(void)
{
  if (m_bHasError) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::iThisNodeNo(),
           aEC[m_xResponse.m_eErrorCode]);
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* print local rows                                                           */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintLocalRows(void)
{
  if (m_bHasError) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::iThisNodeNo(),
           aEC[m_xResponse.m_eErrorCode]);
    return;
  }
  
  if (AM::bMetadata())
    vTrace("metadata: %s\n"_Stz, m_sMetadata.szStz());
  
  if (*m_xResponse.m_ixRowsProcessed)
    vTrace("%s"_Stz, m_xResponse.m_sResponse.azChar());

    vTrace("success: %lld rows affected\n"_Stz, *m_xResponse.m_ixRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* print errors                                                               */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintNodeErrors(void)
{
  i8 iResponses = m_pResponseVec->nItems();
  RASSERT(iResponses == AM::nDefinedNodes());
  
  if (m_bHasError) {
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
  }
  
}
/*----------------------------------------------------------------------------*/
/* print node rows                                                            */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintNodeRows(void)
{
  i8 iResponses = m_pResponseVec->nItems();
  
  if (m_bHasError) {
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    return;
  }
  
  RASSERT(iResponses == AM::pGrid()->nConnectedNodes());
  
  if (AM::bMetadata()) {
    vTrace("metadata: %s\n"_Stz, m_sMetadata.szStz());
  }
  
  i8 iRowsProcessed = 0;
  
  for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
    vTrace("%s\n"_Stz, m_pResponseVec->pGet(i_Node)->m_sResponse.azChar());
    iRowsProcessed += *m_pResponseVec->pGet(i_Node)->m_ixRowsProcessed;
  }
  vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* print zero rows                                                            */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintNodeZero(void)
{
  //! RAY HACK 5 NOV 2109 re universal job cf
  if (! m_pResponseVec) {
    vTrace("success: 0 rows affected\n"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iResponses = m_pResponseVec->nItems();
  
  if (m_bHasError) {
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    return;
  }
  
  RASSERT(iResponses == AM::pGrid()->nConnectedNodes());
  
  vTrace("success: 0 rows affected\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print local zero rows                                                      */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintLocalZero(void)
{
  if (m_bHasError) {
    vTrace("error: (node%lld) %s\n"_Stz,
           AM::iThisNodeNo(),
           aEC[m_xResponse.m_eErrorCode]);
    return;
  }
  
  vTrace("success: 0 rows affected\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print queue rows                                                            */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintQueueRows(void)
{
  if ( m_bHasError ) {
    i8 iResponses = m_pResponseVec->nItems();
    RASSERT(iResponses == AM::nDefinedNodes());
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    return;
  }
  
  StreamDef* pStreamDef = nullptr;
  
  for (;;) {
    
    Operand* pOperand = _pOperand(m_pBufferQueue->pPop());
    
    if (! pOperand) {
      AM::vNapUsec(250,CN_NONE);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
        // -- -- -- -- -- --
        
      case CN_CANCELMARKER: {
//        vTrace("Query %lld cancelled!!! (rows fetched so far %lld)\n"_Stz, iJobId(),
//               *m_xResponse.m_ixRowsProcessed);
        vTrace("Query cancelled!!! (rows fetched so far %lld)\n"_Stz,
               *m_xResponse.m_ixRowsProcessed);
        delete (_pCancelMarker(pOperand));
        goto L_EXIT;
        
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        m_xResponse.m_ixRowsProcessed += pCrwBuffer->nRowCount();
        pCrwBuffer->vPrintRows(pStreamDef);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_CSVBUFFER: {
        CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
        m_xResponse.m_ixRowsProcessed += pCsvBuffer->nRecords();
        pCsvBuffer->vPrintRows();
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_ENDMARKER: {
        // print the responses received
        vTrace("success: %lld rows fetched\n"_Stz, *m_xResponse.m_ixRowsProcessed);
        AM::vDeleteOperand(pOperand);
        goto L_EXIT;
        
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        m_xResponse.m_ixRowsProcessed += pEntBuffer->nItems();
        pEntBuffer->vPrintRows();
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand);
        m_xResponse.m_ixRowsProcessed += pIrwBuffer->nItems();
        pIrwBuffer->vPrintRows(pStreamDef);
        delete pIrwBuffer;
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        m_xResponse.m_ixRowsProcessed += pSrtBuffer->nItems();
        pSrtBuffer->vPrintRows(pStreamDef);
        delete pSrtBuffer;
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
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
  
  delete pStreamDef;
  return;
  
}

/*----------------------------------------------------------------------------*/
/* get response string                                                        */
/*----------------------------------------------------------------------------*/
Stz* JobResult::szResponse(void)
{
  Stz* szResponse = m_xResponse.m_sResponse;
  
  return szResponse;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
