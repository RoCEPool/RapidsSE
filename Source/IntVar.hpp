/*============================================================================*/
/* IntVar.hpp - integer value                                                 */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class IntVar : public Object
  {
  private:
    i8     m_iInt;
    Footer m_xFooter;

  public:
    void* operator new(size_t);
  
    IntVar(void);
   ~IntVar(void) noexcept(false);
  
    void operator =(i8 iVal);
  
    i8   iInt(void);
    i8   nSize(void);
    void vToString(Stz* aWorkspace);
  
  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
