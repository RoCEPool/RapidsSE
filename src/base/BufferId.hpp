/*============================================================================*/
/* BufferId.h -- BufferId                                                     */
/*============================================================================*/

#pragma once

class BufferId
{
public:
  union {
    struct {
      bool m_bIdFlag: 1; // Bit flag, 0 is address, 1 is buffer id
      u8 m_uSpare: 7; // Usually 0
      u8 m_uPageFile: 16; // Page file number
      u8 m_uBuffOffset: 40; // Buffer offset inside page file
    };

    Object *m_pAddress;
    i8 m_iAsI8;
  };
};

getAddressFromId(BufferId )
    {
  //Eventually have hashmap here?
  //
  return (Object*);
    }


