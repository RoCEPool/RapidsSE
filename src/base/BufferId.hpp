/*============================================================================*/
/* BufferId.hpp -- BufferId                                                   */
/*============================================================================*/

#pragma once

namespace BASE
{
  class BufferId
  {
  public:
    union {
      struct {
        u8   m_uBuffOffset   : 40 ; // Buffer offset inside page file
        u8   m_uPageFile     : 16 ; // Page file number
        u8   m_uSpare        :  7 ; // Usually 0
        bool m_bIdFlag       :  1 ; // Bit flag, 0 is address, 1 is buffer id
      };

      void*  m_vAsAddress;
      i8     m_iAsI8;
    };

    static void* vAddressFromId(BufferId oBuffId);
    static BufferId vBufferIdFromNo(i8 iBufferNo);

  };
}
