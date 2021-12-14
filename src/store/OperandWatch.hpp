/*============================================================================*/
/* OperandWatch.hpp - Operand Watch                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class OperandWatch : public Object
  {
  public:
  
    struct
    {
      i8 m_nCnt;
      i8 m_iFirst;
      i8 m_iLast;
    }
    m_xaOperand[ZK_ENUMS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OperandWatch(void);
   ~OperandWatch(void) noexcept(false);
      
    i8   nCnt(i8 i_Operand);
    i8   iFirst(i8 i_Operand);
    i8   iLast(i8 i_Operand);
    void vReset(void);
    void vSaw(i8 i_Operand, i8 iAt);

    Footer* aFooter(void);
  };

  OperandWatch* _pOperandWatch(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
