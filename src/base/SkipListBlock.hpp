/*============================================================================*/
/* SkipListBlock.hpp -- Block of Node and Item                                */
/*============================================================================*/
#pragma once

#include "SkipListNode.hpp"

#define member_size(type, member) sizeof(((type *)0)->member)

namespace BASE
{
  struct conceptual__item{
    i8 iKey;

    //body start here
  };

  // mini-block
  class SkipListBlock
  {
  public:
    SkipListNode  m_xNode;
    i8 m_xItem[1];

  public:
    SkipListBlock(eCN eClassNo, i8 nSizeEnt);
    ~SkipListBlock(void) noexcept(false);

    void* aItem(void);
    i8 iKey(void);

    SkipListBlock* aNext(void);

    Row* aRow(void);

    void vSetKey(i8 iKey);

  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
