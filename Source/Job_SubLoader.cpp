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
  
  m_pLoadChunk  = new LoadChunk;
  m_bMoreChunks = false;
  
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
}


/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  s256 sPath;
  sPath = m_pJob_LoadTable->m_sFileName;
  
  // open the chunked file
  m_pLoadChunk->vOpenFile(sPath);
  
  // read first chunk
  m_bMoreChunks = bReadChunk();
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  //for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
  //  ASSERT(! m_pWorkingBuffer[i_Partition]);
  //}
  
  // close the file being loaded
  m_pLoadChunk->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
bool Job_SubLoader::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  Stz* aRawData;
  i8 iLength;
  bool bValid = false;
  bool bBlankLine;
  i8 i_Node = gpAttachMap->m_pGrid->m_iThisNode;
  
  Formatters* pFormatters = &m_pJob_LoadTable->m_xFormatters;
  Table* pTable = m_pJob_LoadTable->m_pTable;
  
  if (! m_bMoreChunks)
    return false;
  
  while (true) {
    
    // fetch next record from current chunk
    aRawData = m_pLoadChunk->aNextRecord(iLength,pFormatters);
    
    if (! aRawData) {
      // read next chunk
      m_bMoreChunks = bReadChunk();
      if (! m_bMoreChunks)
        return false;
      continue;
    }
    
    if (iLength > C_MAX_RECORD) {
      m_xNode[i_Node].m_ziRowsInvalid.iInc();
      if ( ! gpAttachMap->m_bMetadata ) {
        vWarn("Skipping row %.20s... - record in file > %lld bytes\n"_Stz,
              aRawData,
              C_MAX_RECORD);
      }
      return false;
    }
    
    bValid = bFormatRow(pTable, pFormatters, aRawData, iLength, bBlankLine);
    
    if (bValid) {
      //m_xNode[i_Node].m_ziRowsValid.iInc();
      return bValid;
    }
    
    if ( ! bBlankLine )
      m_xNode[i_Node].m_ziRowsInvalid.iInc();
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* process records                                                            */
/*----------------------------------------------------------------------------*/
bool Job_SubLoader::bProcessRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  Session* pSession = m_pJob_LoadTable->m_pSession;
  Table* pTable = m_pJob_LoadTable->m_pTable;
  i8 iParentJobId = m_pJob_LoadTable->m_iJobId;
  
  while (bFetchRecord()) {
      
    // attempt to add record into a buffer
    eEC eErrorCode = eInsertRow(pSession, pTable, DS_LOAD, iParentJobId);
  
    // was there an error ?
    if (eErrorCode != EC_NONE) {
      vTrace("Inserting row into buffer got error %s\n"_Stz, aEC[eErrorCode]);
      return false;
    }
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* read next chunk from file                                                  */
/*----------------------------------------------------------------------------*/
bool Job_SubLoader::bReadChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  i8 iSeek;
  i8 nBytes;
  i8 iChunk;
  i8 nChunks;
  
  LoadMap* pLoadMap = m_pJob_LoadTable->m_pLoadMap;
  
  // get information about next chunk
  for (;;) {
    
    bool bMapLoaded = pLoadMap->bDone();
    
    bool bMoreChunks = pLoadMap->bNextChunk(iSeek,nBytes,iChunk,nChunks);
    
    if (bMapLoaded && ! bMoreChunks)
      return false;
    
    if (bMoreChunks)
      break;
    
    //vTrace("Waiting for map\n"_Stz);
  }
  
  if (gpAttachMap->m_bProgress) {
    if (0 == iChunk % 100) {
      vInfo("read chunk %lld of %lld        \r"_Stz,iChunk,nChunks);
      fflush(stdout);
    }
  }
  
  // read the chunk
  m_pLoadChunk->vReadChunk(iSeek,nBytes);
  
  return true;
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
