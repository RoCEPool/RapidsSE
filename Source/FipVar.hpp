/*============================================================================*/
/* FipVar.hpp - Forth Instruction Pointer Sen Variable                        */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class FipVar : public Object
  {
  private:
    Cell*  m_aFip;
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    
    FipVar(void);
   ~FipVar(void) noexcept(false);

    void operator =(Cell* aCell);
    
    Cell* aFipPtr(void);
    i8    nSize(void);
    void  vToString(Stz* szStz);
  
  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
