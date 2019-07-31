/*============================================================================*/
/* JoinOp.hpp -- Join                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class JoinOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    JoinOp(Stz* szOpLabel, Jobs::Job* pJob);
   ~JoinOp(void) noexcept(false);
  
    static void vIndexJoin(Queue* pQueue, IndexTree* pLeftIndexTree, IndexTree* pRightIndexTree);
    
  private:
    static RefBuffer* pAddRowRefs(Queue* pQueue, RefBuffer* pRefBuffer,
                                  Ent* aLeftEntry, Ent* aRightEntry);
  
  private:
    Footer* aFooter(void);
  };

  JoinOp* _pJoinOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
