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
    bool    m_bDisable;
      
  public:
    LifeGuard_t(CLASS* pInstance, eCN eClassNo, bool bDisable=false);
   ~LifeGuard_t(void) noexcept(false);

    void vAssertDelete(bool bTrace=false);
    void vAssertNew(bool bTrace=false);
    void vAssertSelf(CLASS* pInstance);

    void vGuardedDelete(eCN eExpecting,
                        CLASS** ppInstance,
                        eCN eDeleter,
                        Object* pDeleter,
                        Stz* szDelFunc);
    
    void vInductIntoNode(CLASS* pInstance);
  };
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
LifeGuard_t<CLASS>::LifeGuard_t(CLASS* pInstance, eCN eThisClassNo, bool bDisable)
{
  m_eInstance = eThisClassNo;
  m_pInstance = pInstance;
  
  m_eExpecting = CN_NONE;
  m_eDeleter   = CN_NONE;
  m_pDeleter   = nullptr;
  m_szDelFunNm  = nullptr;
  
  m_bTrace = false;
  m_bDisable = bDisable;

  vAssertNew(true);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
LifeGuard_t<CLASS>::~LifeGuard_t(void) noexcept(false)
{
  if (m_bDisable)
    return;
  
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
  // formwork

  else if (CN_PIPEWORK == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // formworkSpec

  else if (CN_PIPESPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logic unit

  else if (CN_LOGICUNIT == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job

  else if (CN_JOB == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op
  
  else if (CN_OP == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // opSpec
  
  else if (CN_OPSPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeline

  else if (CN_PIPELINE == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipelineSpec
    
  else if (CN_PIPELINESPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stage
  
  else if (CN_STAGE == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stageSpec
  
  else if (CN_STAGESPEC == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task

  else if (CN_TASK == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stream
  
  else if (CN_STREAM == m_eInstance) {
    if (m_bTrace && bTrace)
      vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz
             ,aCN[m_eInstance],m_eInstance,m_pInstance);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // huh???
  
  else {
    vTrace("LifeGuard_t new %12s:%-3lld %p\n"_Stz,"(unknown)"_Stz
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
  m_eExpecting = eExpecting;
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
/* post recv() induction                                                      */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void LifeGuard_t<CLASS>::vInductIntoNode(CLASS* pInstance)
{
  
  m_pInstance = pInstance;
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
