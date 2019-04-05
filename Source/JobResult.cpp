/*============================================================================*/
/* JobResult.cpp -- Job Result                                                */
/*============================================================================*/

#include "StorageEngine.h"
#include "Jobs.h"

extern AttachMap*      gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* JobResult::operator new(size_t)
{
  vFatal("fatal: JobResult class cannot be new'd\n"_Stz);
  
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
  
  for (i8 i_Node = 0;i_Node < C_MAX_NODES; i_Node++) {
    m_xNode[i_Node].m_sResponse      = ""_Stz;
    m_xNode[i_Node].m_ixRowsProcessed = 0;
    m_xNode[i_Node].m_ixRowsInvalid   = 0;
    m_xNode[i_Node].m_ixRowsSkipped   = 0;
    m_xNode[i_Node].m_eErrorCode     = EC_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobResult::~JobResult(void) noexcept(false)
{
  if (m_pResponseVec)
    delete m_pResponseVec;
}

/*----------------------------------------------------------------------------*/
/* print errors                                                               */
/*----------------------------------------------------------------------------*/
void JobResult::vAddError(eEC eError)
{
  // get the node id
  i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
  
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

  RASSERT(iResponses == gpAttachMap->m_pGrid->m_nNodes);
  
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
    vTrace("error: (node%lld) %s\n"_Stz, gpAttachMap->m_pGrid->iThisNode(),
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
    vTrace("error: (node%lld) %s\n"_Stz, gpAttachMap->m_pGrid->iThisNode(),
           aEC[m_xResponse.m_eErrorCode]);
    return;
  }
  
  if (gpAttachMap->m_bMetadata)
    vTrace("metadata: %s\n"_Stz, m_sMetadata.aStz());
  
  if (*m_xResponse.m_ixRowsProcessed)
    vTrace("%s"_Stz, m_xResponse.m_sResponse.aChar());

    vTrace("success: %lld rows affected\n"_Stz, *m_xResponse.m_ixRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* print errors                                                               */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintNodeErrors(void)
{
  i8 iResponses = m_pResponseVec->nItems();
  RASSERT(iResponses == gpAttachMap->m_pGrid->m_nNodes);
  
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
  
  RASSERT(iResponses == gpAttachMap->m_pGrid->m_nNodes);
  
  if (gpAttachMap->m_bMetadata) {
    vTrace("metadata: %s\n"_Stz, m_sMetadata.aStz());
  }
  
  i8 iRowsProcessed = 0;
  
  for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
    vTrace("%s\n"_Stz, m_pResponseVec->pGet(i_Node)->m_sResponse.aChar());
    iRowsProcessed += *m_pResponseVec->pGet(i_Node)->m_ixRowsProcessed;
  }
  vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* print success                                                              */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintZeroRows(void)
{
  i8 iResponses = m_pResponseVec->nItems();
  
  if (m_bHasError) {
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[m_pResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    return;
  }
  
  RASSERT(iResponses == gpAttachMap->m_pGrid->m_nNodes);
  
  vTrace("success: 0 rows affected\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
