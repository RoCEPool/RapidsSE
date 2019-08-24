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
    
    SortOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~SortOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    
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
