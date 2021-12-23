/*============================================================================*/
/* Locks.hpp -- lock manager                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace BASE
{
  class Locks : public Object
  {
  public:
  
    DdlLock m_oDdlMasterLock;
    DicLock m_oDicMasterLock;
    TccLock m_oTccMasterLock;
    
    Atomic  m_ixReadGrabs[CN_ENUMS];       // number of read grabs
    Atomic  m_ixReadCollisions[CN_ENUMS];  // number of read collisions
    
    Atomic  m_ixWriteGrabs[CN_ENUMS];      // number of write grabs
    Atomic  m_ixWriteCollisions[CN_ENUMS]; // number of write collisions
  
    // must be last
    Footer m_xFooter;

  public:
    Locks(void);

    void vReport(void);
    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
