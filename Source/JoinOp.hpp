/*============================================================================*/
/* JoinOp.hpp -- Join                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class JoinOp
  {
  private:
  
  public:
    static void vIndexJoin(IndexTree* pLeftIndexTree, IndexTree* pRightIndexTree);
    static void vIndexJoin_v2(IndexTree* pLeftIndexTree, IndexTree* pRightIndexTree);
  };

  JoinOp* _pJoinOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
