/*============================================================================*/
/* MpdPtn.hpp -- Mapped Partiton                                              */
/*============================================================================*/
#pragma once

#include "LglPtn.hpp"

class MpdPtn : public SEtype
{
  private:
  union {
    struct {
      u8 m_uLglPtn   : 16 ; // 0..65,535
      u8 m_uNodeId   : 16 ; // 0..65,535
      u8 m_uStreamId : 32 ; // 0..4B
      //              ---
      //               64
      //              ===
    };
    i8 m_iAsI8;
  };

  public:

  MpdPtn(void);
  MpdPtn(i8 iAsI8);
  
  // auto conversion
  operator i8(void);

  void operator =(i8 iAsI8);
  void operator =(MpdPtn tMpdPtn);

  i8 operator ++(int); // increment (postfix form)
  bool operator <(i8 iAsI8);

  i8   iAsI8(void);
  void vAsI8(i8 iAsI8);

  LglPtn tLglPtn(void);
  void   vLglPtn(LglPtn tLglPtn);
  
  i8   iNodeId(void);
  void vNodeId(i8 iNodeId);

  void vSet(LglPtn tLglPtn, i8 iNodeId, i8 iStreamNo);

  i8   iStreamId(void);
  void vStreamId(i8 iStreamId);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
