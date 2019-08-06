/*============================================================================*/
/* ActLog.hpp -- Action log helper                              TO BE RECODED */
/*============================================================================*/
#pragma once

namespace Logging
{
  class ActLog
  {
  public:
    static void vAdd(i8 i_RootAct, i8 iJobId, eCN eClassNo,
                     eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
    static void vAdd(Jobs::Job* pJob,
                     eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
    static void vAdd(Jobs::Job* pJob,eAN eAction,
                     Jobs::Job* pSystemJob,i8 iArg0=0, i8 iArg1=0);
    static void vAdd(Jobs::Job* pJob,eAN eAction, Stz* szText);
    static i8   iBegin(i8 iJobId,  eCN eClassNo,
                       eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
    static void vEnd(Jobs::Job* pJob);
    
    static void vRoot(Jobs::Job* pJob);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
