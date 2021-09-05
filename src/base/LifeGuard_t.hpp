/*============================================================================*/
/* LifeGuard_t.hpp -- lifecycle rules and helper for EE componentry           */
/*============================================================================*/
#pragma once

#include <typeinfo>

#include "AM.hpp"

namespace BASE
{
  template<class CLASS>
  class LifeGuard_t
  {
  public:
    eCN     m_eInstance;
    CLASS*  m_pInstance;
  
    eCN     m_eExpecting;
    eCN     m_eDeleter;
    Object* m_pDeleter;
    Stz*    m_szDelFunNm;
  
    bool    m_bTrace;
      
  public:
    LifeGuard_t(CLASS* pInstance, eCN eClassNo);
   ~LifeGuard_t(void) noexcept(false);

    void vAssertDelete(bool bTrace=false);
    void vAssertNew(bool bTrace=false);
    void vAssertSelf(CLASS* pInstance);

    void vGuardedDelete(eCN eExpecting,
                        CLASS** ppInstance,
                        eCN eDeleter,
                        Object* pDeleter,
                        Stz* szDelFunc);
  
    void vGuardedDeleteExplicit(eCN eExpecting,
                                eCN eDeleter,
                                Object* pDeleter,
                                Stz* szDelFunc);
  
    void vInductGuard(CLASS* pInstance);
  };
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
LifeGuard_t<CLASS>::LifeGuard_t(CLASS* pInstance, eCN eThisClassNo)
{
  m_eInstance = eThisClassNo;
  m_pInstance = pInstance;
  
  m_eExpecting = CN_NONE;
  m_eDeleter  = CN_NONE;
  m_pDeleter  = nullptr;
  m_szDelFunNm = nullptr;
  
  m_bTrace = false;
  
  vAssertNew(true);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
LifeGuard_t<CLASS>::~LifeGuard_t(void) noexcept(false)
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
void LifeGuard_t<CLASS>::vAssertDelete(bool bTrace)
{
  AM::EE::vAssertEEDelete(m_eInstance,m_pInstance,
                          m_eExpecting,m_eDeleter,m_pDeleter,
                          m_szDelFunNm,m_bTrace && bTrace);
 
#if 0
  if (false)
    {;}

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // freeChunk
  
  else if (CN_FREECHUNK == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,this,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logic unit

  else if (CN_LOGICUNIT == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (   m_szDelFunNm->bSame("~Job"_Stz)
//               || m_szDelFunNm->bSame("~Op"_Stz)
//               || m_szDelFunNm->bSame("vDelete"_Stz)
//               || m_szDelFunNm->bSame("vResetForCascade"_Stz)
//              )
//            );
//
//    RASSERT(CN_API   == m_eDeleter ||
//            CN_OP    == m_eDeleter  ||
//            CN_JOB   == m_eDeleter);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job

  else if (CN_JOB == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (   m_szDelFunNm->bSame("vDeleteJob"_Stz)
//               || m_szDelFunNm->bSame("vJobDelete"_Stz)
//               || m_szDelFunNm->bSame("pFindJob"_Stz)
//               || m_szDelFunNm->bSame("~Task"_Stz)
//              )
//            );
//
//    RASSERT(CN_API   == m_eDeleter ||
//            CN_TASK == m_eDeleter ||
//            CN_JOBLIST == m_eDeleter);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op
  
  else if (CN_OP == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (    m_szDelFunNm->bSame("~Stream"_Stz)
//              )
//            );
//    RASSERT(CN_STREAM == m_eDeleter);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // opSpec
  
  else if (CN_OPSPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (   m_szDelFunNm->bSame("vStageFirstSend"_Stz)
//               || m_szDelFunNm->bSame("vStageSubsequentSend"_Stz)
//               || m_szDelFunNm->bSame("~StageSpec"_Stz)
//              )
//           );
//    RASSERT(CN_STAGESPEC == m_eDeleter ||
//            CN_PIPELINESPEC == m_eDeleter);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeline

  else if (CN_PIPELINE == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);

    if (! m_szDelFunNm)
      vDebugPoint();
//
//    RASSERT(m_szDelFunNm &&
//              (    m_szDelFunNm->bSame("~LogicUnit"_Stz)
//              )
//            );
//    RASSERT(CN_LOGICUNIT == m_eDeleter);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipelineSpec
    
  else if (CN_PIPELINESPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (   m_szDelFunNm->bSame("~Pipeline"_Stz)
//               || m_szDelFunNm->bSame("vPipelineSpecDelete"_Stz)
//               || m_szDelFunNm->bSame("pCompile"_Stz)
//              )
//            );
//    RASSERT(CN_DYNAMO == m_eDeleter   ||
//            CN_PIPELINE == m_eDeleter ||
//            CN_TASK == m_eDeleter);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stage
  
  else if (CN_STAGE == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (    m_szDelFunNm->bSame("~Pipeline"_Stz)
//              )
//            );
//    RASSERT(CN_PIPELINE == m_eDeleter);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stageSpec
  
  else if (CN_STAGESPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (    m_szDelFunNm->bSame("~PipelineSpec"_Stz)
//              )
//            );
//    RASSERT(CN_PIPELINESPEC == m_eDeleter);
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task

  else if (CN_TASK == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (   m_szDelFunNm->bSame("vTaskSelf_DeleteThisTask"_Stz)
//               || m_szDelFunNm->bSame("???"_Stz)
//              )
//            );
//
//    RASSERT(CN_API     == m_eDeleter);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stream
  
  else if (CN_STREAM == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[m_eInstance],m_eInstance,m_pInstance,m_szDelFunNm,m_pDeleter);
    if (! m_szDelFunNm)
      vDebugPoint();

//    RASSERT(m_szDelFunNm &&
//              (    m_szDelFunNm->bSame("~Stage"_Stz)
//              )
//            );
//    RASSERT(CN_STAGE == m_eDeleter);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // huh???
  
  else {
    vTrace("Unrecognised %p class being deleted\n"_Stz,m_pInstance);
    vTrace("m_eInstance=%lld:%s\n"_Stz
           ,m_eInstance
           ,aCN[m_eInstance]
           );
    vTrace("m_szDelFunNm=%s\n"_Stz
           ,m_szDelFunNm);
    vTrace("m_eDeleter=%lld:%s\n"_Stz
           ,m_eDeleter
           ,aCN[m_eDeleter]
           );
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* assert new                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void LifeGuard_t<CLASS>::vAssertNew(bool bTrace)
{
  if (false)
    {;}

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logic unit

  else if (CN_LOGICUNIT == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job

  else if (CN_JOB == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op
  
  else if (CN_OP == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // opSpec
  
  else if (CN_OPSPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeline

  else if (CN_PIPELINE == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipelineSpec
    
  else if (CN_PIPELINESPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stage
  
  else if (CN_STAGE == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stageSpec
  
  else if (CN_STAGESPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task

  else if (CN_TASK == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stream
  
  else if (CN_STREAM == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
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
void LifeGuard_t<CLASS>::vAssertSelf(CLASS* pInstance)
{
  RASSERT(pInstance == m_pInstance);

  RASSERT(m_eInstance == m_pInstance->eGetClassNo());
}

/*----------------------------------------------------------------------------*/
/* delete this                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void LifeGuard_t<CLASS>::vGuardedDelete(eCN eExpecting,
                                        CLASS** ppInstance,
                                        eCN eDeleterClassNo,
                                        Object* pDeleter,
                                        Stz* szDelFunc)
{
  m_eExpecting  = eExpecting;
  m_eDeleter   = eDeleterClassNo;
  m_pDeleter   = pDeleter;
  m_szDelFunNm = szDelFunc;
  
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
  
//  if (CN_PIPELINE == m_eInstance)
//    vTrace("vGuardedDelete: delete m_pInstance (PIPELINE)\n"_Stz);
  
  delete m_pInstance;
  
//  if (CN_PIPELINE == m_eInstance)
//    vTrace("vGuardedDelete: return from delete m_pInstance (PIPELINE)\n"_Stz);

  * ppInstance = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set who is going to explicitly delete this object                          */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void LifeGuard_t<CLASS>::vGuardedDeleteExplicit(eCN eExpecting,
                                                eCN eDeleter,
                                                Object* pDeleter,
                                                Stz* szDelFunNm)
{
  m_eExpecting  = eExpecting;
  m_eDeleter   = eDeleter;
  m_pDeleter   = pDeleter;
  m_szDelFunNm = szDelFunNm;

  vAssertDelete(false);
}

/*----------------------------------------------------------------------------*/
/* post recv() induction                                                      */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void LifeGuard_t<CLASS>::vInductGuard(CLASS* pInstance)
{
  
  m_pInstance    = pInstance;
  m_eInstance = m_pInstance->eGetClassNo();
  
  RASSERT(! m_pDeleter);
  RASSERT(! m_pDeleter);
  RASSERT(! m_eDeleter);
  RASSERT(! m_szDelFunNm);
  
//  vTrace("rcv %12s:%-3lld %p\n"_Stz
//         ,aCN[m_eInstance],m_eInstance,m_pInstance);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
