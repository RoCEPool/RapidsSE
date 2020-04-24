/*============================================================================*/
/* Report.hpp -- reporting                                                    */
/*============================================================================*/
#pragma once

namespace Correctness
{
  void  vJot1(char cFlag, Stz* szFormat,...);
  void  vJot2(char cFlag, Stz* szFun, Stz* szFormat,...);

  void  vJotDiv1(void);
  void  vJotDiv2(void);
  void  vJotDiv3(void);

  void  vJotDivA(void);
  void  vJotDivB(void);
  void  vJotDivC(void);

  void  vJotDivES(void);
  void  vJotDivMS(void);
  void  vJotDivPS(void);

  void  vJotDivX(void);
  void  vJotDivY(void);
  void  vJotDivZ(void);

  i8    iJotDivision(void);
  i8    iJotDivision(char cFlag, Stz* szArea, Stz* szFun, Stz* szDivision);

  i8    iJotSection(void);
  i8    iJotSection(char cFlag, Stz* szFun, Stz* szSection);

  i8    iJotSummary(void);
  i8    iJotSummary(char cFlag, Stz* szFun, Stz* szSection);

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
