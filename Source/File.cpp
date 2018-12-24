/*============================================================================*/
/* File.cpp -- os file                                                        */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern ResourceTracker gxResourceTracker;

/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
File* System::_pFile(void* aAddr)
{
  File* pFile = reinterpret_cast<File*>(aAddr);
  
  RASSERT_TYPE(pFile,CN_FILE);
  
  return pFile;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* File::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_FILE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* File::operator new(size_t, File* pFile)
{
  return pFile;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void File::operator delete(void* pFileAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pFileAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
File::File(void)
: Object(CN_FILE)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  vInit();
 
  _checkInitialised_(CN_FILE);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
File::File(Stz* aFilePath, i8 iTraceLevel)
: Object(CN_FILE)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  vInit();
  
  m_sFilePath = aFilePath;
  m_iTraceLevel = iTraceLevel;

  _checkInitialised_(CN_FILE);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
File::File(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel)
: Object(CN_FILE)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  vInit();
  
  m_sFilePath = aFilePath;
  m_sOpenMode = aOpenMode;
  m_iTraceLevel = iTraceLevel;
  
  _checkInitialised_(CN_FILE);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
File::~File(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_pFile) {
    vWarn("File::~File file not closed: %s %s\n"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());

    vClose();
    
    if (m_xSysCall.bHadError())
      vWarn("File::~File: file could not be closed: %s"_Stz,m_sFilePath.aChar());
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
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
    
  m_sFilePath   = ""_Stz;
  m_sOpenMode   = ""_Stz;
  m_iTraceLevel = 0;
  m_pFile    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void File::vClose(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();

  // never close stdin
  if (bIsStdin()) {
    vReset();
    return;
  }
  
  if (m_iTraceLevel)
    vTrace("File::vClose: closing file %s mode %s"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_pFile ) {
    m_xSysCall.vFailure(0);
    vWarn("File::vClose: file not open: %s"_Stz,m_sFilePath.aChar());
    return;
  }
  
  i8 rCode = fclose(m_pFile);
  
  if ((char)EOF == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vClose: fclose(%p) error; errno=%d : %s %s"_Stz,
          m_pFile,errno,m_sFilePath.aChar(),m_sOpenMode.aChar());
    return;
  }
  
  gxResourceTracker.vDecFileOpen();

  vReset();
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void File::vFileDump(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  vTrace("%sFile:\n"_Stz, m_sFilePath.aChar());
  vTrace("%s  Open Mode %s\n"_Stz, m_sOpenMode.aChar());
  vTrace("%s  Trace level %lld\n"_Stz, m_iTraceLevel);
  vTrace("%s  File Pointer 0x%p\n"_Stz, m_pFile);
}

/*---------------------------------------------------------------------------*/
/* test if named file exists                                                 */
/*---------------------------------------------------------------------------*/
bool File::bExists(Stz* psFileName)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  struct stat xStat;
  
  int rCode = stat(psFileName->aChar(),&xStat);
  
  if (0 == rCode && (xStat.st_mode & S_IFREG)) {
    m_xSysCall.vSuccess();
    return true;
  }
  
  if (rCode && ENOENT == errno) {
    m_xSysCall.vFailure(0);
    return false;
  }
  
  m_xSysCall.vError(errno);
  vWarn("File::bExists: %s stat() error; errno=%d"_Stz, psFileName->aChar(), errno);
  
  return false;
}

/*---------------------------------------------------------------------------*/
/* flush file                                                                */
/*---------------------------------------------------------------------------*/
void File::vFlush(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 rCode = fflush(m_pFile);

  if ((char)EOF == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vFlush: fflush() error; errno=%d"_Stz,errno);
    return;
  }

  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* get next character                                                         */
/*----------------------------------------------------------------------------*/
char File::cGetChar(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_pFile )
    return 0;

  char cChar = getc(m_pFile);
  if ((char)EOF == cChar)
    cChar = 0;
  
  return cChar;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void File::vInit(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vSetMagic();
  
  //m_sFilePath
  //m_sOpenMode
  m_pFile    = nullptr;
  m_iTraceLevel = 0;
  
  m_aLimit    = nullptr;
  m_aNext     = nullptr;
  m_pScratch  = nullptr;
  m_pRecord   = nullptr;
  m_iRecordLength = 0;
  
  m_xSysCall.vClear();
}

/*----------------------------------------------------------------------------*/
/* is file close?                                                             */
/*----------------------------------------------------------------------------*/
bool File::bIsClosed(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_pFile )
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is file open?                                                              */
/*----------------------------------------------------------------------------*/
bool File::bIsOpen(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if (m_pFile)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is stdin?                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bIsStdin(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if (stdin == m_pFile)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is stdout?                                                                 */
/*----------------------------------------------------------------------------*/
bool File::bIsStdout(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if (stdout == m_pFile)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFile) {
    vWarn("File::bOpen: previous file not closed: %s"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  m_pFile = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_pFile ) {
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d"_Stz,
          m_sFilePath.aChar(),m_sOpenMode.aChar(),errno);
    return false;
  }
  
  gxResourceTracker.vIncFileOpen();

  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(Stz* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFile) {
    vWarn("File::bOpen: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sOpenMode   = (Stz*)aOpenMode;
  m_iTraceLevel = iTraceLevel;
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s\n"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  m_pFile = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_pFile ) {
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),m_sOpenMode.aChar(),errno);
    return false;
  }
  
  gxResourceTracker.vIncFileOpen();

  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();

  if (m_pFile) {
    vWarn("File::bOpen: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  vSet((Stz*)aFilePath,aOpenMode,iTraceLevel);
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s\n"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  m_pFile = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_pFile ) {
    m_xSysCall.vError(errno);
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),m_sOpenMode.aChar(),errno);
    return false;
  }
  
  gxResourceTracker.vIncFileOpen();

  return true;
}

/*----------------------------------------------------------------------------*/
/* get path                                                                   */
/*----------------------------------------------------------------------------*/
s256& File::sPath(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  return m_sFilePath;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void File::vPath(Stz* aFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFile) {
    vWarn("File::vPath: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sFilePath = (Stz*)aFilePath;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void File::vPath(StrVal& sFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFile) {
    vWarn("File::vPath: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sFilePath = sFilePath;
}

/*----------------------------------------------------------------------------*/
/* formatted print                                                            */
/*----------------------------------------------------------------------------*/
void File::vPrint(Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  va_list args;

  va_start(args,aFormat);
  i8 nPrinted = vfprintf(m_pFile,aFormat->aChar(),args);
  va_end(args);

  if (-1 == nPrinted) {
    m_xSysCall.vError(errno);
    vWarn("File::vPrint: vfprintf() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* read bytes                                                                 */
/*----------------------------------------------------------------------------*/
i8 File::iReadBytes(void* aDst, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 iBytesRead = fread(aDst,1,nBytes,m_pFile);
  
  return iBytesRead;
}

/*----------------------------------------------------------------------------*/
/* read next line                                                             */
/*----------------------------------------------------------------------------*/
bool File::bReadLine(Stz* aBuffer)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  char *y = aBuffer->aChar();
  
  *y = '\0';
  for (;;) {
    char c = getc(m_pFile);
    if ('\n' == c || (char)EOF ==c) {
      break;
    }
    *y++ = c;
  }
  
  *y = '\0';
  
  if (*aBuffer->aChar()) {
    m_xSysCall.vSuccess();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* remove specified file                                                      */
/*----------------------------------------------------------------------------*/
bool File::bRemove(s256& rPath)
{
  char* aPath = rPath.aChar();
  
  int rCode = unlink(aPath);
  
  if (-1 == rCode) {
    vWarn("File::bRemove: rmdir() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* reset file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vReset(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_pFile = nullptr;
  vSet(""_Stz,""_Stz);
}

/*----------------------------------------------------------------------------*/
/* seek to offset                                                             */
/*----------------------------------------------------------------------------*/
void File::vSeek(i8 iOffset)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 rCode = fseeko(m_pFile,iOffset,SEEK_SET);
  
  if (rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* seek to end                                                                */
/*----------------------------------------------------------------------------*/
void File::vSeekEnd(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 rCode = fseeko(m_pFile,0,SEEK_END);
  
  if (rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* set file arguments                                                         */
/*----------------------------------------------------------------------------*/
void File::vSet(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFile) {
    vWarn("File::vSet: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sFilePath   = (Stz*)aFilePath;
  m_sOpenMode   = (Stz*)aOpenMode;
  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* set file arguments                                                         */
/*----------------------------------------------------------------------------*/
void File::vSet(FILE* pFilePtr, Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFile) {
    vWarn("File::vSet: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sFilePath   = (Stz*)aFilePath;
  m_sOpenMode   = (Stz*)aOpenMode;
  m_pFile    = pFilePtr;
  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* set file buffering                                                         */
/*----------------------------------------------------------------------------*/
void File::vSetBuffering(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  int iResult;
  
  if (C_NO_BUFFERING == nBytes)
    // set non-buffered file
    iResult = setvbuf(m_pFile,NULL,_IONBF,(int)0);
  else
    // set fully buffered file, with buffer created/managed along with file
    iResult = setvbuf(m_pFile,NULL,_IOFBF,(int)nBytes);
  
  if (iResult)
    vWarn("File::vBuffer: setvbuf failed: %s"_Stz,m_sFilePath.aChar());
}

/*----------------------------------------------------------------------------*/
/* get SysCall                                                                */
/*----------------------------------------------------------------------------*/
SysCall& File::rSysCall(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  return m_xSysCall;
}

/*----------------------------------------------------------------------------*/
/* where are we?                                                              */
/*----------------------------------------------------------------------------*/
i8 File::iTell(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  
  i8 rCode = ftello(m_pFile);
  
  if (-1 == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::iTell: ftello() error; errno=%d"_Stz,errno);
    return -1;
  }
  
  m_xSysCall.vSuccess();
  
  return rCode;
}

/*----------------------------------------------------------------------------*/
/* get trace level                                                            */
/*----------------------------------------------------------------------------*/
i8 File::iTraceLevel(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  return m_iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* set trace level                                                            */
/*----------------------------------------------------------------------------*/
void File::vTraceLevel(i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* unget character                                                            */
/*----------------------------------------------------------------------------*/
void File::vUngetChar(char cChar)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_pFile )
    return;
  
  i8 iResult = ungetc(cChar,m_pFile);
  
  if ((char)EOF == iResult)
    vWarn("File::vUngetChar: unget failed: '%c' %s\n"_Stz,cChar,m_sFilePath.aChar());
}

/*----------------------------------------------------------------------------*/
/* set trace level                                                            */
/*----------------------------------------------------------------------------*/
bool File::bWriteBytes(void* pBuffer, i8 nSize)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  
  i8 iResult = fwrite(pBuffer , 1, nSize, m_pFile);
  if (iResult != nSize) {
    vWarn("File::vWriteBytes: fwrite(%p) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),errno);
    m_xSysCall.vError(errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* zero this structure -- used only in special/nesting cases                  */
/*----------------------------------------------------------------------------*/
void File::vZero(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  m_sFilePath   = ""_Stz;
  m_sOpenMode   = ""_Stz;
  m_iTraceLevel = 0;
  m_pFile    = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* File::aFooter(void)
{
  _precondition_(_MAGIC_,CN_FILE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
