/*============================================================================*/
/* Combined.hpp -- Tub-Based Inflate using Dynamic Code                       */
/*============================================================================*/
#pragma once

#include "CrwBuffer.hpp"
#include "RowBuffer.hpp"

namespace STORE
{

  class Combined : public Object
  {
  private:
    Combined_T m_xCombinedT;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Combined(LogicUnit* pLogicUnit);
   ~Combined(void) noexcept(false);
    
    static void vRepair(Object* pObject);


  public:
    Combined_T* aCombinedT(void);
  
    Footer* aFooter(void);
  };
  
  Combined* _pCombined(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
