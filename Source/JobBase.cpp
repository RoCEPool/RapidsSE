/*============================================================================*/
/* JobBase.cpp -- base of Job                                                 */
/*============================================================================*/

#include "Jobs.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor (for job__find)                                                */
/*----------------------------------------------------------------------------*/
JobBase::JobBase(eCN eClassNo)
: Focal(eClassNo,C_UNKNOWN,C_UNKNOWN)
, m_xReqTuple(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  ASSERT(CN_JOB__FIND == eClassNo);

  m_aJobClass = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobBase::JobBase(JobClass* aJobClass, eCN eClassNo, ReqTuple* aReqTuple, i8 iJobId, i8 iForId)
: Focal(eClassNo,iJobId,iForId)
, m_xReqTuple(aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  ASSERT(aReqTuple);
  ASSERT(aJobClass->m_eClass == eClassNo);
  ASSERT(CN_JOB__FIND != eClassNo);

  m_aJobClass = aJobClass;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobBase::~JobBase(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
