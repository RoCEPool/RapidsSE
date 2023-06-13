/*============================================================================*/
/* SkipListSet.hpp -- Set of Node and Item                                   */
/*============================================================================*/
#pragma once

#include "SkipListNode.hpp"

namespace BASE
{
  class SkipListSet
  {
  public:
    SkipListNode  m_xNode;
    i16 m_iItem;

  public:
    SkipListSet(eCN eClassNo, i8 nSizeEnt);
    ~SkipListSet(void) noexcept(false);

    void vSetKey(i16 iKey);

  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
