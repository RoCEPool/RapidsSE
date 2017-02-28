/*============================================================================*/
/* Entry.hpp -- index entry                                                   */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Entry : public RowId
  {
  private:
    i8    m_nDataBytes;  // number of data bytes
    // ..data..
    
  public:
    static i8 iBytesRequired(i8 iDataBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(i8 iDataBytes);
    Entry(EntWsp& xEntWsp);
    ~Entry(void);
    
    void  vContentDump(void);

    RowId& xRowId(void);
    void   vRowId(RowId& xRowId);

    void   vEntry(EntWsp& xEntWsp);

    Key&   xKey(void);
    Key*   pKey(void);
    void   vKey(Key& xKey);
    void   vKey(KeyWsp& xKeyWsp);
  
    //------
  
    char* pDataArea(void);
    i8    nDataBytes(void);
    void  vDataBytes(i8 iDataBytes);
    char* pDataNext(void);
    i8    nSize(void);
  
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
