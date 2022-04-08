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
      u8 m_uLglPtn : 16 ; // 64K
      
      u8 m_uNodeId : 16 ; // 64K
      u8 m_uPtnNo  : 16 ; // 64K
      
      u8 m_uSpare  : 16 ; // 64K
      //            ---
      //             64
      //            ===
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

  bool bIsNode(i8 iNodeId);
  bool bIsThisNode(void);

  i8     iLglPtn(void);
  LglPtn tLglPtn(void);
  void   vLglPtn(i8 iLglPtn);
  void   vLglPtn(LglPtn tLglPtn);
  
  void vMap(void);

  i8   iNodeId(void);
  void vNodeId(i8 iNodeId);
  i8   iPtnNo(void);
  void vPtnNo(i8 iPtnNo);

  //void vSet(LglPtn tLglPtn, i8 iNodeId, i8 iPtnNo);

  bool bValid(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
