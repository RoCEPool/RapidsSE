/*============================================================================*/
/* KeyVal.hpp -- Key based on Value Semantics                                 */
/*============================================================================*/
#pragma once

#include "Structures.h"

namespace Structures
{
  class KeyVal : public Key
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
    
    KeyVal(void);
    ~KeyVal(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
  private:
    
    Footer* pFooter(void);
    
  };
  
  KeyVal* _pKeyVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/