/*============================================================================*/
/* IntVal.hpp - integer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class IntVal : public Value,
                 public Dtm
  {
  private:
    
  public:
    void* operator new(size_t);
    
    IntVal(void);
    
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8* aInt(void);
    i8 iInt(void) const;
    
    void vToString(Stz* aWorkspace);
  
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
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
