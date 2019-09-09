/*============================================================================*/
/* OutputOp.hpp -- Output                                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class OutputOp : public Op
  {
  private:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OutputOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec=nullptr);
   ~OutputOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  OutputOp* _pOutputOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
