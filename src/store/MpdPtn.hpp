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
      u8 m_u_LglPtn        : 32 ; // 4B
      u8 m_uMappedNodeId   : 16 ; // 0..65,535
      u8 m_uMappedStreamId : 16 ; // 0..65,535
      //                    ---
      //                     64
      //                    ===
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

  i8   i_LglPtn(void);
  void v_LglPtn(i8 i_LglPtn);
  
  LglPtn tLglPtn(void);
  void   vLglPtn(LglPtn tLglPtn);
  
  void vMap(void);

  i8   iMappedNodeId(void);
  void vMappedNodeId(i8 iMappedNodeId);
  i8   iMappedStreamId(void);
  void vMappedStreamId(i8 iMappedStreamId);

  void vSet(LglPtn tLglPtn, i8 iNodeId, i8 iStreamId);

  bool bValid(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
