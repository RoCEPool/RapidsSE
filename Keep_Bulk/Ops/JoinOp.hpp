/*============================================================================*/
/* JoinOp.hpp -- Join                                                         */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class JoinOp : public Op
  {
  private:
    
    bool m_bEndLeftOperands;
    bool m_bEndRightOperands;
    
    EntBuffer* m_pLeftBuffer;
    EntBuffer* m_pRightBuffer;
    
    Ent* m_aLeftEntry;
    Ent* m_aRightEntry;
    
    eNE  m_eNextEntry;
    
    SrtBuffer* m_pSrtBuffer;
    
    bool m_bGotLeft;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    JoinOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~JoinOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR  eCall(void* aThis);
    
  private:
    void vAddRowRefs(Ent* aLeftEntry, Ent* aRightEntry);
    void vIndexJoin(EntBuffer* pLeftBuffer, EntBuffer* pRightBuffer);
    eOR  eJoinLoop(void);
  
  private:
    eOR ePopLeftOperand(void);
    eOR ePopRightOperand(void);
    
    Footer* aFooter(void);
  };

  JoinOp* _pJoinOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
