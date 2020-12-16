/*============================================================================*/
/* Jot.cpp - Jot Debug Rig                                                    */
/*============================================================================*/

#include "Resources.h"

i8    _g_iDebug   = 0;
char* _g_sPhrase  = nullptr;

char* _g_sArea = nullptr;

i8    _g_iDivision = 0;
char* _g_sDivision;

i8    _g_iSection = 0;
char* _g_sSection = nullptr;

i8    _g_iSummary = 0;
char* _g_sSummary = nullptr;

i8    _g_nDepth = 0;
i8    _g_iLine  = 0;

i8    _g_iTriggerDivision = 0;
i8    _g_iTriggerDebug    = 0;


i8    _g_iDisableDivision = 0;
i8    _g_iDisableDebug    = 0;


#define C_JOT_BUFF 1024 EndDefine
#define C_JOT_TAB    70 EndDefine

//
// joting //////////////////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Resources::vJot1(char cFlag, Stz* szFormat, ...)
{
  if (! _g_iDebug)
    return;
  
  va_list xVaArgs;
  
  char caBuff[C_JOT_BUFF];

  ASSERT(cFlag != '>' && cFlag != '<');
  
  if ('<' == cFlag && _g_nDepth)
    _g_nDepth--;

  StrLib::vSprint(caBuff,
                  "#%s::%s.%04x.%02x.%03x# %x%c "_Stz,
                  _g_sArea,
                  _g_sDivision,
                  _g_iDivision,
                  _g_iSection,
                  _g_iLine++,
                  _g_nDepth,
                  cFlag
                  );
  
  if (_g_nDepth)
    StrLib::vSprintCat((Stz*)caBuff,"%s"_Stz, Stz::szTab(_g_nDepth));
  
  if ('r' == cFlag)
    StrLib::vSprintCat((Stz*)caBuff,"- "_Stz);

  va_start(xVaArgs,szFormat);
  StrLib::vSprintCat(caBuff,xVaArgs,szFormat->azChar());
  va_end(xVaArgs);
  
  if ('>' == cFlag || '<' == cFlag)
    StrLib::vSprintCat((Stz*)caBuff,"()"_Stz,szFormat);

  if (! _g_sPhrase)
    AM::vWriteTrace(YA_SYS_TRACE,"%s\n"_Stz,caBuff);
  else
  if (szFormat->bContains((Stz*)_g_sPhrase))
    AM::vWriteTrace(YA_SYS_TRACE,"%s\n"_Stz,caBuff);

  if ('>' == cFlag)
     _g_nDepth++;

}

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Resources::vJot2(char cFlag, Stz* szFun, Stz* szFormat, ...)
{
  if (! _g_iDebug)
    return;
  
  va_list xVaArgs;
  
  char caBuff[C_JOT_BUFF];

  if ('<' == cFlag && _g_nDepth)
     _g_nDepth--;

  StrLib::vSprint(caBuff,
                  "#%s::%s.%04x.%02x.%03x# %x%c "_Stz,
                  _g_sArea,
                  _g_sDivision,
                  _g_iDivision,
                  _g_iSection,
                  _g_iLine++,
                  _g_nDepth,
                  cFlag
                  );

  
  if (_g_nDepth)
    StrLib::vSprintCat((Stz*)caBuff,"%s"_Stz, Stz::szTab(_g_nDepth));
  
  if ('r' == cFlag)
    StrLib::vSprintCat((Stz*)caBuff,"- "_Stz);
  
  StrLib::vSprintCat((Stz*)caBuff,"%s"_Stz,szFun);
  
  if ('>' == cFlag || '<' == cFlag)
    StrLib::vSprintCat((Stz*)caBuff,"()"_Stz,szFun);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (*szFormat->azChar()) {
    StrLib::vPad(caBuff,C_JOT_BUFF-1,' ');
  
    char* aczCursor = StrLib::aczTabLoc(caBuff,C_JOT_TAB,' ');

    *aczCursor = '\0';
  
    StrLib::vSprintCat((Stz*)caBuff,"+ "_Stz,szFun);
  
    va_start(xVaArgs,szFormat);
    StrLib::vSprintCat(caBuff,xVaArgs,szFormat->azChar());
    va_end(xVaArgs);
  }
  
  if (! _g_sPhrase)
    AM::vWriteTrace(YA_SYS_TRACE,"%s\n"_Stz,caBuff);
  else
  if (szFun->bContains((Stz*)_g_sPhrase))
    AM::vWriteTrace(YA_SYS_TRACE,"%s\n"_Stz,caBuff);

  if ('>' == cFlag)
     _g_nDepth++;
}
/*----------------------------------------------------------------------------*/
//void Resources::vJot2(char cFlag, Stz* szFun, Stz* aFormat, ...)
//{
//  if (! _g_bJotting)
//    return;
//
//  va_list xVaArgs;
//
//  AM::vWriteTrace(YA_SYS_TRACE,"#%s::%s.%04x.%02x.%03x# %x%c "_Stz,
//                  _g_sArea,
//                  _g_sDivision,
//                  _g_iDivision,
//                  _g_iSection,
//                  _g_iLine++,
//                  _g_nDepth,
//                  cFlag
//                  );
//
//  if (_g_nDepth) {
//    AM::vWriteTrace(YA_SYS_TRACE,"%s"_Stz, Stz::szTab(_g_nDepth));
//  }
//
//  AM::vWriteTrace(YA_SYS_TRACE,"%s"_Stz,szFun);
//  if ('>' == cFlag || '<' == cFlag)
//    AM::vWriteTrace(YA_SYS_TRACE,"()"_Stz,szFun);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  va_start(xVaArgs,aFormat);
//  AM::vWriteTrace_vargs(YA_SYS_TRACE,aFormat,xVaArgs);
//  va_end(xVaArgs);
//
//  AM::vWriteTrace(YA_SYS_TRACE,"\n"_Stz, Stz::szTab(_g_nDepth));
//}

/*----------------------------------------------------------------------------*/
/* get current jot section                                                    */
/*----------------------------------------------------------------------------*/
i8 Resources::iJotSection(void)
{
  return _g_iSection;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 Resources::iJotSection(char cFlag, Stz* szFun, Stz* szInfo)
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
  
  vJot2(cFlag,szFun,szInfo);
  
  return _g_iSection;
}

/*----------------------------------------------------------------------------*/
/* get current jot section                                                    */
/*----------------------------------------------------------------------------*/
i8 Resources::iJotSummary(void)
{
  return _g_iSummary;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 Resources::iJotSummary(char cFlag, Stz* szFun, Stz* szInfo)
{
  _g_sSummary = szFun->azChar();
  
  _g_iSection++;
  _g_iSummary++;
  
//  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
//    "= = = = = = = = = = = = = = = = = = = = "
//    "= = = = = = = = = = = = = = = = = = = = "
//    "= = = = = = = = = = = = = = = = = = = = "
//    );
  
  vJot1('=',"%s"_Stz,
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
i8 Resources::iJotDivision(void)
{
  return _g_iDivision;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 Resources::iJotDivision(char cFlag, Stz* szArea, Stz* szFun, Stz* SzInfo)
{
  _g_sArea     = szArea->azChar();
  _g_sDivision = szFun->azChar();
  
  _g_iDivision++;
  _g_iSection = 0;
  _g_iLine = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (_g_iDivision == _g_iDisableDivision)
    _g_iDebug = _g_iDisableDebug;
    
  if (_g_iDivision == _g_iTriggerDivision)
    _g_iDebug = _g_iTriggerDebug;
  
  if (! _g_iDebug)
    return _g_iDivision;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! _g_sPhrase) {
    AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
      "////////////////////////////////////////"
      "////////////////////////////////////////"
      "////////////////////////////////////////"
    );
  
    vJot2(cFlag,szFun,SzInfo);
  }
  
  return _g_iDivision;
}


/*----------------------------------------------------------------------------*/
/* get jot debug                                                              */
/*----------------------------------------------------------------------------*/
i8 Resources::iJotDebug(void)
{
  return _g_iDebug;
}

/*----------------------------------------------------------------------------*/
/* set jot debug                                                              */
/*----------------------------------------------------------------------------*/
void Resources::vJotDebug(i8 iDebug)
{
  _g_iDebug = iDebug;
}

/*----------------------------------------------------------------------------*/
/* tab in for next jotting                                                    */
/*----------------------------------------------------------------------------*/
void Resources::vJotFilter(Stz* szPhrase)
{
  _g_sPhrase = szPhrase->azChar();
}



/*----------------------------------------------------------------------------*/
/* tab in for next jotting                                                    */
/*----------------------------------------------------------------------------*/
void Resources::vJotTabIn(void)
{
  //_g_nDepth++;
}

/*----------------------------------------------------------------------------*/
/* tab out for next jotting                                                   */
/*----------------------------------------------------------------------------*/
void Resources::vJotTabOut(void)
{
//  if (_g_nDepth)
//    _g_nDepth--;
}

/*----------------------------------------------------------------------------*/
/* tab out for next jotting                                                   */
/*----------------------------------------------------------------------------*/
void Resources::vJotTrigger(i8 iDivision, i8 iDebug)
{
  _g_iTriggerDivision = iDivision;
  _g_iTriggerDebug    = iDebug;
}

/*----------------------------------------------------------------------------*/
/* tab out for next jotting                                                   */
/*----------------------------------------------------------------------------*/
void Resources::vJotDisable(i8 iDivision, i8 iDebug)
{
  _g_iDisableDivision = iDivision;
  _g_iDisableDebug    = iDebug;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* jot dividing line 1                                                        */
/*----------------------------------------------------------------------------*/
void Resources::vJotDiv1(void)
{
  if (! _g_iDebug)
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
void Resources::vJotDiv2(void)
{
  if (! _g_iDebug)
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
void Resources::vJotDivES(void)
{
  if (! _g_iDebug)
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
void Resources::vJotDivMS(void)
{
  if (! _g_iDebug)
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
void Resources::vJotDivPS(void)
{
  if (! _g_iDebug)
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
void Resources::vJotDivA(void)
{
  if (! _g_iDebug)
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
void Resources::vJotDivB(void)
{
  if (! _g_iDebug)
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
void Resources::vJotDivC(void)
{
  if (! _g_iDebug)
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
void Resources::vJotDivX(void)
{
  if (! _g_iDebug)
    return;

  AM::vWriteTrace(YA_SYS_TRACE,"\n%s\n"_Stz,
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
  );
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
