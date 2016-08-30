/*============================================================================*/
/* Row.hpp -- Row                                                             */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Row : public mn,
              public cn
  {
  private:
    i8  m_iTxId;       // transaction id
    i8  m_iDataBytes;  // number of data bytes (incl null teminator)
    // ..data..
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Row*);
    
    Row(void);
    ~Row(void);
    
    char* pData(void);
    i8    iDataBytes(void);
    void  vDump(void);
    i8    iSize(void);
    i8    iTxId(void);
    void  vTxId(i8 iTxId);
    
    static i8 iBytesRequired(const char* w);
    static i8 iBytesRequired(i8 iDataBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/