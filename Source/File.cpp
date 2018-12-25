/*============================================================================*/
/* File.cpp -- os file                                                        */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern ResourceTracker gxResourceTracker;

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
  
  //m_sFilePath
  //m_sOpenMode
  m_aFile    = nullptr;
  
  m_xSysCall.vClear();

  _checkInitialised_(CN_FILE);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
File::~File(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_aFile) {
    vWarn("File::~File file not closed: %s %s\n"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());

    vClose();
    
    if (m_xSysCall.bHadError())
      vWarn("File::~File: file could not be closed: %s"_Stz,m_sFilePath.aChar());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* test if given file exists in file system                                  */
/*---------------------------------------------------------------------------*/
bool File::bExists(Stz* aFilePath)
{
  struct stat xStat;
  
  int iResult = stat(aFilePath->aChar(),&xStat);
  
  if (0 == iResult && (xStat.st_mode & S_IFREG))
    return true;
  
  if (iResult && ENOENT == errno)
    return false;
  
  vWarn("File::bExists: %s stat() error; errno=%d"_Stz, aFilePath->aChar(), errno);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* remove given file from file system                                         */
/*----------------------------------------------------------------------------*/
bool File::bRemove(Stz* aFilePath)
{
  int iResult = unlink(aFilePath->aChar());
  
  if (-1 == iResult) {
    vWarn("File::bRemove: unlink() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  
  if ( ! m_aFile ) {
    m_xSysCall.vFailure(0);
    vWarn("File::vClose: file not open: %s"_Stz,m_sFilePath.aChar());
    return;
  }
  
  i8 iResult = fclose(m_aFile);
  
  if ((char)EOF == iResult) {
    m_xSysCall.vError(errno);
    vWarn("File::vClose: fclose(%p) error; errno=%d : %s %s"_Stz,
          m_aFile,errno,m_sFilePath.aChar(),m_sOpenMode.aChar());
    return;
  }
  
  gxResourceTracker.vDecFileOpen();
  
  m_aFile = nullptr;

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
  vTrace("%s  File Pointer 0x%p\n"_Stz, m_aFile);
}

/*---------------------------------------------------------------------------*/
/* flush file                                                                */
/*---------------------------------------------------------------------------*/
void File::vFlush(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 iResult = fflush(m_aFile);

  if ((char)EOF == iResult) {
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

  if ( ! m_aFile )
    return 0;

  char cChar = getc(m_aFile);
  if ((char)EOF == cChar)
    cChar = 0;
  
  return cChar;
}

/*----------------------------------------------------------------------------*/
/* is file close?                                                             */
/*----------------------------------------------------------------------------*/
bool File::bIsClosed(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFile )
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

  if (m_aFile)
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

  if (stdin == m_aFile)
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

  if (stdout == m_aFile)
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

  
  if (m_aFile) {
    vWarn("File::bOpen: previous file not closed: %s"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_xSysCall.vClear();

  //vTrace("File::bOpen: opening file %s mode %s"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  m_aFile = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_aFile ) {
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
bool File::bOpen(Stz* aFilePath, Stz* aOpenMode)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if (m_aFile) {
    vWarn("File::bOpen: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_xSysCall.vClear();
  
  m_sFilePath   = aFilePath;
  m_sOpenMode   = aOpenMode;

  m_aFile = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_aFile ) {
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
/* formatted print                                                            */
/*----------------------------------------------------------------------------*/
void File::vPrint(Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  va_list args;

  va_start(args,aFormat);
  i8 nPrinted = vfprintf(m_aFile,aFormat->aChar(),args);
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
  
  i8 iBytesRead = fread(aDst,1,nBytes,m_aFile);
  
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
    char c = getc(m_aFile);
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
/* reset file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vReset(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if (m_aFile) {
    vWarn("File::vReset: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }

  m_xSysCall.vClear();
  
  m_sFilePath   = ""_Stz;
  m_sOpenMode   = ""_Stz;
  m_aFile       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* seek to offset                                                             */
/*----------------------------------------------------------------------------*/
void File::vSeek(i8 iOffset)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 iResult = fseeko(m_aFile,iOffset,SEEK_SET);
  
  if (iResult) {
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
  
  i8 iResult = fseeko(m_aFile,0,SEEK_END);
  
  if (iResult) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_xSysCall.vSuccess();
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
    iResult = setvbuf(m_aFile,NULL,_IONBF,(int)0);
  else
    // set fully buffered file, with buffer created/managed along with file
    iResult = setvbuf(m_aFile,NULL,_IOFBF,(int)nBytes);
  
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
  
  i8 iResult = ftello(m_aFile);
  
  if (-1 == iResult) {
    m_xSysCall.vError(errno);
    vWarn("File::iTell: ftello() error; errno=%d"_Stz,errno);
    return -1;
  }
  
  m_xSysCall.vSuccess();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* unget character                                                            */
/*----------------------------------------------------------------------------*/
void File::vUngetChar(char cChar)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFile )
    return;
  
  i8 iResult = ungetc(cChar,m_aFile);
  
  if ((char)EOF == iResult)
    vWarn("File::vUngetChar: unget failed: '%c' %s\n"_Stz,cChar,m_sFilePath.aChar());
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void File::vUseStdin(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_aFile) {
    vWarn("File::vUseStdin: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_xSysCall.vClear();
  
  m_sFilePath   = "stdin"_Stz;
  m_sOpenMode   = C_OPEN_READ;
  m_aFile       = stdin;
}

/*----------------------------------------------------------------------------*/
/* set trace level                                                            */
/*----------------------------------------------------------------------------*/
bool File::bWriteBytes(void* pBuffer, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  
  i8 iResult = fwrite(pBuffer , 1, nBytes, m_aFile);
  
  if (iResult != nBytes) {
    vWarn("File::vWriteBytes: fwrite(%s) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),errno);
    m_xSysCall.vError(errno);
    return false;
  }
  
  return true;
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
