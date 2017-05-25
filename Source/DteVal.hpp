/*============================================================================*/
/* DteVal.hpp - date value                                                    */
/*============================================================================*/
#pragma once

namespace Values
{
  class DteVal : public Value,
                 public Dte
  {
  private:
    
  public:
    void* operator new(size_t);
    
    DteVal(void);
    
    void operator =(Dte xDte);
    void operator =(i8 iVal);
  
    i8 nSize(void) const;
    i8* aInt(void);
    i8 iDte(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  DteVal* _pDteVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
