/*============================================================================*/
/* LimitOp.hpp -- Limit                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class LimitOp : public Op
  {
  private:
  
    i8 m_nLimit;    // what is the limit?
    i8 m_nSoFar;    // how many so far
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    LimitOp(Stz* szOpLabel, Digest* pDigest);
   ~LimitOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel, Digest* pDigest, 
                    TokenVec* pTknVec, i8& i_Tkn_);
    static eOR eCall(void* aThis);
  
    eOR eLimitLoop(void);

  private:
    Footer* aFooter(void);
  };

  LimitOp* _pLimitOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
