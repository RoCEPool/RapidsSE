/*============================================================================*/
/* FipVar.hpp - Forth Instruction Pointer Sen Variable                        */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class FipVar : public SenVar
  {
  private:
    Cell* m_aFip;

  public:
    void* operator new(size_t);
    
    void  operator =(Cell* aFip);
    Cell* aFipPtr(void);
  
    i8    nStackBytes(void);
    void  vToString(Stz* szStz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
