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
//, m_xReqTuple(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  ASSERT(CN_JOB__FIND == eClassNo);

  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobBase::JobBase(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iJobId)
: Focal(aJobClass->m_eClass,iJobId,aReqTuple->m_iForId)
//, m_xReqTuple(aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,aJobClass->m_eClass);
  
  ASSERT(aReqTuple);
  ASSERT(aJobClass);

  
  _postcondition_(_ADDRESS_,_MAGIC_,aJobClass->m_eClass);
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
