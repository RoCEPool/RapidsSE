/*============================================================================*/
/* Filter.hpp -- Filter                                                       */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter : public Operand
  {
  protected:
    s32       m_sName;
    TokenVec* m_pTokenVec;
  
  public:
    void operator delete(void*);
  
    Filter(eCN eClass, TokenVec* pTokenVec);
   ~Filter(void) noexcept(false);
  
    void vAsString(s4K& sFilterStr);
  };

  Filter* _pFilter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
