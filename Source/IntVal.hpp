/*============================================================================*/
/* IntVal.hpp - integer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class IntVal : public Value
  {
  private:
    i8  m_iInt;
  
  public:
    void* operator new(size_t);
    
    IntVal(void);
    
    void operator =(i8 iVal);
    
    i8   iInt(void) const;
    i8   nSize(void) const;
    void vToString(Stz* aWorkspace);
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
