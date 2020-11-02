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
  
  // first check for job errors
  if (m_bJobHasError != EC_NONE) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::iThisNodeNo(),
           aEC[m_xJobResponse.m_eErrorCode]);
    return;
  }
  
  switch (eResponse) {
    
    case RS_NONE                   : /* do nothing */              break;
    case RS_LOCAL_RESPONSE         : vPrintLocalResponse();        break;
    case RS_DIS2PC_RESPONSE        : vPrintDist2pcResponse();      break;
    
    //! TO GO
    case RS_OLD_JOB_RESPONSE       : vPrintOldJobResponses();      break;
      
    default:
      vTrace("Invalid response : %s\n"_Stz,aRS[eResponse]);
  }
}

/*----------------------------------------------------------------------------*/
/* print dist2pc response                                                     */
/*----------------------------------------------------------------------------*/
void Result::vPrintDist2pcResponse(void)
{
  i8 iResponses = m_pOriginResponseVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Errors
  if (m_bPartHasError) {
    vTrace("error: "_Stz);
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("node%lld: %s; "_Stz,
             m_pOriginResponseVec->pGet(i_Node)->m_iNode,
             aEC[m_pOriginResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    vTrace("\n"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Responses
  i8 iRowsProcessed = 0;
  i8 iRowsInvalid   = 0;
  i8 iRowsSkipped   = 0;
  
  for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
    
    Response* pResponse = m_pOriginResponseVec->pGet(i_Node);
    iRowsProcessed += *pResponse->m_ixRowsProcessed;
    iRowsInvalid   += *pResponse->m_ixRowsInvalid;
    iRowsSkipped   += *pResponse->m_ixRowsSkipped;
  }
  
  if (iRowsProcessed || iRowsInvalid || iRowsSkipped ) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // success / error
    if (iRowsInvalid || iRowsSkipped)
      vTrace("error: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             iRowsProcessed-iRowsSkipped,iRowsInvalid,iRowsSkipped);
    else
      vTrace("\nsuccess: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             iRowsProcessed-iRowsSkipped,iRowsInvalid,iRowsSkipped);
  }
  else {
    vTrace("\nsuccess: %lld rows fetched\n"_Stz, *m_xJobResponse.m_ixRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* print job response                                                         */
/*----------------------------------------------------------------------------*/
void Result::vPrintLocalResponse(void)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Errors should already be handled
  ASSERT( ! m_bJobHasError );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // success
  vTrace("\nsuccess: %lld rows affected\n"_Stz, *m_xJobResponse.m_ixRowsProcessed);
}

//! TO GO
/*----------------------------------------------------------------------------*/
/* print participant node responses                                           */
/*----------------------------------------------------------------------------*/
void Result::vPrintOldJobResponses(void)
{
  i8 iResponses = m_pOriginResponseVec->nItems();
  RASSERT(iResponses == AM::pGrid()->nConnectedNodes());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Errors
  if (m_bPartHasError) {
    vTrace("error: "_Stz);
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("node%lld: %s; "_Stz, i_Node, aEC[m_pOriginResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    vTrace("\n"_Stz);
    return;
  }
  
  i8 iRowsProcessed = 0;
  
  for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
    
    char* azRsp = m_pOriginResponseVec->pGet(i_Node)->m_sResponse.azChar();
    if (*azRsp)
      vTrace("%s\n"_Stz,azRsp);
    
    i8 nRows = *m_pOriginResponseVec->pGet(i_Node)->m_ixRowsProcessed;
    iRowsProcessed += nRows;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Success
  vTrace("\nsuccess: %lld rows affected\n"_Stz, iRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* print buffer queue results                                                 */
/*----------------------------------------------------------------------------*/
void Result::vProcessBufferQueue(void)
{
  SP_DataBuffer spDataBuffer;
  
  StreamDef* pStreamDef = nullptr;
  bool bHasError = false;
  i8 iTotalUnits = C_UNKNOWN;
  i8 iProgressUnits = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait till the job has a buffer queue
  while ( m_pOriginBufferQueue == nullptr ) {
    // do something else release version optimizes this out
    
    // exit early if job has errors
    if (m_bJobHasError != EC_NONE) {
//      vTrace("error: (node%lld) %s\n"_Stz, AM::iThisNodeNo(),
//             aEC[m_xJobResponse.m_eErrorCode]);
      goto L_EXIT;
    }
    
    AM::vNapUsec(1000,CN_RESULT);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process the buffer queue
  for (;;) {
    
    Operand* pOperand = _pOperand(m_pOriginBufferQueue->pPop());
    
    if (! pOperand) {
      AM::vNapUsec(1000,CN_RESULT);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pCrwBuffer->nRowCount();
        vTrace("\n"_Stz);
        pCrwBuffer->vPrintRows(pStreamDef);
        AM::vDeleteOperand(pOperand);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_CSVBUFFER: {
        CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pCsvBuffer->nRecords();
        vTrace("\n"_Stz);
        pCsvBuffer->vPrintRecords();
        AM::vDeleteOperand(pOperand);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pEntBuffer->nItems();
        vTrace("\n"_Stz);
        pEntBuffer->vPrintRows();
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
        m_xJobResponse.m_ixRowsProcessed += pInfoSet->nInfoRows();
        vTrace("\n"_Stz);
        pInfoSet->vPrintRows(pStreamDef);
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
           
          case OM_ERROREDMARKER: {
            AM::vDeleteOperand(pOperand);
            break;
          }
            
          case OM_EXITMARKER: {
            //vTrace("Pipeline Job: EXITMARKER\n"_Stz);
            //if ( ! bHasError )
            //  vTrace("success: %lld rows fetched\n"_Stz, *m_xJobResponse.m_ixRowsProcessed);
            AM::vDeleteOperand(pOperand);
            goto L_EXIT;
            break;
          }
            
          case OM_PULSEMARKER: {
            //vTrace("Pipeline Job: PULSEMARKER\n"_Stz);
            AM::vDeleteOperand(pOperand);
            break;
          }
            
          case OM_PULSEDMARKER: {
            //vTrace("Pipeline Job: PULSEDMARKER\n"_Stz);
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
        
      case CN_RSBBUFFER: {
        RsbBuffer* pRsbBuffer = _pRsbBuffer(pOperand);
//        if (AM::bDeveloper())
//          vTrace("[[RsbBuffer %s ; Rsbs=%lld]]\n"_Stz,
//                 spDataBuffer.sz(pRsbBuffer),
//                 pRsbBuffer->nRsb());
        m_xJobResponse.m_ixRowsProcessed += pRsbBuffer->nRsb();
        vTrace("\n"_Stz);
        pRsbBuffer->vPrintRows();
        AM::vDeleteOperand(pOperand);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        m_xJobResponse.m_ixRowsProcessed += pSrtBuffer->nItems();
        vTrace("\n"_Stz);
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
            s4K sMetadata;
            pStreamDef->vToMetadata(sMetadata.szStz());
            vTrace("\n%s\n"_Stz, sMetadata.szStz());
          }
        }
        break;
      }
        
      // -- -- -- -- -- --
      case CN_STREAMSET: {
        StreamSet* pStreamSet = _pStreamSet(pOperand);
        ASSERT( ! pStreamSet->szInfoSetNm()->bSame(C_NO_NAME) );
        m_xJobResponse.m_ixRowsProcessed += pStreamSet->nInfoRows();
        vTrace("\n"_Stz);
        pStreamSet->vPrintRows(pStreamDef);
        AM::vDeleteOperand(pOperand);
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
