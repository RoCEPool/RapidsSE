/*============================================================================*/
/* PtnId.hpp -- Node.Partiton identifier                                      */
/*============================================================================*/
#pragma once

class PtnId : public Gid_t<CN_PTNSTREAM>
{
  public:

  PtnId(void);
  PtnId(i8 iAsI8);

  // auto conversion
  operator PtnId*(void);
  
  void operator =(i8 iAsI8);
  void operator =(PtnId tPtnId);

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
