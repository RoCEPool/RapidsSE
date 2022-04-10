/*============================================================================*/
/* CmdId.hpp -- Command Id (NodeId,CmdNo)                                     */
/*============================================================================*/
#pragma once

#include "SEtype.hpp"

class CmdId : public SEtype
{
  protected:

  union {
    struct {
      u8 m_uNode : 16 ; // 0..64K
      u8 m_uCmd  : 48 ; // 0..256G
      //          ---
      //           64
      //          ===
    };
    i8 m_iAsI8;
  };
  
  public:

  CmdId(void);

  // auto conversion
  operator i8(void);

  i8   iCmdNo(void);

  void vNextCmdId(void);

  i8   iNodeId(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
