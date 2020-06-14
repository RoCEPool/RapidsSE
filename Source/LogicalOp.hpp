/*============================================================================*/
/* LogicalOp.hpp -- LogicalOp                                                       */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class LogicalOp : public Operand
  {
  protected:
    s32       m_sName;
    TokenVec* m_pTokenVec;
  
  public:
    void operator delete(void*);
  
    LogicalOp(eCN eClass, TokenVec* pTokenVec);
   ~LogicalOp(void) noexcept(false);
  
    void vAsString(s4K& sFilterStr);
  };

  LogicalOp* _pFilter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
