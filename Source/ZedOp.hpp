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
  
    ZedOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~ZedOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
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
