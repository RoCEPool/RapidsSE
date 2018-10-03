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
  false,                  // job is not prompt enabled
  "<help>"_Stz,           // help
  "<example>"_Stz,        // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_SubLoader_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubTruncate_class;
  
  return &xJobClass_SubLoader;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubLoader* JobsModel::_pJob_SubLoader(void* aAddress)
{
  Job_SubLoader* pJob_SubLoader = reinterpret_cast<Job_SubLoader*>(aAddress);
  
  RASSERT( !pJob_SubLoader || CN_JOB_SUBLOADER == pJob_SubLoader->eClass());
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubLoader::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_SubLoader* pJob_SubLoader =
    static_cast<Job_SubLoader*>(HeapMem::pAllocate(CN_JOB_SUBLOADER));
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubLoader::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
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
Job_SubLoader::Job_SubLoader(Interface::SEitf* pSEitf)
: Job(CN_JOB_SUBLOADER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  m_pJob_LoadTable = nullptr;
  m_pLoaderSub     = nullptr;
  
  m_pLoadChunk  = new LoadChunk;
  m_bMoreChunks = false;
  
  //m_aRecord
  m_iLength     = 0;
  m_iPartition  = 0;
  
  m_iValidRecords   = 0;
  m_iInvalidRecords = 0;

  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pWorkingBuffer[i_Partition]   = nullptr;
    m_nBuffersLoadable[i_Partition] = 0;
  }
  
  _checkInitialised_(CN_JOB_SUBLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubLoader::~Job_SubLoader(void)
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
  RASSERT(CN_JOB_SUBLOADER == eClass);
  RASSERT(pObject->eClass() == CN_JOB_SUBLOADER);
  
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
/* set loadersub                                                              */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vSubLoader(SubLoader* pSubLoader)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  m_pLoaderSub = pSubLoader;
  m_pAncillary = m_pLoaderSub;
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
/* hash the first field of record                                             */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vHashRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);

  Table* pTable = m_pJob_LoadTable->m_pTable;

  m_iPartition = pTable->iRowPartition(m_aRecord);
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
bool Job_SubLoader::bInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  // check for working buffer
  if (! m_pWorkingBuffer[m_iPartition]) {
    
    // create a new working buffer for this partition
    vNewWorkingBuffer();
    
    if (! m_pWorkingBuffer[m_iPartition]) {
      vTrace("EntryBuffer allocation failed\n"_Stz);
      return false;
    }
  }
  
  // get working buffer
  WorkingBuffer* pWorkingBuffer = m_pWorkingBuffer[m_iPartition];
  
  // get the tx id
  //SEitf* pSEitf = m_pLoadTable->pSEitf();
  //Tx* pTx = pSEitf->pTx();
  //i8 iDistTxId = pTx->iDistTxId();
  i8 iDistTxId = 0;
  
  RowWrk xRowWrk;
  
  xRowWrk.vTxId(iDistTxId);
  xRowWrk.vEmbedded(m_aRecord.aChar());
  xRowWrk.vRowSize(m_iLength);
  
  // insert row into working buffer
  bool bAppended = pWorkingBuffer->bAppendRow(xRowWrk);
  
  //pEntryBuffer->vShow();
  
  return bAppended;
}

/*----------------------------------------------------------------------------*/
/* create a new working buffer                                                */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vNewWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);

  m_pWorkingBuffer[m_iPartition] =
    new WorkingBuffer(m_pJob_LoadTable->m_pTable,m_iPartition);

  m_nBuffersLoadable[m_iPartition]++;
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
Job* JobsModel::vJob_SubLoader_pre(Interface::SEitf* pSEitf)
{
  Job_SubLoader* pJob_SubLoader = new Job_SubLoader(pSEitf);
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_SubLoader_new(Job* pArg)
{
  Job_SubLoader* pJob_SubLoader = new Job_SubLoader(nullptr);
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SubLoader_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pJob);
  
  delete pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SubLoader_show(Job* pJob)
{
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pJob);

  pJob_SubLoader->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
