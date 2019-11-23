/*============================================================================*/
/* FipVar.hpp - Forth Instruction Pointer Sen Variable                        */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class FipVar
  {
  private:
    _Header m_xHeader;
    Cell*   m_aFip;

  public:
    void* operator new(size_t);
    
    void operator =(Cell* aCell);
    
    Cell* aFipPtr(void);
    i8    nSize(void);
    void  vToString(Stz* szStz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
