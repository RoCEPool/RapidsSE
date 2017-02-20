/*============================================================================*/
/* ValWsp.hpp - workspace for manipulating values                             */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class ValWsp : public Value
  {
    static const i8 C_VALWSP_DATAAREA = C_64K; // less than medium block
    
  private:
    char m_mDataArea[C_VALWSP_DATAAREA];
    
  public:
    void* operator new(size_t);
    ValWsp(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(char* pStr);
    void operator =(const char* pStr);
    
    void vAssignBinary(Object* pObject);
  
    char* pDataArea(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
