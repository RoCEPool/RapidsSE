/*============================================================================*/
/* StrLib.cpp -- char* string utility                                         */
/*============================================================================*/

// STRING LIBRARY PRINCIPLES & RATIONALE
// *************************************
//
// These are some core principles that the string library is working towards.
// It isn't there yet, but it should help guide us with any new APIs as well
// as correcting the existing APIs.
//
// (1) All string operations must know the size of the underlying buffer.
//     That is, no blindly writing to a char* without knowing its size
//     and the size of the string you're writing.
//
// (2) The StrLib method MUST check whether a string truncation would occur.
//     More on how this is handled in a little bit.
//
// (3) The StrLib methods MUST include the ability for the caller to know that
//     a string truncation has occurred.
//     In many cases a string truncation should never happen, but in some cases
//     it is unavoidable (e.g. a SQL command concatenating column values) and
//     it is the responsibility of the caller to check for truncation and then
//     handle this error themselves.
//
// (4) If the caller is not checking for truncation then the system should at
//     least record that it has happened. It could do this by writing information
//     to the logfile about the nature of the operation and how it got there.
//     e.g. a stack trace.
//     This is important so that we as developers can check to see if a truncation
//     is occurring because at the time of writing we don't have any callers
//     actually check for truncation. Also, if it occurs at runtime then we would
//     like to know more about this.
//
// (5) All source/destination strings and arguments are properly null terminated
//     such that we can rely on strlen() as needed.
//
// (6) All applicable string operations should come through StrLib as opposed to
//     naked calls to system string routines throughout the code.
//     This allows for a common place for handling of string operations, a
//     common methodology, interface and semantics, and a common way to handle errors.
//
// (7) StrLib should have consistent calling semantics. This includes things like:
//      (a) The name of the destination buffer should be called psDst and in
//          general it should usually be the first parameter in the function.
//      (b) The size of the destination buffer should come immediately after the
//           pointer to the destination buffer in the parameter list.
//      (c) When the API is cursor based rather than buffer and size based then
//          the next parameter after the cursor should be the last valid char
//          in the buffer. i.e. StrLib *must* know the limit of the buffer.
//      (d) A writable buffer should be a char* and not a Stz*. Stz should be
//          considered a read-only data structure.
//
//
// Note: All of the above is a work in progress.

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- Externs -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* like: search for regexp anywhere in given string                           */
/*----------------------------------------------------------------------------*/
extern "C" i8 iExtern_StrLibLike2(char* azStr, char* azRegExp) {
  _static_

  i8 i = StrLib::iLike2(azStr, azRegExp);
  return i;
}

/*----------------------------------------------------------------------------*/
/* like: search for regexp anywhere in given string                           */
/*----------------------------------------------------------------------------*/
extern "C" i8 iExtern_StrLibLike3(char* azStr, char* azRegExp, char cEsc) {
  _static_

  i8 i = StrLib::iLike3(azStr, azRegExp, cEsc);
  return i;
}

/*----------------------------------------------------------------------------*/
/* flip the bytes in each word of src string into dst                         */
/*----------------------------------------------------------------------------*/
extern "C" i8 iExtern_StrLibWordFlipBack(char* aDst, char* aSrc) {
  _static_

  return StrLib::iWordFlipBack(aDst, aSrc);
}

/*----------------------------------------------------------------------------*/

static i8 iDaysPerMonth[12]     = { 31,28,31,30,31,30,31,31,30,31,30,31};
static i8 iDaysPerMonthLeap[12] = { 31,29,31,30,31,30,31,31,30,31,30,31};



static const char C_COMMA = ',';

static Stz* szaAscii[128] = {
  "NUL"_Stz,"SOH"_Stz,"STX"_Stz,"ETX"_Stz,"EOT"_Stz,"ENQ"_Stz,"ACK"_Stz,"BEL"_Stz,
  "BS"_Stz, "HT"_Stz, "LF"_Stz, "VT"_Stz, "FF"_Stz, "CR"_Stz, "SO"_Stz, "SI"_Stz,
  "DLE"_Stz,"DC1"_Stz,"DC2"_Stz,"DC3"_Stz,"DC4"_Stz,"NAK"_Stz,"SYN"_Stz,"ETB"_Stz,
  "CAN"_Stz, "EM"_Stz,"SUB"_Stz,"ESC"_Stz, "FS"_Stz, "GS"_Stz, "RS"_Stz, "US"_Stz,
  " "_Stz,  "!"_Stz, "\""_Stz,  "#"_Stz,  "$"_Stz,  "%"_Stz,  "&"_Stz,  "'"_Stz,
  "("_Stz,  ")"_Stz,  "*"_Stz,  "+"_Stz,  ","_Stz,  "-"_Stz,  "."_Stz,  "/"_Stz,
  "0"_Stz,  "1"_Stz,  "2"_Stz,  "3"_Stz,  "4"_Stz,  "5"_Stz,  "6"_Stz,  "7"_Stz,
  "8"_Stz,  "9"_Stz,  ":"_Stz,  ";"_Stz,  "<"_Stz,  "="_Stz,  ">"_Stz,  "?"_Stz,
  "@"_Stz,  "A"_Stz,  "B"_Stz,  "C"_Stz,  "D"_Stz,  "E"_Stz,  "F"_Stz,  "G"_Stz,
  "H"_Stz,  "I"_Stz,  "J"_Stz,  "K"_Stz,  "L"_Stz,  "M"_Stz,  "N"_Stz,  "O"_Stz,
  "P"_Stz,  "Q"_Stz,  "R"_Stz,  "S"_Stz,  "T"_Stz,  "U"_Stz,  "V"_Stz,  "W"_Stz,
  "X"_Stz,  "Y"_Stz,  "Z"_Stz,  "["_Stz,  "\\"_Stz, "]"_Stz,  "^"_Stz,  "_"_Stz,
  "`"_Stz,  "a"_Stz,  "b"_Stz,  "c"_Stz,  "d"_Stz,  "e"_Stz,  "f"_Stz,  "g"_Stz,
  "h"_Stz,  "i"_Stz,  "j"_Stz,  "k"_Stz,  "l"_Stz,  "m"_Stz,  "n"_Stz,  "o"_Stz,
  "p"_Stz,  "q"_Stz,  "r"_Stz,  "s"_Stz,  "t"_Stz,  "u"_Stz,  "v"_Stz,  "w"_Stz,
  "x"_Stz,  "y"_Stz,  "z"_Stz,  "{"_Stz,  "|"_Stz,  "}"_Stz,  "~"_Stz,"DEL"_Stz
};

/*----------------------------------------------------------------------------*/
/* like: search for regexp anywhere in given string                           */
/*----------------------------------------------------------------------------*/
i8 StrLib_iLike2(char* azStr, char* azRegExp) {
  _static_

  i8 i = StrLib::iLike2(azStr, azRegExp);
  return i;
}

/*----------------------------------------------------------------------------*/
/* like: search for regexp anywhere in given string                           */
/*----------------------------------------------------------------------------*/
i8 StrLib_iLike3(char* azStr, char* azRegExp, char cEsc) {
  _static_

  i8 i = StrLib::iLike3(azStr, azRegExp, cEsc);
  return i;
}

/*----------------------------------------------------------------------------*/
/* flip the bytes in each word of src string into dst                         */
/*----------------------------------------------------------------------------*/
i8 StrLib_iWordFlipBack(char* aDst, char* aSrc) {
  _static_

  return StrLib::iWordFlipBack(aDst, aSrc);
}


/*----------------------------------------------------------------------------*/
/* A                                  A                                     A */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append character to end of workspace                                       */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendChar(char** aazDstCrs, const char* azDstLastChar, char cChar, bool* pbResult)
{
  _static_

  char* d = *aazDstCrs;

  // Check if we are out of room before even starting
  if (d >= azDstLastChar) {
    i8 iReqBufSize = 2; // the char + null pointer
    i8 iDstBufSize = (d == azDstLastChar ? 1 : 0);

    bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
    UNUSED(bRet);
    return;
  }

  *(*aazDstCrs)++ = cChar;
  *(*aazDstCrs) = '\0';

  if (pbResult) {
    *pbResult = true;
  }
}

/*----------------------------------------------------------------------------*/
/* append number in decimal to workspace                                      */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendDecimal(char** aazDstCrs, const char* azDstLastChar, i8 iDecVal, bool* pbResult)
{
  _static_

  i8 iSpaceNeeded = 1; // start with the null terminator
  i8 iDstBufSize = (byte*) azDstLastChar - (byte*) *aazDstCrs;

  if (iDstBufSize < 0) {
    iDstBufSize = 0;
  }
  
  if (iDecVal < 0) {
    ++iSpaceNeeded;
    if (*aazDstCrs < azDstLastChar) {
      *((*aazDstCrs)++) = '-';
    }
    
    iDecVal *= -1;
  }
  
  char* pBwd = *aazDstCrs;
  char* pFwd = *aazDstCrs;

  i8 v = iDecVal;
  
  do
  {
    ++iSpaceNeeded;
    if (pBwd < azDstLastChar) {
      *pBwd++ = (v % 10) + '0';
    }

    v /= 10;
  } while (v);

  // Did we run out of room?
  if (iSpaceNeeded > iDstBufSize) {
    if (pBwd == azDstLastChar) {
      *pBwd = 0;
    }

    if (pbResult) {
      *pbResult = false;
    } else {
      vReportStringTruncation(__func__, iSpaceNeeded, iDstBufSize);
    }

    *aazDstCrs = pBwd;
    return;
  }

  // We had enough space to write the value backwards. Now reverse it.
  char* pEnd = pBwd;
  
  while (--pBwd > pFwd)
  {
    char cTmp = *pBwd;
    *pBwd = *pFwd;
    *pFwd++ = cTmp;
  }
  
  *pEnd = '\0';
  
  *aazDstCrs = pEnd;
}

/*----------------------------------------------------------------------------*/
/* vAppendHex: append number in hexadecimal to workspace                      */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendHex(char** aazDstCrs, const char* azDstLastChar, i8 iDecVal, bool* pbResult)
{
  _static_

  i8 iSpaceNeeded = 1; // start with the null terminator
  i8 iDstBufSize = (byte*) azDstLastChar - (byte*) *aazDstCrs;

  if (iDstBufSize < 0) {
    iDstBufSize = 0;
  }
  
  char* pBwd = *aazDstCrs;
  char* pFwd = *aazDstCrs;
  
  i8 v = iDecVal;
  
  do {
    ++iSpaceNeeded;
    if (pBwd < azDstLastChar) {
      *pBwd++ = cToHex(v % 16);
    }

    v /= 16;
  } while (v);

  // Did we run out of room?
  if (iSpaceNeeded > iDstBufSize) {
    if (pBwd == azDstLastChar) {
      *pBwd = 0;
    }

    if (pbResult) {
      *pbResult = false;
    } else {
      vReportStringTruncation(__func__, iSpaceNeeded, iDstBufSize);
    }

    *aazDstCrs = pBwd;
    return;
  }

  // We had enough space to write the value backwards. Now reverse it.
  char* pEnd = pBwd;
  
  while (--pBwd > pFwd)
  {
    char cTmp = *pBwd;
    *pBwd = *pFwd;
    *pFwd++ = cTmp;
  }

  *pEnd = '\0';

  *aazDstCrs = pEnd;
}

/*----------------------------------------------------------------------------*/
/* vAppendPad: pad string with spaces to given length padLen only if the      */
/* current string is shorter than nPadLen. nPadLen does not include null term.*/
/* aazDstCrs must be pointing to the null term or the first char to be padded.*/
/*----------------------------------------------------------------------------*/
void StrLib::vAppendPad(char** aazDstCrs, const char* psDst, i8 iDstBufSize, i8 nPadLen, bool* pbResult)
{
  _static_
  
  i8 nCurLen = StrLib::nLength(psDst);
  i8 iReqBufSize = (nCurLen > nPadLen ? nCurLen : nPadLen) + 1;

  bool bWillTruncate = !bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bWillTruncate);
  // We continue whether it will truncate or not.

  if (nCurLen >= nPadLen)
    return;
  
  char *z = *aazDstCrs;
  
  while (nCurLen < nPadLen && nCurLen < iDstBufSize - 1) {
    ++nCurLen;
    *z++ = ' ';
  }

  *z = '\0';
  *aazDstCrs = z;
}

/*----------------------------------------------------------------------------*/
/* vAppendNull: append null character to end of workspace                     */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendNull(char** aazDstCrs, const char* azDstLastChar, bool* pbResult)
{
  _static_

  char* d = *aazDstCrs;

  if (d > azDstLastChar) {
    bool bWillTruncate = !bCheckBufferSpace(__func__, 1, 0, pbResult);
    UNUSED(bWillTruncate);
    return;
  }
  
  *(*aazDstCrs)++ = '\0';
}
  
/*----------------------------------------------------------------------------*/
/* vAppendString: append string to current cursor.                            */
/* azDstLastChar is the last valid char pointer in the buffer.                */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendString(char** aazDstCrs, const char* azDstLastChar,
                           const char* pSrc, bool* pbResult)
{
  _static_
  
  char* z = *aazDstCrs;
  const char* s = pSrc;

  // Check if we are out of room before even starting
  if (z >= azDstLastChar) {
    i8 iReqBufSize = nLength(pSrc) + 1;
    i8 iDstBufSize = 0;

    bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
    UNUSED(bRet);
    return;
  }
  
  while (*s && z != azDstLastChar) {
    *z++ = *s++;
  }

  // Check if we were going to overflow
  if (*s) {
    // We report the required and available space from before the append operation
    i8 iReqBufSize = nLength(pSrc) + 1;
    i8 iDstBufSize = azDstLastChar - *aazDstCrs;

    bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
    UNUSED(bRet);

  } else if (pbResult) {
    *pbResult = true;
  }

  *z = '\0';
  *aazDstCrs = z;
}

/*----------------------------------------------------------------------------*/
/* vAppendString: append string to workspace with line numbers                */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendString(char** aazDstCrs, const char* azDstLastChar, const char* pSrc,
                           i8* piLineNo, bool* pbResult)
{
  _static_

  char* z = *aazDstCrs;
  const char* s = pSrc;

  bool bInQuote = false;
  i8 iLineNumber = *piLineNo;

  // Check if we are out of room before even starting
  if (z >= azDstLastChar) {
    i8 iReqBufSize = nLength(pSrc) + 1; // doesn't account for line numbers
    i8 iDstBufSize = 0;

    bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
    UNUSED(bRet);
    return;
  }

  while (*s && z != azDstLastChar) {
    if (*s == '"') {
      if (bInQuote)
        bInQuote = false;
      else
        bInQuote = true;
    }

    if (*s == '\n' && !bInQuote ) {
      s32 sBuf;
      vSprint(sBuf.azChar(), sBuf.nAreaBytes(), pbResult, "\n/* %04d */ "_Stz, iLineNumber++);
      vAppendString(&z, azDstLastChar, sBuf.azChar(), pbResult);
      s++;

    } else {
      *z++ = *s++;
    }
  }

  // Check if we were going to overflow
  if (*s) {
    // We report the required and available space from before the append operation
    i8 iReqBufSize = nLength(pSrc) + 1; // does not account for line numbers
    i8 iDstBufSize = azDstLastChar - *aazDstCrs;

    bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
    UNUSED(bRet);

  } else if (pbResult) {
    *pbResult = true;
  }

  *z = '\0';
  *aazDstCrs = z;
  *piLineNo = iLineNumber;
}

/*----------------------------------------------------------------------------*/
/* azAscii: get 3-letter ascii abbreviation for char                          */
/*----------------------------------------------------------------------------*/
char* StrLib::azAscii(i8 i_Char)
{
  _static_
  
  return (char*)szaAscii[i_Char];
}

/*----------------------------------------------------------------------------*/
/* A                                  A                                     A */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan backwards to find given character                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bBackward(char** ppsSrc, const char* psBegin, const char cUntil)
{
  _static_
  
  char *p = *ppsSrc;

  while (*p-- != cUntil)
  {
    if (p <= psBegin)
      return false;
  }
  
  *ppsSrc = ++p;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bBeginsWith: test if arg is a leading subset of s                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bBeginsWith(const char* psSrc, const char* psArg)
{
  _static_
  i8 nBytes = nLength(psArg);
  i8 iResult = iCompare(psSrc, psArg, nBytes, C_NO_CASE);
  return iResult == 0;
}

/*----------------------------------------------------------------------------*/
/* decode char into a string                                                  */
/*----------------------------------------------------------------------------*/
Stz* StrLib::szCharToStr(unsigned char cArg)
{
  _static_
  

  if (cArg <= 0x7f)
    return szaAscii[(i8)cArg];
  else
    return ">7f"_Stz;
}

/*----------------------------------------------------------------------------*/
/* Checks if there is sufficient buffer space & optionally sets the           */
/* callerResult if not null. Otherwise reports the truncation in the log file.*/
/* Returns true if everything is ok or false if a string truncation will occur*/
/*----------------------------------------------------------------------------*/
inline bool StrLib::bCheckBufferSpace(const char* azFuncName, i8 iReqBufSize, i8 iDstBufSize, bool* pbCallerResult)
{
  ASSERT(azFuncName);
  ASSERT(iReqBufSize >= 0);
  ASSERT(iDstBufSize >= 1);  // need at least room for a null terminator

  // Is there enough space?
  if (iDstBufSize < iReqBufSize) {

    if (pbCallerResult) {
      // (Optional) Caller is checking for and handling the error condition
      *pbCallerResult = false;

    } else {
      // No handling of this error condition. At least log it.
      vReportStringTruncation(azFuncName, iReqBufSize, iDstBufSize);
    }

    return false;
  }

  if (pbCallerResult) {
    // (Optional) Caller is checking for the error condition
    *pbCallerResult = true;
  }

  // No truncation will occur
  return true;
}

/*----------------------------------------------------------------------------*/
/* check char                                                                 */
/*----------------------------------------------------------------------------*/
void StrLib::vCheckIsNot(char cArg, char cIsNot) {
  _static_

  if (cIsNot == cArg) {
    vTrace("cArg is cIsNot\n"_Stz);
    RASSERT(cIsNot != cArg);
  }
}

/*----------------------------------------------------------------------------*/
/* click arg. Increment the ascii characters by 1 starting from the right     */
/*----------------------------------------------------------------------------*/
void StrLib::vClick(char* psDst)
{
  _static_
  
  char* pChar = acLast(psDst);
  
  for (;;) {
    
    if ('z' == *pChar) {
      *pChar = 'a';
      pChar--;
      continue;
    }
    if ('Z' == *pChar) {
      *pChar = 'A';
      pChar--;
      continue;
    }

    *pChar += 1;
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* clip arg to given maxiumum length (not including null terminator).         */
/* Returns true if it is resized smaller                                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bClip(char* psDst, i8 iMaxLen)
{
  _static_
  
  i8 iCurLen = StrLib::nLength(psDst);
  
  if (iCurLen <= iMaxLen)
    return false;

  psDst[iMaxLen] = '\0';
  return true;
}

/*----------------------------------------------------------------------------*/
/* comma seperate the integer argument                                        */
/*----------------------------------------------------------------------------*/
void StrLib::vComma(char* psDst, i8 iDstBufSize, i8 iArg, bool* pbResult)
{
  _static_
  
  char caInteger[32];
  vToString(caInteger, sizeof(caInteger), iArg, pbResult);
  
  char caReverse[32];
  vReverse(caReverse, sizeof(caReverse), caInteger, pbResult);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  char caRevCommas[32];

  char* aChar = caRevCommas;
  char* aDigit = caReverse;

  u8 uBytesReq = 0;
  
  i8 nDigit = 0;
  while (*aDigit) {
    if (nDigit == 3) {
      if (uBytesReq < sizeof(caRevCommas) - 1) {
        *aChar++ = C_COMMA;
      }
      nDigit = 0;
      ++uBytesReq;
    }

    if (uBytesReq < sizeof(caRevCommas) - 1) {
      *aChar++ = *aDigit++;
    }
    nDigit++;
    ++uBytesReq;
  }
  
  if (iArg < 0 && uBytesReq < sizeof(caRevCommas) - 1)
    *aChar++ = '-';
  
  *aChar = '\0';

  bool bRet = bCheckBufferSpace(__func__, uBytesReq, iDstBufSize, pbResult);
  UNUSED(bRet);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vReverse(psDst, sizeof(psDst), caRevCommas, pbResult);
}

/*----------------------------------------------------------------------------*/
/* iCompare: standard compare of the two args                                 */
/*----------------------------------------------------------------------------*/
i8 StrLib::iCompare(const char* psArg1, const char* psArg2)
{
  _static_
  
  i8 iDiff = strcmp(psArg1, psArg2);
  
  return iDiff;
}

/*----------------------------------------------------------------------------*/
/* iCompare: standard case-insensitive compare of the two args                */
/*----------------------------------------------------------------------------*/
i8 StrLib::iCompare(const char* psArg1, const char* psArg2, i8 iCase)
{
  _static_
  
  i8 iDiff;
  
  if (C_NO_CASE == iCase)
    iDiff = strcasecmp(psArg1, psArg2);
  else
    iDiff = strcmp(psArg1, psArg2);
  
  return iDiff;
}

/*----------------------------------------------------------------------------*/
/* iCompare: standard case-insensitive compare of the two args                */
/*----------------------------------------------------------------------------*/
i8 StrLib::iCompare(const char* psArg1, const char* psArg2, i8 nBytes, i8 iCase)
{
  _static_
  
  i8 iDiff;
  
  if (C_NO_CASE == iCase)
    iDiff = strncasecmp(psArg1, psArg2, nBytes);
  else
    iDiff = strncmp(psArg1, psArg2, nBytes);
  
  return iDiff;
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate character to workspace                                */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* psDst, i8 iDstBufSize, const char cChar, bool* pbResult)
{
  _static_
  
  i8 iDstStrLen = nLength(psDst);     // how much space in the dest buffer (not including null terminator)
  i8 iReqBufSize = iDstStrLen + 2;  // how much space required (including extra char & null terminator)

  if (!bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult)) {
    // No space to add 1 char, so we can't do this operation.
    return;
  }

  psDst[iDstStrLen] = cChar;
  psDst[iDstStrLen + 1] = '\0';
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate arg onto dst string                                   */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* psDst, i8 iDstBufSize, const char* psArg, bool* pbResult)
{
  _static_

  i8 iDstStrLen = nLength(psDst);   // how much space in the dest buffer (not including null terminator)
  i8 iArgStrLen = nLength(psArg);   // length of the argument
  i8 iReqBufSize = iDstStrLen + iArgStrLen + 1;  // how much space required (including extra char & null terminator)

  bool bWillTruncate = !bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bWillTruncate);
  // We continue whether it will truncate or not.

  // The remaining space for writing the arg string, not including the null terminator.
  i8 iRemainingSpace = iDstBufSize - iDstStrLen - 1;

  strncat(psDst,psArg, iRemainingSpace);
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate up to nCatLen characters of arg onto dst string       */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* psDst, i8 iDstBufSize, const char* psArg, i8 nCatLen, bool* pbResult)
{
  _static_

  i8 iDstStrLen = nLength(psDst);   // how much space in the dest buffer (not including null terminator)
  i8 iArgStrLen = nLength(psArg);   // length of the argument
//i8 iMaxArgLen = iArgStrLen > nCatLen ? iArgStrLen : nCatLen;
//  i8 iReqBufSize = iDstStrLen + iMaxArgLen + 1;  // how much space required (including extra char & null terminator)

  // will strncat the minimum of nCatLen or nLength(psArg)
  i8 iMinArgLen = nCatLen < iArgStrLen ? nCatLen : iArgStrLen;
  i8 iReqBufSize = iDstStrLen + iMinArgLen + 1;  // how much space required (including extra char & null terminator)

  bool bWillTruncate = !bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bWillTruncate);
  // We continue whether it will truncate or not.

  strncat(psDst, psArg, nCatLen);
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate arg1 & arg2 onto dst string                           */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* psDst, i8 iDstBufSize, const char* psArg1, const char* psArg2, bool* pbResult)
{
  _static_

  vConcat(psDst, iDstBufSize, psArg1, pbResult);
  vConcat(psDst, iDstBufSize, psArg2, pbResult);
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate i8 arg onto dst string                                */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* psDst, i8 iDstBufSize, i8 iArg, bool* pbResult)
{
  _static_
  
  char aInt[32];

  vToString(aInt, sizeof(aInt), iArg);  // This should never fail
  vConcat(psDst, iDstBufSize, aInt, pbResult);
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bContains(const char* psSrc, const char cArg)
{
  _static_
  
  const char* f = strchr(psSrc, cArg);

  return (f != nullptr);
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bContains(const char* psSrc, const char* sArg)
{
  _static_

  const char* f = strstr(psSrc, sArg);

  return (f != nullptr);
}

/*----------------------------------------------------------------------------*/
/* copy src into dst, return bytes copied (not including '\0' terminator)     */
/*----------------------------------------------------------------------------*/
i8 StrLib::nCopy(char* psDst, i8 iDstBufSize, const char* psSrc, bool* pbResult)
{
  _static_

  i8 iReqBufSize = nLength(psSrc) + 1;  // how much space required (including the null terminator)

  bool bWillTruncate = !bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bWillTruncate);
  // We continue whether it will truncate or not.

  void* pvLastAddr = psDst + iDstBufSize - 1;

  i8 nBytes = 0;
  
  while ( (psDst != pvLastAddr) && (*psDst++ = *psSrc++)) {
    nBytes++; // won't be executed when \0 is copied
  }

  if (psDst == pvLastAddr) {
    // We need to null terminate the truncated string
    *psDst = 0;
  }
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* vCopy: copy src into dst                                                   */
/*----------------------------------------------------------------------------*/
void StrLib::vCopy(char* psDst, i8 iDstBufSize, const char* psSrc, bool* pbResult)
{
  _static_

  i8 iReqBufSize = nLength(psSrc) + 1;  // how much space required (including the null terminator)

  bool bWillTruncate = !bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bWillTruncate);

  // Note, using strncpy() instead of calling nCopy() above as strncpy() is
  // likely optimised for more efficient copying than byte by byte.
  // Note that strncpy() doesn't guarantee that the buffer is null terminated
  // if length(psSrc) >= iDstBufSize. If length(psSrc) < iDstBufSize then
  // nulls are written to the remaining bytes.
  strncpy(psDst, psSrc, iDstBufSize);
  psDst[iDstBufSize - 1] = 0;
}

/*----------------------------------------------------------------------------*/
/* vCopyLen: copy psSrc into psDst up till nBytes characters (not including   */
/* the null terminator) or until the end of psSrc.                            */
/* NOTE: The semantic on the string truncation warning is slightly different  */
/* here because it will only warn if the truncation occurs because the buffer */
/* is exhausted before we hit a null terminator in psSrc or we run nBytes     */
/* down to zero.                                                              */
/*----------------------------------------------------------------------------*/
void StrLib::vCopyLen(char* psDst, i8 iDstBufSize, const char* psSrc, i8 nBytes, bool* pbResult)
{
  _static_
  ASSERT(nBytes >= 0);

  // We don't know whether nBytes is larger or smaller than length(psSrc), so
  // we need to count the number of bytes copied as we copy them.

  i8 iBytesCopied = 0;
  i8 iBytesLeft = nBytes;

  char *d = psDst;
  const char *s = psSrc;

  // Copy bytes, ensuring we leave 1 byte at the end for the null terminator
  while (*s && iBytesLeft-- > 0 && ++iBytesCopied < iDstBufSize)
    *d++ = *s++;

  if (iDstBufSize > 0) {
    // Handle corner case of iDstBufSize == 0
    *d = '\0';
  }

  // Check if we were going to overflow. This is a subtly different semantic
  // than vCopy() because the caller supplied a maximum number of bytes to copy.
  // If we stopped because of that then we strictly didn't truncate the string
  // because it is what the caller requested.
  if (*s && iBytesLeft > 0) {
    // We report the required and available space from before the append operation
    i8 iSrcLen = nSize(psSrc);
    i8 iReqBufSize = iSrcLen < nBytes + 1 ? iSrcLen : nBytes + 1;

    bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
    UNUSED(bRet);

  } else if (pbResult) {
    *pbResult = true;
  }

}

/*----------------------------------------------------------------------------*/
/* vCopySplit: copy src into w1, then w2 divided at cSplit                    */
/*----------------------------------------------------------------------------*/
void StrLib::vCopySplit(char* psDst1, i8 iDst1BufSize,
                        char* psDst2, i8 iDst2BufSize,
                        const char* psSrc, char cSplit, bool* pbResult)
{
  _static_

  // We cannot tell ahead of time whether a string truncation will occur without
  // the penalty of walking the psSrc string. So instead we report a truncation
  // if we detect one.
  i8 iBytesCopied = 0;
  bool bInterimResult1 = true;
  bool bInterimResult2 = true;

  const char *s = psSrc;
  char *d1 = psDst1;

  while (*s && *s != cSplit) {
    if (iBytesCopied < iDst1BufSize - 1) {
      ++iBytesCopied;
      *d1++ = *s++;
    }
  }

  if (iDst1BufSize > 0) {
    // Handle corner case of iDstBufSize == 0
    *d1 = '\0';
  }

  // Check if we were going to overflow while writing to psDst1
  if (*s && iBytesCopied == iDst1BufSize) {
    // We report the required and available space from before the append operation
    i8 iReqBufSize = nSize(psSrc);

    bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDst1BufSize, &bInterimResult1);
    UNUSED(bRet);

  } else if (pbResult) {
    *pbResult = true;
  }
  
  if ( ! *s ) {
    if (iDst2BufSize > 0) {
      // Handle corner case of iDstBufSize == 0
      *psDst2 = '\0';
    }
    return;
  }
  
  // skip the split character
  s++;
  vCopy(psDst2, iDst2BufSize, s, &bInterimResult2);

  // If a truncation occurred on the first copy then alert the caller
  if (pbResult) {
    *pbResult = (bInterimResult1 && bInterimResult2);
  }
}
  
/*----------------------------------------------------------------------------*/
/* vCopyTill: copy src into dst uptill but not including ccTill               */
/*----------------------------------------------------------------------------*/
void StrLib::vCopyTill(char* psDst, i8 iDstBufSize, const char* psSrc, char ccTill, bool* pbResult)
{
  _static_

  // We cannot tell ahead of time whether a string truncation will occur without
  // the penalty of walking the psSrc string. So instead we report a truncation
  // if we detect one.
  i8 iBytesCopied = 0;

  char *d = psDst;
  const char *s = psSrc;
  
  while (*s && ccTill != *s && ++iBytesCopied < iDstBufSize)
    *d++ = *s++;

  if (iDstBufSize > 0) {
    // Handle corner case of iDstBufSize == 0
    *d = '\0';
  }

  // Check if we were going to overflow.
  if (*s && iBytesCopied == iDstBufSize) {
    // We report the required and available space from before the append operation
    i8 iReqBufSize = nSize(psSrc);

    bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
    UNUSED(bRet);

  } else if (pbResult) {
    *pbResult = true;
  }
}

/*----------------------------------------------------------------------------*/
/* convert ascii digit arg to i8                                              */
/*----------------------------------------------------------------------------*/
i8 StrLib::iDigitToI8(const char cDigit)
{
  _static_
  
  i8 i = cDigit - '0';
    
  return i;
}

/*----------------------------------------------------------------------------*/
/* vDump: escape and dump string to the log file                              */
/*----------------------------------------------------------------------------*/
void StrLib::vDump(const char* psSrc, i8 nMaxChars, const char* psSuffix, bool* pbResult)
{
  _static_

  const i8 iDstBufSize = C_STRING_ARRAY;
  char caString[iDstBufSize];
  char *z = caString;
  char *psDstLastChar = caString + sizeof(caString) - 1;

  i8 iReqBytes = 0;
  
  for (int i = 0; i < nMaxChars; i++)
  {
    ++iReqBytes;

    if (isprint(psSrc[i])) {
      if (z < psDstLastChar) {
        *z++ = psSrc[i];
      }

    } else if ('\n' == psSrc[i]) {
      ++iReqBytes;  // cater for second escaped char
      if (z < psDstLastChar) {
        *z++ = '\\';
        if (z < psDstLastChar) {
          *z++ = 'n';
        }
      }

    } else if ('\0' == psSrc[i]) {
      ++iReqBytes;  // cater for second escaped char
      if (z < psDstLastChar) {
        *z++ = '\\';
        if (z < psDstLastChar) {
          *z++ = '0';
        }
      }

    } else {
      // Non-printable character. Replace with a period.
      if (z < psDstLastChar) {
        *z++ = '.';
      }
    }
    
    if ('\0' == psSrc[i])
      break;
  }

  // Check for truncation
  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);

  if (bRet) {
    // vAppendString will null terminate what has been written
    vAppendString(&z, psDstLastChar, psSuffix, pbResult);

  } else {
    *z = '\0';
  }

  vTrace("%s"_Stz,caString);
}

/*----------------------------------------------------------------------------*/
/* bEqualTo: test if the two args match                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bEqualTo(const char* psArg1, const char* psArg2)
{
  _static_

  i8 iDiff = strcmp(psArg1, psArg2);

  if (0 == iDiff)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bEqualTo: test if the two args match up to nLen number of characters       */
/*----------------------------------------------------------------------------*/
bool StrLib::bEqualTo(const char* psArg1, i8 nLen, const char* psArg2)
{
  _static_

  i8 iDiff = strncmp(psArg1, psArg2, nLen);

  if (0 == iDiff)
    return true;
  else
    return false;
}


/*----------------------------------------------------------------------------*/
/* bEqualTo: are two arguments equal (case or no case)                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bEqualTo(const char* psArg1, const char* psArg2, i8 iCase)
{
  _static_

  i8 iDiff;

  if (C_NO_CASE == iCase)
    iDiff = strcasecmp(psArg1, psArg2);
  else
    iDiff = strcmp(psArg1, psArg2);

  if (0 == iDiff)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bEnds: test if string ends with given char                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bEnds(const char* psArg, const char c)
{
  _static_
  
  const char* z = acLast(psArg);
  
  if (c == *z)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bEnds: test if string ends with given substr arg                           */
/*----------------------------------------------------------------------------*/
bool StrLib::bEnds(const char* psArg, const char* psEnd)
{
  _static_
  
  i8 nLen  = nLength(psArg);
  i8 nBack = nLength(psEnd);
  
  if (nBack > nLen)
    return false;
  
  const char* sFrom = psArg + nLen - nBack;
  
  if (bEqualTo(sFrom, psEnd))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* vEscape: put a '\' before any escapable char                               */
/*----------------------------------------------------------------------------*/
void StrLib::vEscape(char* psDst, i8 iDstBufSize, const char* psSrc, bool* pbResult)
{
  _static_

  i8 iReqBytes = 0;
  
  while (*psSrc) {

    char cChr = *psSrc++;

    if (bIsEscapable(cChr)) { /* null term + escape char */
      if (iReqBytes < iDstBufSize - 2) {
        *psDst++ = '\\';
        *psDst++ = cEscapeFor(cChr);
      }
      ++iReqBytes;  // for the escape char
    }
    else if (iReqBytes < iDstBufSize - 1) {
      *psDst++ = cChr;
    }

    ++iReqBytes;
  }

  *psDst = '\0';
  ++iReqBytes;

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
}

/*----------------------------------------------------------------------------*/
/* bEscapeConvert: test if cArg is an escape character                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bEscapeConvert(const char cArg,
                            char cEnclosedBy_Leading,
                            char cEnclosedBy_Trailing,
                            char cEscapeChar,
                            char cDelimiter,
                            char cTerminator)
{
  _static_
  
  if (cEnclosedBy_Leading == cArg) return true;
  if (cEnclosedBy_Trailing == cArg) return true;
  if (cEscapeChar == cArg) return true;
  if (cDelimiter == cArg) return true;
  if (cTerminator == cArg) return true;
  
  if ('b' == cArg) return true;
  if ('f' == cArg) return true;
  if ('n' == cArg) return true;
  if ('r' == cArg) return true;
  if ('t' == cArg) return true;
  if ('Z' == cArg) return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bEscapeConvert: test if cArg is an escape character                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bEscapeConvertDefault(const char cArg,
                            char cEscapeChar,
                            char cDelimiter)
{
  _static_
  
  if (cEscapeChar == cArg) return true;
  if (cDelimiter == cArg) return true;
  
  if ('b' == cArg) return true;
  if ('f' == cArg) return true;
  if ('n' == cArg) return true;
  if ('r' == cArg) return true;
  if ('t' == cArg) return true;
  if ('Z' == cArg) return true;
  
  return false;
}
/*----------------------------------------------------------------------------*/
/* cEscapeConvert: convert an escaped character to actual special character   */
/*----------------------------------------------------------------------------*/
char StrLib::cEscapeConvert(const char cArg)
{
  _static_
  
  if ('b' == cArg) return '\b';
  if ('f' == cArg) return '\f';
  if ('n' == cArg) return '\n';
  if ('r' == cArg) return '\r';
  if ('t' == cArg) return '\t';
  if ('Z' == cArg) return 26;
  
  return cArg;
}

/*----------------------------------------------------------------------------*/
/* vEscape: put a '\' before an '"' or an existing '\'                        */
/*----------------------------------------------------------------------------*/
void StrLib::vEscapeDblQuote(char* psDst, i8 iDstBufSize, const char* psSrc, bool* pbResult)
{
  _static_
  
  char* d = psDst;
  const char* s = psSrc;

  i8 iReqBytes = 0;

  while (*s) {

    if ('\"' == *s) {
      if (iReqBytes < iDstBufSize - 2) {
        *d++ = '\\';
      }
      ++iReqBytes;  // for the escape char

    } else if ('\\' == *s) {
      if (iReqBytes < iDstBufSize - 2) {
        *d++ = '\\';
      }
      ++iReqBytes;  // for the escape char
    }

    if (iReqBytes < iDstBufSize - 1) {
      *d++ = *s++;
    }
    ++iReqBytes;
  }
  
  *d = '\0';
  ++iReqBytes;

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
}

/*----------------------------------------------------------------------------*/
/* cEscapeFor: convert a special character to escaped character form          */
/*----------------------------------------------------------------------------*/
char StrLib::cEscapeFor(const char cArg)
{
  _static_
  
  if ('\0' == cArg) return '0';
  if ('\b' == cArg) return 'b';
  if ('\f' == cArg) return 'f';
  if ('\n' == cArg) return 'n';
  if ('\r' == cArg) return 'r';
  if ('\t' == cArg) return 't';
  
  return cArg;
}

/*----------------------------------------------------------------------------*/
/* insert cWith before any instance of cChar in azStr                         */
/*----------------------------------------------------------------------------*/
void StrLib::vEscapeInsert(char* psDst, i8 iDstBufSize, char cChar, char cWith, bool* pbResult)
{
  _static_

  char* azDstCrs = psDst;
  i8 iReqBytes = 0;

  while (*azDstCrs) {

    if (cChar != *azDstCrs) {
      ++iReqBytes;
      azDstCrs++;
      continue;
    }

    if (iReqBytes < iDstBufSize - 2) {
      vInsertChar(azDstCrs, iDstBufSize - iReqBytes, cWith, pbResult);
    }

    azDstCrs  += 2;
    iReqBytes += 2;
  }

  // One more needed for the null terminator which should have been moved during vInsertChar().
  ++iReqBytes;

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
}

/*----------------------------------------------------------------------------*/
/* convert a special character to escaped character form                      */
/*----------------------------------------------------------------------------*/
char StrLib::cEscapeInverse(const char cArg)
{
  _static_
  
  if ('\b' == cArg) return 'b';
  if ('\f' == cArg) return 'f';
  if ('\n' == cArg) return 'n';
  if ('\r' == cArg) return 'r';
  if ('\t' == cArg) return 't';
  if ((char)26 == cArg) return 'Z';
  
  return cArg;
}

/*----------------------------------------------------------------------------*/
/* escape character processing                                                */
/*----------------------------------------------------------------------------*/
char StrLib::cEscapeProcessing(const char** aaSrc, char cEscapeChar)
{
  _static_
  
  // test for escape
  if (cEscapeChar != **aaSrc) {
    // not escaped, return char, advance pointer
    return *(*aaSrc)++;
  }
  
  // skip escape character
  (*aaSrc)++;
  
  // must have some non-zero character
  if ( ! **aaSrc )
    return '\0';
  
  // convert the char, advance pointer
  char cConverted = StrLib::cEscapeConvert(*(*aaSrc)++);
  
  return cConverted;
}

/*----------------------------------------------------------------------------*/
/* bEscapedSpecial: test for special character which should be escaped        */
/*----------------------------------------------------------------------------*/
bool StrLib::bEscapedSpecial(const char cArg)
{
  _static_
  
  if ('\b' == cArg) return true;
  if ('\f' == cArg) return true;
  if ('\n' == cArg) return true;
  if ('\r' == cArg) return true;
  if ('\t' == cArg) return true;
  if ((char)26 == cArg) return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bExtractBoolean: extract a boolean from substr                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractBoolean(char** ppsSrcCrs, i8& iBoolean)
{
  _static_
  
  // assume the boolean is false
  iBoolean = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // numeric processing

  // test for number
  if (StrLib::bIsDigit(**ppsSrcCrs)) {
    
    // - numeric processing
    
    // collect all numeric digits
    i8 iResult = 0;
    while (StrLib::bIsDigit(**ppsSrcCrs)) {
      iResult = iResult * 10 + StrLib::iDigitToI8(**ppsSrcCrs);
      (*ppsSrcCrs)++;
    }

    // a non-zero number is true
    if (iResult > 0)
      iBoolean = true;
    
    // saw a valid boolean
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // true processing
  
  // test for possible true
  if ('t' ==  **ppsSrcCrs || 'T' == **ppsSrcCrs) {

    // - true processing
    
    iBoolean = true;
    
    // next 3 characters could be rue to make true
    (*ppsSrcCrs)++;
    if ('r' !=  **ppsSrcCrs && 'R' != **ppsSrcCrs) return true;  // saw only t
    (*ppsSrcCrs)++;
    if ('u' !=  **ppsSrcCrs && 'U' != **ppsSrcCrs) return false; // saw only tr
    (*ppsSrcCrs)++;
    if ('e' !=  **ppsSrcCrs && 'E' != **ppsSrcCrs) return false; // saw only tru
    (*ppsSrcCrs)++;
    
    return true; // saw true
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // false processing
  
  // test for possible false
  if ('f' ==  **ppsSrcCrs || 'F' == **ppsSrcCrs) {
    
    // - false processing
    
    // next 4 characters could be alse to make alse
    (*ppsSrcCrs)++;
    if ('a' !=  **ppsSrcCrs && 'A' != **ppsSrcCrs) return true;  // saw only f
    (*ppsSrcCrs)++;
    if ('l' !=  **ppsSrcCrs && 'L' != **ppsSrcCrs) return false; // saw only fa
    (*ppsSrcCrs)++;
    if ('s' !=  **ppsSrcCrs && 'S' != **ppsSrcCrs) return false; // saw only fal
    (*ppsSrcCrs)++;
    if ('e' !=  **ppsSrcCrs && 'E' != **ppsSrcCrs) return false; // saw only fals
    (*ppsSrcCrs)++;
    
    return true; // saw false
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // saw nothing that could be recognised as a boolean
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bExtractDate: extract a date from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDate(char** ppsSrcCrs, i8& iYear, i8& iMonth, i8& iDay)
{
  _static_
  
  u8 uSubField;
  
  iYear = iMonth = iDay = 0;
  
  // extract year
  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField, 4) )
    return false;
  
  // assume a leap year
  bool bLeapYear = true;
  if (uSubField % 4 == 0) {
    if ((uSubField % 100) == 0 && (uSubField % 400) != 0) {
      // not a leap year
      bLeapYear = false;
    }
  }
  iYear = uSubField;
  
  // skip - separator
  if ('-' != *(*ppsSrcCrs)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField, 2) )
    return false;
  if (uSubField < 1 || uSubField > 12)
    return false;
  iMonth = uSubField;
  
  // skip - separator
  if ('-' != *(*ppsSrcCrs)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField, 2) )
    return false;
  if (bLeapYear) {
    if ((i8)uSubField > iDaysPerMonthLeap[iMonth-1])
      return false;
  }
  else {
    if ((i8)uSubField > iDaysPerMonth[iMonth-1])
      return false;
  }
  iDay = uSubField;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractDate: extract a date from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDate2(char** ppsSrcCrs, i8& iYear, i8& iMonth, i8& iDay,
                           eDC* pDateComponents, char cDateSeparator)
{
  _static_
  
  for (i8 i_Component = 0; i_Component < 3; i_Component++) {
  
    eDC eDateComponent = pDateComponents[i_Component];
    
    switch (eDateComponent) {
      
      case DC_DD:
        if (! bExtractDay(ppsSrcCrs, iDay))
          return false;
        break;
      
      case DC_MM:
        if (! bExtractMonth(ppsSrcCrs, iMonth))
          return false;
        break;
      
      case DC_YYYY:
        if (! bExtractYear(ppsSrcCrs, iYear))
          return false;
        break;
      
      default:
        return false;
    }
    
    // skip separator after components  0 and 1
    if (i_Component < 2) {
      if (cDateSeparator != *(*ppsSrcCrs)++)
        return false;
    }
    
  }
  
  if (! bValidateMonth(iDay, iMonth, iYear))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractDay: extract a day from substr                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDay(char** ppsSrcCrs, i8& iDay)
{
  _static_
  
  u8 uSubField;

  iDay = 0;

  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField, 2) )
    return false;

  iDay = uSubField;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractDecimal: extract a decimal from substr                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDecimal(char** ppsSrcCrs, i8& iDecimal, i8 iLength, i8 iScale)
{
  _static_
  
  i8 iPrecision = iLength - iScale;
  
  u8 uUnsignedResult = 0;
  
  // test for negative or positive sign
  bool bNegative = false;
  if (**ppsSrcCrs && '-' == **ppsSrcCrs ) {
    bNegative = true;
    (*ppsSrcCrs)++;
  }
  else
    if (**ppsSrcCrs && '+' == **ppsSrcCrs ) {
      (*ppsSrcCrs)++;
    }
  
  bool bOnce = false;
  if (**ppsSrcCrs && (isdigit(**ppsSrcCrs) || '.' == **ppsSrcCrs))
    bOnce = true;
  
  // ignore leading 0's
  while (StrLib::bIsDigit(**ppsSrcCrs) && StrLib::iDigitToI8(**ppsSrcCrs) == 0) {
    (*ppsSrcCrs)++;
  }
  
  // collect leading digits (till '.')
  while (StrLib::bIsDigit(**ppsSrcCrs)) {
    uUnsignedResult = uUnsignedResult * 10 + StrLib::iDigitToI8(**ppsSrcCrs);
    (*ppsSrcCrs)++;
    iPrecision--;
  }
  
  // have we exceeded maximum precision digits?
  if (iPrecision < 0)
    return false;
  
  if ('.' == **ppsSrcCrs) {
    
    (*ppsSrcCrs)++; // skip '.'
    
    while (StrLib::bIsDigit(**ppsSrcCrs) && iScale) {
      uUnsignedResult = uUnsignedResult * 10 + StrLib::iDigitToI8(**ppsSrcCrs);
      (*ppsSrcCrs)++;
      iScale--;
    }
  }
  
  while (iScale--)
    uUnsignedResult = uUnsignedResult * 10 + 0;
  
  // have we exceeded the max ?
  if (uUnsignedResult > (u8)C_I8_MAX)
    return false;
  
  // round up if there any more digits after decimal
  if (StrLib::bIsDigit(**ppsSrcCrs)) {
    if (StrLib::iDigitToI8(**ppsSrcCrs) >= 5)
      uUnsignedResult += 1;
    while (StrLib::bIsDigit(**ppsSrcCrs))
      (*ppsSrcCrs)++;
  }
  
  // compute final result
  uUnsignedResult = bNegative ? - uUnsignedResult : uUnsignedResult;
  
  // have we gone beyond the min ?
  if (uUnsignedResult != 0 && bNegative && (uUnsignedResult < (u8)C_I8_MIN))
    return false;
  
  iDecimal = uUnsignedResult;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* extract value from a decimal literal                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDecLit(char*& ppsSrcCrs, i8& iValue, i8& nFractionDigits)
{
  _static_
  
  iValue = 0;
  nFractionDigits = 0;
    
  // collect whole number part (till '.')
  while (StrLib::bIsDigit(*ppsSrcCrs))
    iValue = iValue * 10 + StrLib::iDigitToI8(*ppsSrcCrs++);

  if ('.' != *ppsSrcCrs)
    return true;
  
  // skip '.'
  ppsSrcCrs++;

  // collect fraction as well
  while (StrLib::bIsDigit(*ppsSrcCrs)) {
    iValue = iValue * 10 + StrLib::iDigitToI8(*ppsSrcCrs++);
    nFractionDigits++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractFloat: extract a float from substr                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractFloat(char** ppsSrcCrs, d8& dFloat)
{
  _static_
  
  d8 dUnsignedResult = 0.0;
  char xBuffer[256];
  char* aChar = xBuffer;
  
  // test for negative or positive sign
  bool bNegative = false;
  if (**ppsSrcCrs && '-' == **ppsSrcCrs ) {
    bNegative = true;
    (*ppsSrcCrs)++;
  }
  else
    if (**ppsSrcCrs && '+' == **ppsSrcCrs ) {
      (*ppsSrcCrs)++;
    }
  
  bool bOnce = false;
  if (**ppsSrcCrs && (isdigit(**ppsSrcCrs) || '.' == **ppsSrcCrs))
    bOnce = true;
  
  // collect leading portion (till '.')
  while (StrLib::bIsDigit(**ppsSrcCrs))
    *aChar++ = *(*ppsSrcCrs)++;
  
  // collect trailing portion (after '.')
  if ('.' == **ppsSrcCrs) {
    *aChar++ = *(*ppsSrcCrs)++;
    while (StrLib::bIsDigit(**ppsSrcCrs))
      *aChar++ = *(*ppsSrcCrs)++;
  }
  
  // test for exponent
  if (**ppsSrcCrs && ('e' == **ppsSrcCrs || 'E' == **ppsSrcCrs) ) {
    
    *aChar++ = *(*ppsSrcCrs)++;
    
    // test for -
    if (**ppsSrcCrs && '-' == **ppsSrcCrs )
      *aChar++ = *(*ppsSrcCrs)++;
    else
    // test for +
    if (**ppsSrcCrs && '+' == **ppsSrcCrs )
      *aChar++ = *(*ppsSrcCrs)++;
    
    // collect exponent digits
    while (StrLib::bIsDigit(**ppsSrcCrs))
      *aChar++ = *(*ppsSrcCrs)++;
  }
  
  // terminate
  *aChar++ = '\0';
  
  // compute final result
  dUnsignedResult = atof(xBuffer);
  dFloat = bNegative ? - dUnsignedResult : dUnsignedResult;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractFraction: extract an unsigned integer as a fraction from substr    */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractFraction(char** ppsSrcCrs, u8* aU8, i8 iDigits)
{
  _static_
  
  char *s = *ppsSrcCrs;
  
  if ( ! *s )
    return false;
  
  bool bOnce = false;
  if (*s && isdigit(*s))
    bOnce = true;
  
  u8 uFraction = 0;
  
  for ( ; *s && iDigits > 0; s++) {
    
    char cDigit = *s;
    
    if (cDigit < '0' || cDigit > '9')
      break;
    
    // decrement no of digits extracted
    iDigits--;
    
    uFraction = (uFraction * 10) + (cDigit - '0');
  }
  
  while (iDigits--)
    uFraction *= 10;
  
  *aU8 = uFraction;
  
  *ppsSrcCrs = s;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractInteger: extract an integer from substr                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractInteger(char** ppsSrcCrs, i8* aI8, bool bWithSign)
{
  _static_
  
  char *aSrc = *ppsSrcCrs;
  
  if ( ! *aSrc )
    return false;
 
  // test for negative or positive sign
  bool bNegative = false;
  if (bWithSign) {
    if (*aSrc && '-' == *aSrc) {
      bNegative = true;
      aSrc++;
    }
    else
    if (*aSrc && '+' == *aSrc) {
      aSrc++;
    }
  }
  
  bool bOnce = false;
  if (*aSrc && isdigit(*aSrc))
    bOnce = true;
  
  u8 uUnsignedResult = 0;
  for ( ; *aSrc; aSrc++) {
    
    char c = *aSrc;
    
    if ( c < '0' || c > '9' )
      break;
    
    uUnsignedResult = (uUnsignedResult * 10) + (c - '0');
    
    // have we exceeded the max ?
    if (uUnsignedResult > (u8)C_I8_MAX)
      return false;
  }
  
  if (bNegative)
    *aI8 = - uUnsignedResult;
  else
    *aI8 = uUnsignedResult;
  
  *ppsSrcCrs = aSrc;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractInteger: extract an integer from substr, counting no of digits     */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractInteger(char** ppsSrcCrs, i8* aI8, i8& nDigits)
{
  _static_
  
  char *aSrc = *ppsSrcCrs;
  
  if ( ! *aSrc )
    return false;
  
  bool bOnce = false;
  if (*aSrc && isdigit(*aSrc))
    bOnce = true;
  
  nDigits = 0;
  
  u8 uUnsignedResult = 0;
  for ( ; *aSrc; aSrc++) {
    
    char c = *aSrc;
    
    if ( c < '0' || c > '9' )
      break;
    
    uUnsignedResult = (uUnsignedResult * 10) + (c - '0');
    nDigits++;
    
    // have we exceeded the max ?
    if (uUnsignedResult > (u8)C_I8_MAX)
      return false;
  }
  
  *aI8 = uUnsignedResult;
  
  *ppsSrcCrs = aSrc;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractInterval: extract an interval from substr                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractInterval(char** ppsSrcCrs, i8& iSecond, i8& iMilli)
{
  _static_
  
  u8 uSubField;
  
  iSecond = iMilli = 0;
  
  // extract second
  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField) )
    return false;
  
  iSecond = uSubField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **ppsSrcCrs) {
    (*ppsSrcCrs)++;
    if ( StrLib::bExtractU8(ppsSrcCrs, &uSubField) )
      iMilli = uSubField;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractItem: extract an item from substr until a space or comma is seen   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractItem(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult )
{
  _static_

  if ( ! *ppsSrcCrs )
    return false;

  i8 iReqBytes = 0;

  char *d = psDst;
  bool bOnce = false;
  
  for ( ; **ppsSrcCrs; (*ppsSrcCrs)++)
  {
    if (' ' == **ppsSrcCrs || ',' == **ppsSrcCrs)
      break;

    bOnce = true;

    ++iReqBytes;
    if (iReqBytes < iDstBufSize) {
      *d++ = **ppsSrcCrs;
    }

  }

  ++iReqBytes;
  *d = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractItem: extract an item from substr                                  */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractItem(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, char cDelimiter, bool* pbResult)
{
  _static_
  
  if ( ! *ppsSrcCrs )
    return false;

  i8 iReqBytes = 0;

  char *a = psDst;
  bool bOnce = false;
  
  for ( ; **ppsSrcCrs; (*ppsSrcCrs)++)
  {
    //if (' ' == **ps || cDelimiter == **ps)
    bOnce = true;
    
    if (cDelimiter == **ppsSrcCrs)
      break;

    ++iReqBytes;
    if (iReqBytes < iDstBufSize) {
      *a++ = **ppsSrcCrs;
    }
  }

  ++iReqBytes;
  *a = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractMonth: extract a month from substr                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractMonth(char** ppsSrcCrs, i8& iMonth)
{
  _static_
  
  u8 uSubField;
  
  iMonth = 0;
  
  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField, 2) )
    return false;
  
  if (uSubField < 1 || uSubField > 12)
    return false;

  iMonth = uSubField;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractName: extract a name from substr until a '/' is hit                */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractName(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult)
{
  _static_
  
  if (! **ppsSrcCrs)
    return false;

  i8 iReqBytes = 0;
  char *d = psDst;
  
  for ( ; **ppsSrcCrs; (*ppsSrcCrs)++) {
    
    if ('/' == **ppsSrcCrs) {
      (*ppsSrcCrs)++;
      break;
    }

    ++iReqBytes;
    if (iReqBytes < iDstBufSize) {
      *d++ = **ppsSrcCrs;
    }
  }

  ++iReqBytes;
  *d = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractPath: extract a path from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractPath(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult )
{
  _static_
  
  if ( ! *ppsSrcCrs )
    return false;
  
  bool bOnce = false;
  if (**ppsSrcCrs && ' ' != **ppsSrcCrs)
    bOnce = true;

  i8 iReqBytes = 0;

  char* s = *ppsSrcCrs;
  char* d = psDst;
  while (*s && ' ' != *s) {

    ++iReqBytes;
    if (iReqBytes < iDstBufSize) {
      *d++ = *s;
    }
    s++;
  }

  *ppsSrcCrs = s;
  ++iReqBytes;
  if (iDstBufSize > 0) {
    *d = '\0';
  }

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractPort: extract a port number from substr                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractPort(char** ppsSrcCrs, u2* pPort)
{
  _static_
  
  if ( ! *ppsSrcCrs )
    return false;
  
  bool bOnce = false;
  if (**ppsSrcCrs && isdigit(**ppsSrcCrs))
    bOnce = true;
  
  i8 v = 0;
  for ( ; **ppsSrcCrs; (*ppsSrcCrs)++)
  {
    char c = **ppsSrcCrs;
    
    if ( ! isdigit(c) )
      break;
    
    v = (v * 10) + c - '0';
  }
  
  *pPort = v;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractQuotedString: extract a double quoted string from substr           */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractQuotedString(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult)
{
  _static_
  
  if ( ! *ppsSrcCrs )
    return false;
  
  if ('"' != **ppsSrcCrs)
    return false;

  i8 iReqBytes = 0;

  char* s = *ppsSrcCrs;
  char* d = psDst;

  s++;
  for ( ; *s; s++) {

    ++iReqBytes;
    if ('"' == *s) {
      s++;
      break;
    }
    if (iReqBytes < iDstBufSize) {
      *d++ = *s;
    }
  }

  *ppsSrcCrs = s;
  ++iReqBytes;
  if (iDstBufSize > 0) {
    *d = '\0';
  }

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractTime: extract a time from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractTime(char** ppsSrcCrs,
                          i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro)
{
  _static_
  
  u8 uSubField;
  
  iHour = iMinute = iSecond = iMicro = 0;
  
  // extract hour
  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField, 2) )
    return false;
  if (uSubField > 23)
    return false;
  iHour = uSubField;
  
  // skip : separator
  if (':' != *(*ppsSrcCrs)++)
    return false;
  
  // extract minute
  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField, 2) )
    return false;
  
  if (uSubField > 59)
    return false;
  iMinute = uSubField;
  
  // skip : separator
  if (':' != *(*ppsSrcCrs)++)
    return false;
  
  // extract second
  if ( ! StrLib::bExtractU8(ppsSrcCrs, &uSubField, 2) )
    return false;
  if (uSubField > 59)
    return false;
  iSecond = uSubField;
  
  // extract fraction (assuming microseconds)
  if ('.' == **ppsSrcCrs) {
    (*ppsSrcCrs)++;
    
    if ( StrLib::bExtractFraction(ppsSrcCrs, &uSubField, 6) ) {
      iMicro = uSubField;
    }
    
    // skip/truncate any remaining digits
    while (bIsDigit(**ppsSrcCrs))
      (*ppsSrcCrs)++;
  }
  
  // skip optional Z (UTC zone designator)
  if ('Z' == **ppsSrcCrs)
    (*ppsSrcCrs)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractTimestamp: extract a timestamp from substr                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractTimestamp(char** ppsSrcCrs,
                               i8& iYear, i8& iMonth,  i8& iDay,
                               i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro)
{
  _static_
    
  // - date portion

  if ( ! bExtractDate(ppsSrcCrs, iYear, iMonth, iDay) )
    return false;
  //if ( ! StrLib::bExtractDate2(aaSrc, iYear, iMonth, iDay,
  //                             pFmt->m_eDateComponent,
  //                             pFmt->m_cDateSeparator) )
  //  return false;
  
  // - time portion
  
  // skip T or space separator
  if ( ! ('T' == **ppsSrcCrs || ' ' == **ppsSrcCrs) ) {
    iHour = iMinute = iSecond = iMicro = 0;
    return true;
  }
  
  (*ppsSrcCrs)++;
  
  if ( ! bExtractTime(ppsSrcCrs, iHour, iMinute, iSecond, iMicro) )
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractTimestamp: extract a timestamp from substr                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractTimestamp2(char** ppsSrcCrs,
                                i8& iYear, i8& iMonth,  i8& iDay,
                                i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro,
                                eDC* pDateComponents, char cDateSeparator)
{
  _static_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date portion
  
  if ( ! StrLib::bExtractDate2(ppsSrcCrs, iYear, iMonth, iDay,
                               pDateComponents, cDateSeparator) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time portion
  
  // skip T or space separator
  if ( ! ('T' == **ppsSrcCrs || ' ' == **ppsSrcCrs) ) {
    iHour = iMinute = iSecond = iMicro = 0;
    return true;
  }
  
  (*ppsSrcCrs)++;
  
  if ( ! bExtractTime(ppsSrcCrs, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractU8: extract an unsigned integer from substr                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractU8(char** ppsSrcCrs, u8* aU8)
{
  _static_
  
  char *aSrc = *ppsSrcCrs;
  
  if ( ! *aSrc )
    return false;
  
  bool bOnce = false;
  if (*aSrc && isdigit(*aSrc))
    bOnce = true;
  
  u8 uUnsignedResult = 0;
  for ( ; *aSrc; aSrc++) {
    
    char cChar = *aSrc;
    
    if (cChar < '0' || cChar > '9')
      break;
    
    uUnsignedResult = (uUnsignedResult * 10) + cChar - '0';
    
    // have we exceeded the max ?
    if (uUnsignedResult > (u8)C_I8_MAX)
      return false;
  }
  
  *aU8 = uUnsignedResult;
  
  *ppsSrcCrs = aSrc;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractU8: extract an unsigned integer from substr up to iDigits          */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractU8(char** ppsSrcCrs, u8* aU8, i8 iDigits)
{
  _static_
  
  char *s = *ppsSrcCrs;
  
  if ( ! *s )
    return false;
  
  bool bOnce = false;
  if (*s && isdigit(*s))
    bOnce = true;
  
  u8 v = 0;
  for ( ; *s && iDigits > 0; s++)
  {
    char c = *s;
    
    // decrement no of digits extracted
    iDigits--;
    
    //    if ( ! isdigit(c) )
    if ( c < '0' || c > '9' )
      break;
    
    v = (v * 10) + c - '0';
  }
  
  *aU8 = v;
  
  *ppsSrcCrs = s;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractVarchar: extract a varchar from substr                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractVarchar(char** ppsSrcCrs,
                             char* psDst,
                             i8 iDstBufLen,
                             i8 iMaxLength,
                             bool bSawLeadingEnclosed,
                             char cEnclosedBy_Leading,
                             char cEnclosedBy_Trailing,
                             char cEscapeChar,
                             char cFieldSeparator,
                             char cRecordTerminator,
                             bool bAllowNonPrintable,
                             i8& nChars,
                             bool* pbResult)
{
  _static_

  i8 iLength = 0;
  char *aDst = (char*)psDst;
  bool bSawTrailingEnclosed = false;

  if (iMaxLength >= iDstBufLen) {
    // Report the potential truncation, limit iMaxLength and carry on
    bCheckBufferSpace(__func__, iMaxLength, iDstBufLen, pbResult);
    iMaxLength = iDstBufLen - 1;
  }

  char cSrc = **ppsSrcCrs;
  while (cSrc) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // special case of double enclosedBy
    
    if (cEnclosedBy_Trailing == cEnclosedBy_Leading) {
      if (cSrc == cEnclosedBy_Leading && *(*ppsSrcCrs + 1) == cEnclosedBy_Leading) {
        
        // skip the first enclosed character
        (*ppsSrcCrs)++;
       
        // append one instance of the enclosedBy character
        if (iLength++ < iMaxLength)
          *aDst++ = cSrc;
      
        (*ppsSrcCrs)++;
        cSrc = **ppsSrcCrs;
        continue;
      }
      //falls thru if not the case of double enclosedBy
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for a non-printable character (e.g. \n or \r)
   
    // Alka: Changed check from ! isprint (not-printable) to isspace and then allowing space
    // this ways we allow for UTF-8 characters to pass this test
    // I think there is more to supporting UTF-8 but this will let us allow for UTF-8 characters to
    // be loaded from files.
    
    //if (! bAllowNonPrintable && ! isprint(**aaSrc) )
    
    // isprint: checks for any printable character including space.
    
    // isspace : checks for white-space characters. In the C and POSIX locales, these are:
    // space, form-feed ('\f'), newline ('\n'), carriage return ('\r'), horizontal tab ('\t'),
    // and vertical tab ('\v').
      
    if (! bAllowNonPrintable && isspace(cSrc) && (cSrc != ' ') )
        break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for normal end
    
    if (bSawLeadingEnclosed) {
      // we saw a leading enclodedBy so test for trailing enclosedBy
      if (cSrc == cEnclosedBy_Trailing) {
        bSawTrailingEnclosed = true;
        (*ppsSrcCrs)++;
        break;
      }
      //falls thru if not trailing enclosedBy
    }
    // test for field separator (because its not enclosed)
    else if (cFieldSeparator && cSrc == cFieldSeparator)
      break;
    // test for record terminator (because its not enclosed)
    else if (cRecordTerminator && cSrc == cRecordTerminator)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for the escape character
    
    if (cSrc == cEscapeChar) {
      
      if (StrLib::bEscapeConvert( *((*ppsSrcCrs) + 1),
                                 cEnclosedBy_Leading, cEnclosedBy_Trailing,
                                 cEscapeChar, cFieldSeparator, cRecordTerminator)) {
        
        // skip the escape character
        (*ppsSrcCrs)++;
        
        // append the escaped character
        if (iLength++ < iMaxLength)
          *aDst++ = StrLib::cEscapeConvert(*(*ppsSrcCrs)++);
        else
          (*ppsSrcCrs)++;
        
        cSrc = **ppsSrcCrs;
        continue;
      }
      else {
        (*ppsSrcCrs)++;
        cSrc = **ppsSrcCrs;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // normal character
    
    // append the normal character
    if (iLength++ < iMaxLength)
      *aDst++ = cSrc;
    
    (*ppsSrcCrs)++;
    cSrc = **ppsSrcCrs;
  }
  
  // terminate
  *aDst++ = '\0';
  
  // its not an enclosed varchar so we consider it to be valid
  if (iLength < iMaxLength) {
    nChars = iLength;

  } else {
    nChars = iMaxLength;
    bool bRet = bCheckBufferSpace(__func__, iLength + 1 /* null term*/, iDstBufLen, pbResult);
    UNUSED(bRet);
  }
  
  // if this an enclosed varchar we should have expected to
  // see the trailing enclosedBy
  if (bSawLeadingEnclosed)
    return bSawTrailingEnclosed;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractVarchar: extract a varchar from substr                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractVarcharDefault(char** ppsSrcCrs,
                                    char* psDst,
                                    i8 iDstBufLen,
                                    i8 iMaxLength,
                                    i8& nChars,
                                    bool* pbResult)
{
  _static_
  
  i8 iLength = 0;
  
//  char cEscapeChar = AM::pFmt()->m_cEscapeChar;
//  char cFieldDelimiter = AM::pFmt()->m_cFieldDelimiter;

  char cEscapeChar = D_ESCAPE_CHAR;
  char cFieldDelimiter = D_FIELD_DELIMITER;

  char *aDst = (char*)psDst;
  char cSrc = **ppsSrcCrs;

  if (iMaxLength >= iDstBufLen) {
    // Report the potential truncation, limit iMaxLength and carry on
    bCheckBufferSpace(__func__, iMaxLength, iDstBufLen, pbResult);
    iMaxLength = iDstBufLen - 1;
  }
  
  while (cSrc) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for a non-printable character (e.g. \n or \r)
    
    // Alka: Changed check from ! isprint (not-printable) to isspace and then allowing space
    // this ways we allow for UTF-8 characters to pass this test
    // I think there is more to supporting UTF-8 but this will let us allow for UTF-8 characters to
    // be loaded from files.
    
    //if (! bAllowNonPrintable && ! isprint(**aaSrc) )
    
    // isprint: checks for any printable character including space.
    
    // isspace : checks for white-space characters. In the C and POSIX locales, these are:
    // space, form-feed ('\f'), newline ('\n'), carriage return ('\r'), horizontal tab ('\t'),
    // and vertical tab ('\v').
    
    if (isspace(cSrc) && (cSrc != ' ') )
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for normal end
    
    // test for field separator (because its not enclosed)
    if (cSrc == cFieldDelimiter)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for the escape character
    
    if (cSrc == cEscapeChar) {
      
      if (StrLib::bEscapeConvertDefault( *((*ppsSrcCrs) + 1),
                                 cEscapeChar, cFieldDelimiter)) {
        
        // skip the escape character
        (*ppsSrcCrs)++;
        
        // append the escaped character
        if (iLength++ < iMaxLength)
          *aDst++ = StrLib::cEscapeConvert(*(*ppsSrcCrs)++);
        else
          (*ppsSrcCrs)++;
        
        cSrc = **ppsSrcCrs;
        continue;
      }
      else {
        (*ppsSrcCrs)++;
        cSrc = **ppsSrcCrs;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // normal character
    
    // append the normal character
    if (iLength++ < iMaxLength)
      *aDst++ = cSrc;
    
    (*ppsSrcCrs)++;
    cSrc = **ppsSrcCrs;
  }
  
  // terminate
  *aDst++ = '\0';
  
  // its not an enclosed varchar so we consider it to be valid
  if (iLength < iMaxLength) {
    nChars = iLength;

  } else {
    nChars = iMaxLength;
    bool bRet = bCheckBufferSpace(__func__, iLength + 1 /* null term*/, iDstBufLen, pbResult);
    UNUSED(bRet);
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractWord: extract a word from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractWord(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult)
{
  _static_
  
  if ( ! *ppsSrcCrs )
    return false;
  
  bool bOnce = false;
  if (**ppsSrcCrs && isalnum(**ppsSrcCrs))
    bOnce = true;

  i8 iReqBytes = 0;
  char *d = psDst;
  for ( ; **ppsSrcCrs; (*ppsSrcCrs)++)
  {
    if ( ! isalnum(**ppsSrcCrs) )
      break;

    ++iReqBytes;
    if (iReqBytes < iDstBufSize) {
      *d++ = **ppsSrcCrs;
    }
  }

  ++iReqBytes;
  *d = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);

  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractWord: extract a word or quoted string                              */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractWordString(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult)
{
  _static_
  
  if ( ! *ppsSrcCrs )
    return false;

  if ('"' == **ppsSrcCrs)
    return bExtractQuotedString(ppsSrcCrs, psDst, iDstBufSize, pbResult);

  if (!isalnum(**ppsSrcCrs) && '_' != **ppsSrcCrs)
    return bExtractItem(ppsSrcCrs, psDst, iDstBufSize, pbResult);

  i8 iReqBytes = 0;
  bool bOnce = false;

  if (**ppsSrcCrs && isalnum(**ppsSrcCrs))
    bOnce = true;
  
  char *d = psDst;

  for ( ; **ppsSrcCrs; (*ppsSrcCrs)++)
  {
    if (!isalnum(**ppsSrcCrs) && '_' != **ppsSrcCrs)
      break;

    ++iReqBytes;
    if (iReqBytes < iDstBufSize) {
      *d++ = **ppsSrcCrs;
    }
  }

  ++iReqBytes;
  *d = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractYear: extract a month from substr                                  */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractYear(char** ppsSrcCrs, i8& iYear)
{
  _static_

  i8 nDigits;
  
  // extract year
  if ( ! bExtractInteger(ppsSrcCrs, &iYear, nDigits) )
    return false;
  
  if (2 == nDigits) {
    iYear += 2000;
    return true;
  }
  
  if (4 == nDigits)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* iFind: find the index position of first occurrence of cArg in ps, otherwise*/
/*        -1 (C_UNKNOWN) is returned.                                         */
/*----------------------------------------------------------------------------*/
i8 StrLib::iFind(const char* psSrc, const char cArg)
{
  _static_

  const char* f = strchr(psSrc, cArg);

  if (f) {
   i8 iPos = f - psSrc;
   return iPos;

  } else {
    return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* bFind: find start of ScanChar in substr                                    */
/*----------------------------------------------------------------------------*/
bool StrLib::bFind(char** ppsSrc, const char cArg)
{
  _static_
  
  char* f = strchr(*ppsSrc, cArg);
  
  if (f)
  {
    *ppsSrc = f;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bFind: find nth ScanChar in substr                                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bFind(char** ppsSrc, const char cArg, i8 iOccurence)
{
  _static_
  
  bool b;
  
  for (i8 i_Count = 0; i_Count < iOccurence; i_Count++) {
    b = bFind(ppsSrc, cArg);
    if (! b)
      return false;
    (*ppsSrc)++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bFind: find start of arg in substr                                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bFind(char** ppsSrc, const char* psArg)
{
  _static_
  
  char* f = strstr(*ppsSrc, psArg);
  
  if (f)
  {
    *ppsSrc = f;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bFind: find start of arg in substr                                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bFind(char** ppsSrc, const char* psArg, i8 iLenArg)
{
  _static_
  
  char *s = *ppsSrc;
  
  for (;;)
  {
    s = strchr(s,*psArg);
    
    // end of substr?
    if (nullptr == s)
      return false;
    
    // match?
    if (0 == strncmp(s, psArg, iLenArg))
    {
      *ppsSrc = s;
      return true;
    }
    
    // advance
    s++;
  }
}

/*----------------------------------------------------------------------------*/
/* vFormatFraction: format a fraction                                         */
/*----------------------------------------------------------------------------*/
void StrLib::vFormatFraction(char* psDst, i8 iDstBufSize, u8 uFraction, i8 iDigits, bool* pbResult)
{
  _static_

  const i8 nFormatSize = 16;
  char caFormat[nFormatSize];
  
  // create the format
  vSprint(caFormat, nFormatSize, pbResult, "%%0%lldlld"_Stz, iDigits);

  // sprint the number as an integer
  vSprint(psDst, iDstBufSize, pbResult, (Stz*) caFormat, uFraction);
}

/*----------------------------------------------------------------------------*/
/* glean an integer from given string (skip leading non-digits)               */
/*----------------------------------------------------------------------------*/
i8 StrLib::iGleanInt(const char* psSrc)
{
  _static_
  
  i8 iVal = 0;
  
  const char* aSrc = psSrc;

  // skip leading non-digits
  while (*aSrc && ! bIsDigit(*aSrc))
    aSrc++;
  
  // convert digit sequence
  while (*aSrc && bIsDigit(*aSrc))
    iVal = iVal * 10 + iDigitToI8(*aSrc++);

  return iVal;
}

/*----------------------------------------------------------------------------*/
/* find end of next line group given line separator and max bytes             */
/*----------------------------------------------------------------------------*/
const char* StrLib::azGroupEnd(const char* psSrc, char cItemSep, i8 nAvailBytes,
                               i8& rnBytesInGroup, i8& rnLinesInGroup)
{
  _static_

  ASSERT(*psSrc);
  ASSERT(nAvailBytes);

  const char* szGroupEnd = psSrc;
  rnLinesInGroup   = 0;
  rnBytesInGroup   = 0;
  i8 nCrs_bytes    = 0;

  for (const char* azCrs_arg = psSrc; nCrs_bytes < nAvailBytes; azCrs_arg++) {

    // increment length of potential group
    nCrs_bytes++;

    if (*azCrs_arg == cItemSep || ! *azCrs_arg) {
      // + line separator (or \0 end)
      // add line to group
      szGroupEnd     = azCrs_arg;
      rnBytesInGroup = nCrs_bytes;
      rnLinesInGroup++;

      if (! *azCrs_arg)
        break;
    }

  }

  // nb. may have exited because last potential line didn't fit within max bytes

  if (rnBytesInGroup) {
    // + subset of items which fit

    ASSERT(rnLinesInGroup);

    // *szGroupEnd will be cItemSep or \0
    ASSERT(*szGroupEnd == cItemSep || !*szGroupEnd);

    // byte length of group is (chars of group) plus (cItemSep or \0)
    ASSERT(szGroupEnd - psSrc + 1 == rnBytesInGroup);

    // group will not be longer than nAvailBytes
    ASSERT(rnBytesInGroup <= nAvailBytes);
  }
  else {
    // - no subset of lines fits into max bytes

    ASSERT(! rnLinesInGroup);

    ASSERT(szGroupEnd == psSrc);
  }

  return szGroupEnd;
}

/*----------------------------------------------------------------------------*/
/* iHexToInw: convert hex character to i8 (-1 on error)                       */
/*----------------------------------------------------------------------------*/
i8 StrLib::iHexToInw(const char c)
{
  _static_
  
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= '0' && c <= '9') return c - '0';
  
  return -1;
}

/*----------------------------------------------------------------------------*/
/* insert char at start of given str, shifting remaining chars down           */
/*----------------------------------------------------------------------------*/
void StrLib::vInsertChar(char* psDst, i8 iDstBufSize, char cArg, bool* pbResult)
{
  _static_

  char* azCrs = azNullTerminator(psDst);

  i8 iReqBytes = nDifference(azCrs, psDst) + 2;

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  if (!bRet) {
    // Not enough room, so don't do anything
    return;
  }

  // Starting from the last char (the null terminator),
  // shift everything down
  *(azCrs+1) = *azCrs;

  while (azCrs != psDst) {
    azCrs--;
    *(azCrs+1) = *azCrs;
  }

  *psDst = cArg;
}

/*----------------------------------------------------------------------------*/
/* insert str at start of given str, shifting remaining chars down and        */
/* truncating them if they don't fit                                          */
/*----------------------------------------------------------------------------*/
void StrLib::vInsertStrTrimRight(char* psDst, i8 iDstBufSize, const char* psSrc, bool* pbResult)
{
  _static_

  i8 iDstLen = nLength(psDst);
  i8 iSrcLen = nLength(psSrc);

  if (iDstLen + iSrcLen + 1 <= iDstBufSize) {
    // There is enough space and no truncation will occur.
    // We can just move the existing psDst contents down by iSrcLen bytes
    // and then write psSrc at the beginning.
    char* psMoveDst = psDst + iSrcLen;
    MemLib::vMove(psMoveDst, psDst, iDstLen + 1, CN_STRLIB);

    const char* s = psSrc;
    char* d = psDst;

    for ( ; *s; s++, d++) {
      *d = *s;
    }

    return;
  }

  // We will need to truncate the right edge of the existing string since it
  // won't fit. This isn't a string truncation error to be reported because
  // it is the semantics of this function, _unless_ psSrc is too big for
  // the buffer in the first place (then we don't know what to trim).

  if (iSrcLen + 1 > iDstBufSize) {
    bCheckBufferSpace(__func__, iSrcLen + 1, iDstBufSize, pbResult);
    // We cannot continue here
    return;
  }

  // Calculate how many bytes of the original string we can keep
  i8 iExcessBytes = (iDstLen + iSrcLen + 1) - iDstBufSize;
  ASSERT (iExcessBytes > 0);

  char* psMoveDst = psDst + iSrcLen;
  i8 iBytesToMove = iDstLen + 1 - iExcessBytes;
  MemLib::vMove(psMoveDst, psDst, iBytesToMove, CN_STRLIB);
  psDst[iDstBufSize - 1] = 0;

  const char* s = psSrc;
  char* d = psDst;

  for ( ; *s; s++, d++) {
    *d = *s;
  }
}

/*----------------------------------------------------------------------------*/
/* bIsAlpha: is char an alphabetic character?                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAlpha(const char c)
{
  _static_
  
  if (isalpha(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAlphaDigit: is char an alphabetic or numeric digit character?           */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAlphaDigit(const char c)
{
  _static_
  
  if (isalpha(c) || isdigit(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAmpersand: is char an ampersand?                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAmpersand(const char c)
{
  _static_
  
  if (c == '&')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAsciiName: is argument a valid name                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAsciiName(const char* psArg)
{
  _static_

  // check starting character
  char cStartChar = *psArg;

  if ( ! (cStartChar >= 'A' && cStartChar <= 'Z') &&
       ! (cStartChar >= 'a' && cStartChar <= 'z') &&
       ! (cStartChar == '_'))
    return false;

  // check for special characters in table name
  for (char c = 33; c <= 47 ; c++)
    if (bContains(psArg, c))
      return false;

  for (char c = 58; c <= 64 ; c++)
    if (bContains(psArg, c))
      return false;

  if (bContains(psArg, '|') ||
      bContains(psArg, '^') ||
      bContains(psArg, '~'))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsAsterisk: is char an asterisk?                                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAsterisk(const char c)
{
  _static_
  
  if (c == '*')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAt: is char an at?                                                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAt(const char c)
{
  _static_
  
  if (c == '@')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsBackQuote: is char an back quote?                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsBackQuote(const char c)
{
  _static_
  
  if (c == '`')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsBackSlash: is char an back slash?                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsBackSlash(const char c)
{
  _static_
  
  if (c == '\\')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsBlank: is char a blank?                                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsBlank(const char c)
{
  _static_
  
  if (c == ' ')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsBoolean: is the string a boolean sequence ?                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsBoolean(const char* psArg)
{
  _static_
  
  if (StrLib::bEqualTo(psArg, "true" , C_NO_CASE)) return true;
  if (StrLib::bEqualTo(psArg, "false", C_NO_CASE)) return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* bIsCircumflex: is char a circumflex?                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsCircumflex(const char c)
{
  _static_
  
  if (c == '^')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsCloseBrace: is char a close brace?                                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsCloseBrace(const char c)
{
  _static_
  
  if (c == '}')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsCloseBracket: is char a close bracket?                                  */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsCloseBracket(const char c)
{
  _static_
  
  if (c == ']')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsCloseParenthesis: is char a close parenthesis?                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsCloseParenthesis(const char c)
{
  _static_
  
  if (c == ')')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsColon: is char a colon?                                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsColon(const char c)
{
  _static_
  
  if (c == ':')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsCommentBlock: is arg a block comment?                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsCommentBlock(const char* psArg)
{
  _static_
  
  if (! bIsSlash(*psArg))
    return false;

  if (! bIsAsterisk(*(psArg + 1)))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsCommentLine: is arg a line comment?                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsCommentLine(const char* psArg)
{
  _static_
  
  if (! bIsSlash(*psArg))
    return false;

  if (! bIsSlash(*(psArg + 1)))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsDecimal: is string a decimal no ?                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsDecimal(const char* psArg)
{
  _static_

  bool bDecimalPoint = false;
  bool bGotDigit = false;

  if ('-' == *psArg || '+' == *psArg)
    (psArg)++;

  for (; *psArg; (psArg)++) {
    if (*psArg == '.') {

      // have we seen a decimal point before ?
      if (bDecimalPoint)
        return false;

      bDecimalPoint = true;
      continue;
    }

    if (!isdigit(*psArg))
      return false;

    bGotDigit = true;
  }

  if ( ! bDecimalPoint || ! bGotDigit)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsDigit: is char a numeric decimal digit?                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsDigit(const char c)
{
  _static_
  
  if (isdigit(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsDollar: is char a dollar sign?                                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsDollar(const char c)
{
  _static_
  
  if (c == '$')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsDot: is char a dot?                                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsDot(const char c)
{
  _static_
  
  if ('.' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsDoubleQuote: is char a double quote?                                    */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsDoubleQuote(const char c)
{
  _static_
  
  if (c == '"')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsEmpty: does the string have zero content?                               */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsEmpty(const char* psArg)
{
  _static_

  return (*psArg == 0);
}

/*----------------------------------------------------------------------------*/
/* bSameAs: is char an equal?                                                */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsEqual(const char c)
{
  _static_
  
  if (c == '=')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsEscapable: test if char needs to be escaped                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsEscapable(const char cArg)
{
  _static_
  
  if ('\0' == cArg) return true;
  if ('\b' == cArg) return true;
  if ('\f' == cArg) return true;
  if ('\n' == cArg) return true;
  if ('\r' == cArg) return true;
  if ('\t' == cArg) return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bIsExclamation: is char an exclamation?                                    */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsExclamation(const char c)
{
  _static_
  
  if (c == '!')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsFloat: is string a floating point no ? Must end in an exponent          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsFloat(const char* psArg) {
  _static_

  bool bDecimalPoint = false;
  bool bExponent = false;

  if ('-' == *psArg || '+' == *psArg)
    (psArg)++;

  for (; *psArg; (psArg)++) {
    if (*psArg == '.') {

      // have we seen a decimal point before ?
      if (bDecimalPoint)
        return false;

      bDecimalPoint = true;
      continue;
    }

    if (*psArg == 'e' || *psArg == 'E') {
      // have we seen an exponent before?
      if (bExponent) {
        return false;
      }

      bExponent = true;

      (psArg)++;
      if (!*psArg) {
        bExponent = false;
        break;
      }

      if ('-' == *psArg || '+' == *psArg)
        (psArg)++;
      else
        (psArg)--;

      continue;
    }

    if (!isdigit(*psArg))
      return false;
  }

  if ( ! bExponent )
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsGreaterThan: is char a greater than?                                    */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsGreaterThan(const char c)
{
  _static_
  
  if (c == '>')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsHash: is char a hash?                                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsHash(const char c)
{
  _static_
  
  if (c == '#')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsHex: is char a hexadecimal digit?                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsHex(const char c)
{
  _static_
  
  if (c >= 'a' && c <= 'f') return true;
  if (c >= 'A' && c <= 'F') return true;
  if (c >= '0' && c <= '9') return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bIsInteger: is the string an integer digit sequence ?                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsInteger(const char* psArg)
{
  _static_
  
  if ('-' == *psArg)
    (psArg)++;

  if ( ! *psArg )
    return false;

  for ( ; *psArg; (psArg)++)
    if (!isdigit(*psArg))
      return false;
  
  return true;
}
/*----------------------------------------------------------------------------*/
/* bIsIPAddr: is the string a valid IP address ?                              */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsIPAddr(const char* psArg)
{
  _static_

  i8 i = 0;
  i8 iDotCount = 0;
  i8 iOctetCount = 0;
  bool bOctetFlag = false;

  while(*psArg && bIsWhite(*psArg))
  {
    psArg++;
    continue;
  }

  while(*psArg)
  {
    i = 0;
    while (*psArg && bIsDigit(*psArg)) {
      i8 j = (*psArg++ - '0');
      i = i * 10 + j;
      bOctetFlag = true;
    }

    if(i < 0 || i > 255)
      return false;

    if(bOctetFlag) {
      iOctetCount++;
      bOctetFlag = false;
    }
    else
      return false;

    if(iOctetCount > 4)
      return false;

    if (*psArg && '.' != *psArg) {
      return false;
    }
    else if (*psArg && '.' == *psArg) {
      psArg++;
      iDotCount++;
      if(iDotCount > 3)
        return false;
    }
  }

  if(iDotCount != 3 || iOctetCount != 4)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* bIsLessThan: is char a less than?                                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsLessThan(const char c)
{
  _static_
  
  if (c == '<')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsLowerCase: is char a lower case char?                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsLowerCase(const char c)
{
  _static_

  if (c >= 'a' and c <= 'z')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsMinus: is char a minus sign?                                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsMinus(const char c)
{
  _static_
  
  if (c == '-')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is char an alphabetic, numeric digit, '_' or '$'?                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsNameBodyChar(const char c)
{
  _static_
  
  if (isalpha(c) || isdigit(c) || '_' == c || '$' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAlphaUnder: is char an alphabetic, '_' or '$'?                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsNameStartChar(const char c)
{
  _static_
  
  if (isalpha(c) || '_' == c || '$' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsNewLine: is char a new line?                                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsNewLine(const char c)
{
  _static_
  
  if (c == '\n')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsNull: is char a null?                                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsNull(const char c)
{
  _static_
  
  if (c == '\0')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsCloseBrace: is char an open brace?                                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsOpenBrace(const char c)
{
  _static_
  
  if (c == '{')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsCloseBracket: is char an open bracket?                                  */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsOpenBracket(const char c)
{
  _static_
  
  if (c == '[')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsCloseParenthesis: is char an open parenthesis?                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsOpenParenthesis(const char c)
{
  _static_
  
  if (c == '(')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsPercent: is char a percent?                                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsPercent(const char c)
{
  _static_
  
  if ('%' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsPipe: is char a pipe?                                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsPipe(const char c)
{
  _static_
  
  if ('|' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsMinus: is char a plus sign?                                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsPlus(const char c)
{
  _static_
  
  if (c == '+')
    return true;
  else
    return false;
}


/*----------------------------------------------------------------------------*/
/* bIsQuestion: is char a question?                                           */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsQuestion(const char c)
{
  _static_
  
  if ('?' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsSemicolon: is char a semicolon?                                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsSemicolon(const char c)
{
  _static_
  
  if (';' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsSign: is char a plus or minus?                                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsSign(const char c)
{
  _static_
  
  if (bIsPlus(c) || bIsMinus(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsSlash: is char a slash?                                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsSlash(const char c)
{
  _static_
  
  if ('/' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is string all whitespace                                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsWhiteSpaces(const char* psArg)
{
  _static_

  for (const char* azCrs = psArg; *azCrs; azCrs++)
    if (! bIsWhite(*azCrs))
      return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsSignleQuote: is char a single quote?                                    */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsSingleQuote(const char c)
{
  _static_
  
  if (c == '\'')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is argument a valid sql name?                                              */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsSqlName(char* ps)
{
  _static_

  char* az = ps;

  // must start with a alpha or underscore
  char c = *az++;
  if (! (isalpha(c) || '_' == c))
    return false;

  // followed by zero or more: alpha, digit or underscore
  while ((c = *az++)) {
    if (! (bIsAlphaDigit(c) || '_' == c))
      return false;
  }

  return true;
}


/*----------------------------------------------------------------------------*/
/* bIsTilde: is char a tilde?                                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsTilde(const char c)
{
  _static_
  
  if ('~' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsTypical: is char a typical char?                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsTypical(const char c)
{
  _static_

  if ('\n' == c)
    return true;

  if (c >= ' ' && c <= '~')
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* bIsTypical: is char a typical string?                                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsTypical(const char* psArg)
{
  _static_

  for (const char* azTest = psArg; *azTest; azTest++)
    if (! StrLib::bIsTypical(*azTest))
      return false;


  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsUnderscore: is char an underscore?                                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsUnderscore(const char c)
{
  _static_
  
  if ('_' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsUpperCase: is char an upper case char?                                  */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsUpperCase(const char c)
{
  _static_

  if (c >= 'A' and c <= 'Z')
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsWhite: is char white space?                                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsWhite(const char c)
{
  _static_
  
  if (c == ' ')  return true;
  if (c == '\n') return true;
  if (c == '\r') return true;
  if (c == '\f') return true;
  if (c == '\t') return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for an 'x' or 'X'                                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsX(const char c)
{
  if ('x' == c || 'X' == c)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* sLast: return address of last char in arg                                  */
/*----------------------------------------------------------------------------*/
char* StrLib::acLast(const char* psSrc)
{
  _static_
  
  i8 nLen  = nLength(psSrc);
  
  const char* z = psSrc + nLen - 1;
  
  return (char*)z;
}

/*----------------------------------------------------------------------------*/
/* iLength: get length in bytes of psSrc (not including terminating null)     */
/*----------------------------------------------------------------------------*/
i8 StrLib::nLength(const char* psSrc, const char cTerm, i8 iMax)
{
  _static_
  
  if (! *psSrc)
    return 0;
  
  i8 iLen = 1;
  
  for (const char* p = psSrc; iLen < iMax; iLen++)
    if (! *p || *p++ == cTerm)
      break;
  
  return iLen;
}

/*----------------------------------------------------------------------------*/
/* iLength: get length in bytes of psSrc (not including terminating null)     */
/*----------------------------------------------------------------------------*/
i8 StrLib::nLengthNonBlank(const char* psSrc, const char cTerm, i8 iMax)
{
  _static_
  
  if (! *psSrc)
    return 0;
  
  i8 iLen = 1;
  
  const char* acNonBlank = psSrc;
  
  for (const char* p = psSrc; iLen < iMax; iLen++) {
    if (*p && ' ' != *p)
      acNonBlank = p;
    
    if (! *p || *p++ == cTerm)
      break;
  }
  
  i8 iLenNonBlank = (acNonBlank == psSrc) ? 0 : acNonBlank - psSrc + 1;
  
  return iLenNonBlank;
}

/*----------------------------------------------------------------------------*/
/* like: search for regexp anywhere in given string                           */
/*----------------------------------------------------------------------------*/
i8 StrLib::iLike2(const char* psSrc, const char* psRegExp, bool* pbResult)
{
  /*

    Current set of metacharacters:

      c  matches any literal character c
      _  matches any single character
      %  matches zero or more occurrences of any character

   */

  s256 sEscapedRegExp = psRegExp;
  StrLib::vInsertChar(sEscapedRegExp.azChar(), sEscapedRegExp.nAreaBytes(), '^', pbResult);
  StrLib::vEscapeInsert(sEscapedRegExp.azChar(), sEscapedRegExp.nAreaBytes(), '*', '\\', pbResult);

  s256 sMatchRegExp;
  char *r = sEscapedRegExp.azChar();
  char *R = sMatchRegExp.azChar();

  // handle empty string and empty regular expression
  if (*psSrc == '\0' && *r == '\0') {
    return true;
  }

  // handle empty regular expression
  if (*r == '\0') {
    return false;
  }

  i8 iReqBytes = 0;
  i8 iDstBufSize = sMatchRegExp.nAreaBytes();

  // iterate the regular expression
  // Need to be careful - because '%' is translated to '.*' the output
  // buffer may get overwritten or truncated.
  while (*r) {

    if (*r == '%') {
      // replace '%' with '.*'
      iReqBytes+=2;

      if (iReqBytes < iDstBufSize - 1) {
        *R++ = '.';
        *R++ = '*';
      }
    }
    else if (*r == '_') {
      // replace '_' with '.'
      ++iReqBytes;
      if (iReqBytes < iDstBufSize) {
        *R++ = '.';
      }
    }
    else {
      ++iReqBytes;
      if (iReqBytes < iDstBufSize) {
        *R++ = *r;
      }
    }

    r++;
  }

  // null terminate the new regular expression
  ++iReqBytes;
  *R++ = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);

  bool bDidMatch = bMatch(psSrc, sMatchRegExp.azChar());

  return bDidMatch;
}


/*----------------------------------------------------------------------------*/
/* like: search for regexp anywhere in given string (with escape character)   */
/*----------------------------------------------------------------------------*/
i8 StrLib::iLike3(const char* psSrc, const char* psRegExp, char cEsc, bool* pbResult)
{
  /*

    Current set of metacharacters:

      c  matches any literal character c
      _  matches any single character
      %  matches zero or more occurrences of any character

   */

  s256 sMatchRegExp;

  const char *r = psRegExp;
  char *R = sMatchRegExp.azChar();

  // handle empty string and empty regular expression
  if (*psSrc == '\0' && *r == '\0') {
    return true;
  }

  // handle empty regular expression
  if (*r == '\0') {
    return false;
  }

  i8 iReqBytes = 0;
  i8 iDstBufSize = sMatchRegExp.nAreaBytes();

  // iterate the regular expression
  while (*r) {

    // escape character
    if (*r == cEsc) {
      r++;
      ++iReqBytes;
      if (iReqBytes < iDstBufSize) {
        *R++ = *r;
      }
    }

    // replace '%' with '.*'
    else if (*r == '%') {
      iReqBytes+=2;

      if (iReqBytes < iDstBufSize - 1) {
        *R++ = '.';
        *R++ = '*';
      }
    }

    // replace '_' with '.'
    else if (*r == '_') {
      ++iReqBytes;
      if (iReqBytes < iDstBufSize) {
        *R++ = '.';
      }
    }
    else {
      ++iReqBytes;
      if (iReqBytes < iDstBufSize) {
        *R++ = *r;
      }
    }

    r++;
  }

  // null terminate the new regular expression
  ++iReqBytes;
  *R++ = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);

  return bMatch(psSrc, sMatchRegExp.azChar());
}

/*----------------------------------------------------------------------------*/
/* match: search for regexp anywhere in given string                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bMatch(const char* psSrc, const char* psRegExp)
{
  /*
  
    Current set of metacharacters:
   
      c  matches any literal character c
      .  matches any single character
      ^  matches the beginning of the input string
      $  matches the end of the input string
      *  matches zero or more occurrences of the previous character

   */
  
  if (psRegExp[0] == '^')
    return bMatchHere(psSrc, psRegExp + 1);
  
  do { /* must look even if string is empty */
    
    if (bMatchHere(psSrc, psRegExp))
      return true;
    
  } while (*psSrc++ != '\0');
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bMatchHere: search for regexp at beginning of string                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bMatchHere(const char* azStr, const char* azRegExp) {
  //vTrace("bMatchHere: %s : %s\n"_Stz,azStr,azRegExp);

  if (azRegExp[0] == '\0' && *azStr == '\0')
    return true;

  if (azRegExp[0] == '\\') {
    return bMatchHere(azStr,azRegExp+1);
  }

  if (azRegExp[1] == '*')
    return bMatchStar(azStr,azRegExp+2,azRegExp[0]);
  
  if (azRegExp[0] == '$' && azRegExp[1] == '\0')
    return (*azStr == '\0');
  
  if (*azStr != '\0' && (azRegExp[0] == '.' || azRegExp[0] == *azStr))
    return bMatchHere(azStr+1,azRegExp+1);

  return false;
}

/*----------------------------------------------------------------------------*/
/* bMatchStar: search for c*regexp at beginning of string                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bMatchStar(const char* azStr, const char* azRegExp, char cChr)
{
  do { /* a * matches zero or more instances */
    
    if (bMatchHere(azStr,azRegExp))
      return true;
    
  } while (*azStr != '\0' && (*azStr++ == cChr || cChr == '.'));
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get ptr to null terminator                                                 */
/*----------------------------------------------------------------------------*/
char* StrLib::azNullTerminator(char* psSrc)
{
  _static_
  
  i8 nLen  = nLength(psSrc);
  
  char* z = psSrc + nLen;
  
  return z;
}

/*----------------------------------------------------------------------------*/
/* iOccurrence: get no of occurrence of given char                            */
/*----------------------------------------------------------------------------*/
i8 StrLib::iOccurrence(char* psSrc, const char cArg)
{
  _static_
  
  i8 iCount = 0;
  
  while(*psSrc) {
    psSrc = strchr(psSrc, cArg);
    if ( ! psSrc )
      break;
    iCount++;
    psSrc++;
  }
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* bOneOf: is given word one of the words in the given set?                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bOneOf(char** ppsSrcSet, const char* psArg)
{
  _static_
  
  for (char** saTest = ppsSrcSet; *saTest; saTest++) {
    if (bSame(*saTest, psArg))
      return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* vPad: pad string to given length with char                                 */
/*----------------------------------------------------------------------------*/
void StrLib::vPad(char* psDst, i8 iDstBufSize, i8 iToLength, const char cWith, bool* pbResult)
{
  _static_
  
  i8 iCurrLength = nLength(psDst);
  
  if (iCurrLength >= iToLength)
    return;

  bool bRet = bCheckBufferSpace(__func__, iToLength + 1, iDstBufSize, pbResult);
  UNUSED(bRet);
  // Continue, even if there is truncation

  i8 iMaxChars = iToLength > iDstBufSize - 1 ? iDstBufSize - 1 : iToLength;

  char* z = azNullTerminator(psDst);
  i8 nChars = iMaxChars - iCurrLength;
  
  for (i8 iChar = 0; iChar < nChars; iChar++)
    *z++ = cWith;
  
  *z = '\0';
}

/*----------------------------------------------------------------------------*/
/* put given cChr into dst at offset                                          */
/*----------------------------------------------------------------------------*/
void StrLib::vPut(char* psDst, i8 iDstBufSize, i8 iAt, char cChr, bool* pbResult)
{
  _static_

  bool bRet = bCheckBufferSpace(__func__, iAt + 1, iDstBufSize, pbResult);
  UNUSED(bRet);

  ASSERT(iAt < nLength(psDst));

  char* acDst = psDst + iAt;
  
  if (*acDst)
    *acDst = cChr;
}

/*----------------------------------------------------------------------------*/
/* put given stz onto dst at offset                                           */
/*----------------------------------------------------------------------------*/
void StrLib::vPut(char* psDst, i8 iDstBufSize, i8 iAt, char* psSrc, bool* pbResult)
{
  _static_

  i8 iSrcLen = nLength(psSrc);
  bool bRet = bCheckBufferSpace(__func__, iAt + iSrcLen + 1, iDstBufSize, pbResult);
  UNUSED(bRet);

  char* psNull = azNullTerminator(psDst);
  ASSERT(psDst + iAt <= psNull);

  i8 iMaxBytes = iDstBufSize - iAt - 1;
  if (iMaxBytes > iSrcLen) {
    iMaxBytes = iSrcLen;
  }

  char* d = psDst + iAt;
  char* s = psSrc;

  while (*s && *d)
    *d++ = *s++;

  // Does it need to be null terminated?
  // This would occur if the psSrc string overwrote the existing null terminator.
  if (/* psDst + iAt <= psNull && */
      psDst + iAt + iSrcLen > psNull) {
    *d = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* R                                  R                                     R */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove any leading occurrences of a given character                        */
/*----------------------------------------------------------------------------*/
void StrLib::vRemove(char* psDst, i8 iDstBufSize, const char cLeading, bool* pbResult)
{
  _static_

  // This will never overflow, but keeping the parameter interface consistent
  // with other methods.
  UNUSED(iDstBufSize);

  if (pbResult) {
    *pbResult = true;
  }
  
  if (*psDst != cLeading || cLeading == '\0')
    return;
  
  char* pTo = psDst;
  char* pFrom = psDst;
  while (*pFrom == cLeading)
    pFrom++;
  
  for (;;) {
    char cCopied = (*pTo++ = *pFrom++);
    if ( ! cCopied )
      return;
  }
}

/*----------------------------------------------------------------------------*/
/* remove any occurrence of given character                                   */
/*----------------------------------------------------------------------------*/
void StrLib::vRemoveChar(char* psDst, i8 iDstBufSize, char* psSrc, const char cChr, bool* pbResult)
{
  _static_
  
  char* azTo = psDst;
  char* azFrom = psSrc;
  i8 iReqBytes = 0;
  
  while (*azFrom) {
    if (cChr == *azFrom) {
      azFrom++;

    } else {
      ++iReqBytes;
      if (iReqBytes < iDstBufSize) {
        *azTo++ = *azFrom;
      }
      azFrom++;
    }
  }

  ++iReqBytes;
  *azTo = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
}
  
/*----------------------------------------------------------------------------*/
/* remove all characters from the start of the string up to & including cSep  */
/*----------------------------------------------------------------------------*/
void StrLib::vRemovePrefix(char* psDst, i8 iDstBufSize, char cSep, bool* pbResult)
{
  _static_

  // This will never overflow, but keeping the parameter interface consistent
  // with other methods.
  UNUSED(iDstBufSize);
  if (pbResult) {
    *pbResult = true;
  }

  i8 i_Sep = iFind(psDst, cSep);
  if (C_UNKNOWN == i_Sep)
    return;

  vShuffleLeft(psDst, i_Sep + 1);
}

/*----------------------------------------------------------------------------*/
/* remove suffix from arg (where suffix is introduced by given char)          */
/*----------------------------------------------------------------------------*/
void StrLib::vRemoveSuffix(char* psDst, i8 iDstBufSize, char cSep, bool* pbResult)
{
  _static_

  // This will never overflow, but keeping the parameter interface consistent
  // with other methods.
  UNUSED(iDstBufSize);
  if (pbResult) {
    *pbResult = true;
  }
  
  char* psSuffix = psDst + nLength(psDst) - 1;
  
  // find the byte where the suffix is introduced
  if (! bBackward(&psSuffix, psDst, cSep))
    return;
  
  // terminate azArg at the suffix introduction
  *psSuffix = '\0';
}

/*----------------------------------------------------------------------------*/
/* remove tail by nChars                                                      */
/*----------------------------------------------------------------------------*/
void StrLib::vRemoveTailBy(char* psDst, i8 iDstBufSize, i8 nChars, bool* pbResult)
{
  _static_
  UNUSED(iDstBufSize);

  // Check for string underflow
  i8 iDstSize = nLength(psDst) + 1;

  bool bRet = bCheckBufferSpace(__func__, nChars, iDstSize, pbResult);
  if (!bRet) {
    // We cannot continue otherwise we will underflow the string.
    return;
  }

  char* z = azNullTerminator(psDst);
  char* y = z - nChars;

  *y = '\0';
}


/*----------------------------------------------------------------------------*/
/* remove integer digits from the tail of psDst                               */
/*----------------------------------------------------------------------------*/
void StrLib::vRemoveTailInt(char* psDst, i8 iDstBufSize, bool* pbResult)
{
  _static_

  // This will never overflow, but keeping the parameter interface consistent
  // with other methods.
  UNUSED(iDstBufSize);
  if (pbResult) {
    *pbResult = true;
  }
  
  char* azTail = psDst + nLength(psDst) - 1;
  
  while (azTail >= psDst && StrLib::bIsDigit(*azTail))
    *azTail-- = '\0';
}

/*----------------------------------------------------------------------------*/
/* remove till given char                                                     */
/*----------------------------------------------------------------------------*/
void StrLib::vRemoveTill(char* psDst, i8 iDstBufSize, const char cTill,
                         bool bRemoveTheTill, bool* pbResult)
{
  _static_

  UNUSED(iDstBufSize);

  char* azSrc = psDst;
  while (*azSrc && *azSrc != cTill)
    azSrc++;

  if (! *azSrc) {
    if (pbResult) {
      *pbResult = true;
    }
    return;
  }

  if (bRemoveTheTill)
    azSrc++;

  vShuffleLeft(psDst, azSrc);
}

/*----------------------------------------------------------------------------*/
/* remove leading token                                                       */
/*----------------------------------------------------------------------------*/
void StrLib::vRemoveToken(char* psDst, i8 iDstBufSize, bool* pbResult)
{
  _static_

  UNUSED(iDstBufSize);

  char* azRight = psDst;

  // skip leading whitespace
  while (*azRight) {
    if (' ' == *azRight || '\n' == *azRight) {
      azRight++;
      continue;
    }
    break;
  }

  // something still to process?
  if (! *azRight) {
    if (pbResult) {
      *pbResult = true;
    }
    return;
  }

  // skip token
  while (*azRight) {
    if (' ' == *azRight || '\n' == *azRight)
      break;
    azRight++;
  }

  // skip whitespace
  while (*azRight) {
    if (' ' == *azRight || '\n' == *azRight) {
      azRight++;
      continue;
    }
    break;
  }

  // something still to process?
  if (! *azRight) {
    if (pbResult) {
      *pbResult = true;
    }
    return;
  }

  vShuffleLeft(psDst, azRight);
}

/*----------------------------------------------------------------------------*/
/* replace first occurrence of psThis with psThat in pDst                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bReplace(char *psDst, i8 iDstBufSize, const char *psThis,
                      const char *psThat, bool* pbResult)
{
  _static_
  
  //note: this function does the replacement within the given string
  //      therefore it must be large enough to handle expansion
  
  // is this in tbe string?
  char* pSubstr = psDst;
  if ( ! bFind(&pSubstr, psThis) ) {
    if (pbResult) {
      *pbResult = true;
    }
    return false;
  }
  
  // compute lengths and difference
  i8 iLenDst  = nLength(psDst);
  i8 iLenThis = nLength(psThis);
  i8 iLenThat = nLength(psThat);
  i8 iLenDiff = abs((int)iLenThis - (int)iLenThat);
  
  // which of 3 variants is it?
  if (iLenThis == iLenThat)
  {
    // (1) replacing same length. No overflow can occur
    if (pbResult) {
      *pbResult = true;
    }
    
    // simply copy pThat into string at that pSubstr
    while (*psThat)
      *pSubstr++ = *psThat++;
  }
  else
  if (iLenThis < iLenThat)
  {
    // (2) replacing with longer

    // Check for buffer overflow/truncation
    i8 iReqBytes = iLenDst + 1 + iLenDiff;
    bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);

    if (!bRet) {
      return false;
    }

    // shuffle to the right to make space
    char* pFrom  = pSubstr + nLength(pSubstr) ;
    char* pTo    = pFrom   + iLenDiff;
    char* pLimit = pSubstr + iLenThis;
    while (pFrom >= pLimit)
      *pTo-- = *pFrom--;

    // copy pThat into string at pSubstr
    while (*psThat)
      *pSubstr++ = *psThat++;
  }
  else
  {
    // (3) replacing with shorter. No overflow can occur.
    if (pbResult) {
      *pbResult = true;
    }
    
    // shuffle to the left to close space
    char* pFrom = pSubstr + iLenThis;
    char* pTo   = pFrom - iLenDiff;
    while (*pFrom)
      *pTo++ = *pFrom++;
    *pTo = '\0';
    
    // copy this into string at that substr
    while (*psThat)
      *pSubstr++ = *psThat++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* replace all occurrences in psSrc of psThis with psThat and write to psDst  */
/*----------------------------------------------------------------------------*/
bool StrLib::bReplace(char *pDst, i8 iDstBufSize, const char* psSrc,
                      const char *psThis, const char *psThat, bool* pbResult)
{
  _static_
  
  bool bReplaced = false;
  i8 iReqBytes = 0;
  
NextIteration:
  
  // scan source string looking for matches
  while (*psSrc) {
  
    // non-matching case
    if (*psSrc != *psThis) {
      // just copy one src char to dst
      if (++iReqBytes < iDstBufSize) {
        *pDst++ = *psSrc;
      }
      psSrc++;
      continue;
    }
    
    // mark start of possible match & replace
    const char* pMatch   = psSrc;
    const char* pReplace = psThis;
    
    // attempt to collect matching chars from src
    while (*pReplace) {
      
      // false match?
      if (*psSrc != *pReplace) {
        // false match, therefore copy the unmatching chars
        do {
          if (++iReqBytes < iDstBufSize) {
            *pDst++ = *pMatch;
          }
          pMatch++;
        } while (pMatch < psSrc);
        goto NextIteration;
      }
      
      // matching so far...
      psSrc++;
      pReplace++;
    }
    
    // copy the replacement substring
    for (const char* pCopy = psThat; *pCopy; ) {
      if (++iReqBytes < iDstBufSize) {
        *pDst++ = *pCopy;
      }
      pCopy++;
    }
    
    // a replacement has been made
    bReplaced = true;
  }

  ++iReqBytes;
  *pDst = '\0';

  bool bRet = bCheckBufferSpace(__func__, iReqBytes, iDstBufSize, pbResult);
  UNUSED(bRet);
  
  return bReplaced;
}

/*----------------------------------------------------------------------------*/
/* replace tail of string, keeping string length the same                     */
/*----------------------------------------------------------------------------*/
void StrLib::vReplaceTail(char* azDst, const char* azTail)
{
  _static_
  
  i8 iLen_dst  = StrLib::nLength(azDst);
  i8 iLen_tail = StrLib::nLength(azTail);
  
  //            01234567
  //            abcdefgh
  //                 ...
  //            8 - 3 = 5
  StrLib::vCopy(azDst + iLen_dst - iLen_tail, iLen_dst + 1, azTail);
}

/*----------------------------------------------------------------------------*/
/* Record internally that a string truncation has occurred                    */
/*----------------------------------------------------------------------------*/
inline void StrLib::vReportStringTruncation(const char* azFuncName, i8 iReqBufSize, i8 iDstBufSize)
{
  vTrace("Warning: Unhandled string truncation occurred in function %s.\n"
         "Destination buffer required %lld bytes, only had %lld bytes.\n"
         "Call stack follows.\n"_Stz,
         azFuncName,
         iReqBufSize,
         iDstBufSize);
  vThreadCallStackDump();
}

/*----------------------------------------------------------------------------*/
/* reverse a string                                                           */
/*----------------------------------------------------------------------------*/
void StrLib::vReverse(char* psDst, i8 iDstBufSize, char* psSrc, bool* pbResult)
{
  _static_

  i8 iSrcLen = nLength(psSrc);
  char* acSrc = psSrc + iSrcLen - 1;
  char* acDst = psDst;

  i8 iReqBufSize = iSrcLen + 1;
  bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);

  if (!bRet) {
    if (iDstBufSize <= 0) {
      return;
    }

    // Truncation will occur unless we adjust the acSrc pointer
    acSrc = psSrc + iDstBufSize - 1;
  }

  while (acSrc >= psSrc)
    *acDst++ = *acSrc--;
  
  *acDst = '\0';
}

/*----------------------------------------------------------------------------*/
/* rotate a string by given number of characters                              */
/*----------------------------------------------------------------------------*/
void StrLib::vRotate(char* zArg, i8 nBy)
{
  _static_
  
  i8 nLen = nLength(zArg);
  ASSERT(nBy < nLen);
  
  char caFront[nBy];
  
  MemLib::vCopy(caFront,zArg,nBy,CN_NONE);
  
  MemLib::vMove(zArg,zArg+nBy,nBy,CN_NONE);
  
  MemLib::vCopy(zArg + nLen - nBy,caFront,nBy,CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* ruler                                                                      */
/*----------------------------------------------------------------------------*/
void StrLib::vRuler(char* azDst, i8 iLength)
{
  _static_
  
  char* azPut = azDst;
  char  cDigit;
  
  for (i8 i_Char = 0; i_Char < iLength; i_Char++) {
    cDigit = '0' + (i_Char % 10);
    *azPut++ = cDigit;
  }
  *azPut = '\0';
}


/*----------------------------------------------------------------------------*/
/* S                                  S                                     S */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* are two arguments equal (no case)                                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bSame(const char c1, const char c2)
{
  _static_

  char c1_lower = cToLower(c1);
  char c2_lower = cToLower(c2);

  if (c1_lower == c2_lower)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* are two arguments equal (no case)                                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bSame(const char* pArg1, const char* pArg2)
{
  _static_
  
  i8 iDiff = strcasecmp(pArg1,pArg2);
  
  if (0 == iDiff)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* Given a word, convert it to a short hand notation                          */
/*----------------------------------------------------------------------------*/
void StrLib::vShortHand(char *azDst, const char *azSrc)
{
  _static_

  char cStart = *azSrc;
  if(cStart >= 97 && cStart <= 122)
  {
    // indexJoin -> iJ
    *azDst = cStart;

    azSrc++;

    while(*azSrc != '\0')
    {
      if(*azSrc >= 65 && *azSrc <= 90)
      {
        azDst++;
        *azDst = *azSrc;
        azDst++;
        *azDst = '\0';
        return;
      }
      azSrc++;
    }
  } else if(cStart >= 65 && cStart <= 90)
  {
    // if the first char is caps
    // IndexJoin -> IJ
    // INDEXJOIN -> I
    *azDst = cStart;

    azSrc++;

    // search for the next caps letter
    bool bSmall = 0;
    while(*azSrc >= 97 && *azSrc <= 122 && *azSrc != '\0')
    {
      azSrc++;
      bSmall = 1;
    }

    // Add the next caps char if we found at lease one small char before
    if(*azSrc != '\0' && bSmall == 1)
    {
      azDst++;
      *azDst = *azSrc;
    }
    azDst++;
    *azDst = '\0';
    return;
  } else
  {
    *azDst = '\0';
  }
}

/*----------------------------------------------------------------------------*/
/* shuffle substring left                                                     */
/*----------------------------------------------------------------------------*/
void StrLib::vShuffleLeft(char* azDst, const char* azSrc)
{
  _static_

  for (;;) {
    char cCopied = (*azDst++ = *azSrc++);
    if (! cCopied)
      return;
  }
}

/*----------------------------------------------------------------------------*/
/* shuffle substring left                                                     */
/*----------------------------------------------------------------------------*/
void StrLib::vShuffleLeft(char* azDst, i8 i_Right)
{
  _static_

  vShuffleLeft(azDst, azDst + i_Right);
}

/*----------------------------------------------------------------------------*/
/* do the two arguments have similar leading sequences (no case)              */
/*----------------------------------------------------------------------------*/
bool StrLib::bSimilar(const char* azArg1, const char* azArg2, const char* azSep)
{
  _static_

  const char* azCrs_Arg1 = azArg1;
  const char* azCrs_Arg2 = azArg2;

  while (true) {

    vTrace("%s : %s : %s\n"_Stz,azCrs_Arg1,azCrs_Arg2,azSep);

    if ((! bSame(*azCrs_Arg1,*azCrs_Arg2)) ||
        (! *azCrs_Arg1 && ! *azCrs_Arg2))
      break;

    if (bBeginsWith(azCrs_Arg1,azSep) && bBeginsWith(azCrs_Arg2,azSep))
      return true;

    azCrs_Arg1++;
    azCrs_Arg2++;
  }

  if (! *azCrs_Arg1 && ! *azCrs_Arg2)
    return true;

  if (bBeginsWith(azCrs_Arg1,azSep) || bBeginsWith(azCrs_Arg2,azSep))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of arg                                                   */
/*----------------------------------------------------------------------------*/
i8 StrLib::nSize(const char* s)
{
  _static_
  
  i8 nBytes = strlen(s) + 1;

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* skip line comment                                                          */
/*----------------------------------------------------------------------------*/
void StrLib::vSkipCommentLine(char** aaCrs)
{
  _static_
  
  RASSERT(bIsSlash(**aaCrs));
  (*aaCrs)++;
  RASSERT(bIsSlash(**aaCrs));
  (*aaCrs)++;

  while (! bIsNull(**aaCrs)) {
    if (bIsNewLine(**aaCrs)) {
      (*aaCrs)++;
      break;
    }
    (*aaCrs)++;
  }
}

/*----------------------------------------------------------------------------*/
/* skip white space and comments                                              */
/*----------------------------------------------------------------------------*/
void StrLib::vSkipCommentsEtc(char **aazCrs)
{
  _static_
  
  for (;;) {
    StrLib::vSkipWhite(aazCrs);
    if (! **aazCrs)
      return;

    if (StrLib::bIsCommentLine(*aazCrs)) {
      StrLib::vSkipCommentLine(aazCrs);
      continue;
    }
  
    break;
  }
}

/*----------------------------------------------------------------------------*/
/* skip non digit sequence in substr                                          */
/*----------------------------------------------------------------------------*/
void StrLib::vSkipDigits(char** aaSrc)
{
  _static_
  
  char *aSrc = *aaSrc;
  
  for ( ; *aSrc; aSrc++)
    if ( ! isdigit(*aSrc) )
      break;

  *aaSrc = aSrc;
}

/*----------------------------------------------------------------------------*/
/* skip till end of line                                                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipEOL(char** ps)
{
  _static_
  
  while (**ps && '\n' != **ps)
    (*ps)++;
  (*ps)++;
  
  if (**ps)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* vSkipLeading: skip leading sequence                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipLeading(char** ps, char cEnclosedBy_Leading)
{
  _static_

  // skip leading spaces first
  while (' ' == **ps)
    (*ps)++;

  // do we see the leading enclose character?
  if (cEnclosedBy_Leading == **ps) {
    // skip the leading enclose character
    (*ps)++;
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* skip non-alphanumeric sequence in substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipNonAlnum(char** ps)
{
  _static_
  
  for ( ; **ps; (*ps)++)
    if (isalnum(**ps))
      break;
  
  if (**ps)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip digit sequence in substr                                              */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipNonDigit(char** ps)
{
  _static_
  
  char *s = *ps;
  
  for ( ; *s; s++)
    if (isdigit(*s))
      break;
  
  *ps = s;
  
  if (*s)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip spaces                                                                */
/*----------------------------------------------------------------------------*/
void StrLib::vSkipSpaces(char** ps)
{
  _static_
  
  while (' ' == **ps)
    (*ps)++;
}

/*----------------------------------------------------------------------------*/
/* skip trailing sequence                                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipTrailing(char** ps,  bool bSawEnclosedBy_Leading,
                           char cEnclosedBy_Trailing)
{
  _static_
  
  // skip any blanks
  while (' ' == **ps)
    (*ps)++;
  
  // if no leading enclosed then we have a valid end of the field
  if (! bSawEnclosedBy_Leading)
    return true;
  
  // test we see the trailing enclosed
  if (cEnclosedBy_Trailing == **ps) {
    // skip trailing enclosed
    (*ps)++;
    //skip any blanks
    while (' ' == **ps)
      (*ps)++;
    return true;
  }

  // we expected to see a trailing enclosed but didn't
  return false;
}

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void StrLib::vSkipWhite(char** ps)
{
  _static_
  
  for ( ; **ps; (*ps)++)
    if (!bIsWhite(**ps))
      break;
}

/*----------------------------------------------------------------------------*/
/* split src into string and integer components                               */
/*----------------------------------------------------------------------------*/
void StrLib::vSplitSeries(char* azStr, char* azInt, char* azSrc)
{
  _static_
  
  *azStr = '\0';
  *azInt = '\0';
  
  char* aSrc = azSrc;
  char* aStr = azStr;
  char* aInt = azInt;
  
  while (*aSrc && ! isdigit(*aSrc))
    *aStr++ = *aSrc++;
  *aStr = '\0';
  
  while (*aSrc && isdigit(*aSrc))
    *aInt++ = *aSrc++;
  *aInt = '\0';
}

/*----------------------------------------------------------------------------*/
/* formatted sprint                                                           */
/*----------------------------------------------------------------------------*/
void StrLib::vSprint(char* psDst, i8 iDstBufSize, bool* pbResult, Stz* szFormat, ...)
{
  _static_
  
  va_list args;

  // Note:vsnprintf() returns the number of chars that would have been printed
  // (not including the null terminator). So if this is larger than iDstBufSize
  // then a truncation has occurred.
  va_start(args,szFormat);
  i8 iReqBufSize = 1 + vsnprintf(psDst, iDstBufSize, szFormat->azChar(), args);
  va_end(args);

  bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bRet);
}

/*----------------------------------------------------------------------------*/
/* formatted sprint                                                           */
/*----------------------------------------------------------------------------*/
void StrLib::vSprint(Stz* psDst, i8 iDstBufSize, bool* pbResult, Stz* szFormat, ...)
{
  _static_
  
  va_list args;

  // Note:vsnprintf() returns the number of chars that would have been printed
  // (not including the null terminator). So if this is larger than iDstBufSize
  // then a truncation has occurred.

  va_start(args,szFormat);
  i8 iReqBufSize = 1 + vsnprintf(psDst->azChar(), iDstBufSize, szFormat->azChar(), args);
  va_end(args);

  bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bRet);
}

/*----------------------------------------------------------------------------*/
/* formatted sprint                                                           */
/*----------------------------------------------------------------------------*/
void StrLib::vSprint(char* psDst, i8 iDstBufSize, va_list args, const char* sFormat, bool* pbResult)
{
  _static_

  // Note:vsnprintf() returns the number of chars that would have been printed
  // (not including the null terminator). So if this is larger than iDstBufSize
  // then a truncation has occurred.
  i8 iReqBufSize = 1 + vsnprintf(psDst, iDstBufSize, sFormat, args);

  bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bRet);
}

/*----------------------------------------------------------------------------*/
/* formatted concatenated sprint                                              */
/*----------------------------------------------------------------------------*/
void StrLib::vSprintCat(Stz* psDst, i8 iDstBufSize, bool* pbResult, Stz* sFormat, ...)
{
  _static_
  
  va_list args;

  i8 nStrLen = nLength(psDst->azChar());
  char* sEnd = psDst->azChar() + nStrLen;
  i8 nBufRemaining = iDstBufSize - nStrLen;

  // Note:vsnprintf() returns the number of chars that would have been printed
  // (not including the null terminator). So if this is larger than iDstBufSize
  // then a truncation has occurred.

  va_start(args,sFormat);
  i8 iReqBufSize = nStrLen + 1 + vsnprintf(sEnd, nBufRemaining, sFormat->azChar(), args);
  va_end(args);

  bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bRet);
}

/*----------------------------------------------------------------------------*/
/* formatted concatenated sprint                                              */
/*----------------------------------------------------------------------------*/
void StrLib::vSprintCat(char* psDst, i8 iDstBufSize, va_list args, const char* sFormat, bool* pbResult)
{
  _static_

  // Note:vsnprintf() returns the number of chars that would have been printed
  // (not including the null terminator). So if this is larger than iDstBufSize
  // then a truncation has occurred.

  i8 nStrLen = nLength(psDst);
  char* sEnd = psDst + nStrLen;
  i8 nBufRemaining = iDstBufSize - nStrLen;

  i8 iReqBufSize = nStrLen + 1 + vsnprintf(sEnd, nBufRemaining, sFormat, args);

  bool bRet = bCheckBufferSpace(__func__, iReqBufSize, iDstBufSize, pbResult);
  UNUSED(bRet);
}

/*----------------------------------------------------------------------------*/
/* test last char and remove                                                 */
/*----------------------------------------------------------------------------*/
void StrLib::vStrip(char* s, char cTrailing)
{
  _static_
  
  i8 nBytes = strlen(s);
  
  if ( ! nBytes )
    return;
  
  i8 iOffset = nBytes - 1;
  
  if (s[iOffset] == cTrailing)
    s[iOffset] = '\0';
}

/*----------------------------------------------------------------------------*/
/* get the sub-path of src (the substr after last '/') and copy it to psDst   */
/*----------------------------------------------------------------------------*/
void StrLib::vSubPath(char* azDst, i8 iDstBufSize, char* azSrc, bool* pbResult)
{
  _static_
  
  char* ps;
  
  ps = StrLib::acLast(azSrc);
  
  StrLib::bBackward(&ps,azSrc,'/');
  bool bSlash = StrLib::bBackward(&ps, azSrc, '/');
  
  if (bSlash)
    StrLib::vCopy(azDst, iDstBufSize, ps+1, pbResult);
  else
    StrLib::vCopy(azDst, iDstBufSize, azSrc, pbResult);
}


/*----------------------------------------------------------------------------*/
/* T                                  T                                     T */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bTailIs(const char *z, char *ss)
{
  _static_

  i8 nSS_len  = nLength(ss);

  const char* y = z - nSS_len;

  //vTrace("%s %lld ]%s[\n"_Stz,ss,nSS_len,y);

  if (bEqualTo(y,nSS_len,ss))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* find tab location in given arg                                             */
/*----------------------------------------------------------------------------*/
char* StrLib::aczTabLoc(char* azArg, i8 i_Tab, char cPad)
{
  _static_

  char* aczLoc = acLast(azArg);

  i8 nTab = nDifference(azArg,aczLoc);
  
  while (aczLoc > azArg && nTab > i_Tab) {
    if (cPad != *aczLoc) {
      aczLoc++;
      aczLoc++;
      break;
    }
    aczLoc--;
    nTab--;
  }
  
  return aczLoc;
}

/*----------------------------------------------------------------------------*/
/* convert arg to bcd                                                         */
/*----------------------------------------------------------------------------*/
i8 StrLib::iToBcd(const char* s)
{
  _static_
  
  //i8 i = atoll(s);

  //return i;
  
  i8 i = 0;
  bool bNegative = false;
  
  if (*s && '-' == *s) {
    bNegative = true;
    s++;
  }
  
  // skip leading 0's
  while (*s && '0' == *s)
    s++;
  
  if (*s && ('x' == *s || 'X' == *s)) {
    s++;
    while (*s && bIsHex(*s)) {
      i8 j = iHexToInw(*s++);
      i = i * 16 + j;
    }
  }
  else {
    while (*s && bIsDigit(*s)) {
      i8 j = (*s++ - '0');
      i = i * 10 + j;
    }
    if (*s && '.' == *s) {
      s++;
      while (*s && bIsDigit(*s)) {
        i8 j = (*s++ - '0');
        i = i * 10 + j;
      }
    }
  }
  
  if (bNegative)
    i *= -1;
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert arg to boolean                                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bToBool(const char* s)
{
  _static_
  
  if (StrLib::bEqualTo(s,"true" ,C_NO_CASE)) return true;
  if (StrLib::bEqualTo(s,"1"    ,C_NO_CASE)) return true;
  
  if (StrLib::bEqualTo(s,"false",C_NO_CASE)) return false;
  if (StrLib::bEqualTo(s,"0"    ,C_NO_CASE)) return false;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* convert arg to flt                                                         */
/*----------------------------------------------------------------------------*/
d8 StrLib::dToFlt(const char* s)
{
  _static_
  
  d8 d = atof(s);
  
  return d;
}

/*----------------------------------------------------------------------------*/
/* convert value to hexadecimal character                                     */
/*----------------------------------------------------------------------------*/
char StrLib::cToHex(i8 i)
{
  _static_
  
  char c;
  
  if (i >= 10)
    c = 'a' + i - 10;
  else
    c = '0' + i;
  
  return c;
}

/*----------------------------------------------------------------------------*/
/* convert arg to i8                                                          */
/*----------------------------------------------------------------------------*/
i8 StrLib::iToI8(const char* s)
{
  _static_
  
  //i8 i = atoll(s);

  //return i;
  
  i8 i = 0;
  bool bNegative = false;
  
  if (*s && '-' == *s) {
    bNegative = true;
    s++;
  }
  
  // skip leading 0's
  while (*s && '0' == *s)
    s++;
  
  if (*s && ('x' == *s || 'X' == *s)) {
    s++;
    while (*s && bIsHex(*s)) {
      i8 j = iHexToInw(*s++);
      i = i * 16 + j;
    }
  }
  else {
    while (*s && bIsDigit(*s)) {
      i8 j = (*s++ - '0');
      i = i * 10 + j;
    }
  }
  
  if (bNegative)
    i *= -1;
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert arg to i8 with overflow checking                                   */
/*----------------------------------------------------------------------------*/
i8 StrLib::iToI8(const char* s, bool* bInvalidValue, bool* bOverflowed)
{
  _static_

  i8 i = 0;
  bool bNegative = false;

  *bInvalidValue = false;
  *bOverflowed = false;

  if (!*s) {
    *bInvalidValue = true;
    return -1;
  }

  if (*s && '-' == *s) {
    bNegative = true;
    s++;
  }

  if (!*s || (!bIsDigit(*s) && *s != 'x' && *s != 'X')) {
    *bInvalidValue = true;
    return -1;
  }

  // skip leading 0's
  while (*s && '0' == *s)
    s++;

  if (*s && ('x' == *s || 'X' == *s)) {
    const i8 iLimit = C_I8_MAX / 16;
    const i8 iModulus = bNegative ? -(C_I8_MIN % 16) : C_I8_MAX % 16;

    s++;
    while (*s && bIsHex(*s)) {
      i8 j = iHexToInw(*s++);

      if (i > iLimit || (i == iLimit && j > iModulus) ) {
        *bOverflowed = true;
        return -1;
      }

      i = i * 16 + j;
    }

  } else {
    const i8 iLimit = C_I8_MAX / 10;
    const i8 iModulus = bNegative ? -(C_I8_MIN % 10) : C_I8_MAX % 10;

    while (*s && bIsDigit(*s)) {
      i8 j = (*s++ - '0');

      if (i > iLimit || (i == iLimit && j > iModulus) ) {
        *bOverflowed = true;
        return -1;
      }

      i = i * 10 + j;
    }
  }

  if (bNegative)
    i *= -1;

  return i;
}

/*----------------------------------------------------------------------------*/
/* convert ascii arg to lower case                                            */
/*----------------------------------------------------------------------------*/
char StrLib::cToLower(char c)
{
  _static_

  char c2 = c;
  if (c >= 'A' && c <= 'Z')
    c2 = c + ('a' - 'A');

  return c2;
}

/*----------------------------------------------------------------------------*/
/* convert arg to lower case                                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vToLower(char* psDst, i8 iDstBufSize, bool* pbResult)
{
  _static_

  // Note: While the iDstBufSize and pbResult parameters seem unnecessary,
  // they are needed for when we support non-English characters. In that scenario
  // the uppercase version of a string can be a different size from the
  // original string with a different number of bytes or a different number
  // of characters.
  
  while (*psDst) {
    *psDst = (char)tolower((unsigned char)*psDst);
    psDst++;
  }

  UNUSED(iDstBufSize);

  if (pbResult) {
    *pbResult = true;
  }
}

/*----------------------------------------------------------------------------*/
/* convert ascii arg to upper case                                            */
/*----------------------------------------------------------------------------*/
char StrLib::cToUpper(char c)
{
  _static_

  char c2 = c;
  if (c >= 'a' && c <= 'z')
    c2 = c - ('a' - 'A');

  return c2;
}

/*----------------------------------------------------------------------------*/
/* convert arg to upper case                                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vToUpper(char* psDst, i8 iDstBufSize, bool* pbResult)
{
  _static_

  // Note: While the iDstBufSize and pbResult parameters seem unnecessary,
  // they are needed for when we support non-English characters. In that scenario
  // the uppercase version of a string can be a different size from the
  // original string with a different number of bytes or a different number
  // of characters.

  while (*psDst) {
    *psDst = (char)toupper((unsigned char)*psDst);
    psDst++;
  }

  UNUSED(iDstBufSize);

  if (pbResult) {
    *pbResult = true;
  }
}

/*----------------------------------------------------------------------------*/
/* remove prefix / suffix from arg (where suffix is introduced by given char) */
/*----------------------------------------------------------------------------*/
void StrLib::vTopTail(char *psDst, i8 iDstBufSize, char cIntro, bool* pbResult)
{
  _static_
  
  vRemovePrefix(psDst, iDstBufSize, cIntro, pbResult);
  vRemoveSuffix(psDst, iDstBufSize, cIntro, pbResult);
}

/*----------------------------------------------------------------------------*/
/* convert given integer into its char/str form                               */
/*----------------------------------------------------------------------------*/
void StrLib::vToPointer(char* psDst, i8 iDstBufSize, const void* pvArg, bool* pbResult)
{
  _static_
  ASSERT(iDstBufSize > 1);

  i8 iArg = reinterpret_cast<i8>(pvArg);

  i8 iSpaceNeeded = 3; // start with the null terminator and the "0x"

  if (iSpaceNeeded <= iDstBufSize) {
    *psDst++ = '0';
    *psDst++ = 'x';
  }

  char *p = psDst, *q = psDst;
  do {
    ++iSpaceNeeded;
    if (iSpaceNeeded <= iDstBufSize) {
      *p++ = cToHex(iArg % 16); //(i % 16) + '0';
    }

    iArg /= 16ll;
  } while (iArg);

  if (iSpaceNeeded > iDstBufSize) {
    // Try to put a value into the destination buffer
    if (iDstBufSize >= 4) {
      psDst[0] = '0'; psDst[2] = 'x'; psDst[3] = '0'; psDst[4] = 0;
    } else {
      psDst[0] = 0;
    }

    if (pbResult) {
      *pbResult = false;
    } else {
      vReportStringTruncation(__func__, iSpaceNeeded, iDstBufSize);
    }
    return;
  }

  // We have enough space. Now reverse the string that was generated.

  *p = '\0';
  while (--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }

  if (pbResult) {
    *pbResult = true;
  }
}

/*----------------------------------------------------------------------------*/
/* convert given integer into its char/str form                               */
/*----------------------------------------------------------------------------*/
void StrLib::vToString(char* psDst, i8 iDstBufSize, bool bArg, bool* pbResult)
{
  _static_

  if (bArg)
    vCopy(psDst, iDstBufSize, "true", pbResult);
  else
    vCopy(psDst, iDstBufSize, "false", pbResult);
}

/*----------------------------------------------------------------------------*/
/* convert given integer into its char/str form                               */
/*----------------------------------------------------------------------------*/
void StrLib::vToString(char* psDst, i8 iDstBufSize, i8 iArg, bool* pbResult)
{
  _static_
  ASSERT(iDstBufSize > 1);

  i8 iSpaceNeeded = 1; // start with the null terminator
  bool bNegative = false;

  if (iArg < 0) {
    iArg = -iArg;
    bNegative = true;
    ++iSpaceNeeded; // Account for the negative now
  }

  char *p = psDst, *q = psDst;
  do {
    ++iSpaceNeeded;
    if (iSpaceNeeded <= iDstBufSize) {
      *p++ = (iArg % 10) + '0';
    }

    iArg /= 10ll;
  } while (iArg);

  if (iSpaceNeeded > iDstBufSize) {
    // Try to put a value into the destination buffer
    if (iDstBufSize > 1) {
      psDst[0] = '0';
      psDst[1] = 0;
    } else if (iDstBufSize == 1) {
      psDst[0] = 0;
    }

    if (pbResult) {
      *pbResult = false;
    } else {
      vReportStringTruncation(__func__, iSpaceNeeded, iDstBufSize);
    }
    return;
  }

  // We have enough space. Now reverse the string that was generated.

  if (bNegative)
    *p++ = '-';

  *p = '\0';
  while (--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }

  if (pbResult) {
    *pbResult = true;
  }
}

/*----------------------------------------------------------------------------*/
/* convert given float into its char/str form                                 */
/*----------------------------------------------------------------------------*/
void StrLib::vToString(char* psDst, i8 iDstBufSize, d8 dArg, bool* pbResult)
{
  _static_
  
  char caFloat[32];
  
  i8 iRet = snprintf(caFloat, sizeof(caFloat), "%g", dArg);

  bool bHasTruncated = !bCheckBufferSpace(__func__, iRet+1, sizeof(caFloat), pbResult);

  if (bHasTruncated && pbResult) {
    // Caller will handle it
    return;
  }

  vCopy(psDst, iDstBufSize, caFloat, pbResult);
}

/*----------------------------------------------------------------------------*/
/* trim trailing char's                                                       */
/*----------------------------------------------------------------------------*/
void StrLib::vTrim(char* azArg, char cTrim)
{
  _static_
  
  char* acEnd = StrLib::acLast(azArg);
  
  while (acEnd >= azArg) {
    if (cTrim == *acEnd)
      *acEnd-- = '\0';
    else
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* V                                  V                                     V */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* validate month                                                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bValidateMonth(i8 iDay, i8 iMonth, i8 iYear)
{
  _static_
  
  // assume a leap year
  bool bLeapYear = true;
  if (iYear % 4 == 0) {
    if ((iYear % 100) == 0 && (iYear % 400) != 0) {
      // not a leap year
      bLeapYear = false;
    }
  }

  if (bLeapYear) {
    if (iDay > (i8)iDaysPerMonthLeap[iMonth-1])
      return false;
  }
  else {
    if (iDay > (i8)iDaysPerMonth[iMonth-1])
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* W                                  W                                     W */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* flip the bytes in each word of src string into dst                         */
/*----------------------------------------------------------------------------*/
i8 StrLib::iWordFlip(char* aDst, char* aSrc)
{
  _static_
  
  i8 iLenSrc  = nLength(aSrc);
  //vTrace("Flipping %s\n"_Stz, aSrc);
  
  i8 iRoundUp = 0;
  i8 iRemainder = iLenSrc % C_BYTES_PER_WORD;
  if (iRemainder)
    iRoundUp = C_BYTES_PER_WORD - iRemainder;
  
  i8 iDstSize = iLenSrc + iRoundUp;
  ASSERT(0 == iDstSize % C_BYTES_PER_WORD);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iSrcLen = StrLib::nSize(aSrc);
  MemLib::vZero(aDst,iDstSize,CN_KEY);
  MemLib::vCopy(aDst, aSrc, iSrcLen, CN_KEY);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  w8* aWrd = (w8*)aDst;
  for (i8 nWords = iDstSize / C_BYTES_PER_WORD; nWords; nWords--)
    MemLib::vWordFlip(aWrd++);
  
  if (iLenSrc == iDstSize) {
     MemLib::vZero(aWrd,8,CN_KEY);
    iDstSize += C_BYTES_PER_WORD;
  }
  
  //MemLib::vDumpWords(aDst, iDstSize);
  
  return iDstSize;
}

/*----------------------------------------------------------------------------*/
/* flip the bytes in each word of src string into dst                         */
/*----------------------------------------------------------------------------*/
i8 StrLib::iWordFlipBack(char* aDst, char* aSrc)
{
  _static_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //vTrace("WORDFLIPBACK\n"_Stz);
  //MemLib::vDumpWords(aSrc, 16);
  
  i8 iLenSrc = StrLib::nSize(aSrc);
  
  i8 iRoundUp = 0;
  i8 iRemainder = iLenSrc % C_BYTES_PER_WORD;
  if (iRemainder)
    iRoundUp = C_BYTES_PER_WORD - iRemainder;
  
  i8 iDstSize = iLenSrc + iRoundUp;
  ASSERT(0 == iDstSize % C_BYTES_PER_WORD);
  
  MemLib::vZero(aDst,iDstSize,CN_KEY);
  MemLib::vCopy(aDst, aSrc, iDstSize, CN_KEY);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  w8* aWrd = (w8*)aDst;
  for (i8 nWords = iDstSize / C_BYTES_PER_WORD; nWords; nWords--)
    MemLib::vWordFlip(aWrd++);
  
  
  //vTrace("\nAFTER BACK : %s\n"_Stz, aDst);
  //MemLib::vDumpWords(aSrc, iDstSize);
  
  return iDstSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
