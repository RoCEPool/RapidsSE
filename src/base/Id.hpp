/*============================================================================*/
/* Id.hpp -- Node.Object identifier                                           */
/*============================================================================*/
#pragma once

class Id
{
  public:
  
  union {
    struct {
      u8 m_uObjectNo  : 48 ; // 0..256T
      u8 m_uNodeNo    : 16 ; // 0..65,535
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
  
  public:

  Id(void);
  
  i8   iAsI8(void);
  void vAsI8(i8 iAsI8);

  i8   nNodeBits(void);
  u8   uNodeMask(void);
  u8   uNodeNo(void);
  void vNodeNo(u8 uNodeNo);

  i8   nObjectBits(void);
  u8   uObjectMask(void);
  u8   uObjectNo(void);
  void vObjectNo(u8 uObjectNo);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
