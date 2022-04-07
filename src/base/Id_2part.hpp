/*============================================================================*/
/* Id_2part.hpp -- 2 part identifier                                          */
/*============================================================================*/
#pragma once

#include "SEtype.hpp"

class Id_2part : public SEtype
{
  protected:

  union {
    struct {
      u8 m_uPartNo  : 48 ; // 0..256T
      u8 m_uNodeId  : 16 ; // 0..65,535
      //             ---
      //              64
      //             ===
    };
    i8 m_iAsI8;
  };
  
  public:

  Id_2part(void);
  Id_2part(i8 iAsI8);

  void operator =(i8 iAsI8); // assignment
  
  i8   iAsI8(void);
  void vAsI8(i8 iAsI8);

  i8   nNodeBits(void);
  u8   uNodeMask(void);
  u8   uNodeId(void);
  void vNodeId(u8 uNodeId);

  i8   nPartBits(void);
  u8   uPartMask(void);
  u8   uPartNo(void);
  void vPartNo(u8 uObjectNo);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
