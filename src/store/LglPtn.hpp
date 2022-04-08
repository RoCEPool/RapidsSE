/*============================================================================*/
/* LglPtn.hpp -- Node.Partiton identifier                                     */
/*============================================================================*/
#pragma once

class LglPtn : public Gid_t<CN_PTNID>
{
  public:

  LglPtn(void);
  LglPtn(i8 iAsI8);

  // auto conversion
  operator LglPtn*(void);
  
  // auto conversion
  operator i8(void);

  void operator =(i8 iAsI8);
  void operator =(LglPtn tLglPtn);

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
