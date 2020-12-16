/*============================================================================*/
/* Stz.cpp -- null terminated str                                             */
/*============================================================================*/

#include "Correctness.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Stz::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Stz class cannot be new'd\n"_Stz);
  
  return (void*)C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stz::Stz(void)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stz::Stz(Stz* szArg)
{
  StrLib::vCopy(azChar(),szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stz::~Stz(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get tab                                                                    */
/*----------------------------------------------------------------------------*/
Stz* Stz::szTab(i8 iDepth)
{
  switch(iDepth) {
    case  0:  return ""_Stz;
    case  1:  return "  "_Stz;
    case  2:  return "    "_Stz;
    case  3:  return "      "_Stz;
    case  4:  return "        "_Stz;
    case  5:  return "          "_Stz;
    case  6:  return "            "_Stz;
    case  7:  return "              "_Stz;
    case  8:  return "                "_Stz;
    case  9:  return "                  "_Stz;
    case 10:  return "                    "_Stz;
    case 11:  return "                      "_Stz;
    case 12:  return "                        "_Stz;
    case 13:  return "                          "_Stz;
    case 14:  return "                            "_Stz;
    case 15:  return "                              "_Stz;
    case 16:  return "                                "_Stz;
    default:  return "  (tabbed-out)                    "_Stz;
  }
}

/*----------------------------------------------------------------------------*/
/* get height string                                                          */
/*----------------------------------------------------------------------------*/
Stz* Stz::szHeight(i8 iHeight)
{
  switch(iHeight) {
    case  0:  return "[0]"_Stz;
    case  1:  return "[0]-[1]"_Stz;
    case  2:  return "[0]-[1]-[2]"_Stz;
    case  3:  return "[0]-[1]-[2]-[3]"_Stz;
    case  4:  return "[0]-[1]-[2]-[3]-[4]"_Stz;
    case  5:  return "[0]-[1]-[2]-[3]-[4]-[5]"_Stz;
    case  6:  return "[0]-[1]-[2]-[3]-[4]-[5]-[6]"_Stz;
    case  7:  return "[0]-[1]-[2]-[3]-[4]-[5]-[6]-[7]"_Stz;
    case  8:  return "[0]-[1]-[2]-[3]-[4]-[5]-[6]-[7]-[8]"_Stz;
    case  9:  return "[0]-[1]-[2]-[3]-[4]-[5]-[6]-[7]-[8]-[9]"_Stz;
    default:  return "[0]-[1]-[2]-[3]-[4]-[5]-[6]-[7]-[8]-[9]-[...]"_Stz;
  }
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* = operator                                                                 */
/*----------------------------------------------------------------------------*/
void Stz::operator =(i8 iArg)
{
  StrLib::wToString(iArg,azChar());
}

/*----------------------------------------------------------------------------*/
/* = operator                                                                 */
/*----------------------------------------------------------------------------*/
void Stz::operator =(Stz* szArg)
{
  StrLib::vCopy(azChar(),szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* = operator                                                                 */
/*----------------------------------------------------------------------------*/
void Stz::operator =(void* szArg)
{
  StrLib::wToPointer(szArg,azChar());
}

/*----------------------------------------------------------------------------*/
/* formatted concatenated sprint                                              */
/*----------------------------------------------------------------------------*/
void Stz::vCatSprint(Stz* szFormat, ...)
{
  va_list args;
  
  char* aEnd = azChar() + nLength();

  va_start(args,szFormat);
  vsprintf(aEnd,szFormat->azChar(),args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get as char*                                                               */
/*----------------------------------------------------------------------------*/
char* Stz::azChar(void)
{
  return (char*)this;
}

/*----------------------------------------------------------------------------*/
/* concat argument                                                            */
/*----------------------------------------------------------------------------*/
i8 Stz::iCompare(Stz* szArg)
{
  i8 iCmp = StrLib::iCompare(azChar(),szArg->azChar(),C_NO_CASE);
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* concat argument                                                            */
/*----------------------------------------------------------------------------*/
void Stz::vConcat(Stz* szArg)
{
  StrLib::vConcat(azChar(),szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* test if this Stz contains given sub-string                                 */
/*----------------------------------------------------------------------------*/
bool Stz::bContains(Stz* szArg)
{
  bool b = StrLib::bContains(azChar(),szArg->azChar());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* copy argument                                                              */
/*----------------------------------------------------------------------------*/
void Stz::vCopy(Stz* szArg)
{
  StrLib::vCopy(azChar(),szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* copy argument                                                              */
/*----------------------------------------------------------------------------*/
void Stz::vCopyLen(Stz* szArg, i8 nChars)
{
  StrLib::vCopyLen(azChar(),szArg->azChar(),nChars);
}

/*----------------------------------------------------------------------------*/
/* bEqual: test if this Stz is equal to arg (same case)                       */
/*----------------------------------------------------------------------------*/
bool Stz::bEqual(Stz* szArg)
{
  i8 iDiff = strcmp(azChar(),szArg->azChar());
  
  if (0 == iDiff)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bEqual: test if this Stz is equal to arg (same case)                       */
/*----------------------------------------------------------------------------*/
bool Stz::bGreater(Stz* szArg)
{
  i8 iDiff = strcmp(azChar(),szArg->azChar());
  
  if (iDiff > 0)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* iLength: get length in bytes of arg (not including terminating null)       */
/*----------------------------------------------------------------------------*/
i8 Stz::nLength(void)
{
  i8 nLen = strlen(azChar());
  
  return nLen;
}

/*----------------------------------------------------------------------------*/
/* bIsSomeFalse: test if this Stz is recognizable as a false                  */
/*----------------------------------------------------------------------------*/
bool Stz::bIsSomeFalse(void)
{
  if (bSame("off"_Stz)   ||
      bSame("0"_Stz)     ||
      bSame("false"_Stz) ||
      bSame("f"_Stz)     ||
      bSame("no"_Stz)    ||
      bSame("n"_Stz)
      )
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsSomeTrue: test if this Stz is recognizable as a true                    */
/*----------------------------------------------------------------------------*/
bool Stz::bIsSomeTrue(void)
{
  if (bSame("on"_Stz)   ||
      bSame("1"_Stz)    ||
      bSame("true"_Stz) ||
      bSame("t"_Stz)    ||
      bSame("yes"_Stz)  ||
      bSame("y"_Stz)
      )
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsNumeric: test if this Stz is numeric                                    */
/*----------------------------------------------------------------------------*/
bool Stz::bIsNumeric(void)
{
  bool bNumeric = StrLib::bIsNumeric(azChar());
  
  return bNumeric;
}

/*----------------------------------------------------------------------------*/
/* bOneOf: test if this Stz is same as one of the args (ignores case)         */
/*----------------------------------------------------------------------------*/
bool Stz::bOneOf(Stz* szArg, ...)
{
  va_list args;
  va_start(args,szArg);
  
  while (szArg) {
    if (bSame(szArg))
      return true;
    szArg = va_arg(args,Stz*);
  }

  va_end(args);

  return false;
}

/*----------------------------------------------------------------------------*/
/* bSame: test if this Stz is same as arg (ignores case)                      */
/*----------------------------------------------------------------------------*/
bool Stz::bSame(Stz* szArg)
{
  i8 iDiff = strcasecmp(azChar(),szArg->azChar());
  
  if (0 == iDiff)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of arg (including terminating null)                      */
/*----------------------------------------------------------------------------*/
i8 Stz::nSize(void)
{
  i8 n = StrLib::nSize(azChar());
  
  return n;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void Stz::vSprint(Stz* szFormat, ...)
{
  va_list args;
  va_start(args,szFormat);
  
  vsprintf(azChar(),szFormat->azChar(),args);
  
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* iToI8: convert arg to i8                                                   */
/*----------------------------------------------------------------------------*/
i8 Stz::iToI8(void)
{
  //i8 i = atoll(s);
  
  //return i;
  
  const char* ps = azChar();
  
  i8 i = 0;
  
  // skip leading 0's
  while (*ps && '0' == *ps)
    ps++;
  
  if (*ps && ('x' == *ps || 'X' == *ps)) {
    ps++;
    while (*ps && StrLib::bIsHex(*ps)) {
      i8 j = StrLib::iHexToInw(*ps++);
      i = i * 16 + j;
    }
  }
  else {
    while (*ps && StrLib::bIsDigit(*ps)) {
      i8 j = (*ps++ - '0');
      i = i * 10 + j;
    }
  }
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
void Stz::vToLower(void)
{
  StrLib::vToLower(azChar());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* _Stz literal suffix (which converts const char* to Stz*)                   */
/*----------------------------------------------------------------------------*/
Stz* Resources::operator "" _Stz(const char* szArg, size_t nSize)
{
  return (Stz*)(szArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
