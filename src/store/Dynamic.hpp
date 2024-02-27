/*============================================================================*/
/* Dynamic.hpp -- Dynamic Code                                                */
/*============================================================================*/
#pragma once

#include "ClangDynamic.h"
#include "CrwBuffer.hpp"

namespace STORE
{

  class Dynamic : public Object
  {
  private:
    w8        m_wDummy;  // important: so that m_xDynamicT can be 128bit aligned
    Dynamic_T m_xDynamicT;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Dynamic(LogicUnit* pLogicUnit);
   ~Dynamic(void) noexcept(false);
    
    static void vRepair(Object* pObject);


  public:
    Dynamic_T* aDynamicT(void);
  
    Footer* aFooter(void);
  };
  
  Dynamic* _pDynamic(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
