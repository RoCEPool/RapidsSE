/*============================================================================*/
/* Report.hpp -- reporting                                                    */
/*============================================================================*/
#pragma once

namespace Debugging
{
  void  vInfo(Stz* aFormat,...);
  void  vTrace(Stz* aFormat,...);
  void  vWarn(Stz* aFormat,...);
  void  vError(Stz* aFormat,...);
  void  vFatal(Stz* aFormat,...);
  
  void  vPrint(Stz* aFormat, va_list xArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
