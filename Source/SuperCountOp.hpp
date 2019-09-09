/*============================================================================*/
/* SuperCountOp.hpp -- Super Count                                            */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  
  class SuperCountOp : public Op
  {
  private:
  
    i8  m_nCount;     
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SuperCountOp(OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR);
   ~SuperCountOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    
    static eOR eCall(void* aThis);
    
  private:
    eOR        eSuperCountLoop(void);
  
    Footer* aFooter(void);
  };

  SuperCountOp* _pSuperCountOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
