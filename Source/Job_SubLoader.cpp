/*============================================================================*/
/* Job_SubLoader.cpp -- Table SubLoader Job                                   */
/*============================================================================*/

// Job Short: SubLoader

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubLoader =
{
  "Job_SubLoader"_Stz,    // name of the job class
  CN_JOB_SUBLOADER,       // class enum
  JK_CHILD,               // job kind
  "table sub-loader"_Stz, // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "subLoader_run"_Stz,    // the _run task seed
  nullptr,                // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  vJob_SubLoader_pre,     // prompt pre-process function
  nullptr,                // prompt post-process function
  pJob_SubLoader_new,     // job new function
  vJob_SubLoader_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_SubLoader_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_SubLoader_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubPublish_class;
  
  return &xJobClass_SubLoader;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubLoader* JobModel::_pJob_SubLoader(void* aAddress)
{
  Job_SubLoader* pJob_SubLoader = reinterpret_cast<Job_SubLoader*>(aAddress);
  
  RASSERT_TYPE(pJob_SubLoader,CN_JOB_SUBLOADER);
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubLoader::operator new(size_t)
{
  Job_SubLoader* pJob_SubLoader =
    static_cast<Job_SubLoader*>(HeapMem::pAllocate(CN_JOB_SUBLOADER));
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubLoader::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::operator delete(void* aAddress)
{
  Job_SubLoader* pJob_SubLoader = static_cast<Job_SubLoader*>(aAddress);
  
  HeapMem::vFree(pJob_SubLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubLoader::Job_SubLoader(Interface::Session* pSession)
: Job(CN_JOB_SUBLOADER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  m_pJob_LoadTable = nullptr;
  m_pLoadChunk  = nullptr;
  
  _checkInitialised_(CN_JOB_SUBLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubLoader::~Job_SubLoader(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  if (m_pLoadChunk)
    delete m_pLoadChunk;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pObject);

  pJob_SubLoader->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get job_loadtable                                                          */
/*----------------------------------------------------------------------------*/
Job_LoadTable* Job_SubLoader::pJob_LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  return m_pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* set job_loadtable                                                          */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vJob_LoadTable(Job_LoadTable* pJob_LoadTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  m_pJob_LoadTable = pJob_LoadTable;
  
  m_pLoadChunk  = new LoadChunk(&m_pJob_LoadTable->m_xFormatters);
}


/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
bool Job_SubLoader::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  s256 sPath;
  sPath = m_pJob_LoadTable->m_sFileName;
  
  // open the chunked file
  bool bOpened = m_pLoadChunk->bOpenRead(sPath);
  
  // set the table and parentJob in the writeBehind Job
  JobWriteBehind::m_pTable = m_pJob_LoadTable->m_pTable;
  JobWriteBehind::m_pParentJob = (Job*)m_pJob_LoadTable;
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* chunk records                                                               */
/*----------------------------------------------------------------------------*/
bool Job_SubLoader::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  i8 i_Node = gpAttachMap->m_pGrid->m_iThisNode;
  Session* pSession = m_pJob_LoadTable->m_pSession;
  Table* pTable = m_pJob_LoadTable->m_pTable;
  i8 iParentJobId = m_pJob_LoadTable->m_iJobId;
  Formatters* pFormatters = &m_pJob_LoadTable->m_xFormatters;

  i8 i_Seek;
  i8 nChunkBytes;
  
  Stz* aRecord;
  i8 nRecordBytes;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read next mapped chunk
  if (! m_pJob_LoadTable->m_pLoadMap->bNextChunk(i_Seek,nChunkBytes))
    return false;
  
  m_pLoadChunk->vReadSpecificChunk(i_Seek,nChunkBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process the records in the chunk
  for (;;) {
    
    // fetch next record from current chunk
    bool bHaveRecord = m_pLoadChunk->bGetRecord(&aRecord,nRecordBytes);
    if (! bHaveRecord)
      return true;
    
    // process that record into write behind
    eEC eError = eProcessRecord(pSession, iParentJobId, pTable,
                                pFormatters,
                                aRecord, nRecordBytes, DS_LOAD);
    
    if (EC_NONE == eError) {
      m_xNode[i_Node].m_ziRowsProcessed.iInc();
      continue;
    }
    
    if (EC_BLANKLINE == eError)
      continue;

    if (EC_INVALID_VALUE == eError) {
      m_xNode[i_Node].m_ziRowsInvalid.iInc();
      continue;
    }
    
    if (EC_MAX_ROW_LENGTH == eError) {
      m_xNode[i_Node].m_ziRowsInvalid.iInc();
      continue;
    }
    
    return false;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  

  // handle any dangling buffers
  vHandleDanglingBuffers(m_pSession, m_pJob_LoadTable->m_pTable,
                         DS_LOAD, m_pJob_LoadTable->m_iJobId);
  
  i8 i_Node = gpAttachMap->m_pGrid->m_iThisNode;
  
  // add invalid rows into parent load table job
  i8 iRowsInvalid = *m_xNode[i_Node].m_ziRowsInvalid;
  m_pJob_LoadTable->m_xNode[i_Node].m_ziRowsInvalid += iRowsInvalid;
  
  // add processed rows into parent load table job
  i8 iRowsProcessed = *m_xNode[i_Node].m_ziRowsProcessed;
  m_pJob_LoadTable->m_xNode[i_Node].m_ziRowsProcessed += iRowsProcessed;
  
  // close the file being loaded
  m_pLoadChunk->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubLoader::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::vJob_SubLoader_pre(Interface::Session* pSession)
{
  Job_SubLoader* pJob_SubLoader = new Job_SubLoader(pSession);
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_SubLoader_new(Job* pArg)
{
  Job_SubLoader* pJob_SubLoader = new Job_SubLoader(nullptr);
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubLoader_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pJob);
  
  delete pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubLoader_show(Job* pJob)
{
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pJob);

  pJob_SubLoader->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
