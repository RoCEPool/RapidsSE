/*============================================================================*/
/*  Row.hpp - Row                                                             */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Row : public Object
  {
    
  private:
    i8    m_iDataBytes;  // number of data bytes
    // ...data..
    
  public:
    void* operator new(size_t);
    
    Row();
    ~Row(void);
    
    char* pData(void);
    char* pDataEnd(void);
    i8    iDataBytes(void) const;
    void  vDump(void);
    Row*  pPrevRow(void) const;
    i8    iSize(void) const;
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/