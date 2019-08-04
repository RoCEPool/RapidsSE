/*============================================================================*/
/* OpVec.hpp -- Operator Set                                                  */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OpVec : public VectorNL_t<CN_OPVEC,Op,32>
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    OpVec(void);
   ~OpVec(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  OpVec* _pOpSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
