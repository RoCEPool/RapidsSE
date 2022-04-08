/*============================================================================*/
/* PhyPtn.hpp -- Node.Partiton identifier                                     */
/*============================================================================*/
#pragma once

class PhyPtn : public Gid_t<CN_PTNID>
{
  public:

  PhyPtn(void);
  PhyPtn(i8 iAsI8);

  // auto conversion
  operator PhyPtn*(void);
  
  // auto conversion
  operator i8(void);

  void operator =(i8 iAsI8);
  void operator =(PhyPtn tPhyPtn);

  i8 operator ++(void); // increment (prefix form)
  i8 operator ++(int);  // increment (postfix)
  bool operator <(i8 iAsI8);

  i8   nPtnBits(void);
  u8   uPtnMask(void);
  u8   uPtnNo(void);
  void vPtnNo(u8 uPtnNo);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
