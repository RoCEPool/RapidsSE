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
  
    Atomic m_ixWorkUnits[CN_ENUMS];       // number of work units (optional)
  
    Atomic m_ixReadGrabs[CN_ENUMS];       // number of read grabs
    Atomic m_ixReadCollisions[CN_ENUMS];  // number of read collisions
    
    Atomic m_ixWriteGrabs[CN_ENUMS];      // number of write grabs
    Atomic m_ixWriteCollisions[CN_ENUMS]; // number of write collisions
  
  public:
  Locks(void);

    void vReport(void);
    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
