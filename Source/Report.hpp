/*============================================================================*/
/* Report.hpp -- reporting                                                    */
/*============================================================================*/
#pragma once

namespace Correctness
{
  void  vJot(Stz* szFormat,...);

  void  vJotDiv1(void);
  void  vJotDiv2(void);
  void  vJotDiv3(void);

  void  vJotDivA(void);
  void  vJotDivB(void);
  void  vJotDivC(void);

  void  vJotDivMS(void);
  void  vJotDivPS(void);

  void  vJotDivX(void);
  void  vJotDivY(void);
  void  vJotDivZ(void);

  i8    iJotSection(Stz* szSubject);

  void  vJotSet(bool bJot);
  void  vJotTabIn(void);
  void  vJotTabOut(void);
 

  void  vInfo(Stz* szFormat,...);
  void  vTrace(Stz* szFormat,...);
  void  vWarn(Stz* szFormat,...);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
