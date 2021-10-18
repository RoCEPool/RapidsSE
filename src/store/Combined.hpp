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
    CrwTubInflate_T m_xCrwTubInflate;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Combined(void);
   ~Combined(void) noexcept(false);
    
    static void vRepair(Object* pObject);


  public:
    CrwTubInflate_T* aCrwTubInflate(void);
  
    Footer* aFooter(void);
  };
  
  Combined* _pCombined(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
