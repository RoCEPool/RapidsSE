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
    Object* m_pDeleter;
    Stz*    m_szFunc;
    
    eCN     m_eClassNo;
    const std::type_info& m_rTypeId;
  
  public:
    DeleteHelper_t(eCN eClassNo, const std::type_info& rTypeId);
   ~DeleteHelper_t(void) noexcept(false);

    void vGuardedDelete(CLASS*& prThis, Object* pDeleter, Stz* szFunc);
    void vGuardedDeleteExplicit(Object* pDeleter, Stz* szFunc);
  };
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::DeleteHelper_t(eCN eClassNo, const std::type_info& rTypeId)
: m_rTypeId(rTypeId)
{
  m_pDeleter = nullptr;
  m_szFunc   = nullptr;
  
  m_eClassNo = eClassNo;
  
  //m_rTypeId  = rTypeId;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::~DeleteHelper_t(void) noexcept(false)
{
  //if (CN_PIPELINESPEC == m_eClassNo)
  //  vTrace("%s deleted by %s()\n"_Stz,aCN[m_eClassNo],m_szFunc);
  
  /*
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job
  
  if (typeid(CLASS) == typeid(Job)) {
    if (! m_szFunc)
      vDebugPoint();
    RASSERT(m_szFunc);
    
    vTrace("Job deleted by %s()\n"_Stz,m_szFunc);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // actual
  
  else
  if (typeid(CLASS) == typeid(Pipeline)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("Pipeline deleted by %s()\n"_Stz,m_szFunc);
  }
  
  else
  if (typeid(CLASS) == typeid(Stage)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("Stage deleted by %s()\n"_Stz,m_szFunc);
  }
  
  else
  if (typeid(CLASS) == typeid(Stream)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("Stream deleted by %s()\n"_Stz,m_szFunc);
  }
  
  else
  if (typeid(CLASS) == typeid(Op)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("Op deleted by %s()\n"_Stz,m_szFunc);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // specs
  
  else
  if (typeid(CLASS) == typeid(PipelineSpec)) {
    if (! m_szFunc)
      vDebugPoint();
    RASSERT(m_szFunc);
    
    vTrace("PipelineSpec deleted by %s()\n"_Stz,m_szFunc);
  }
  
  else
  if (typeid(CLASS) == typeid(StageSpec)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("StageSpec deleted by %s()\n"_Stz,m_szFunc);
  }
  
  else
  if (typeid(CLASS) == typeid(OpSpec)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("OpSpec deleted by %s()\n"_Stz,m_szFunc);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task
  
  else
  if (typeid(CLASS) == typeid(Task)) {
//    if (! m_szFunc)
//      vDebugPoint();
//    RASSERT(m_szFunc);
    
    vTrace("Task deleted by %s()\n"_Stz,m_szFunc);
  }

  
  else {
    // ?
  }
*/
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* delete this                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vGuardedDelete(CLASS*& prThis,
                                        Object* pDeleter,
                                        Stz* szFunc)
{
  m_pDeleter = pDeleter;
  m_szFunc   = szFunc;

  delete (CLASS*)this;
  
  prThis = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set who is going to explicitly delete this object                          */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vGuardedDeleteExplicit(Object* pDeleter, Stz* szFunc)
{
  m_pDeleter = pDeleter;
  m_szFunc   = szFunc;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
