/*============================================================================*/
/* JobId.hpp -- Node.Job identifier                                           */
/*============================================================================*/
#pragma once

class JobId : public Gid_t<CN_JOB>
{
  public:

  JobId(void);
    
  operator JobId*(void);
  
  void operator =(i8 iAsI8);
  void operator =(JobId tJobId);

  i8   nJobBits(void);
  u8   uJobMask(void);
  u8   uJobNo(void);
  void vJobNo(u8 uJobNo);
  
  void vNextJobId(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
