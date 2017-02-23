/*============================================================================*/
/* PtrVal.hpp - pointer value                                                 */
/*============================================================================*/

#pragma once

#include "Values.h"

#include "Object.hpp"

namespace Values
{
  class PtrVal : public Value
  {
  private:
    Object* m_pPtr;
        
  public:
    void* operator new(size_t);
    
    PtrVal(void);
    
    void operator =(Object* pObject);
    
    i8      iSize(void) const;
    Object* pPtr(void) const;
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
  };
  
  PtrVal* _pPtrVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
