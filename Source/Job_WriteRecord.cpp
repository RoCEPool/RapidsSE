/*============================================================================*/
/* Job_WriteRecord.cpp -- write record job                                    */
/*============================================================================*/

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_WriteRecord =
{
  "Job_WriteRecord"_Stz,  // name of the job class
  CN_JOB_WRITERECORD,     // class enum
  JK_GRANDKID,            // job kind
  "write record"_Stz,     // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "WriteRecord_run"_Stz,  // the _run task seed
  nullptr,                // the _req task seed
  "WriteRecord_kid"_Stz,  // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  true,                   // buffer attached?
  JT_NONE,                // job trace
  
  nullptr,                // prompt check function
  pJob_WriteRecord_pre,   // prompt pre-process function
  nullptr,                // prompt post-process function
  pJob_WriteRecord_new,   // job new function
  vJob_WriteRecord_delete,// job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_WriteRecord_show,  // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_WriteRecord_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WriteRow_class;
  
  return &xJobClass_WriteRecord;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_WriteRecord* Jobs::_pJob_WriteRecord(void* aAddress)
{
  Job_WriteRecord* pJob_WriteRecord = reinterpret_cast<Job_WriteRecord*>(aAddress);
  
  RASSERT_TYPE(pJob_WriteRecord,CN_JOB_WRITERECORD);
  
  return pJob_WriteRecord;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_WriteRecord::operator new(size_t)
{
  Job_WriteRecord* pJob_WriteRecord =
    static_cast<Job_WriteRecord*>(HeapMem::pAllocate(CN_JOB_WRITERECORD));
  
  return pJob_WriteRecord;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_WriteRecord::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_WriteRecord::operator delete(void* aAddress)
{
  Job_WriteRecord* pJob_WriteRecord = static_cast<Job_WriteRecord*>(aAddress);
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  HeapMem::vFree(pJob_WriteRecord);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_WriteRecord::Job_WriteRecord(Interface::Session* pSession)
: Job(CN_JOB_WRITERECORD, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITERECORD,_FOOTER_);
  
  m_iExportJobId = C_UNKNOWN;
  
  
  _checkInitialised_(CN_JOB_WRITERECORD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITERECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_WriteRecord::~Job_WriteRecord(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITERECORD,_FOOTER_);
  
  delete m_pCurrentBuffer;
  m_pCurrentBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert row into table                                                      */
/*----------------------------------------------------------------------------*/
void Job_WriteRecord::vWriteRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITERECORD,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_WriteRecord::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_WriteRecord* pJob_WriteRecord = _pJob_WriteRecord(pObject);

  pJob_WriteRecord->vShow();
}

/*----------------------------------------------------------------------------*/
/* new a WriteRecord job                                                      */
/*----------------------------------------------------------------------------*/
Job_WriteRecord* Job_WriteRecord::pNewJobWriteRecord(Interface::Session* pSession,
                                                     i8 iExportJobId)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a write buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_WriteRecord = JobClass::pFind(CN_JOB_WRITERECORD);

  Job_WriteRecord* pJob_WriteRecord =
    _pJob_WriteRecord(pJobClass_WriteRecord->pPreJob(pSession));

  pJob_WriteRecord->vSeed(SS_SEED_RUN);

  return pJob_WriteRecord;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_WriteRecord::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITERECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_WriteRecord::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITERECORD);
  
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
Job* Jobs::pJob_WriteRecord_pre(Interface::Session* pSession)
{
  Job_WriteRecord* pJob_WriteRecord = new Job_WriteRecord(pSession);
  
  return pJob_WriteRecord;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_WriteRecord_new(Job* pArg)
{
  Job_WriteRecord* pJob_WriteRecord = new Job_WriteRecord(nullptr);
  
  return pJob_WriteRecord;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_WriteRecord_delete(Interface::Session* pSession, Job* pJob)
{
  Job_WriteRecord* pJob_WriteRecord = _pJob_WriteRecord(pJob);
  
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  delete pJob_WriteRecord;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_WriteRecord_show(Job* pJob)
{
  Job_WriteRecord* pJob_WriteRecord = _pJob_WriteRecord(pJob);

  pJob_WriteRecord->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
