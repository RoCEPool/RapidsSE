/*============================================================================*/
/* PtrVal.hpp - pointer value                                                 */
/*============================================================================*/

#pragma once

#include "StackValues.h"

namespace StackValues
{
  class PtrVal : public OnStack
  {
  private:
    void* m_pPtr;
    
  public:
    void* operator new(size_t);
    
    PtrVal(void);
    
    void operator =(void* pPtr);
    
    i8      iSize(void) const;
    void*   pPtr(void) const;
    Object* pObject(void) const;
    
    void vFetch(ValWsp& xValWsp);
    void vStore(ValWsp& xValWsp);
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
