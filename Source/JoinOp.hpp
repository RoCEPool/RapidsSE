/*============================================================================*/
/* JoinOp.hpp -- Join                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class RefBuffer;
  
  enum eNE // next entry
  {
    /*  0 */ NE_NEXT_LEFT=0,
    /*  1 */ NE_NEXT_RIGHT,
    /*  2 */ NE_NEXT_LEFT_RIGHT,
    /*  3 */ NE_NEXT_NONE,
    /*  4 */ NE_NEXT_EXIT,
    /*  5 */ NE_ENUMS_
  };
  
  static Stz* aNE[]
  {
    /*  0 */ "NEXT_LEFT"_Stz,
    /*  1 */ "NEXT_RIGHT"_Stz,
    /*  2 */ "NEXT_LEFT_RIGHT"_Stz,
    /*  3 */ "NEXT_NONE"_Stz,
    /*  4 */ "NEXT_EXIT"_Stz,
    /*  5 */ "ENUMS_"_Stz
  };

  class JoinOp
  {
  private:
  
  public:
    static void vIndexJoin(IndexTree* pLeftIndexTree, IndexTree* pRightIndexTree);
    
  private:
    static RefBuffer* pAddRowRefs(RefBuffer* pRefBuffer, Ent* aLeftEntry, Ent* aRightEntry);
  };

  JoinOp* _pJoinOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
