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
    void* m_pPtr;
        
  public:
    void* operator new(size_t);
    
    PtrVal(void);
    
    void operator =(void* pPtr);
    
    i8      iSize(void) const;
    void*   pPtr(void) const;
    Header* pHeader(void) const;
    Object* pObject(void) const;
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
  };
  
  PtrVal* _pPtrVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
