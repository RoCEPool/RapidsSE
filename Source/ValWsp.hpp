/*============================================================================*/
/* ValWsp.hpp - workspace for moving values to/from thread and task stacks    */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class ValWsp : public Value
  {
    static const i8 C_VALWSP_WORKSPACE = C_64K; //C_1KB * 15;
    
  private:
    char m_wWorkspace[C_VALWSP_WORKSPACE];
    
  public:
    void* operator new(size_t);
    ValWsp(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(char* pStr);
    void operator =(const char* pStr);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
