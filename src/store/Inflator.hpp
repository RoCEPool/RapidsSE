/*============================================================================*/
/* Inflator.hpp -- Tub-Based Inflate using Dynamic Code                       */
/*============================================================================*/
#pragma once

#include "CrwBuffer.hpp"
#include "RowBuffer.hpp"

namespace STORE
{
  class Inflator : public Object
  {
  private:
    CrwTubInflate_T m_xCrwTubInflate;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Inflator(void);
   ~Inflator(void) noexcept(false);
    
    static void vRepair(Object* pObject);


  public:
    CrwTubInflate_T* aCrwTubInflate(void);
  
    Footer* aFooter(void);
  };
  
  Inflator* _pInflator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
