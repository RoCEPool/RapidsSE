/*============================================================================*/
/* GetKeyOp.hpp -- Output                                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class GetKeyOp : public Op
  {
  private:
  
    s256 m_sKey;
    i8   m_iKeyCol;
    i8   m_iColVec;
    bool m_bKeyIsLeft;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    GetKeyOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~GetKeyOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  GetKeyOp* _pGetKeyOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
