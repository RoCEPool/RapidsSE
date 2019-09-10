/*============================================================================*/
/* AggOp.hpp -- Aggregator                                                    */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class AggOp : public Op
  {
  private:
  
    StreamDef* m_pOriginalStreamDef;
    i8         m_iWhichVec[C_MAX_COLUMNS];
    i8         m_iWhichCol[C_MAX_COLUMNS];
    i8         m_nSum;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AggOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~AggOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);
    
    eOR eOpLoop(void);
    eOR eDrainLoop(void);
    
  private:
    void vUpdateStreamDef(void);
    
    Footer* aFooter(void);  
  };

  AggOp* _pAggOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
