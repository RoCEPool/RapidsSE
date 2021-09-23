/*============================================================================*/
/* SsnId.hpp -- Node.Ssn identifier                                           */
/*============================================================================*/
#pragma once

class SsnId : public Gid_t<CN_SESSION>
{
  public:

  SsnId(void);
    
  operator SsnId*(void);
  
  void operator =(i8 iAsI8);
  void operator =(SsnId tSsnId);

  i8   nSsnBits(void);
  u8   uSsnMask(void);
  u8   uSsnNo(void);
  void vSsnNo(u8 uSsnNo);
  
  void vNextSsnId(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
