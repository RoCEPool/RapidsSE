/*============================================================================*/
/* Focal.cpp - Focal Object                                                   */
/*============================================================================*/

#include "Correctness.h"

/*----------------------------------------------------------------------------*/
/* constructor (for non-tracked focal objects -- mainly locks in HeapBuffer)  */
/*----------------------------------------------------------------------------*/
Focal::Focal(eCN eClassNo)
: Object(eClassNo)
{
  m_i_Focal   = C_UNKNOWN;
  m_i_Log     = C_UNKNOWN;
  
  m_iFocalId  = C_UNKNOWN;
  m_iForId    = C_UNKNOWN;
  
  m_bForceLog = false;
  m__spare    = 0;

  m_iStepNo   = C_UNKNOWN;
  m_iStepTk   = AM::iRdTimePoint();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Focal::Focal(eCN eClassNo, i8 iUniqueId, i8 iForId)
: Object(eClassNo)
{
  m_i_Focal   = C_UNKNOWN;
  m_i_Log     = C_UNKNOWN;
  
  m_iFocalId  = iUniqueId;
  m_iForId    = iForId;
  
  m_bForceLog = false;
  m__spare    = 0;

  m_iStepNo   = C_UNKNOWN;
  m_iStepTk   = AM::iRdTimePoint();

  AM::vFocalAdd(this);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Focal::~Focal(void) noexcept(false)
{
  if (C_UNKNOWN != m_i_Focal)
    AM::vFocalRemove(this);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get last step sequence number                                              */
/*----------------------------------------------------------------------------*/
i8 Focal::iStepNo(void)
{
  return m_iStepNo;
}

/*----------------------------------------------------------------------------*/
/* inc last step sequence number                                              */
/*----------------------------------------------------------------------------*/
void Focal::vIncStepNo(void)
{
  m_iStepNo++;
  
  m_iStepTk = AM::iRdTimePoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
