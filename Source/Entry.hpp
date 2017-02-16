/*============================================================================*/
/* Entry.hpp -- index entry                                                   */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Entry : public Header
  {
  private:
    RowId m_xRowId;      // row number
    i8    m_iDataBytes;  // number of data bytes
    // ..data..
    
  public:
    static i8 iBytesRequired(i8 iDataBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(i8 iDataBytes);
    ~Entry(void);
    
    void  vContentDump(void);

    RowId& xRowId(void);
    void   vRowId(RowId& xRowId);

    Key&   xKey(void);
    void   vKey(Key& xKey);
  
    //------
  
    char* pData(void);
    i8    iDataBytes(void);
    void  vDataBytes(i8 iDataBytes);
    i8    iSize(void);
  
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
