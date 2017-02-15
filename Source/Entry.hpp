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
    i8  m_iTxId;       // transaction id
    i8  m_iDataBytes;  // number of data bytes (incl null teminator)
    // ..data..
    
  public:
    static i8 iBytesRequired(const char* w);
    static i8 iBytesRequired(i8 iDataBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(i8 iDataBytes);
    ~Entry(void);
    
    void  vContentDump(void);
    char* pData(void);
    i8    iDataBytes(void);
    void  vDataBytes(i8 iDataBytes);
    i8    iSize(void);
    i8    iTxId(void);
    void  vTxId(i8 iTxId);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
