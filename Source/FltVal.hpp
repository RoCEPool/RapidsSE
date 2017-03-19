/*============================================================================*/
/* FltVal.hpp - float value                                                    */
/*============================================================================*/
#pragma once

namespace Values
{
  class FltVal : public Value
  {
  private:
    i8 m_iFlt;
    
  public:
    void* operator new(size_t);
    
    FltVal(void);
    
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8 iFlt(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  FltVal* _pFltVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
