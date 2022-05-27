/*============================================================================*/
/* ChildExit.hpp -- Exit Operand                                            */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ChildExit : public Operand
  {
  protected:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);

    ChildExit(void);
   ~ChildExit(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ChildExit* _pChildExit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
