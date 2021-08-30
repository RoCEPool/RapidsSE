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

    void vDeleteBy(Object* pDeleter, Stz* szFunc);
    void vDeleteThis(Object* pDeleter, Stz* szFunc);
  };
  
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::DeleteHelper_t(void)
{
  m_pDeleter = nullptr;
  m_szFunc   = nullptr;
}

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template<class CLASS>
DeleteHelper_t<CLASS>::~DeleteHelper_t(void) noexcept(false)
{
//  if (typeid(CLASS) == typeid(Job)) {
//    vTrace("~DeleteHelper_t: testing Job\n"_Stz);
//  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set who is going to delete this object                                     */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vDeleteBy(Object* pDeleter, Stz* szFunc)
{
  m_pDeleter = pDeleter;
  m_szFunc   = szFunc;
}

/*----------------------------------------------------------------------------*/
/* delete this                                                                */
/*----------------------------------------------------------------------------*/
template<class CLASS>
void DeleteHelper_t<CLASS>::vDeleteThis(Object* pDeleter, Stz* szFunc)
{
  m_pDeleter = pDeleter;
  m_szFunc   = szFunc;

  delete (CLASS*)this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
