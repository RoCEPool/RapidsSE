/*============================================================================*/
/* IntVal.hpp - integer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class IntVal : public Value,
                 public Int
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
  };
  
  IntVal* _pIntVal(void* aAddr);
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline IntVal* _pIntVal(void* aAddr)
  {
  IntVal* pIntVal = reinterpret_cast<IntVal*>(aAddr);
  
  ASSERT( !pIntVal || CN_INTVAL == pIntVal->eClass() );
  
  return pIntVal;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
