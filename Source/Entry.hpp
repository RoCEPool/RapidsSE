/*============================================================================*/
/* Entry.hpp -- index entry                                                   */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Entry
  {
  private:
    RowId  m_xRowId;
    WspVbl m_xWsp;
  
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
  
    i8    nSize(void);
  
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
