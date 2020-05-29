/*============================================================================*/
/* KeyCpp.hpp -- Key based on C++ Semantics                                   */
/*============================================================================*/
#pragma once

#include "Structures.h"

namespace Structures
{
  class KeyCpp : public Key
  {
  public:
    i4  m_nElements;  // number of elements in this key
    eRO m_eRelnOp[C_KEY_ELEMENTS]; // set of relation operators
    ValReg m_xRelnVal[C_KEY_ELEMENTS]; // set of relation values

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t iSize, i4 iLevel);
    void  operator delete(void*);
    
    KeyCpp(void);
    ~KeyCpp(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
  private:
    
    Footer* pFooter(void);
    
  };
  
  KeyCpp* _pKeyCpp(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/