/*============================================================================*/
/* ValSet.hpp - set of values                                                 */
/*============================================================================*/

#pragma once

#include "StackValues.h"

namespace StackValues
{
  class ValSet : public OnStack
  {
    static const i8 C_VALSET_SPACE = C_1KB * 16 - 128; // fudge
  private:
    i8   m_iBytes; // bytes used out of space
    char m_mSpace[C_VALSET_SPACE];
    
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