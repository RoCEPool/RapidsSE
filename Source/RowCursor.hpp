/*============================================================================*/
/* RowCursor.hpp - row cursor                                                 */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class RowCursor : public Object
  {
  private:

  public:
    void* operator new(size_t);
    void* operator new(size_t, void* pRowCursorAddress);
    void  operator delete(void*);
    
    RowCursor(void);
    ~RowCursor(void);
    
  };  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/