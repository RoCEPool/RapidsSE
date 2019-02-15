/*============================================================================*/
/* Job_SubExporter.cpp -- Sub Export Job                                        */
/*============================================================================*/

// Job Short: SubExport

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubExporter =
{
  "Job_SubExporter"_Stz,     // name of the job class
  CN_JOB_SUBEXPORTER,          // class enum
  JK_CHILD,                  // job kind
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
  
  nullptr,                   // prompt check function
  pJob_SubExporter_pre,      // prompt pre-process function
  vJob_SubExporter_post,     // prompt post-process function
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
  m_pIndexTree = nullptr;
  m_nEntryBuffers = C_UNKNOWN;
  m_iEntryBuffer  = 0;
  
  m_pEntryBuffer = nullptr;
  m_pCsvBuffer = nullptr;
  
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
  
  i8 i_HiddenIndex  = pTable->pHiddenIndex()->iIndexNo();
  TablePtn* pTblPtn = pTable->pPartition(m_i_Slot);
  m_pIndexTree      = pTblPtn->pIndexTree(i_HiddenIndex);
  
  m_pEntryBuffer  = m_pIndexTree->pRoot();
  m_nEntryBuffers = m_pEntryBuffer->iLengthList();
  
  if (m_bMark)
    ActivityMap::vMarkJob(this,
                          "IndexTree[%lld] for table %s has %lld buffers"_Stz,
                          m_i_Slot,
                          pTable->aTableName(),
                          m_nEntryBuffers);
  
  ActionSequence::vAdd(this,AN_JOB_BEGIN,m_nEntryBuffers);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* chunk                                                                      */
/*----------------------------------------------------------------------------*/
bool Job_SubExporter::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
  
  if (! m_pEntryBuffer)
    return false;
  
  if (m_bMark)
    ActivityMap::vMarkJob(this,
                          "Exporting Table %s EntryBuffer[%lld/%lld] %p"_Stz,
                          m_pJobExport->m_pTable->aTableName(),
                          m_iEntryBuffer,
                          m_nEntryBuffers,
                          m_pEntryBuffer);
  
  ActionSequence::vAdd(this,AN_JOB_CHUNK,m_iEntryBuffer);

  if (m_pEntryBuffer->nItems())
    vSprintRows();
  
  m_iEntryBuffer++;
  m_pEntryBuffer = m_pEntryBuffer->pNextBuffer();

  return true;
}

/*----------------------------------------------------------------------------*/
/* end                                                                         */
/*----------------------------------------------------------------------------*/
void Job_SubExporter::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);

  ActionSequence::vAdd(this,AN_JOB_END,m_iEntryBuffer);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubExporter::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void Job_SubExporter::vSprintRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORTER,_FOOTER_);
  
  s48K sRowString;
  
  ColumnVec* pColVec = m_pJobExport->m_pTable->pColumnVec();
  
  for (Item* aItem = m_pEntryBuffer->aLeadEntry();
       aItem;
       aItem = m_pEntryBuffer->aNextEntry(aItem,0)) {

    RowData* aRow = _aRowData(aItem->aDataArea());
    
    aRow->vAsBinaryStringVLQ(sRowString,pColVec);
    
    vTrace("]]%s[[\n"_Stz,sRowString.aStz());
  }
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
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubExporter_post(Interface::Session* pSession, Job* pJob)
{
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pJob);
  
  if ( pJob_SubExporter->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SubExporter->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
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
