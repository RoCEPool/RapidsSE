/*============================================================================*/
/* Focal.cpp - Focal Object                                                   */
/*============================================================================*/

#include "Correctness.h"

#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/* constructor (for non-tracked focal objects -- mainly locks in HeapBuffer)  */
/*----------------------------------------------------------------------------*/
Focal::Focal(eCN eClassNo)
: Object(eClassNo)
{
  m_i_Focal   = C_UNKNOWN;
  m_iUniqueId = C_UNKNOWN;
  m_i_Log     = C_UNKNOWN;
  m_iTick     = AM::iTimePoint();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Focal::Focal(eCN eClassNo, i8 iUniqueId)
: Object(eClassNo)
{
  m_i_Focal   = C_UNKNOWN;
  m_iUniqueId = iUniqueId;
  m_i_Log     = C_UNKNOWN;
  m_iTick     = AM::iTimePoint();

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
