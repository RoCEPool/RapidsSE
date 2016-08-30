/*============================================================================*/
/* File.cpp -- os file                                                        */
/*============================================================================*/

#include "File.hpp"

// Opens for reading. If the file does not exist or cannot be found, the
// fopen call fails.
const char* C_OPEN_READ = "r";

// Opens an empty file for writing. If the given file exists, its contents
// are destroyed.
const char* C_OPEN_WRITE = "w";

// Opens for writing at the end of the file (appending) without removing
// the end-of-file (EOF) marker before new data is written to the file.
// Creates the file if it does not exist.
const char* C_OPEN_APPEND = "a";

// Opens for both reading and writing. The file must exist.
const char* C_OPEN_READ_WRITE_EXIST = "r+";

// Opens an empty file for both reading and writing. If the file exists, its
// contents are destroyed.
const char* C_OPEN_READ_WRITE_CREATE = "w+";

// Opens for reading and appending. The appending operation includes the
// removal of the EOF marker before new data is written to the file. The EOF
// marker is not restored after writing is complete. Creates the file if it
// does not exist.
const char* C_OPEN_READ_APPEND = "a+";

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* File::operator new(size_t, void* pFileAddress)
{
  return pFileAddress;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void File::operator delete(void* pFileAddress)
{
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
File::File(void)
: cn(CN_FILE)
{
  vInit();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
File::~File(void)
{
  if (m_pFilePtr) {
    vWarn("File::~File file not closed: %s %s\n",m_sFilePath.a(),m_sOpenMode.a());

    vClose();
    
    if (m_xSysCall.bHadError())
      vWarn("File::~File: file could not be closed: %s",m_sFilePath.a());
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vClear(void)
{
  m_xSysCall.vClear();
    
  m_sFilePath   = "";
  m_sOpenMode   = "";
  m_iTraceLevel = 0;
  m_pFilePtr    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void File::vClose(void)
{
  m_xSysCall.vClear();

  // never close stdin
  ASSERT(! bIsStdin() );
  
  if (m_iTraceLevel)
    vTrace("File::vClose: closing file %s mode %s",m_sFilePath.a(),m_sOpenMode.a());
  
  if ( ! m_pFilePtr ) {
    m_xSysCall.vFailure(0);
    vWarn("File::vClose: file not open: %s",m_sFilePath.a());
    return;
  }
  
  i8 rCode = fclose(m_pFilePtr);
  
  if (EOF == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vClose: fclose(%p) error; errno=%d : %s %s",
          m_pFilePtr,errno,m_sFilePath.a(),m_sOpenMode.a());
    return;
  }
  
  vReset();
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void File::vDump(void)
{
  vTrace("%sFile:\n", m_sFilePath.a());
  vTrace("%s  Open Mode %s\n", m_sOpenMode.a());
  vTrace("%s  Trace level %lld\n", m_iTraceLevel);
  vTrace("%s  File Pointer 0x%p\n", m_pFilePtr);
}

/*---------------------------------------------------------------------------*/
/* bExists: test if named file exists                                        */
/*---------------------------------------------------------------------------*/
bool File::bExists(const char *psFileName)
{
  m_xSysCall.vClear();
  
  struct stat xStat;
  
  int rCode = stat(psFileName,&xStat);
  
  if (0 == rCode && (xStat.st_mode & S_IFREG)) {
    m_xSysCall.vSuccess();
    return true;
  }
  
  if (rCode && ENOENT == errno) {
    m_xSysCall.vFailure(0);
    return false;
  }
  
  m_xSysCall.vError(errno);
  vWarn("File::bExists: stat() error; errno=%d",errno);
  
  return false;
}

/*---------------------------------------------------------------------------*/
/* vFlush: flush file                                                        */
/*---------------------------------------------------------------------------*/
void File::vFlush(void)
{
  m_xSysCall.vClear();
  
  i8 rCode = fflush(m_pFilePtr);

  if (EOF == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vFlush: fflush() error; errno=%d",errno);
    return;
  }

  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* get next character                                                         */
/*----------------------------------------------------------------------------*/
char File::cGetChar(void)
{
  if ( ! m_pFilePtr )
    return 0;

  char cChar = getc(m_pFilePtr);
  if (EOF == cChar)
    cChar = 0;
  
  return cChar;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void File::vInit(void)
{
  m_xSysCall.vSetMagic();
  
  m_sFilePath.vInit();
  m_sOpenMode.vInit();
  m_pFilePtr    = nullptr;
  m_iTraceLevel = 0;
  
  m_xSysCall.vClear();
}

/*----------------------------------------------------------------------------*/
/* is file close?                                                             */
/*----------------------------------------------------------------------------*/
bool File::bIsClosed(void)
{
  if ( ! m_pFilePtr )
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is file open?                                                              */
/*----------------------------------------------------------------------------*/
bool File::bIsOpen(void)
{
  if (m_pFilePtr)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is stdin?                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bIsStdin(void)
{
  if (stdin == m_pFilePtr)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is stdout?                                                                 */
/*----------------------------------------------------------------------------*/
bool File::bIsStdout(void)
{
  if (stdout == m_pFilePtr)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(void)
{
  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s",m_sFilePath.a());
    vClose();
  }
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s",m_sFilePath.a(),m_sOpenMode.a());
  
  m_pFilePtr = fopen(m_sFilePath.a(),m_sOpenMode.a());
  
  if ( ! m_pFilePtr ) {
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d",
          m_sFilePath.a(),m_sOpenMode.a(),errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(const char* aOpenMode, i8 iTraceLevel)
{
  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sOpenMode   = aOpenMode;
  m_iTraceLevel = iTraceLevel;
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s\n",m_sFilePath.a(),m_sOpenMode.a());
  
  m_pFilePtr = fopen(m_sFilePath.a(),m_sOpenMode.a());
  
  if ( ! m_pFilePtr ) {
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n",
          m_sFilePath.a(),m_sOpenMode.a(),errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(const char* aFilePath, const char* aOpenMode, i8 iTraceLevel)
{
  m_xSysCall.vClear();

  if (m_pFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  vSet(aFilePath,aOpenMode,iTraceLevel);
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s\n",m_sFilePath.a(),m_sOpenMode.a());
  
  m_pFilePtr = fopen(m_sFilePath.a(),m_sOpenMode.a());
  
  if ( ! m_pFilePtr ) {
    m_xSysCall.vError(errno);
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n",
          m_sFilePath.a(),m_sOpenMode.a(),errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get path                                                                   */
/*----------------------------------------------------------------------------*/
s256& File::sPath(void)
{
  return m_sFilePath;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void File::vPath(const char* aFilePath)
{
  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::vPath: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sFilePath = aFilePath;
  
}

/*---------------------------------------------------------------------------*/
/* vPrint: formatted print                                                   */
/*---------------------------------------------------------------------------*/
void File::vPrint(const char* sFormat, ...)
{
  m_xSysCall.vClear();
  
  va_list args;

  va_start(args,sFormat);
  i8 nPrinted = vfprintf(m_pFilePtr,sFormat,args);
  va_end(args);

  if (-1 == nPrinted) {
    m_xSysCall.vError(errno);
    vWarn("File::vPrint: vfprintf() error; errno=%d",errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* read chunk                                                                 */
/*----------------------------------------------------------------------------*/
i8 File::iReadChunk(void* pChunk, i8 nBytes)
{
  m_xSysCall.vClear();
  
  i8 iBytesRead = fread(pChunk,1,nBytes,m_pFilePtr);
  
  return iBytesRead;
}

/*----------------------------------------------------------------------------*/
/* read next line                                                             */
/*----------------------------------------------------------------------------*/
bool File::bReadLine(char* aBuffer)
{
  m_xSysCall.vClear();
  
  // nb: man fscanf does not indicate that errno gets set???
  
  //i8 nItems;
  
  //  if (stdin == m_pFilePtr)
  //    nItems = fscanf(m_pFilePtr,"%[^\n]%*c",s.a());
  //  else
  //    nItems = fscanf(m_pFilePtr,"%[^\n]\n",s.a());
  
  char *y = aBuffer;
  
  *y = '\0';
  for (;;) {
    char c = getc(m_pFilePtr);
    if ('\n' == c || EOF ==c) {
      break;
    }
    *y++ = c;
  }
  
  *y = '\0';
  
  if (*aBuffer) {
    m_xSysCall.vSuccess();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* read next line                                                             */
/*----------------------------------------------------------------------------*/
bool File::bReadLine(s& s)
{
  m_xSysCall.vClear();
  
  // nb: man fscanf does not indicate that errno gets set???

  //i8 nItems;
  
//  if (stdin == m_pFilePtr)
//    nItems = fscanf(m_pFilePtr,"%[^\n]%*c",s.a());
//  else
//    nItems = fscanf(m_pFilePtr,"%[^\n]\n",s.a());

  *s.a() = '\0';
  for (;;) {
     char c = getc(m_pFilePtr);
     if ('\n' == c || EOF ==c)
       break;
     else
       String::vConcat(s.a(),c);
  }
  
  if (*s.a()) {
    m_xSysCall.vSuccess();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* reset file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vReset(void)
{
  m_pFilePtr = nullptr;
  vSet("","");
}

/*---------------------------------------------------------------------------*/
/* vSeek: seek to offset                                                     */
/*---------------------------------------------------------------------------*/
void File::vSeek(i8 iOffset)
{
  m_xSysCall.vClear();
  
  i8 rCode = fseek(m_pFilePtr,iOffset,SEEK_SET);
  
  if (rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseek() error; errno=%d",errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*---------------------------------------------------------------------------*/
/* vSeek: seek to end                                                        */
/*---------------------------------------------------------------------------*/
void File::vSeekEnd(void)
{
  m_xSysCall.vClear();
  
  i8 rCode = fseek(m_pFilePtr,0,SEEK_END);
  
  if (rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseek() error; errno=%d",errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* set file arguments                                                         */
/*----------------------------------------------------------------------------*/
void File::vSet(const char* aFilePath, const char* aOpenMode, i8 iTraceLevel)
{
  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::vSet: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sFilePath   = aFilePath;
  m_sOpenMode   = aOpenMode;
  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* set file arguments                                                         */
/*----------------------------------------------------------------------------*/
void File::vSet(FILE* pFilePtr, const char* aFilePath, const char* aOpenMode, i8 iTraceLevel)
{
  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::vSet: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sFilePath   = aFilePath;
  m_sOpenMode   = aOpenMode;
  m_pFilePtr    = pFilePtr;
  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* get SysCall                                                                 */
/*----------------------------------------------------------------------------*/
SysCall& File::rSysCall(void)
{
  return m_xSysCall;
}

/*----------------------------------------------------------------------------*/
/* get trace level                                                            */
/*----------------------------------------------------------------------------*/
i8 File::iTraceLevel(void)
{
  return m_iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* set trace level                                                            */
/*----------------------------------------------------------------------------*/
void File::vTraceLevel(i8 iTraceLevel)
{
  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* unget character                                                            */
/*----------------------------------------------------------------------------*/
void File::vUngetChar(char cChar)
{
  if ( ! m_pFilePtr )
    return;
  
  i8 iResult = ungetc(cChar,m_pFilePtr);
  
  if (EOF == iResult)
    vWarn("File::vUngetChar: unget failed: '%c' %s\n",cChar,m_sFilePath.a());
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


//
///*---------------------------------------------------------------------------*/
///* File: constructor                                                         */
///*---------------------------------------------------------------------------*/
//File::File(FILE* pFile, const char* sFilePath, const char* sOpenMode, inw iTraceLevel)
//{
//  _TRACK_INC_(File);
//  
//  m_xFilePath.vSet(sFilePath);
//  m_xOpenMode.vSet(sOpenMode);
//  m_pFilePtr     = pFile;
//  m_bCheckSum    = false;
//  m_uCheckSum    = 0;
//  m_iTraceLevel  = iTraceLevel;
//
//};
//  
///*---------------------------------------------------------------------------*/
///* ~File: destructor                                                         */
///*---------------------------------------------------------------------------*/
//File::~File(void)
//{
//  _TRACK_DEC_(File);
//  
//  if (m_pFilePtr) {
//    
//    vReportWarning("~File warning; file not closed: %s",m_xFilePath.s());
//    
//    Canon canon;
//    vClose(canon);
//    if (canon.bIsError())
//      vReportWarning("~File warning; could not closed: %s",m_xFilePath.s());
//
//  }
//}
//
////---------------------------------------------------------------------------//
//// vGive: give a reference                                                   //
////---------------------------------------------------------------------------//
//void File::vGive(wrd wArg)
//{
//  ((File*)wArg)->vGiveRef((void*)wArg,File::vDelete,0);
//}
//
////---------------------------------------------------------------------------//
//// vTake: take a reference                                                   //
////---------------------------------------------------------------------------//
//void File::vTake(wrd wArg)
//{
//  ((File*)wArg)->vTakeRef();
//}
//
///*---------------------------------------------------------------------------*/
///* vDelete: delete this                                                      */
///*---------------------------------------------------------------------------*/
//void File::vDelete(void* pvThis, wrd wArg)
//{
//  File* pFile = (File*)pvThis;
//  
//  vReportInfo("File delete: %p",pFile);
//  
//  tDelete(&pFile);
//}
//
///*---------------------------------------------------------------------------*/
///* uCheckSum: get check sum                                                  */
///*---------------------------------------------------------------------------*/
//u16 File::uCheckSum(void)
//{
//  ASSERT(m_bCheckSum); // logic check
//  
//  return m_uCheckSum;
//}
//
///*---------------------------------------------------------------------------*/
///* vCheckSum: set compute check sum                                          */
///*---------------------------------------------------------------------------*/
//void File::vCheckSum(bool b)
//{
//  m_bCheckSum = b;
//}
//
///*---------------------------------------------------------------------------*/
///* vChmod: change mode                                                       */
///*---------------------------------------------------------------------------*/
//void File::vChmod(Canon& canon)
//{
//  inw rCode = chmod("./DitCfg/ditto.cfg",
//                     S_IRUSR /* R for owner */ |
//                     S_IWUSR /* W for owner */ |
//                
//                     S_IRGRP /* R for group */ |
//                     S_IWGRP /* W for group */ |
//                
//                     S_IROTH /* R for other */ |
//                     S_IWOTH /* W for other */
//                    );
//  
//  if (-1 == rCode) {
//    canon.vError(errno);
//    vReportWarning("chmod() error; errno=%d",errno);
//    return;
//  }
//  
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* vClose: close currently open file                                         */
///*---------------------------------------------------------------------------*/
//void File::vClose(void)
//{
//  Canon canon;
//  vClose(canon);
//}
//
///*---------------------------------------------------------------------------*/
///* vClose: close currently open file                                         */
///*---------------------------------------------------------------------------*/
//void File::vClose(Canon& canon)
//{
//  if ( ! m_pFilePtr ) {
//    canon.vFailure();
//    vReportWarning("vClose: file not open %s",m_xFilePath.s());
//    return;
//  }
//  
//  if (m_iTraceLevel)
//    vReportTrace("Closing file %s; %s",m_xFilePath.s(),m_xOpenMode.s());
//  
//  inw rCode = fclose(m_pFilePtr);
//  
//  if (EOF == rCode) {
//    canon.vError(errno);
//    vReportWarning("fclose() error; errno=%d",errno);
//    return;
//  }
//  
//  m_xFilePath.vSet("");
//  m_xOpenMode.vSet("");
//  
//  m_pFilePtr = NULL;
//  
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* sFileName: get name of file                                               */
///*---------------------------------------------------------------------------*/
//const char* File::sFileName(void)
//{
//  return m_xFilePath.s();
//}
//
///*---------------------------------------------------------------------------*/
///* vFlush: flush file                                                        */
///*---------------------------------------------------------------------------*/
//void File::vFlush(Canon& canon)
//{
//  inw rCode = fflush(m_pFilePtr);
//  
//  if (EOF == rCode) {
//    canon.vError(errno);
//    vReportWarning("fflush() error; errno=%d",errno);
//    return;
//  }
//  
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* iGetPos: get current position in file                                     */
///*---------------------------------------------------------------------------*/
//inw File::iGetPos(Canon& canon)
//{
//  inw rCode = ftell(m_pFilePtr);
//  
//  if (-1 == rCode) {
//    canon.vError(errno);
//    vReportWarning("ftell() error; errno=%d",errno);
//    return 0;
//  }
//  
//  canon.vSuccess();
//  return rCode;
//}
//
///*---------------------------------------------------------------------------*/
///* vInclude: copy into file as a quoted string                               */
///*---------------------------------------------------------------------------*/
//void File::vInclude(const char* sString, Canon& canon)
//{
//  const char* s = sString;
//  while (*s)
//  {
//    vPrint(canon,"  \"");
//    if ( ! canon.bIsSuccess() )
//      return;
//    
//    while(*s && '\n' != *s)
//    {
//      if ('\"' == *s)
//        vWrite((void*)"\\",1,canon);
//      vWrite((void*)s++,1,canon);
//    }
//    
//    if (*s)
//      s++;
//    
//    vPrint(canon,"\\n\"\n");
//    if ( ! canon.bIsSuccess() )
//      return;
//  }
//  
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* bOpened: test to see if file was opened                                   */
///*---------------------------------------------------------------------------*/
//bool File::bOpened(void)
//{
//  if (m_pFilePtr)
//    return true;
//  else
//    return false;
//}
//
///*---------------------------------------------------------------------------*/
///* vPrint: formatted print                                                   */
///*---------------------------------------------------------------------------*/
//void File::vPrint(Canon& canon, const char* psFormat, ...)
//{
//  va_list args;
//
//  va_start(args,psFormat);
//  inw nPrinted = vfprintf(m_pFilePtr,psFormat,args);
//  va_end(args);
//  
//  if (-1 == nPrinted) {
//    canon.vError(errno);
//    vReportWarning("vfprintf() error; errno=%d",errno);
//    return;
//  }
//  
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* vPrintArgs: formatted print                                               */
///*---------------------------------------------------------------------------*/
//void File::vPrintArgs(Canon& canon, const char* psFormat,va_list* pxArgs)
//{
//  inw nPrinted = vfprintf(m_pFilePtr,psFormat,*pxArgs);
//  
//  if (-1 == nPrinted) {
//    canon.vError(errno);
//    vReportWarning("vfprintf() error; errno=%d",errno);
//    return;
//  }
//  
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* vRead: read bytes from file                                               */
///*---------------------------------------------------------------------------*/
//void File::vRead(void* pBuffer, inw nBytes, Canon& canon)
//{
//  inw nRead = fread(pBuffer,nBytes,1,m_pFilePtr);
//  
//  if (1 == nRead) {
//    canon.vSuccess();
//    return;
//  }
//  
//  if (errno) {
//    canon.vError(errno);
//    vReportWarning("fread() error; errno=%d",errno);
//    return;
//  }
//  
//  canon.vFailure();
//}
//
///*---------------------------------------------------------------------------*/
///* pReadContents: read contents of file as varchar                           */
///*---------------------------------------------------------------------------*/
//VarChar* File::pReadContents(Canon& canon)
//{
//  //! should be dynamic, needs to check file is open etc
//  
//  char sBuffer[C_16K];
//  inw i = fread(sBuffer,1,C_16K,m_pFilePtr);
//  sBuffer[i] = '\0';
//  
//  canon.vSuccess();
//  VarChar* pContents = new VarChar(sBuffer,true);
//  return pContents;
//}
//
///*---------------------------------------------------------------------------*/
///* iReadInw: read inw from file (bFailure on EOF)                            */
///*---------------------------------------------------------------------------*/
//inw File::iReadInw(Canon& canon)
//{
//  inw iVal;
//  inw nItems = fscanf(m_pFilePtr,"%lld",&iVal);
//
//  if (nItems > 0) {
//    canon.vSuccess();
//    return iVal;
//  }
//
//  if (EOF == nItems && errno) {
//    canon.vError(errno);
//    vReportError("fscanf() error; errno=%d",errno);
//    return -1;
//  }
//  
//  canon.vFailure();
//  return -1;
//}
//
///*---------------------------------------------------------------------------*/
///* iReadLine: read line from file into inw                                   */
///*---------------------------------------------------------------------------*/
//inw File::iReadLine(Canon& canon)
//{
//  inw iVal;
//  inw nItems = fscanf(m_pFilePtr,"%lld%*[^\n]\n",&iVal);
//  
//  if (nItems > 0) {
//    canon.vSuccess();
//    return iVal;
//  }
//  
//  if (EOF == nItems && errno) {
//    canon.vError(errno);
//    vReportWarning("fscanf() error; errno=%d",errno);
//    return -1;
//  }
//  
//  canon.vFailure();
//  return -1;
//}
//
///*---------------------------------------------------------------------------*/
///* vReadLine: read line from file into buffer                                */
///*---------------------------------------------------------------------------*/
//void File::vReadLine(char* w, Canon& canon)
//{
//  inw nItems = fscanf(m_pFilePtr,"%[^\n]\n",w);
//
//  if (nItems > 0) {
//    canon.vSuccess();
//    return;
//  }
//  
//// man fscanf does not indicate that errno gets set???
////  if (EOF == nItems && errno) {
////    canon.vError(errno);
////    vReportWarning("fscanf() error; errno=%d",errno);
////    return;
////  }
//  
//  canon.vFailure();
//}
//
///*---------------------------------------------------------------------------*/
///* ppsReadTokens: read next line of tokens from file                         */
///*---------------------------------------------------------------------------*/
//char** File::ppsReadTokens(Canon& canon)
//{
//  char** asTokens;
//  char w[C_16K];
//
//  for (;;)
//  {
//    vReadLine(w,canon);
//    if ( ! canon.bIsSuccess()) {
//      return NULL;
//    }
//
//    asTokens = Source::asBurst(w);
//    
//    if (! asTokens)
//      continue;
//    
//    return asTokens;
//  }
//}
//
///*---------------------------------------------------------------------------*/
///* vRename: rename file                                                      */
///*---------------------------------------------------------------------------*/
//void File::vRename(const char *sOld, const char *sNew, Canon& canon)
//{
//  inw rCode = rename(sOld,sNew);
//  
//  if ( ! rCode ) {
//    canon.vSuccess();
//    return;
//  }
//  
//  canon.vError(errno);
//  vReportError("rename() error; errno=%d",errno);
//}
//
///*---------------------------------------------------------------------------*/
///* vSeek: seek to offset                                                     */
///*---------------------------------------------------------------------------*/
//void File::vSeek(inw iOffset, Canon& canon)
//{
//  inw rCode = fseek(m_pFilePtr,iOffset,SEEK_SET);
//  
//  if (rCode) {
//    canon.vError(errno);
//    vReportError("fseek() error; errno=%d",errno);
//    return;
//  }
//
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* vSkipLine: skip line of file                                              */
///*---------------------------------------------------------------------------*/
//void File::vSkipLine(Canon& canon)
//{
//  inw nItems = fscanf(m_pFilePtr,"%*[^\n]\n");
//
//  if (nItems >= 0) {
//    canon.vSuccess();
//    return;
//  }
//  
//  if (EOF == nItems && errno) {
//    canon.vError(errno);
//    vReportError("fscanf() error; errno=%d",errno);
//    return;
//  }
//  
//  canon.vFailure();
//}
//
///*---------------------------------------------------------------------------*/
///* iSize: size of file in bytes                                              */
///*---------------------------------------------------------------------------*/
//inw File::iSize(Canon& canon)
//{
//  inw nBytes = File::iSizeFile(m_xFilePath.s(),canon);
//  if ( ! canon.bIsSuccess()) {
//    return -1;
//  }
//  
//  canon.vSuccess();
//  return nBytes;
//}
//
///*---------------------------------------------------------------------------*/
///* vWrite: write buffer to file                                              */
///*---------------------------------------------------------------------------*/
//void File::vWrite(void* pBuffer, inw nBytes, Canon& canon)
//{
//  ASSERT( ! m_bCheckSum ); // logic check
//  
//  inw nWritten = fwrite(pBuffer,nBytes,1,m_pFilePtr);
//  
//  if (1 == nWritten) {
//    canon.vSuccess();
//    return ;
//  }
//  
//  canon.vError(errno);
//  vReportError("fwrite() error; errno=%d",errno);
//}
//
///*---------------------------------------------------------------------------*/
///* vWriteCS: compute checksum & write buffer to file                         */
///*---------------------------------------------------------------------------*/
//void File::vWriteCS(void* pBuffer, inw nBytes, Canon& canon)
//{
//  ASSERT(m_bCheckSum); // logic check
//  
//  m_uCheckSum = CheckSum::uBSDsum(m_uCheckSum, pBuffer, nBytes);
//  
//  m_bCheckSum = false;
//  vWrite(pBuffer,nBytes,canon);
//  m_bCheckSum = true;
//  
//  canon.vSuccess();
//}
//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//
///*---------------------------------------------------------------------------*/
///* bExists: test if named file exists                                        */
///*---------------------------------------------------------------------------*/
//bool File::bExists(const char *psFileName, Canon& canon)
//{
//  struct stat xStat;
//  
//  int rCode = stat(psFileName,&xStat);
//  
//  if (0 == rCode && (xStat.st_mode & S_IFREG)) {
//    canon.vSuccess();
//    return true;
//  }
//  
//  if (rCode && ENOENT == errno) {
//    canon.vFailure();
//    return false;
//  }
//  
//  canon.vError(errno);
//  vReportError("stat() error; errno=%d",errno);
//  return false;
//}
//
///*---------------------------------------------------------------------------*/
///* pOpen: open given file path                                              */
///*---------------------------------------------------------------------------*/
//FILE* File::pOpen(const char* sFilePath, const char* sMode)
//{
//  FILE* pFilePtr = fopen(sFilePath,sMode);
//  
//  if ( ! pFilePtr )
//    vReportError("File::pOpen: fopen(%s,%s) error: errno=%d",sFilePath,sMode,errno);
//  
//  return pFilePtr;
//}
//  
///*---------------------------------------------------------------------------*/
///* iSizeFile: size of named file in bytes                                    */
///*---------------------------------------------------------------------------*/
//inw File::iSizeFile(const char* psFileName, Canon& canon)
//{
//  struct stat xStat;
//  
//  inw rCode = stat(psFileName,&xStat);
//  
//  if (rCode) {
//    canon.vError(errno);
//    vReportError("stat() error; errno=%d",errno);
//    return -1;
//  }
//  
//  canon.vSuccess();
//  inw nBytes = xStat.st_size;
//  return nBytes;
//}
//
///*---------------------------------------------------------------------------*/
///* vDeleteFile: delete given file                                            */
///*---------------------------------------------------------------------------*/
//void File::vDeleteFile(const char* pFilePath, Canon& canon)
//{
//  inw rCode = remove(pFilePath);
//
//  if (rCode) {
//    canon.vError(errno);
//    vReportError("remove() error; errno=%d",errno);
//    return;
//  }
//  
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* vExtractFileName: extract file name from path                             */
///*---------------------------------------------------------------------------*/
//void File::vExtractFileName(char* sFileName, const char* sFilePath)
//{
//  // position at end
//  const char* s = sFilePath + Str::iLength(sFilePath) - 1;
//  
//  // find beginning of file name
//  for ( ; s != sFilePath; s--)
//    if (*s == '/')
//    {
//      s++;
//      break;
//    }
//  
//  // copy filename
//  char* d = sFileName;
//  while (*s)
//    *d++ = *s++;
//  *d = '\0';
//}
//  
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//  
///*---------------------------------------------------------------------------*/
///* bGet: read char* string from file                                         */
///*---------------------------------------------------------------------------*/
//bool File::bGet(char* w, inw nSize)
//{
//  char* rValue = fgets(w,(int)nSize,m_pFilePtr);
//
//  if (errno) {
//    vReportWarning("File::bGet: fgets() error; errno=%d",errno);
//    return false;
//  }
//  
//  if (rValue)
//    return true;
//  else
//    return false;
//  
//}
//
///*---------------------------------------------------------------------------*/
///* bGet: read char* string from file                                         */
///*---------------------------------------------------------------------------*/
//bool File::bGet(WSP& xWsp)
//{
//  return bGet(xWsp.w(),xWsp.nSize());
//}
//
///*---------------------------------------------------------------------------*/
///* bPut: write char* string to file                                          */
///*---------------------------------------------------------------------------*/
//bool File::bPut(const char* w)
//{
//  inw rCode = fputs(w,m_pFilePtr);
//  fflush(m_pFilePtr);
//  
//  if (errno) {
//    vReportWarning("File::bPut: fgets() error; errno=%d",errno);
//    return false;
//  }
//  
//  if (rCode == EOF)
//    return false;
//  else
//    return true;
//}
//  
///*---------------------------------------------------------------------------*/
///* vPut: write char* string to file                                          */
///*---------------------------------------------------------------------------*/
//void File::vPut(const char* w)
//{
//  fputs(w,m_pFilePtr);
//  
//  if (errno) {
//    vReportFatal("File::vPut: fputs() error; errno=%d",errno);
//    vExitFatal();
//  }
//}
//  
//
///*===========================================================================*/
///* end of file                                                               */
///*===========================================================================*/