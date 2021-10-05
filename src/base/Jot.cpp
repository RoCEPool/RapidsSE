/*============================================================================*/
/* Jot.cpp - Jot Debug Rig                                                    */
/*============================================================================*/

#include "BASE.h"

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
void BASE::vJot1(char cFlag, Stz* szFormat, ...)
{
  if (! _g_iDebug)
    return;
  
  va_list xVarArgs;
  
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

  va_start(xVarArgs,szFormat);
  StrLib::vSprintCat(caBuff,xVarArgs,szFormat->azChar());
  va_end(xVarArgs);
  
  if ('>' == cFlag || '<' == cFlag)
    StrLib::vSprintCat((Stz*)caBuff,"()"_Stz,szFormat);

  if (! _g_sPhrase)
    vTrace("%s\n"_Stz,caBuff);
  else
  if (szFormat->bContains((Stz*)_g_sPhrase))
    vTrace("%s\n"_Stz,caBuff);

  if ('>' == cFlag)
     _g_nDepth++;

}

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void BASE::vJot2(char cFlag, Stz* szFun, Stz* szFormat, ...)
{
  if (! _g_iDebug)
    return;
  
  va_list xVarArgs;
  
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
  
    va_start(xVarArgs,szFormat);
    StrLib::vSprintCat(caBuff,xVarArgs,szFormat->azChar());
    va_end(xVarArgs);
  }
  
  if (! _g_sPhrase)
    vTrace("%s\n"_Stz,caBuff);
  else
  if (szFun->bContains((Stz*)_g_sPhrase))
    vTrace("%s\n"_Stz,caBuff);

  if ('>' == cFlag)
     _g_nDepth++;
}

/*----------------------------------------------------------------------------*/
/* get current jot section                                                    */
/*----------------------------------------------------------------------------*/
i8 BASE::iJotSection(void)
{
  return _g_iSection;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 BASE::iJotSection(char cFlag, Stz* szFun, Stz* szInfo)
{
  _g_sSection = szFun->azChar();
  
  _g_iSection++;
  
//  vTrace("\n%s\n"_Stz,
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
i8 BASE::iJotSummary(void)
{
  return _g_iSummary;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 BASE::iJotSummary(char cFlag, Stz* szFun, Stz* szInfo)
{
  _g_sSummary = szFun->azChar();
  
  _g_iSection++;
  _g_iSummary++;
  
//  vTrace("\n%s\n"_Stz,
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
i8 BASE::iJotDivision(void)
{
  return _g_iDivision;
}

/*----------------------------------------------------------------------------*/
/* new jot section                                                            */
/*----------------------------------------------------------------------------*/
i8 BASE::iJotDivision(char cFlag, Stz* szArea, Stz* szFun, Stz* SzInfo)
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
    vTrace("\n%s\n"_Stz,
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
i8 BASE::iJotDebug(void)
{
  return _g_iDebug;
}

/*----------------------------------------------------------------------------*/
/* set jot debug                                                              */
/*----------------------------------------------------------------------------*/
void BASE::vJotDebug(i8 iDebug)
{
  _g_iDebug = iDebug;
}

/*----------------------------------------------------------------------------*/
/* tab in for next jotting                                                    */
/*----------------------------------------------------------------------------*/
void BASE::vJotFilter(Stz* szPhrase)
{
  _g_sPhrase = szPhrase->azChar();
}



/*----------------------------------------------------------------------------*/
/* tab in for next jotting                                                    */
/*----------------------------------------------------------------------------*/
void BASE::vJotTabIn(void)
{
  //_g_nDepth++;
}

/*----------------------------------------------------------------------------*/
/* tab out for next jotting                                                   */
/*----------------------------------------------------------------------------*/
void BASE::vJotTabOut(void)
{
//  if (_g_nDepth)
//    _g_nDepth--;
}

/*----------------------------------------------------------------------------*/
/* tab out for next jotting                                                   */
/*----------------------------------------------------------------------------*/
void BASE::vJotTrigger(i8 iDivision, i8 iDebug)
{
  _g_iTriggerDivision = iDivision;
  _g_iTriggerDebug    = iDebug;
}

/*----------------------------------------------------------------------------*/
/* tab out for next jotting                                                   */
/*----------------------------------------------------------------------------*/
void BASE::vJotDisable(i8 iDivision, i8 iDebug)
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
void BASE::vJotDiv1(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "----------------------------------------"
    "----------------------------------------"
    "----------------------------------------"
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line 2                                                        */
/*----------------------------------------------------------------------------*/
void BASE::vJotDiv2(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "========================================"
    "========================================"
    "========================================"
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line equals&space                                             */
/*----------------------------------------------------------------------------*/
void BASE::vJotDivES(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "= = = = = = = = = = = = = = = = = = = = "
    "= = = = = = = = = = = = = = = = = = = = "
    "= = = = = = = = = = = = = = = = = = = = "
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line minus&space                                               */
/*----------------------------------------------------------------------------*/
void BASE::vJotDivMS(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "- - - - - - - - - - - - - - - - - - - - "
    "- - - - - - - - - - - - - - - - - - - - "
    "- - - - - - - - - - - - - - - - - - - - "
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line minus&space                                               */
/*----------------------------------------------------------------------------*/
void BASE::vJotDivPS(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "+ + + + + + + + + + + + + + + + + + + + "
    "+ + + + + + + + + + + + + + + + + + + + "
    "+ + + + + + + + + + + + + + + + + + + + "
  );
}


/*----------------------------------------------------------------------------*/
/* jot dividing line A                                                        */
/*----------------------------------------------------------------------------*/
void BASE::vJotDivA(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "++++++++++++++++++++++++++++++++++++++++"
    "++++++++++++++++++++++++++++++++++++++++"
    "++++++++++++++++++++++++++++++++++++++++"
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line B                                                        */
/*----------------------------------------------------------------------------*/
void BASE::vJotDivB(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "+ + + + + + + + + + + + + + + + + + + + "
    "+ + + + + + + + + + + + + + + + + + + + "
    "+ + + + + + + + + + + + + + + + + + + + "
  );
}

/*----------------------------------------------------------------------------*/
/* jot dividing line B                                                        */
/*----------------------------------------------------------------------------*/
void BASE::vJotDivC(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "########################################"
    "########################################"
    "########################################"
  );
}

/*----------------------------------------------------------------------------*/
/* jot section                                                                */
/*----------------------------------------------------------------------------*/
void BASE::vJotDivX(void)
{
  if (! _g_iDebug)
    return;

  vTrace("\n%s\n"_Stz,
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
    "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
  );
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
