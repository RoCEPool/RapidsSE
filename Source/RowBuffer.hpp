/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class RowBuffer : public SegmentBuffer
  {
  private:
    
  public:
    RowBuffer(void);
    ~RowBuffer(void);
    
    bool bInsertRow(StrVal* pRow);
    
    RowBuffer* pNextBuffer(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/