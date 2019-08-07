/*============================================================================*/
/* LockTracker.hpp -- lock statistics tracker                                 */
/*============================================================================*/
#pragma once

#include "Locking.h"

namespace Locking
{
  class LockTracker
  {
  public:
    Atomic m_ixWorkUnits[LK_ENUMS];       // number of work units (optional)
  
    Atomic m_ixReadGrabs[LK_ENUMS];       // number of read grabs
    Atomic m_ixReadCollisions[LK_ENUMS];  // number of read collisions
    
    Atomic m_ixWriteGrabs[LK_ENUMS];      // number of write grabs
    Atomic m_ixWriteCollisions[LK_ENUMS]; // number of write collisions
  
    void vMonitorPortrait(s48K& sPortrait);
    void vReport(void);
    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
