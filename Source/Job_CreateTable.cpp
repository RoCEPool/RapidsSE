/*============================================================================*/
/* Job_CreateTable.cpp -- Create Table Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_CreateTable (
  "Job_CreateTable"_Stz,  // name of the job class
  CN_JOB_CREATETABLE,     // class enum
  "comment"_Stz,          // comment about the job
  "create"_Stz,           // the prompt command
  "R35table"_Stz,         // the second word
  "createTableR35"_Stz,   // the task seed for this job
  false,                  // detached job?
  pJob_CreateTable_pre,   // prompt pre-process function
  vJob_CreateTable_post,  // prompt post-process function
  true
);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateTable* JobsModel::_pJob_CreateTable(void* aAddress)
{
  Job_CreateTable* pJob_CreateTable = reinterpret_cast<Job_CreateTable*>(aAddress);
  
  RASSERT( !pJob_CreateTable || CN_JOB_CREATETABLE == pJob_CreateTable->eClass());
  
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateTable* pJob_CreateTable =
    static_cast<Job_CreateTable*>(Heap::pAllocate(CN_JOB_CREATETABLE));
  
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateTable::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::operator delete(void* aAddress)
{
  Job_CreateTable* pJob_CreateTable = static_cast<Job_CreateTable*>(aAddress);
  
  Heap::vFree(pJob_CreateTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateTable::Job_CreateTable(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATETABLE), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  m_sSeed             = mJobNote_CreateTable.pJobClass()->m_aSeed;
  m_sCheckResources   = "Job_CreateTable.checkResources"_Stz;
  m_sReleaseResources = "Job_CreateTable.relResources"_Stz;
  
  m_eTableType = TT_NONE;
  m_iColumns   = 0;
  
  for (i8 i_Column = 0; i_Column < C_MAX_COLUMNS; i_Column++) {
    m_xColumns[i_Column].m_iColumnNo = -1;
    m_xColumns[i_Column].m_sColumnName = ""_Stz;
    m_xColumns[i_Column].m_eColumnType = CT_NONE;
    m_xColumns[i_Column].m_iColumnLength = -1;
    m_xColumns[i_Column].m_iColumnScale = -1;
    m_xColumns[i_Column].m_bIsNullable = true;
    m_xColumns[i_Column].m_bIsUnique = false;
    m_xColumns[i_Column].m_bIsPartitionKey = false;
    m_xColumns[i_Column].m_eDatumType = DM_NONE;
  }
  
  _checkInitialised_(CN_JOB_CREATETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateTable::~Job_CreateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATETABLE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATETABLE);
  
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pObject);

  pJob_CreateTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  vTrace("\n\nJob_CreateTable: %s\n"_Stz, m_sObjectName.aChar());
  
  //vTrace("m_sExecute = %s\n"_Stz,m_sExecute.aChar());
  vTrace("m_eTableType = %s\n"_Stz,aTT[m_eTableType]);
  vTrace("m_iColumns = %lld\n"_Stz,m_iColumns);
  
  for (i8 i_Column = 0; i_Column < m_iColumns; i_Column++) {
    vTrace("\nm_xColumns[%lld].m_iColumnNo = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnNo);
    vTrace("m_xColumns[%lld].m_sColumnName = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_sColumnName.aChar());
    vTrace("m_xColumns[%lld].m_eColumnType = %s\n"_Stz, i_Column, aCT[m_xColumns[i_Column].m_eColumnType]);
    vTrace("m_xColumns[%lld].m_iColumnLength = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnLength);
    vTrace("m_xColumns[%lld].m_iColumnScale = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnScale);
    vTrace("m_xColumns[%lld].m_bIsNullable = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsNullable ? "true" : "false");
    vTrace("m_xColumns[%lld].m_bIsUnique = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsUnique ? "true" : "false");
    vTrace("m_xColumns[%lld].m_bIsPartitionKey = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsPartitionKey ? "true" : "false");
    vTrace("m_xColumns[%lld].m_eDatumType = %s\n"_Stz, i_Column, aDT[m_xColumns[i_Column].m_eDatumType]);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_CreateTable_pre(Prompt::SePrompt* pPrompt)
{
  SEitf* pSEitf = pPrompt->pSEitf();
  Job_CreateTable* pJob_CreateTable = new Job_CreateTable(pSEitf);
  
  bool bResult = pPrompt->bCreateTableR35(pJob_CreateTable);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_CreateTable->m_bHasError = true;
    pJob_CreateTable->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateTable_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJob);
  
  if ( pJob_CreateTable->m_bHasError ) {
  
    // Note: take care of errors / meta data ????
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_CreateTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  
  // now delete the job
  delete pJob_CreateTable;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
