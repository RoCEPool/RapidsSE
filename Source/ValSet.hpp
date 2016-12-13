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
    static const i8 C_VALSET_WORKSPACE = C_1KB * 14;
    
  private:
    i8  m_iBytes; // bytes used in work space
    DBobjects::RowBuffer* m_pRowBuffer;
    i8  m_iRowBufferBytes;
    char m_wWorkspace[C_VALSET_WORKSPACE];
    
  public:
    void* operator new(size_t);
    ValSet(void);
    
    void vAppend(char*& pCursor, ValWsp& xVal);
    bool bExtract(char*& pCursor, ValWsp*& pVal);
    i8   iSize(void);
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);

  private:
    char* pFirstByte(void);
    char* pLastByte(void);
    ValWsp* pToValWsp(char* pCursor);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
