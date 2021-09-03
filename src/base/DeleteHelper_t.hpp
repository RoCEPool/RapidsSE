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
DeleteHelper_t<CLASS>::DeleteHelper_t(eCN eThisClassNo
                                      //, const std::type_info& rTypeId
                                      )
//: m_rTypeId(rTypeId)
{
  m_eDeleterClassNo = CN_NONE;
  m_szFunc          = nullptr;
  
  m_eThisClassNo    = eThisClassNo;
  
  //m_rTypeId  = rTypeId;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::~DeleteHelper_t(void) noexcept(false)
{
//  if (! m_szFunc)
//    vDebugPoint();
//
//  if (! m_eDeleterClassNo)
//    vDebugPoint();
//
//  RASSERT(m_szFunc);
//  RASSERT(m_eDeleterClassNo);
  
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
    
  //if (CN_PIPELINESPEC == m_eThisClassNo)
  //  vTrace("%s being deleted from %s()\n"_Stz,aCN[m_eClassNo],m_szFunc);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spec
  
//  if (CN_PIPELINESPEC == m_eThisClassNo) {
//    if (bTrace)
//      vTrace("PipelineSpec deleted from %s()\n"_Stz,m_szFunc);
//    if (! m_szFunc)
//      vDebugPoint();
//
//    RASSERT(m_szFunc &&
//              (   m_szFunc->bSame("~Pipeline"_Stz)
//               || m_szFunc->bSame("vPipelineDeleteSpec"_Stz)
//               || m_szFunc->bSame("pCompile"_Stz)
//              )
//            );
//    RASSERT(CN_DYNAMO == m_eDeleterClassNo   ||
//            CN_PIPELINE == m_eDeleterClassNo ||
//            CN_TASK == m_eDeleterClassNo);
//  }
//
//  else if (CN_STAGESPEC == m_eThisClassNo) {
//    if (bTrace)
//      vTrace("StageSpec deleted from %s()\n"_Stz,m_szFunc);
//    if (! m_szFunc)
//      vDebugPoint();
//
//    RASSERT(m_szFunc &&
//              (    m_szFunc->bSame("~PipelineSpec"_Stz)
//              )
//            );
//    RASSERT(CN_PIPELINESPEC == m_eDeleterClassNo);
//  }
//
//  else if (CN_OPSPEC == m_eThisClassNo) {
//    if (bTrace)
//      vTrace("OpSpec deleted from %s()\n"_Stz,m_szFunc);
//    if (! m_szFunc)
//      vDebugPoint();
//
//    RASSERT(m_szFunc &&
//              (   m_szFunc->bSame("vStageFirstSend"_Stz)
//               || m_szFunc->bSame("vStageSubsequentSend"_Stz)
//               || m_szFunc->bSame("~StageSpec"_Stz)
//              )
//           );
//    RASSERT(CN_STAGESPEC == m_eDeleterClassNo ||
//            CN_PIPELINESPEC == m_eDeleterClassNo);
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job

  if (CN_JOB == m_eThisClassNo) {
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
  // logic unit

  if (CN_LOGICUNIT == m_eThisClassNo) {
    if (bTrace)
      vTrace("LogicUnit %p being deleted from %s()\n"_Stz,this,m_szFunc);
//    if (! m_szFunc)
//      vDebugPoint();
//
//    RASSERT(m_szFunc &&
//              (   m_szFunc->bSame("~Job"_Stz)
//               || m_szFunc->bSame("~Op"_Stz)
//               || m_szFunc->bSame("vDelete"_Stz)
//              )
//            );
//
//    RASSERT(CN_API   == m_eDeleterClassNo ||
//            CN_OP    == m_eDeleterClassNo  ||
//            CN_JOB   == m_eDeleterClassNo);
//
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // actual
  
  //else
  if (CN_PIPELINE == m_eThisClassNo) {
    if (bTrace)
      vTrace("Pipeline %p being deleted from %s()\n"_Stz,this,m_szFunc);
    if (! m_szFunc)
      vDebugPoint();

//    RASSERT(m_szFunc &&
//              (    m_szFunc->bSame("~LogicUnit"_Stz)
//              )
//            );
//    RASSERT(CN_LOGICUNIT == m_eDeleterClassNo);
  }
  
//  else if (CN_STAGE == m_eThisClassNo) {
//    if (bTrace)
//      vTrace("Stage being deleted from %s()\n"_Stz,m_szFunc);
//    if (! m_szFunc)
//      vDebugPoint();
//
//    RASSERT(m_szFunc &&
//              (    m_szFunc->bSame("~Pipeline"_Stz)
//              )
//            );
//    RASSERT(CN_PIPELINE == m_eDeleterClassNo);
//  }
//
//  else if (CN_STREAM == m_eThisClassNo) {
//    if (bTrace)
//      vTrace("Stream being deleted from %s()\n"_Stz,m_szFunc);
//    if (! m_szFunc)
//      vDebugPoint();
//
//    RASSERT(m_szFunc &&
//              (    m_szFunc->bSame("~Stage"_Stz)
//              )
//            );
//    RASSERT(CN_STAGE == m_eDeleterClassNo);
//  }
//
//  else if (CN_OP == m_eThisClassNo) {
//    if (bTrace)
//      vTrace("Op being deleted from %s()\n"_Stz,m_szFunc);
//    if (! m_szFunc)
//      vDebugPoint();
//
//    RASSERT(m_szFunc &&
//              (    m_szFunc->bSame("~Stream"_Stz)
//              )
//            );
//    RASSERT(CN_STREAM == m_eDeleterClassNo);
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  else {
//    // ???
//  }


  
  /*
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // actual
  
  
  
  else
  if (typeid(CLASS) == typeid(Stream)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("Stream being deleted from %s()\n"_Stz,m_szFunc);
  }
  
  else
  if (typeid(CLASS) == typeid(Op)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("Op being deleted from %s()\n"_Stz,m_szFunc);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // specs
  
  else
  if (typeid(CLASS) == typeid(PipelineSpec)) {
    if (! m_szFunc)
      vDebugPoint();
    RASSERT(m_szFunc);
    
    vTrace("PipelineSpec being deleted from %s()\n"_Stz,m_szFunc);
  }
  
  else
  if (typeid(CLASS) == typeid(StageSpec)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("StageSpec being deleted from %s()\n"_Stz,m_szFunc);
  }
  
  else
  if (typeid(CLASS) == typeid(OpSpec)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("OpSpec being deleted from %s()\n"_Stz,m_szFunc);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task
  
  else
  if (typeid(CLASS) == typeid(Task)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("Task being deleted from %s()\n"_Stz,m_szFunc);
  }

  
  else {
    // ?
  }
*/
  
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
