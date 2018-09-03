/*============================================================================*/
/* SePrompt.cpp -- se> prompt                                                 */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Arguments     gxArguments;
extern pthread_key_t giTLS_ThreadKey;

PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SePrompt::operator new(size_t iSize)
{
  RASSERT(iSize);
  SePrompt* pSePrompt = (SePrompt*)Heap::pAllocate(CN_SEPROMPT);
  
  return pPoolCheck<void*>(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::operator delete(void* pSePromptAddress)
{
  SePrompt* pSePrompt = static_cast<SePrompt*>(pSePromptAddress);
  
  Heap::vFree(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(void)
: Object(CN_SEPROMPT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iPartition = 0;
  m_bContinue = true;

  eEC eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,
                                  "rapidsse"_Stz,
                                  "public"_Stz,
                                  iPartition,
                                  /*trace*/ 0);

  ASSERT(EC_NONE == eResult);

  m_pJob = nullptr;
  
  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RapidsSE_SEitfDelete(m_pSEitf, /*trace*/ 0);
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SEPROMPT);
  RASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(pObject);
  
  vTrace("SePrompt %p seitf %p \n"_Stz,
         pSePrompt,
         pSePrompt->m_pSEitf);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/






/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bBlankLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get command processor                                                      */
/*----------------------------------------------------------------------------*/
CommandProcessor* SePrompt::pCommandProcessor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf->m_pCommandProcessor;
}

/*----------------------------------------------------------------------------*/
/* get continue                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bContinue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_bContinue;
}

/*----------------------------------------------------------------------------*/
/* set continue                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vContinue(bool bContinue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_bContinue = bContinue;
}

/*----------------------------------------------------------------------------*/
/* set continue                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vDeleteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  delete m_pJob;
  m_pJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  PromptFun* pPromptFun =
    gxPromptCatalog.pFindFun(pTkn(0)->aText(), pTkn(1)->aText());
  
  if (! pPromptFun)
    return false;
  
  (*pPromptFun->m_fPrompt)(this);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* find job class                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bFindJobClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vGetCommand();
}

/*----------------------------------------------------------------------------*/
/* call post-job function                                                     */
/*----------------------------------------------------------------------------*/
void SePrompt::vPostJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* call pre-job function                                                     */
/*----------------------------------------------------------------------------*/
Job* SePrompt::pPreJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the seitf                                                              */
/*----------------------------------------------------------------------------*/
SEitf* SePrompt::pSEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vSpawnJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf->m_pCommandProcessor->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* SePrompt::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn = (*pTknVec)[i_Token];
  
  return pTkn;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SePrompt::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SePrompt* Prompt::_pSePrompt(void* aAddr)
{
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(aAddr);
  
  RASSERT( !pSePrompt || CN_SEPROMPT == pSePrompt->eClass() );
  
  return pSePrompt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
