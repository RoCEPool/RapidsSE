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
void JobResult::vPrintErrors(void)
{
  i8 iResponses = m_pResponseVec->nItems();
  RASSERT(iResponses == gpAttachMap->m_pGrid->m_nNodes);
  
  RASSERT(m_bHasError);
  
  for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
    vTrace("error: (node%lld) %s\n"_Stz, i_Node, m_pResponseVec->pGet(i_Node)->m_eErrorCode);
  }
  
}

/*----------------------------------------------------------------------------*/
/* print result                                                               */
/*----------------------------------------------------------------------------*/
void JobResult::vPrintResult(void)
{
  i8 iResponses = m_pResponseVec->nItems();
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
void JobResult::vPrintSuccess(void)
{
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
