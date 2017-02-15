/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Key : public Part
  {
  private:
    i8  m_nElements;   // number of elements in the key
    i8  m_iDataBytes;  // number of data bytes
    // ..data..
    
  public:
    static i8 iBytesRequired(const char* w);
    static i8 iBytesRequired(i8 iDataBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Key*);
    
    Key(i8 iDataBytes);
    ~Key(void);
    
    void  vContentDump(void);
    char* pData(void);
    i8    iDataBytes(void);
    void  vDataBytes(i8 iDataBytes);
    i8    iSize(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
