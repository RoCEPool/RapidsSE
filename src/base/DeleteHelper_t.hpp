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
  
  public:
    DeleteHelper_t(void);
   ~DeleteHelper_t(void) noexcept(false);

    void vGuardedDelete(CLASS*& prThis, Object* pDeleter, Stz* szFunc);
    void vGuardedDeleteExplicit(Object* pDeleter, Stz* szFunc);
  };
  
}

#include "Job.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Pipeline.hpp"
#include "PipelineSpec.hpp"
#include "Stage.hpp"
#include "StageSpec.hpp"
#include "Stream.hpp"
#include "Task.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::DeleteHelper_t(void)
{
  m_pDeleter = nullptr;
  m_szFunc   = nullptr;
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::~DeleteHelper_t(void) noexcept(false)
{
  if (typeid(CLASS) == typeid(Job)) {
    if (! m_szFunc)
      vDebugPoint();
    RASSERT(m_szFunc);
  }
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
