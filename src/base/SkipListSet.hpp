/*============================================================================*/
/* SkipListSet.hpp -- Set of Node and Item                                   */
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
  class SkipListSet
  {
  public:
    SkipListNode  m_xNode;
    char m_xItem[1];

  public:
    SkipListSet(eCN eClassNo, i8 nSizeEnt);
    ~SkipListSet(void) noexcept(false);

    void* aItemBody(void);
    void* aItemKey(void);
    i8 iKey(void);

    void vSetKey(i8 iKey);

  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
