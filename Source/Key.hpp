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
    i8  m_nDataBytes;  // number of data bytes in data area
    // ..dataArea..
    
  public:
    static i8 iBytesRequired(const char* w);
    static i8 iBytesRequired(i8 iDataBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Key*);
    
    Key(i8 nDataBytes);
    Key(KeyWsp& xKeyWsp);
    ~Key(void);
  
    bool operator <(Key* pKeyArg);

    void  vContentDump(void);
  
    void  vKey(Key& xKey);
    void  vKey(KeyWsp& xKeyWsp);
  
    //----
  
    char* pDataArea(void);
    i8    nDataBytes(void);
    void  vDataBytes(i8 iDataBytes);
    char* pDataNext(void);
    i8    iSize(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
