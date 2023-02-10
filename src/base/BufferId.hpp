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
        u8   m_uSeekOffset    : 40 ; // Buffer seek offset inside page file, multiple off a megabyte
        u8   m_uPageFile      : 16 ; // Page file number
        u8   m_uSpare         :  7 ; // Usually 0
        bool m_bIdFlag        :  1 ; // Bit flag, 0 is address, 1 is buffer id
      };

      void*  m_pvAsAddress;
      i8     m_iAsI8;
    };

    BufferId(void);
    BufferId(void* pAddr);
    BufferId(i8 iVal);

    static void* aAddressFromId(BufferId oBufferId);
    static BufferId vBufferIdFromNo(i8 iPoolBufferNo);
    static bool bIsAddress(BufferId oBufferId);
    static bool bIsBufferId(BufferId oBufferId);
    static bool bIsBufferId(void* aAddr);
    void vSetBuffSubscript(u8 uBuffSubscript);
    void vSetPageFile(u8 uPageFile);
    void vSetIdFlag(bool bFlag);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/