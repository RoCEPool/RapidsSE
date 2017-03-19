/*============================================================================*/
/* TmeVal.hpp - time value                                                    */
/*============================================================================*/
#pragma once

namespace Values
{
  class TmeVal : public Value
  {
  private:
    i8 m_iTme;
    
  public:
    void* operator new(size_t);
    
    TmeVal(void);
    
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8 iTme(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  TmeVal* _pTmeVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
