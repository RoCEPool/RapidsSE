/*============================================================================*/
/* SkipListNode.hpp -- SkipListNode (SLN)                                     */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_SLN_PADDING = 2;
  typedef u2 tSO; // opaque type for Skip Node Offset

  class SkipListNode
  {
  public:
    tSO m_taNextSkipNode[C_MAX_LVL_LEVELS]; // offsets to next entry by level (note: next entry = offset * 16 to get # of bytes

    char m_cPadding[C_SLN_PADDING];

  public:
    SkipListNode(void);
    ~SkipListNode(void) noexcept(false);

    u2 uNext(i8 iLevel);
    void vNext(i8 iLevel, u2 uOffset);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
