/*============================================================================*/
/* Gid.h -- Identifier within a Grid                                          */
/*============================================================================*/
#pragma once

namespace Entries
{
  static const i8 C_NEO_SIZE = sizeof(i4);
  
  class NEO // next entry offset array
  {
  public:
    i4 m_iOffset[1];  // offset in buffer where the next entry is
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
