/*============================================================================*/
/* FltVal.hpp - float value                                                    */
/*============================================================================*/
#pragma once

namespace Values
{
  class FltVal : public Value
  {
  private:
    d8 m_dFlt;
    
  public:
    void* operator new(size_t);
    
    FltVal(void);
    
    void operator =(d8 dVal);
    
    i8 nSize(void) const;
    d8 dFlt(void) const;
    d8* aFlt(void);
  
    void vToString(Str* aWorkspace);
  };
  
  FltVal* _pFltVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
