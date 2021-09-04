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
    eCN     m_eDeleterClassNo;
    Stz*    m_szFunc;
    
    eCN     m_eThisClassNo;
    //const std::type_info& m_rTypeId;
  
  public:
    DeleteHelper_t(eCN eClassNo);//, const std::type_info& rTypeId);
   ~DeleteHelper_t(void) noexcept(false);

    void vAssertDelete(bool bTrace=false);

    void vGuardedDelete(CLASS*& prThis, Object* pDeleter, Stz* szFunc);
    void vGuardedDeleteExplicit(Object* pDeleter, Stz* szFunc);
  };
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::DeleteHelper_t(eCN eThisClassNo)
{
  m_eDeleterClassNo = CN_NONE;
  m_szFunc          = nullptr;
  
  m_eThisClassNo    = eThisClassNo;
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
/* destructor                                                                 */
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
      vTrace("FreeChunk %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logic unit

  else if (CN_LOGICUNIT == m_eThisClassNo) {
    if (bTrace)
      vTrace("LogicUnit %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (   m_szFunc->bSame("~Job"_Stz)
               || m_szFunc->bSame("~Op"_Stz)
               || m_szFunc->bSame("vDelete"_Stz)
               || m_szFunc->bSame("vResetForCascade"_Stz)
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
      vTrace("Job %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (   m_szFunc->bSame("vDeleteJob"_Stz)
               || m_szFunc->bSame("vJobDelete"_Stz)
               || m_szFunc->bSame("pFindJob"_Stz)
               || m_szFunc->bSame("~Task"_Stz)
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
      vTrace("Op %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (    m_szFunc->bSame("~Stream"_Stz)
              )
            );
    RASSERT(CN_STREAM == m_eDeleterClassNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // opSpec
  
  else if (CN_OPSPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("OpSpec %p deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (   m_szFunc->bSame("vStageFirstSend"_Stz)
               || m_szFunc->bSame("vStageSubsequentSend"_Stz)
               || m_szFunc->bSame("~StageSpec"_Stz)
              )
           );
    RASSERT(CN_STAGESPEC == m_eDeleterClassNo ||
            CN_PIPELINESPEC == m_eDeleterClassNo);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeline

  else if (CN_PIPELINE == m_eThisClassNo) {
    if (bTrace)
      vTrace("Pipeline %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (    m_szFunc->bSame("~LogicUnit"_Stz)
              )
            );
    RASSERT(CN_LOGICUNIT == m_eDeleterClassNo);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipelineSpec
    
  else if (CN_PIPELINESPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("PipelineSpec %p deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (   m_szFunc->bSame("~Pipeline"_Stz)
               || m_szFunc->bSame("vPipelineSpecDelete"_Stz)
               || m_szFunc->bSame("pCompile"_Stz)
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
      vTrace("Stage %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (    m_szFunc->bSame("~Pipeline"_Stz)
              )
            );
    RASSERT(CN_PIPELINE == m_eDeleterClassNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stageSpec
  
  else if (CN_STAGESPEC == m_eThisClassNo) {
    if (bTrace)
      vTrace("StageSpec %p deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (    m_szFunc->bSame("~PipelineSpec"_Stz)
              )
            );
    RASSERT(CN_PIPELINESPEC == m_eDeleterClassNo);
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task

  else if (CN_TASK == m_eThisClassNo) {
    if (bTrace)
      vTrace("Task %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (   m_szFunc->bSame("vTaskSelf_DeleteThisTask"_Stz)
               || m_szFunc->bSame("???"_Stz)
              )
            );

    RASSERT(CN_API     == m_eDeleterClassNo);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stream
  
  else if (CN_STREAM == m_eThisClassNo) {
    if (bTrace)
      vTrace("Stream %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

    RASSERT(m_szFunc &&
              (    m_szFunc->bSame("~Stage"_Stz)
              )
            );
    RASSERT(CN_STAGE == m_eDeleterClassNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // huh???
  
  else {
    vTrace("Unrecognised %p class being deleted\n"_Stz,this);
    vTrace("m_eThisClassNo=%lld:%s\n"_Stz
           ,m_eThisClassNo
           ,aCN[m_eThisClassNo]
           );
    vTrace("m_szFunc=%s\n"_Stz
           ,m_szFunc);
    vTrace("m_eDeleterClassNo=%lld:%s\n"_Stz
           ,m_eDeleterClassNo
           ,aCN[m_eDeleterClassNo]
           );
  }

}

/*----------------------------------------------------------------------------*/
/* delete this                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vGuardedDelete(CLASS*& prThis,
                                           Object* pDeleter,
                                           Stz* szFunc)
{
  m_eDeleterClassNo = pDeleter ? pDeleter->eGetClassNo() : CN_API;
  m_szFunc          = szFunc;
  
  vAssertDelete(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete (CLASS*)this;
  
  prThis = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set who is going to explicitly delete this object                          */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vGuardedDeleteExplicit(Object* pDeleter, Stz* szFunc)
{
  m_eDeleterClassNo = pDeleter ? pDeleter->eGetClassNo() : CN_API;
  m_szFunc          = szFunc;

  vAssertDelete(false);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
