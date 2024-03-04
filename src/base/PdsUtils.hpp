/*============================================================================*/
/* PdsUtils.hpp -- Utility structures and functions for Pds                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class PdsUtils {
  public:
    static i8 iHashMapEntryKeyCompare(void* aItem_l, void* aItem_r);
    static i8 iIntCompare(void* aItem_l, void* aItem_r);
    static i8 iLglBufSetKeyCompare(void* aItem_l, void* aItem_r);
    static i8 iPotPdsItemKeyCompare(void* aItem_l, void* aItem_r);
  };
}