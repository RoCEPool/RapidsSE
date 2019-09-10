/*============================================================================*/
/* ZedOp.hpp -- Zed                                                           */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ZedOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ZedOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn);
   ~ZedOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
                    TokenVec* pTknVec, i8& i_Tkn_);
    static eOR eCall(void* aThis);
  
    eOR eOpLoop(void);
  
  private:
    Footer* aFooter(void);
  };

  ZedOp* _pZedOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
