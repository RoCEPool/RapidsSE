/*============================================================================*/
/* DeleteHelper_t.hpp -- a delete helper                                      */
/*============================================================================*/
#pragma once

#include <typeinfo>

namespace BASE
{
  template<class CLASS>
  class DeleteHelper_t
  {
  public:
    CLASS*  m_pInstance;
    eCN     m_eThisClassNo;
  
    Object* m_pDeleter;
    eCN     m_eDeleterClassNo;
    Stz*    m_szDelFunc;
      
  public:
    DeleteHelper_t(CLASS* pInstance, eCN eClassNo);
   ~DeleteHelper_t(void) noexcept(false);

    void vAssertDelete(bool bTrace=false);
    void vAssertNew(bool bTrace=false);
    void vAssertSelf(CLASS* pInstance);

    void vGuardedDelete(CLASS** ppInstance, Object* pDeleter, Stz* szDelFunc);
    void vGuardedDeleteExplicit(Object* pDeleter, Stz* szDelFunc);
  
    void vInductGuard(CLASS* pInstance);
  };
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::DeleteHelper_t(CLASS* pInstance, eCN eThisClassNo)
{
  m_pInstance       = pInstance;
  m_pDeleter        = nullptr;
  m_eDeleterClassNo = CN_NONE;
  m_szDelFunc          = nullptr;
  
  m_eThisClassNo    = eThisClassNo;
  
  vAssertNew(true);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::~DeleteHelper_t(void) noexcept(false)
{
  vAssertDelete(false);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assert delete                                                              */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vAssertDelete(bool bTrace)
{
  if (false)
    {;}

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // freeChunk
  
  else if (CN_FREECHUNK == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,this,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logic unit

  else if (CN_LOGICUNIT == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (   m_szDelFunc->bSame("~Job"_Stz)
               || m_szDelFunc->bSame("~Op"_Stz)
               || m_szDelFunc->bSame("vDelete"_Stz)
               || m_szDelFunc->bSame("vResetForCascade"_Stz)
              )
            );

    RASSERT(CN_API   == m_eDeleterClassNo ||
            CN_OP    == m_eDeleterClassNo  ||
            CN_JOB   == m_eDeleterClassNo);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job

  else if (CN_JOB == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (   m_szDelFunc->bSame("vDeleteJob"_Stz)
               || m_szDelFunc->bSame("vJobDelete"_Stz)
               || m_szDelFunc->bSame("pFindJob"_Stz)
               || m_szDelFunc->bSame("~Task"_Stz)
              )
            );

    RASSERT(CN_API   == m_eDeleterClassNo ||
            CN_TASK == m_eDeleterClassNo ||
            CN_JOBLIST == m_eDeleterClassNo);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op
  
  else if (CN_OP == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (    m_szDelFunc->bSame("~Stream"_Stz)
              )
            );
    RASSERT(CN_STREAM == m_eDeleterClassNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // opSpec
  
  else if (CN_OPSPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (   m_szDelFunc->bSame("vStageFirstSend"_Stz)
               || m_szDelFunc->bSame("vStageSubsequentSend"_Stz)
               || m_szDelFunc->bSame("~StageSpec"_Stz)
              )
           );
    RASSERT(CN_STAGESPEC == m_eDeleterClassNo ||
            CN_PIPELINESPEC == m_eDeleterClassNo);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeline

  else if (CN_PIPELINE == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);

    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (    m_szDelFunc->bSame("~LogicUnit"_Stz)
              )
            );
    RASSERT(CN_LOGICUNIT == m_eDeleterClassNo);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipelineSpec
    
  else if (CN_PIPELINESPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (   m_szDelFunc->bSame("~Pipeline"_Stz)
               || m_szDelFunc->bSame("vPipelineSpecDelete"_Stz)
               || m_szDelFunc->bSame("pCompile"_Stz)
              )
            );
    RASSERT(CN_DYNAMO == m_eDeleterClassNo   ||
            CN_PIPELINE == m_eDeleterClassNo ||
            CN_TASK == m_eDeleterClassNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stage
  
  else if (CN_STAGE == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (    m_szDelFunc->bSame("~Pipeline"_Stz)
              )
            );
    RASSERT(CN_PIPELINE == m_eDeleterClassNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stageSpec
  
  else if (CN_STAGESPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (    m_szDelFunc->bSame("~PipelineSpec"_Stz)
              )
            );
    RASSERT(CN_PIPELINESPEC == m_eDeleterClassNo);
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task

  else if (CN_TASK == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (   m_szDelFunc->bSame("vTaskSelf_DeleteThisTask"_Stz)
               || m_szDelFunc->bSame("???"_Stz)
              )
            );

    RASSERT(CN_API     == m_eDeleterClassNo);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stream
  
  else if (CN_STREAM == m_eThisClassNo) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance,m_szDelFunc,m_pDeleter);
    if (! m_szDelFunc)
      vDebugPoint();

    RASSERT(m_szDelFunc &&
              (    m_szDelFunc->bSame("~Stage"_Stz)
              )
            );
    RASSERT(CN_STAGE == m_eDeleterClassNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // huh???
  
  else {
    vTrace("Unrecognised %p class being deleted\n"_Stz,m_pInstance);
    vTrace("m_eThisClassNo=%lld:%s\n"_Stz
           ,m_eThisClassNo
           ,aCN[m_eThisClassNo]
           );
    vTrace("m_szDelFunc=%s\n"_Stz
           ,m_szDelFunc);
    vTrace("m_eDeleterClassNo=%lld:%s\n"_Stz
           ,m_eDeleterClassNo
           ,aCN[m_eDeleterClassNo]
           );
  }

}

/*----------------------------------------------------------------------------*/
/* assert new                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vAssertNew(bool bTrace)
{
  if (false)
    {;}

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logic unit

  else if (CN_LOGICUNIT == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job

  else if (CN_JOB == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op
  
  else if (CN_OP == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // opSpec
  
  else if (CN_OPSPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeline

  else if (CN_PIPELINE == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipelineSpec
    
  else if (CN_PIPELINESPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stage
  
  else if (CN_STAGE == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stageSpec
  
  else if (CN_STAGESPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task

  else if (CN_TASK == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stream
  
  else if (CN_STREAM == m_eThisClassNo) {
    if (bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // huh???
  
  else {
    vTrace("new %12s:%-3lld %p\n"_Stz,"(unknown)"_Stz
           ,m_pInstance);
  }

}

/*----------------------------------------------------------------------------*/
/* assert self                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vAssertSelf(CLASS* pInstance)
{
  RASSERT(pInstance == m_pInstance);

  RASSERT(m_eThisClassNo == m_pInstance->eGetClassNo());
}

/*----------------------------------------------------------------------------*/
/* delete this                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vGuardedDelete(CLASS** ppInstance,
                                           Object* pDeleter,
                                           Stz* szDelFunc)
{
  m_pDeleter        = pDeleter;
  m_eDeleterClassNo = pDeleter ? pDeleter->eGetClassNo() : CN_API;
  m_szDelFunc       = szDelFunc;
  
  vAssertDelete(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //delete (CLASS*)this;
  if (*ppInstance != m_pInstance) {
    vTrace("vGuardedDelete: *ppInstance %p != m_pInstance %p\n"_Stz
           ,*ppInstance
           ,m_pInstance
           );
    
    vDebugPoint();
  }
  
  RASSERT(*ppInstance == m_pInstance);
  
  if (CN_PIPELINE == m_eThisClassNo)
    vTrace("vGuardedDelete: delete m_pInstance (PIPELINE)\n"_Stz);
  delete m_pInstance;
  if (CN_PIPELINE == m_eThisClassNo)
    vTrace("vGuardedDelete: return from delete m_pInstance (PIPELINE)\n"_Stz);

  * ppInstance = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set who is going to explicitly delete this object                          */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vGuardedDeleteExplicit(Object* pDeleter,
                                                   Stz* szDelFunc)
{
  m_pDeleter        = pDeleter;
  m_eDeleterClassNo = pDeleter ? pDeleter->eGetClassNo() : CN_API;
  m_szDelFunc          = szDelFunc;

  vAssertDelete(false);
}

/*----------------------------------------------------------------------------*/
/* post recv() induction                                                      */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vInductGuard(CLASS* pInstance)
{
  
  m_pInstance    = pInstance;
  m_eThisClassNo = m_pInstance->eGetClassNo();
  
  RASSERT(! m_pDeleter);
  RASSERT(! m_pDeleter);
  RASSERT(! m_eDeleterClassNo);
  RASSERT(! m_szDelFunc);
  
  vTrace("rcv %12s:%-3lld %p\n"_Stz
         ,aCN[m_eThisClassNo],m_eThisClassNo,m_pInstance);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
