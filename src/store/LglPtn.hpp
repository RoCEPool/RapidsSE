/*============================================================================*/
/* LglPtn.hpp -- Logical Partiton identifier                                  */
/*============================================================================*/
#pragma once

//class LglPtn : public Gid_t<CN_PTNID>
namespace STORE
{

class LglPtn : public SEtype
{
  private:
    i8 m_iLglPtn;
  
  public:

  LglPtn(void);
  LglPtn(i8 iAsI8);

  
  // auto conversion
  operator i8(void);

  void operator =(i8 iAsI8);
  void operator =(LglPtn tLglPtn);

  i8 operator ++(int);  // increment (postfix)
  bool operator <(i8 iAsI8);
  
  i8   iAsI8(void);
  void vAsI8(i8 iAsI8);
};

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
