/*============================================================================*/
/* IntVal.hpp -- Layout of an i8 Integer when on Stack                        */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class IntVal : public StkVal
  {
  private:
    i8  m_iInt;
  
  public:
    void* operator new(size_t);
    
    IntVal(void);
    
    void operator =(i8 iVal);
    i8   iInt(void);
  
    i8   nStackBytes(void);
    void vToString(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline IntVal* _aIntVal(void* aAddr)
  {
    IntVal* aIntVal = reinterpret_cast<IntVal*>(aAddr);
  
    ASSERT( !aIntVal || CN_INTVAL == aIntVal->eClass() );
  
    return aIntVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
