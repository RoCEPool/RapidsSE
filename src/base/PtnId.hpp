/*============================================================================*/
/* PtnId.hpp -- Node.Partiton identifier                                      */
/*============================================================================*/
#pragma once

class PtnId : public Gid_t<CN_PTNSTREAM>
{
  public:

  PtnId(void);
  
  operator PtnId*(void);
  
  void operator =(i8 iAsI8);
  void operator =(PtnId tPtnId);

  i8   nPtnBits(void);
  u8   uPtnMask(void);
  u8   uPtnNo(void);
  void vPtnNo(u8 uPtnNo);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
