/*============================================================================*/
/* Locks.hpp -- lock statistics tracker                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace BASE
{
  class Locks
  {
  public:
  
    DdlLock   m_oDdlMasterLock;
    DicLock   m_oDicMasterLock;
    TccLock   m_oTccMasterLock;
  
    Atomic m_ixWorkUnits[LK_ENUMS];       // number of work units (optional)
  
    Atomic m_ixReadGrabs[LK_ENUMS];       // number of read grabs
    Atomic m_ixReadCollisions[LK_ENUMS];  // number of read collisions
    
    Atomic m_ixWriteGrabs[LK_ENUMS];      // number of write grabs
    Atomic m_ixWriteCollisions[LK_ENUMS]; // number of write collisions
  
  public:
  Locks(void);

    void vMonitorPortrait(s48K& sPortrait);
    void vReport(void);
    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
