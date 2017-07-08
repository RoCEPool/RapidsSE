/*============================================================================*/
/* LockTrack.hpp -- lock statistics tracker                                   */
/*============================================================================*/
#pragma once

namespace System
{
  class LockTrack
  {
  public:
    Atomic m_zReadGrabs;       // number of read grabs
    Atomic m_zReadCollisions;  // number of read collisions
    
    Atomic m_zWriteGrabs;      // number of write grabs
    Atomic m_zWriteCollisions; // number of write collisions
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
