/*============================================================================*/
/* Job_Cat.cpp -- Cat Job                                                     */
/*============================================================================*/

// Job Short: Cat

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Cat =
{
  "Job_Cat"_Stz,      // name of the job class
  CN_JOB_CAT,         // class enum
  JK_LOCAL,           // job kind
  "cat data"_Stz,     // comment about the job
  "cat"_Stz,          // the prompt command
  nullptr,            // the second word
  nullptr,            // the _run task seed
  "cat_req"_Stz,      // the _req task seed
  nullptr,             // the _rcv task seed
  nullptr,             // the _exe task seed
  nullptr,             // the _end task seed
  nullptr,             // the _can task seed
  false,               // check resources?
  false,               // detached job?
  false,               // buffer attached?
  JT_NONE,             // job trace
  pJob_Cat_pre,        // prompt pre-process function
  vJob_Cat_post,       // prompt post-process function
  pJob_Cat_new,        // job new function
  vJob_Cat_delete,     // job delete function
  nullptr,             // job pre-send function
  nullptr,             // job post-receive function
  vJob_Cat_show,       // job show function
  
  // help and example
  true,
  "cat <inPipeNm> <outPipeNm>"_Stz,
  "cat \"/rapids/SensorPipe\" \"/rapids/Stage1Pipe\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_Cat_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Cf_class;
  
  return &xJobClass_Cat;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Cat* JobsModel::_pJob_Cat(void* aAddress)
{
  Job_Cat* pJob_Cat = reinterpret_cast<Job_Cat*>(aAddress);
  
  RASSERT_TYPE(pJob_Cat,CN_JOB_CAT);
  
  return pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Cat::operator new(size_t)
{
  Job_Cat* pJob_Cat =
    static_cast<Job_Cat*>(HeapMem::pAllocate(CN_JOB_CAT));
  
  return pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Cat::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Cat::operator delete(void* aAddress)
{
  Job_Cat* pJob_Cat = static_cast<Job_Cat*>(aAddress);
  
  HeapMem::vFree(pJob_Cat);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Cat::Job_Cat(Interface::Session* pSession)
: Job(CN_JOB_CAT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
  
  m_pInChunk  = new IoChunk;
  m_pOutChunk = new IoChunk;
  
  m_bPrint = false;
  
  _checkInitialised_(CN_JOB_CAT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Cat::~Job_Cat(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
  
  delete m_pInChunk;
  delete m_pOutChunk;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Cat::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Cat* pJob_Cat = _pJob_Cat(pObject);

  pJob_Cat->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Cat::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Cat::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Cat_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Cat* pJob_Cat = new Job_Cat(pSession);

  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 2 tokens
  if (nTokens < 2)
    return pJob_Cat;

  // set the file name
  if (pTknVec->pGet(2)->sText().bSame("pipe"_Stz))
    pJob_Cat->m_sFileName = pTknVec->pGet(2)->aText();
  else
    pJob_Cat->m_sFileName = pTknVec->pGet(1)->aText();
  
  if (pTknVec->pGet(nTokens - 1)->sText().bSame("print"_Stz) ||
      pTknVec->pGet(nTokens - 1)->sText().bSame("display"_Stz) ||
      pTknVec->pGet(nTokens - 1)->sText().bSame("displayOn"_Stz))
    pJob_Cat->m_bPrint = true;
  
  return pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Cat_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Cat_new(Job* pArg)
{
  Job_Cat* pJob_Cat = new Job_Cat(nullptr);
  
  return pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Cat_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Cat* pJob_Cat = _pJob_Cat(pJob);
  
  delete pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Cat_show(Job* pJob)
{
  Job_Cat* pJob_Cat = _pJob_Cat(pJob);

  pJob_Cat->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cat data                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Cat::vCat(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
  
  vTrace("Cat data from %s\n"_Stz,m_sFileName.aStr());
  
  m_pInChunk->vOpenRead(m_sFileName);
  
  i8 nBytesSoFar = 0;
  
#define C_NEXT_INTERVAL (C_1GB *10)
  i8 nNextLimit = C_NEXT_INTERVAL;
  
  s32 sAvgMag;
  s32 sSumMag;

  Timer xElapsedTimer;
  xElapsedTimer.vStart();
  
  for (;;) {
  
    i8 nBytesRead = m_pInChunk->nReadBytes(-1);
    if (! nBytesRead)
      break;
    
    if (m_bPrint)
      vTrace("[[chunk]]\n%s\n[[end-chunk]]\n"_Stz,m_pInChunk->aStr());
    
    nBytesSoFar += nBytesRead;
    
    if (nBytesSoFar && nBytesSoFar >= nNextLimit) {
      
      xElapsedTimer.vUpdate();
      
      i8 iAvgBytesPerSec = DIV(nBytesSoFar,xElapsedTimer.iSeconds());
      Magnitudes::vByteMag(iAvgBytesPerSec,2,sAvgMag);
      Magnitudes::vByteMag(nBytesSoFar,2,sSumMag);

      vTrace("Elapsed %llds: %lls sofar; %s/s\n"_Stz,
             xElapsedTimer.iSeconds(),
             sSumMag.aStr(),
             sAvgMag.aStr());
      
      nNextLimit += C_NEXT_INTERVAL;
    }
  
  }
  
  xElapsedTimer.vUpdate();
  
  i8 iAvgBytesPerSec = DIV(nBytesSoFar,xElapsedTimer.iSeconds());
  
  Magnitudes::vByteMag(iAvgBytesPerSec,2,sAvgMag);
  
  vTrace("Elapsed %llds: %lls sofar; %s/s\n"_Stz,
         xElapsedTimer.iSeconds(),
         sSumMag.aStr(),
         sAvgMag.aStr());

  i8 iExtrapolatedPerDay = iAvgBytesPerSec * 60 * 60 * 24;
  Magnitudes::vByteMag(iExtrapolatedPerDay,2,sAvgMag);
  
  vTrace("Extrapolated %s/day\n"_Stz,
         sAvgMag.aStr());

  m_pInChunk->vCloseFile();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
