/*============================================================================*/
/* PFileUtility.cpp - share memory utility                                    */
/*============================================================================*/

#include "BASE.h"

bool  mbCreatePFile = false;
bool  mbDeletePFile = false;
bool  mbDumpPFile = false;

char* gaSePFileRegionAddress = nullptr;
s32    miPFileRegionSize;
s256   msPFilePath;

extern Header* gaSeShmRegion;

/*----------------------------------------------------------------------------*/
/* run page file utility                                                      */
/*----------------------------------------------------------------------------*/
void PFileUtility::vRun(int argc, Stz* argv[])
{
  vTrace("PFile...\n"_Stz);

  miPFileRegionSize = ""_Stz;
  msPFilePath = ""_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // parse arguments

  for (i8 i_Arg = 1; i_Arg < argc; i_Arg++) {

    if (argv[i_Arg]->bSame("--Create"_Stz)) {
      mbCreatePFile = true;
      continue;
    }

    if (argv[i_Arg]->bSame("--Name"_Stz)) {
      msPFilePath = (Stz*)argv[++i_Arg];
      continue;
    }

    if (argv[i_Arg]->bSame("--Delete"_Stz)) {
      mbDeletePFile = true;
      continue;
    }

    if (argv[i_Arg]->bSame("--Dump"_Stz)) {
      mbDumpPFile = true;
      continue;
    }

    if (argv[i_Arg]->bSame("--Size"_Stz)) {
      miPFileRegionSize = (Stz*)argv[++i_Arg];
      continue;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // determine what to do

  if (mbCreatePFile) {
    PFileUtility::vCreate();
  }
  else if (mbDeletePFile) {
    PFileUtility::vDelete();
  }
  else if (mbDumpPFile) {
    PFileUtility::vDump();
  }
  else
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Unrecognised PFile command\n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* create a new page file on disk                                             */
/*----------------------------------------------------------------------------*/
void PFileUtility::vCreate(void)
{
  vTrace("PFile::vCreate\n"_Stz);

  if(1 == File::bExists(msPFilePath, true))
  {
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Page file already exists!\n"_Stz);
  }
  else
  {
#ifdef WITH_APPLE
    s2K sCmdNm;
    StrLib::vSprint(sCmdNm, "mkfile -n %s %s"_Stz, miPFileRegionSize.azChar(), msPFilePath.azChar());

    int rv = system(sCmdNm.azChar());
    if(0 != rv)
    {
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "Error creating page file\n"_Stz);
    }

    vTrace("Executed cmd: %s\n"_Stz, sCmdNm.azChar());
#endif
  }
}

/*----------------------------------------------------------------------------*/
/* delete a page file on disk                                                 */
/*----------------------------------------------------------------------------*/
void PFileUtility::vDelete(void)
{
  vTrace("PFile::vDelete\n"_Stz);

  if(0 == File::bExists(msPFilePath, true))
  {
    vTrace("%s does not exist\n"_Stz, msPFilePath.azChar());
    return;
  }
  else
  {
    s2K sCmdNm;
    StrLib::vSprint(sCmdNm, "rm %s"_Stz, msPFilePath.azChar());

    int rv = system(sCmdNm.azChar());
    if(0 != rv)
    {
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "Error deleting page file %s\n"_Stz, msPFilePath.azChar());
    }

    vTrace("Deleted page file: %s\n"_Stz, sCmdNm.azChar());
  }
}

/*----------------------------------------------------------------------------*/
/* dump info of page file                                                     */
/*----------------------------------------------------------------------------*/
void PFileUtility::vDump(void)
{
  vTrace("PFile::vDump\n"_Stz);
}
