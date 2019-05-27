/*============================================================================*/
/* Job_SubExporter.cpp -- Sub Export Job                                        */
/*============================================================================*/

// Job Short: SubExport

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubExporter =
{
  CN_JOB_SUBEXPORTER,        // class enum
  JK_CHILD,                  // job kind
  RK_NODE_ERRORS,            // response kind
  "sub-export rows"_Stz,     // comment about the job
  "subExporter"_Stz,         // the prompt command
  nullptr,                   // the second word
  "subExporter_run"_Stz,     // the _run task seed
  nullptr,                   // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_NONE,                   // ddl lock model
  PY_NORMAL,         // priority
  JF_NONE,           // flags

  nullptr,                   // prompt check function
  pJob_SubExporter_pre,      // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_SubExporter_delete,   // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubExporter_show,     // job show function
  
  // help and example
  true,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SubExporter_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubFetch_class;
  
  return &xJobClass_SubExporter;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubExporter* Jobs::_pJob_SubExporter(void* aAddress)
{
  Job_SubExporter* pJob_SubExporter = reinterpret_cast<Job_SubExporter*>(aAddress);
  
  RASSERT_TYPE(pJob_SubExporter,CN_JOB_SUBEXPORTER);
  
  return pJob_SubExporter;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubExporter::operator new(size_t)
{
  Job_SubExporter* pJob_SubExporter =
    static_cast<Job_SubExporter*>(HeapMem::pAllocate(CN_JOB_SUBEXPORTER));
  
  return pJob_SubExporter;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubExporter::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubExporter::operator delete(void* aAddress)
{
  Job_SubExporter* pJob_SubExporter = static_cast<Job_SubExporter*>(aAddress);
  
  HeapMem::vFree(pJob_SubExporter);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubExporter::Job_SubExporter(Interface::Session* pSession)
: Job(CN_JOB_SUBEXPORTER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
    
  m_pJobExport = nullptr;
  m_nRowBuffers = C_UNKNOWN;
  m_iRowBuffer  = 0;
  
  m_pRowBuffer = nullptr;
  
  m_nExportBuffers = 0;
  m_pExportBuffer = nullptr;
  
  _checkInitialised_(CN_JOB_SUBEXPORTER);
  
  ActionSequence::vAdd(this,AN_CONSTRUCTOR,CN_JOB_SUBEXPORTER,_EG_CN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubExporter::~Job_SubExporter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubExporter::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pObject);

  pJob_SubExporter->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin                                                                      */
/*----------------------------------------------------------------------------*/
bool Job_SubExporter::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
  
  Table* pTable = m_pJobExport->m_pTable;
  TablePtn* pTblPtn = pTable->pPartition(m_i_Slot);
  
  m_pRowBuffer = pTblPtn->pRootRowBuffer();
  m_nRowBuffers = pTblPtn->nRowBuffers();
  
  if (m_bMarkActivity)
    ActivityMap::vMarkJob(this,
                          "TableTpn[%lld] for table %s has %lld buffers"_Stz,
                          m_i_Slot,
                          pTable->aTableName(),
                          m_nRowBuffers);
  
  ActionSequence::vAdd(this,AN_JOB_BEGIN,m_nRowBuffers);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* chunk                                                                      */
/*----------------------------------------------------------------------------*/
bool Job_SubExporter::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
  
  if (! m_pRowBuffer)
    return false;
  
  if (m_bMarkActivity)
    ActivityMap::vMarkJob(this,
                          "Exporting Table %s RowBuffer[%lld/%lld] %p"_Stz,
                          m_pJobExport->m_pTable->aTableName(),
                          m_iRowBuffer,
                          m_nRowBuffers,
                          m_pRowBuffer);
  
  ActionSequence::vAdd(this,AN_JOB_CHUNK,m_iRowBuffer);

  if (m_pRowBuffer->nRowCount())
    vSprintRows();
  
  m_iRowBuffer++;
  m_pRowBuffer = m_pRowBuffer->pNextBuffer();

  return true;
}

/*----------------------------------------------------------------------------*/
/* end                                                                         */
/*----------------------------------------------------------------------------*/
void Job_SubExporter::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);

  if (m_pExportBuffer)
    vSpawnKidExport(true);
  
  ActionSequence::vAdd(this,AN_JOB_END,m_iRowBuffer);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubExporter::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
  
  vTrace("  Job_SubExport %p %03lld "
         "i_Slot %lld\n"_Stz,
         m_pJobExport, m_pJobExport->m_iJobId,
         m_i_Slot
         );
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* spawn kid-export to drag export buffer to export node and write            */
/*----------------------------------------------------------------------------*/
void Job_SubExporter::vSpawnKidExport(bool bLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
  
  JobClass* pJobClass_KidExport = JobClass::pFind(CN_JOB_KIDEXPORT);
  Job* pNewJob = pJobClass_KidExport->pNewJob(nullptr);
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pNewJob);
  
  pJob_KidExport->m_iExportJobId = m_pJobExport->m_iJobId;
  pJob_KidExport->m_pSendWith    = m_pExportBuffer;
  
  if (bLast)
    pJob_KidExport->m_nBuffersExpected = m_nExportBuffers;
  else
    pJob_KidExport->m_nBuffersExpected = C_UNKNOWN;

  m_pExportBuffer = nullptr;
  
  IGNORE_RETURN(Api::bSpawnJob(nullptr,pJob_KidExport,SS_SEED_RUN));
}

/*----------------------------------------------------------------------------*/
/* sprint rows into export buffer                                             */
/*----------------------------------------------------------------------------*/
void Job_SubExporter::vSprintRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
  
  //s48K sRecord;
  
  char* aRecord = nullptr;
  i8 nBytes = 0;
  
  //Record* pRecord = new Record;
  HV_Rec hvRec;

  i8 nRowCount = m_pRowBuffer->nRowCount();
  ColumnVec* pColVec = m_pRowBuffer->pTable()->pColumnVec();

  RowTray* pRowTray = m_pRowBuffer->pRowTray();
  
  for (i8 i_Row = 0; i_Row < nRowCount; i_Row++) {

    HeadLine* aHeadLine = pRowTray->pNub()->aGetHead(i_Row);
    RowCoder::vRecord(hvRec,aHeadLine,pColVec);
    StrLib::vConcat(hvRec->szRecStr()->aChar(),'\n');            //! sub-optimal
    //vTrace("]]%s[[\n"_Stz,sRecord.aStz());
    
    nBytes = hvRec->szRecStr()->nSize();
    
    if (! m_pExportBuffer) {
      m_pExportBuffer = new ExportBuffer;
      m_nExportBuffers++;
    }
    
    if (m_pExportBuffer->nFreeBytes() < nBytes + 1) {
      vSpawnKidExport(false);
      m_pExportBuffer = new ExportBuffer;
      m_nExportBuffers++;
    }
    
    aRecord = m_pExportBuffer->aAllotRecord(nBytes - 1);
    StrLib::vCopyLen(aRecord,hvRec->szRecStr()->aChar(),nBytes - 1);
  }

  if (aRecord)
    aRecord[nBytes-1] = '\0';
  
  //delete pRecord;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubExporter::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SubExporter_pre(Interface::Session* pSession)
{
  Job_SubExporter* pJob_SubExporter = new Job_SubExporter(pSession);
  
  return pJob_SubExporter;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubExporter_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pJob);
  
  delete pJob_SubExporter;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubExporter_show(Job* pJob)
{
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pJob);

  pJob_SubExporter->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
