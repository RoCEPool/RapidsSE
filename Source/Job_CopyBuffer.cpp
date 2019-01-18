/*============================================================================*/
/* Job_CopyBuffer.cpp -- CopyBuffer Job                                       */
/*============================================================================*/

// Job Short: SubInserter

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_CopyBuffer =
{
  "Job_CopyBuffer"_Stz,     // name of the job class
  CN_JOB_COPYBUFFER,       // class enum
  JK_LOCAL,                 // job kind
  "copy recordBuffer"_Stz,  // comment about the job
  ""_Stz,                   // the prompt command
  ""_Stz,                   // the second word
  nullptr,                  // the _run task seed
  "Job_CopyBuffer_req"_Stz, // the _req task seed
  nullptr,                  // the _rcv task seed
  nullptr,                  // the _exe task seed
  nullptr,                  // the _end task seed
  nullptr,                  // the _can task seed
  false,                    // check resources?
  false,                    // detached job?
  false,                    // buffer attached?
  JT_NONE,                  // job trace
  pJob_CopyBuffer_pre,      // prompt pre-process function
  vJob_CopyBuffer_post,     // prompt post-process function
  pJob_CopyBuffer_new,      // job new function
  vJob_CopyBuffer_delete,   // job delete function
  nullptr,                  // job pre-send function
  nullptr,                  // job post-receive function
  vJob_CopyBuffer_show,     // job show function
  
  // help and example
  true,
  ""_Stz,
  ""_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_CopyBuffer_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_CreateCatalog_class;
  
  return &xJobClass_CopyBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CopyBuffer* JobModel::_pJob_CopyBuffer(void* aAddress)
{
  Job_CopyBuffer* pJob_CopyBuffer = reinterpret_cast<Job_CopyBuffer*>(aAddress);
  
  RASSERT_TYPE(pJob_CopyBuffer,CN_JOB_COPYBUFFER);
  
  return pJob_CopyBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CopyBuffer::operator new(size_t)
{
  Job_CopyBuffer* pJob_CopyBuffer =
    static_cast<Job_CopyBuffer*>(HeapMem::pAllocate(CN_JOB_COPYBUFFER));
  
  return pJob_CopyBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CopyBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CopyBuffer::operator delete(void* aAddress)
{
  Job_CopyBuffer* pJob_CopyBuffer = static_cast<Job_CopyBuffer*>(aAddress);
  
  HeapMem::vFree(pJob_CopyBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CopyBuffer::Job_CopyBuffer(Interface::Session* pSession)
: Job(CN_JOB_COPYBUFFER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COPYBUFFER,_FOOTER_);
  
  m_pJob_SubInserter = nullptr;
  m_pRecordBuffer = nullptr;
  
  _checkInitialised_(CN_JOB_COPYBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_COPYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CopyBuffer::~Job_CopyBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COPYBUFFER,_FOOTER_);
  delete m_pRecordBuffer;
  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CopyBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_CopyBuffer* pJob_CopyBuffer = _pJob_CopyBuffer(pObject);

  pJob_CopyBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy the buffer                                                            */
/*----------------------------------------------------------------------------*/
void Job_CopyBuffer::vCopy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COPYBUFFER,_FOOTER_);
  
  //m_pRecordBuffer->vShow();
  //MemLib::vHexDump(m_pRecordBuffer, 500);
  
  // for each record in the record buffer
  char* aRecord = nullptr;
  
  for (aRecord = m_pRecordBuffer->aFirstRecord();
       aRecord;
       aRecord = m_pRecordBuffer->aNextRecord(aRecord)) {
    
    // get the length of the record
    u4 uRecordLength = *((u4*)aRecord);
    
    // check length of record
    if (uRecordLength > C_MAX_RECORD) {
      //eErrorCode = EC_MAX_ROW_LENGTH;
      //return eErrorCode;
      //RASSERT(0);
      continue;
    }
    
    // format the row
    bool bBlankLine;
    Stz* aData = aAdd<Stz*>(aRecord, 4);
    
    m_pJob_SubInserter->vGrabWrite();
    bool bValid = m_pJob_SubInserter->bFormatRow(aData, uRecordLength,
                                                 m_aRow,
                                                 m_nRowBytes,
                                                 bBlankLine);
    
    // is the record invalid ?
    if (! bValid) {
      vTrace("Error inserting row %lld %s\n"_Stz, uRecordLength, aData);
      //RASSERT(0);
      //eErrorCode = EC_INVALID_VALUE;
      //return eErrorCode;
      m_pJob_SubInserter->vDropWrite();
      continue;
    }
    
    // insert row into a working buffer in job InsertRow
    eEC eErrorCode = m_pJob_SubInserter->eInsertRow(m_aRow, m_nRowBytes);
    RASSERT(eErrorCode == EC_NONE);
    m_pJob_SubInserter->vDropWrite();
  }
  
  ++m_pJob_SubInserter->m_ziCopiedBuffers;
  
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CopyBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COPYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CopyBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COPYBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_CopyBuffer_pre(Interface::Session* pSession)
{
  Job_CopyBuffer* pJob_CopyBuffer = new Job_CopyBuffer(pSession);
  
  return pJob_CopyBuffer;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_CopyBuffer_post(Interface::Session* pSession, Job* pJob)
{
  Job_CopyBuffer* pJob_CopyBuffer = _pJob_CopyBuffer(pJob);
  
  if ( pJob_CopyBuffer->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_CopyBuffer->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_CopyBuffer_new(Job* pArg)
{
  Job_CopyBuffer* pJob_CopyBuffer = new Job_CopyBuffer(nullptr);
  
  return pJob_CopyBuffer;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_CopyBuffer_delete(Interface::Session* pSession, Job* pJob)
{
  Job_CopyBuffer* pJob_CopyBuffer = _pJob_CopyBuffer(pJob);
  
  //vTrace("DELETING INSERTROW\n"_Stz);
  delete pJob_CopyBuffer;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_CopyBuffer_show(Job* pJob)
{
  Job_CopyBuffer* pJob_CopyBuffer = _pJob_CopyBuffer(pJob);

  pJob_CopyBuffer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
