/*============================================================================*/
/* OpSet.hpp -- Operator Set                                                  */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OpSet : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    OpSet(void);
   ~OpSet(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  OpSet* _pOpSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
