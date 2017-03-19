/*============================================================================*/
/* TspVal.hpp - integer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class TspVal : public Value
  {
  private:
    i8 m_iTsp;
    
  public:
    void* operator new(size_t);
    
    TspVal(void);
    
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8 iTsp(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  TspVal* _pTspVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
