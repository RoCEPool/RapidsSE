/*============================================================================*/
/* Report.hpp -- reporting                                                    */
/*============================================================================*/
#pragma once

namespace Correctness
{
  void  vFatal(Stz* aFormat,...);
  void  vInfo(Stz* aFormat,...);
  void  vTrace(Stz* aFormat,...);
  void  vWarn(Stz* aFormat,...);
  
  void  vPrint(Stz* aFormat, va_list xArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
