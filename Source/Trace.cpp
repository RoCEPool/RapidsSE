/*============================================================================*/
/* Trace.cpp - message reporter                                               */
/*============================================================================*/

#include "Correctness.h"

bool  _g_bJotting = false;

i8    _g_nDepth = 0;
i8    _g_iSection = 0;
char* _g_sSection;

i8    _g_iDivision = 0;
char* _g_sDivision;

i8    _g_iSummary = 0;
char* _g_sSummary;

i8    _g_iLine = 0;

//
// joting //////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Correctness::vJot1(char cFlag, Stz* aFormat, ...)
{
  if (! _g_bJotting)
    return;
  
  va_list xVaArgs;

  AM::vWriteTrace(YA_SYS_TRACE,"#%s.%04x.%02x.%03x# %x%c "_Stz,
                  _g_sDivision,
                  _g_iDivision,
                  _g_iSection,
                  _g_iLine++,
                  _g_nDepth,
                  cFlag
                  );
  
  if (_g_nDepth)
    AM::vWriteTrace(YA_SYS_TRACE,"%s"_Stz, Stz::szTab(_g_nDepth));

  if ('r' == cFlag)
    AM::vWriteTrace(YA_SYS_TRACE,"- "_Stz, Stz::szTab(_g_nDepth));

  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_TRACE,aFormat,xVaArgs);
  va_end(xVaArgs);
  
  AM::vWriteTrace(YA_SYS_TRACE,"\n"_Stz, Stz::szTab(_g_nDepth));
}

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Correctness::vJot2(char cFlag, Stz* szFun, Stz* aFormat, ...)
{
  if (! _g_bJotting)
    return;
  
  va_list xVaArgs;

  AM::vWriteTrace(YA_SYS_TRACE,"#%s.%04x.%02x.%03x# %x%c "_Stz,
                  _g_sDivision,
                  _g_iDivision,
                  _g_iSection,
                  _g_iLine++,
                  _g_nDepth,
                  cFlag
                  );
  
  if (_g_nDepth) {
    AM::vWriteTrace(YA_SYS_TRACE,"%s"_Stz, Stz::szTab(_g_nDepth));
  }
  
  AM::vWriteTrace(YA_SYS_TRACE,"%s"_Stz,szFun);
  if ('>' == cFlag || '<' == cFlag)
    AM::vWriteTrace(YA_SYS_TRACE,"()"_Stz,szFun);

  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_TRACE,aFormat,xVaArgs);
  va_end(xVaArgs);
  
  AM::vWriteTrace(YA_SYS_TRACE,"\n"_Stz, Stz::szTab(_g_nDepth));
}

/*----------------------------------------------------------------------------*/
/* jot dividing line 1                                                        */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDiv1(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "----------------------------------------"
    "----------------------------------------"
    "----------------------------------------"
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line 2                                                        */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDiv2(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "========================================"
    "========================================"
    "========================================"
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line equals&space                                             */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDivES(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "= = = = = = = = = = = = = = = = = = = = "
    "= = = = = = = = = = = = = = = = = = = = "
    "= = = = = = = = = = = = = = = = = = = = "
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line minus&space                                               */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDivMS(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "- - - - - - - - - - - - - - - - - - - - "
    "- - - - - - - - - - - - - - - - - - - - "
    "- - - - - - - - - - - - - - - - - - - - "
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line minus&space                                               */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDivPS(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "+ + + + + + + + + + + + + + + + + + + + "
    "+ + + + + + + + + + + + + + + + + + + + "
    "+ + + + + + + + + + + + + + + + + + + + "
  );
}


/*----------------------------------------------------------------------------*/
/* jot dividing line A                                                        */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDivA(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "++++++++++++++++++++++++++++++++++++++++"
    "++++++++++++++++++++++++++++++++++++++++"
    "++++++++++++++++++++++++++++++++++++++++"
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line B                                                        */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDivB(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "+ + + + + + + + + + + + + + + + + + + + "
    "+ + + + + + + + + + + + + + + + + + + + "
    "+ + + + + + + + + + + + + + + + + + + + "
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line B                                                        */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDivC(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "########################################"
    "########################################"
    "########################################"
  );
}

/*----------------------------------------------------------------------------*/
/* jot section                                                                */
/*----------------------------------------------------------------------------*/
void Correctness::vJotDivX(void)
{
  if (! _g_bJotting)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
  );
}

/*----------------------------------------------------------------------------*/
/* tab in for next jotting                                                    */
/*----------------------------------------------------------------------------*/
void Correctness::vJotSet(bool bJot)
{
  _g_bJotting = bJot;
}


/*----------------------------------------------------------------------------*/
/* get current jot section                                                    */
/*----------------------------------------------------------------------------*/
i8 Correctness::iJotSection(void)
{
  return _g_iSection;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 Correctness::iJotSection(char cFlag, Stz* szFun, Stz* szInfo)
{
  _g_sSection = szFun->azChar();
  
  _g_iSection++;
  
//  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
//    "- - - - - - - - - - - - - - - - - - - - "
//    "- - - - - - - - - - - - - - - - - - - - "
//    "- - - - - - - - - - - - - - - - - - - - "
//    );
  
  vJot1('-',"%s"_Stz,
         "- - - - - - - - - - - - - - - - - - - - "
         "- - - - - - - - - - - - - - - - - - - - "
         "- - - - - - - - - - - - - - - - - - - - "
       );
  
  vJot2(cFlag,szFun,"%s"_Stz,szInfo);
  
  return _g_iSection;
}

/*----------------------------------------------------------------------------*/
/* get current jot section                                                    */
/*----------------------------------------------------------------------------*/
i8 Correctness::iJotSummary(void)
{
  return _g_iSummary;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 Correctness::iJotSummary(char cFlag, Stz* szFun, Stz* szInfo)
{
  _g_sSummary = szFun->azChar();
  
  _g_iSection++;
  _g_iSummary++;
  
//  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
//    "= = = = = = = = = = = = = = = = = = = = "
//    "= = = = = = = = = = = = = = = = = = = = "
//    "= = = = = = = = = = = = = = = = = = = = "
//    );
  
  vJot1(cFlag,"%s"_Stz,
         "= = = = = = = = = = = = = = = = = = = = "
         "= = = = = = = = = = = = = = = = = = = = "
         "= = = = = = = = = = = = = = = = = = = = "
       );
  
  vJot1(cFlag,"%s"_Stz,szInfo);
  
  return _g_iSummary;
}

/*----------------------------------------------------------------------------*/
/* get current jot division                                                   */
/*----------------------------------------------------------------------------*/
i8 Correctness::iJotDivision(void)
{
  return _g_iDivision;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 Correctness::iJotDivision(char cFlag, Stz* szFun, Stz* SzInfo)
{
  _g_sDivision = szFun->azChar();
  
  _g_iDivision++;
  _g_iSection = 0;
  _g_iLine = 0;
  
  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "////////////////////////////////////////"
    "////////////////////////////////////////"
    "////////////////////////////////////////"
    );
  
  vJot2(cFlag,szFun,"%s"_Stz,SzInfo);
  
  return _g_iDivision;
}

/*----------------------------------------------------------------------------*/
/* tab in for next jotting                                                    */
/*----------------------------------------------------------------------------*/
void Correctness::vJotTabIn(void)
{
  _g_nDepth++;
}

/*----------------------------------------------------------------------------*/
/* tab out for next jotting                                                   */
/*----------------------------------------------------------------------------*/
void Correctness::vJotTabOut(void)
{
  if (_g_nDepth)
    _g_nDepth--;
}

//
// tracing /////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* report an information message                                              */
/*----------------------------------------------------------------------------*/
void Correctness::vInfo(Stz* aFormat, ...)
{
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_INFO,aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Correctness::vTrace(Stz* aFormat, ...)
{
  va_list xVaArgs;

  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_TRACE,aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*----------------------------------------------------------------------------*/
/* report a warning message                                                   */
/*----------------------------------------------------------------------------*/
void Correctness::vWarn(Stz* aFormat, ...)
{
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_WARN,aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
