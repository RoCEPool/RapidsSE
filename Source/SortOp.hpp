/*============================================================================*/
/* SortOp.hpp -- Sort                                                        */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class SortOp : public Op
  {
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SortOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn);
   ~SortOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
                    TokenVec* pTknVec, i8& i_Tkn_);
    
    static eOR eCall(void* aThis);
    
  private:
    eOR eSortLoop(void);
    Footer* aFooter(void);
  };

  SortOp* _pSortOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
