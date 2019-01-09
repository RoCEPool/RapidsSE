/*============================================================================*/
/* Job_WriteRow.cpp -- write row Job                                          */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/* example:
 
 ld lineitem "/tpch/SF1/lineitem.csv" ;
 
 */

JobClass xJobClass_WriteRow =
{
  "Job_WriteRow"_Stz,  // name of the job class
  CN_JOB_WRITEROW,     // class enum
  JK_GRANDKID,         // job kind
  "write row"_Stz,     // comment about the job
  nullptr,             // the prompt command
  nullptr,             // the second word
  "WriteRow_run"_Stz,  // the _run task seed
  nullptr,             // the _req task seed
  "WriteRow_kid"_Stz,  // the _rcv task seed
  nullptr,             // the _exe task seed
  nullptr,             // the _end task seed
  nullptr,             // the _can task seed
  false,               // check resources?
  false,               // detached job?
  true,                // buffer attached?
  JT_NONE,             // job trace
  pJob_WriteRow_pre,   // prompt pre-process function
  nullptr,             // prompt post-process function
  pJob_WriteRow_new,   // job new function
  vJob_WriteRow_delete,// job delete function
  vJob_send,           // job pre-send function
  vJob_recv,           // job post-receive function
  vJob_WriteRow_show,  // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_WriteRow_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)nullptr;
  
  return &xJobClass_WriteRow;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_WriteRow* JobModel::_pJob_WriteRow(void* aAddress)
{
  Job_WriteRow* pJob_WriteRow = reinterpret_cast<Job_WriteRow*>(aAddress);
  
  RASSERT_TYPE(pJob_WriteRow,CN_JOB_WRITEROW);
  
  return pJob_WriteRow;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_WriteRow::operator new(size_t)
{
  Job_WriteRow* pJob_WriteRow =
    static_cast<Job_WriteRow*>(HeapMem::pAllocate(CN_JOB_WRITEROW));
  
  return pJob_WriteRow;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_WriteRow::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_WriteRow::operator delete(void* aAddress)
{
  Job_WriteRow* pJob_WriteRow = static_cast<Job_WriteRow*>(aAddress);
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  HeapMem::vFree(pJob_WriteRow);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_WriteRow::Job_WriteRow(Interface::Session* pSession)
: Job(CN_JOB_WRITEROW, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEROW,_FOOTER_);
  
  m_pTable       = nullptr;
  m_iPartitionId = C_UNKNOWN;
  m_iParentJobId = C_UNKNOWN;
  
  //m_aRecord
  m_nRecordBytes = false;
  
  _checkInitialised_(CN_JOB_WRITEROW);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_WriteRow::~Job_WriteRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEROW,_FOOTER_);
  
  delete m_pCurrentBuffer;
  m_pCurrentBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert row into table                                                      */
/*----------------------------------------------------------------------------*/
void Job_WriteRow::vInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEROW,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert partition to node slot
  i8 iSlot = TablePtn::iPartitionToSlot(m_iPartitionId);

  i8 iTxId = 0;

  m_pTable->vInsertRow(iTxId, iSlot, m_aRecord, m_nRecordBytes);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_WriteRow::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pObject);

  pJob_WriteRow->vShow();
}

/*----------------------------------------------------------------------------*/
/* new a WriteRow job                                                         */
/*----------------------------------------------------------------------------*/
Job_WriteRow* Job_WriteRow::pNewJobWriteRow(Interface::Session* pSession,
                                   Stz* sTableName,
                                   i8 iParentJobId)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a write buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_WriteRow = JobClass::pFind(CN_JOB_WRITEROW);

  Job_WriteRow* pJob_WriteRow =
  static_cast<Job_WriteRow*>(pJobClass_WriteRow->pPreJob(pSession));

  pJob_WriteRow->vSeed(SS_SEED_RUN);

  pJob_WriteRow->m_iParentJobId = iParentJobId;
  pJob_WriteRow->m_sObjectName  = sTableName;
  pJob_WriteRow->m_iPartitionId = C_UNKNOWN;

  return pJob_WriteRow;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fromat a row                                                               */
/*----------------------------------------------------------------------------*/
bool Job_WriteRow::bFormatRow(Table* pTable, Stz* aRawData, i8 nRawDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEROW,_FOOTER_);
  
  bool bBlankLine;
  Formatters xFormatters;
  
  // set the enclosed_by to be a '
  xFormatters.m_bDefault = false;
  xFormatters.m_cEnclosedBy_Leading = '\'';
  xFormatters.m_cEnclosedBy_Trailing = '\'';
  
  // format the raw data into a row
  bool bValid = Row::bFormatRow(pTable->pColumnVec(),
                                &xFormatters,
                                aRawData, nRawDataBytes,
                                m_aRecord.aStz(),
                                m_nRecordBytes,
                                bBlankLine,
                                false /* allowNonPrintable */);

  return bValid;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_WriteRow::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_WriteRow::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEROW);
  
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
Job* JobModel::pJob_WriteRow_pre(Interface::Session* pSession)
{
  Job_WriteRow* pJob_WriteRow = new Job_WriteRow(pSession);
  
  return pJob_WriteRow;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_WriteRow_new(Job* pArg)
{
  Job_WriteRow* pJob_WriteRow = new Job_WriteRow(nullptr);
  
  return pJob_WriteRow;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_WriteRow_delete(Interface::Session* pSession, Job* pJob)
{
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJob);
  
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  delete pJob_WriteRow;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_WriteRow_show(Job* pJob)
{
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJob);

  pJob_WriteRow->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
