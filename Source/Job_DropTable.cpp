/*============================================================================*/
/* Job_DropTable.cpp -- drop table Job                                        */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_DropTable (
  "Job_DropTable"_Stz,  // name of the job class
  CN_JOB_DROPTABLE,     // class enum
  "comment"_Stz,        // comment about the job
  "drop"_Stz,           // the prompt command
  "R35Table"_Stz,       // the second word
  "dropTableR35"_Stz,   // the task seed for this job
  false,                // detached job?
  bJob_DropTable_pre,   // prompt pre-process function
  vJob_DropTable_post,  // prompt post-process function
  true
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropTable* JobsModel::_pJob_DropTable(void* aAddress)
{
  Job_DropTable* pJob_DropTable = reinterpret_cast<Job_DropTable*>(aAddress);
  
  RASSERT( !pJob_DropTable || CN_JOB_DROPTABLE == pJob_DropTable->eClass());
  
  return pJob_DropTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropTable* pJob_DropTable =
    static_cast<Job_DropTable*>(Heap::pAllocate(CN_JOB_DROPTABLE));
  
  return pJob_DropTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropTable::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropTable::operator delete(void* aAddress)
{
  Job_DropTable* pJob_DropTable = static_cast<Job_DropTable*>(aAddress);
  
  Heap::vFree(pJob_DropTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropTable::Job_DropTable(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPTABLE), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
    
  m_sSeed = mJobNote_DropTable.pJobClass()->m_aSeed;
  
  _checkInitialised_(CN_JOB_DROPTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropTable::~Job_DropTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPTABLE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPTABLE);
  
  Job_DropTable* pJob_DropTable = _pJob_DropTable(pObject);

  pJob_DropTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
bool JobsModel::bJob_DropTable_pre(Prompt::SePrompt* pPrompt)
{
  SEitf* pSEitf = pPrompt->pSEitf();
  Job_DropTable* pJob_DropTable = new Job_DropTable(pSEitf);
  
  i8 i_Token;
  i8 iNumTokens;
  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("R35Table"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTokenVec->pGet(i_Token)->bHasText())
      break;
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    pJob_DropTable->m_bResult = false;
    pJob_DropTable->m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  // set the table name
  pJob_DropTable->m_sObjectName = pTokenVec->pGet(2)->aText();
  pPrompt->vJob(pJob_DropTable);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropTable_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
  Job_DropTable* pJob_DropTable = _pJob_DropTable(pJob);
  
  // Note: take care of errors / meta data ????
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    vTrace("NODE%lld: %s\n"_Stz, i_Node, aEC[pJob_DropTable->m_xNode[i_Node].m_eErrorCode]);
  }
  
  // now delete the job
  delete pJob_DropTable;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
