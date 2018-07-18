/*============================================================================*/
/* Job_Space.cpp -- Space Job                                                 */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Docol (
  "docol"_Stz,      // name of the job class
  CN_JOB_DOCOL,     // class enum
  "comment"_Stz,    // comment about the job
  "docol"_Stz,      // the prompt command
  nullptr,          // the second word
  "docol"_Stz,      // the task seed for this job
  false,            // detached job?
  pJob_Docol_pre,   // prompt pre-process function
  vJob_Docol_post,  // prompt post-process function
  true              // job is enabled
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Docol* JobsModel::_pJob_Docol(void* aAddress)
{
  Job_Docol* pJob_Docol = reinterpret_cast<Job_Docol*>(aAddress);
  
  //RASSERT( !Job_Docol || CN_JOB_DOCOL == pJob_Docol->eClass());
  
  return pJob_Docol;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Docol::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Docol* pJob_Docol =
    static_cast<Job_Docol*>(Heap::pAllocate(CN_JOB_DOCOL));
  
  return pJob_Docol;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Docol::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Docol::operator delete(void* aAddress)
{
  Job_Docol* pJob_Docol = static_cast<Job_Docol*>(aAddress);
  
  Heap::vFree(pJob_Docol);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Docol::Job_Docol(Interface::SEitf* pSEitf)
: Job(CN_JOB_DOCOL), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
  
  m_sSeed    = mJobNote_Docol.pJobClass()->m_aSeed;

  _checkInitialised_(CN_JOB_DOCOL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Docol::~Job_Docol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Docol::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DOCOL == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DOCOL);
  
  Job_Docol* pJob_Docol = _pJob_Docol(pObject);

  pJob_Docol->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Docol::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Docol::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Docol_pre(Prompt::SePrompt* pPrompt)
{
  SEitf* pSEitf = pPrompt->pSEitf();
  
  Job_Docol* pJob_Docol = new Job_Docol(pSEitf);
  
  i8 i_Token;
  i8 iNumTokens;
  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT(pTokenVec->pGet(0)->bSame("docol"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTokenVec->pGet(i_Token)->bHasText())
      break;
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return nullptr;
  }
  
  // set the table name
  pJob_Docol->m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return pJob_Docol;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Docol_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
  Job_Docol* pJob_Docol = _pJob_Docol(pJob);
  
  // Note: take care of errors / meta data ????
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    vTrace("%s\n"_Stz, pJob_Docol->m_xNode[i_Node].m_sResponse.aChar());
  }

  // now delete the job
  delete pJob_Docol;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
