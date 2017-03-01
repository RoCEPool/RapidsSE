/*============================================================================*/
/* ValSet.hpp - set of values                                                 */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace DBobjects {
  class RowBuffer;
}

namespace Values
{
  class ValSet : public Value
  {
    static const i8 C_VALSET_DATAAREA = C_1K * 63; // has to fit inside valwsp
    
  private:
    i8  m_iBytes; // bytes used in work space
    DBobjects::RowBuffer* m_pRowBuffer;
    i8  m_iRowBufferBytes;
    char m_wWorkspace[C_VALSET_DATAAREA];
    
  public:
    void* operator new(size_t);
    ValSet(void);
    
    void vAppend(char*& pCursor, ValWsp& xVal);
    bool bExtract(char*& pCursor, ValWsp*& pVal);
    bool bExtract(char*& pCursor, eCN& eClassNo, ValWsp*& pVal);
    
    DBobjects::RowBuffer* pRowBuffer(void);
    void vRowBuffer(DBobjects::RowBuffer* pRowBuffer);
    
    DBobjects::RowBuffer* pGetRowBuffer();
    bool bHasRowBuffer();
    bool bSetRowBuffer(DBobjects::RowBuffer* pRowBuffer);
    
    i8   nSize(void);
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);

  private:
    char* pFirstByte(void);
    char* pLastByte(void);
    ValWsp* pToValWsp(char* pCursor);
  };
  
  ValSet* _pValSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
