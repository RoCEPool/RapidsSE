/*============================================================================*/
/* Op_ToDo.hpp -- To Do Operator                                              */
/*============================================================================*/
#pragma once

namespace CompilerOperators
{
  class Op_ToDo : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op_ToDo(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~Op_ToDo(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    void vPromptHandOff(Cmd_Select* pCmd_Select);
    Footer* aFooter(void);
  };

  Op_ToDo* _pOp_ToDo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
