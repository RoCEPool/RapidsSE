/*============================================================================*/
/* Job_InsertRow.cpp -- Insert Row Job                                        */
/*============================================================================*/

// Job Short: InsertRow

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_InsertRow =
{
  "Job_InsertRow"_Stz,    // name of the job class
  CN_JOB_INSERTROW,       // class enum
  JK_LOCAL,               // job kind
  "insert into tableNm"_Stz, // comment about the job
  "insert"_Stz,           // the prompt command
  "into"_Stz,             // the second word
  nullptr,                // the _run task seed
  "insertRow_req"_Stz,    // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_InsertRow_pre,     // prompt pre-process function
  vJob_InsertRow_post,    // prompt post-process function
  pJob_InsertRow_new,     // job new function
  vJob_InsertRow_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_InsertRow_show,    // job show function
  true,                   // job is enabled
  "<help>"_Stz,           // help
  "<example>"_Stz,        // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_InsertRow_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Lf_class;
  
  return &xJobClass_InsertRow;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_InsertRow* JobsModel::_pJob_InsertRow(void* aAddress)
{
  Job_InsertRow* pJob_InsertRow = reinterpret_cast<Job_InsertRow*>(aAddress);
  
  //RASSERT( !pJob_InsertRow || CN_JOB_INSERTROW == pJob_InsertRow->eClass());
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_InsertRow::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_InsertRow* pJob_LoadTable =
    static_cast<Job_InsertRow*>(HeapMem::pAllocate(CN_JOB_INSERTROW));
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_InsertRow::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::operator delete(void* aAddress)
{
  Job_InsertRow* pJob_InsertRow = static_cast<Job_InsertRow*>(aAddress);
  
  HeapMem::vFree(pJob_InsertRow);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_InsertRow::Job_InsertRow(Interface::Session* pSession)
: Job(CN_JOB_INSERTROW, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  m_pTable = nullptr;
  
  // set the enclosined_by to be a '
  m_xFormatters.m_cEnclosedBy_Leading = '\'';
  m_xFormatters.m_cEnclosedBy_Trailing = '\'';
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pWorkingBuffer[i_Partition] = nullptr;
    m_nBuffersToBeInserted[i_Partition] = 0;
  }
  
  _checkInitialised_(CN_JOB_INSERTROW);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_InsertRow::~Job_InsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_INSERTROW == eClass);
  RASSERT(pObject->eClass() == CN_JOB_INSERTROW);
  
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pObject);

  pJob_InsertRow->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_InsertRow::bInsertRow(Stz* aRow, i8 iRowPartition, i8 iRowLength, eEC& eErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  // check for working buffer
  if (! m_pWorkingBuffer[iRowPartition]) {
    m_pWorkingBuffer[iRowPartition] = new WorkingBuffer(m_pTable, iRowPartition);
    m_nBuffersToBeInserted[iRowPartition]++;
    
    if (! m_pWorkingBuffer[iRowPartition]) {
      vTrace("WorkingBuffer allocation failed\n"_Stz);
      eErrorCode = EC_LOW_BUFFERS;
      return false;
    }
  }
  
  // get tx id
  i8 iDistTxId = -1;
  
  RowWrk xRowWrk;
  
  xRowWrk.vTxId(iDistTxId);
  xRowWrk.vEmbedded((char*)aRow);
  xRowWrk.vRowSize(iRowLength);
  
  // append row into working buffer
  if (m_pWorkingBuffer[iRowPartition]->bAppendRow(xRowWrk)) {
    return true;
  }
  
  // buffer is full
  return false;
}

/*----------------------------------------------------------------------------*/
/* insert last rowbuffer into current table                                   */
/*----------------------------------------------------------------------------*/
eEC Job_InsertRow::eInsertLastBuffer(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  //vTrace("%p inserting entries from row buffer into table..%lld.\n"_Stz, this, iPartition);
  
  if (! m_pWorkingBuffer[iPartition]) {
    m_pWorkingBuffer[iPartition] = new WorkingBuffer(m_pTable, iPartition);
    if (! m_pWorkingBuffer[iPartition]) {
      vTrace("EntryBuffer allocation failed\n"_Stz);
      return EC_LOW_BUFFERS;
    }
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_InsertRow::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  ASSERT(pTokenVec->pGet(0)->bSame("insert"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("into"_Stz));
  
  // get no of tokens in token vector
  i8 nTokens = pTokenVec->nTokens();;
  
  // there should be at least 3 tokens
  if (nTokens != 3)
    return nullptr;
  
  // get the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();

  return true;
}

/*----------------------------------------------------------------------------*/
/* create a new working buffer                                                */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::vNewWorkingBuffer(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  m_pWorkingBuffer[iPartition] = new WorkingBuffer(m_pTable, iPartition);
  m_nBuffersToBeInserted[iPartition]++;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_InsertRow::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW);
  
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
Job* JobsModel::pJob_InsertRow_pre(Interface::Session* pSession)
{
  Job_InsertRow* pJob_InsertRow = new Job_InsertRow(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_InsertRow->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_InsertRow->m_bHasError = true;
    pJob_InsertRow->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRow_post(Interface::Session* pSession, Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
  
  if ( pJob_InsertRow->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_InsertRow->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_InsertRow_new(Job* pArg)
{
  Job_InsertRow* pJob_InsertRow = new Job_InsertRow(nullptr);
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRow_delete(Interface::Session* pSession, Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
  
  vTrace("DELETING INSERTROW\n"_Stz);
  delete pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRow_show(Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);

  pJob_InsertRow->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
