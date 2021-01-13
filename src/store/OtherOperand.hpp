/*============================================================================*/
/* OtherOperand.hpp -- Other Operand                                          */
/*============================================================================*/
#pragma once

#include "eOM.h"

namespace STORE
{
  class OtherOperand : public Operand
  {
  protected:
    Object* m_pObject;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    OtherOperand(Object* pObject);
   ~OtherOperand(void) noexcept(false);

    Object* pObject(void);
  
  private:
    Footer* aFooter(void);
  };

  OtherOperand* _pOtherOperand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
