/*============================================================================*/
/* Id_2part.hpp -- 2 part identifier                                          */
/*============================================================================*/
#pragma once

class Id_2part
{
  public:
  
  union {
    struct {
      u8 m_uObjectNo  : 48 ; // 0..256T
      u8 m_uNodeId    : 16 ; // 0..65,535
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
  
  public:

  Id_2part(void);
  
  i8   iAsI8(void);
  void vAsI8(i8 iAsI8);

  i8   nNodeBits(void);
  u8   uNodeMask(void);
  u8   uNodeId(void);
  void vNodeId(u8 uNodeId);

  i8   nObjectBits(void);
  u8   uObjectMask(void);
  u8   uObjectNo(void);
  void vObjectNo(u8 uObjectNo);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
