/*============================================================================*/
/* FipVal.hpp -- Layout of a Forth Instruction Pointer when on Stack          */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class FipVal : public AnyVal
  {
  private:
        
  public:
    void* operator new(size_t);
    
    void  operator =(Cell* aFip);
   
    i8    nStackUsage(void);
    void  vToString(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline FipVal* _aFipVal(void* aAddr)
  {
    FipVal* aFipVal = reinterpret_cast<FipVal*>(aAddr);
  
    RASSERT_TYPE(aFipVal,CN_FIPVAL);
  
    return aFipVal;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
