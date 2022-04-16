/*============================================================================*/
/* ShmUtility.cpp - share memory utility                                      */
/*============================================================================*/

#include "BASE.h"

bool  mbCreatePFile = false;
bool  mbDeletePFile = false;
bool  mbDumpPFile = false;

char* gaSePFileRegionAddress = nullptr;
//i8    miPFileRegionSize = 0;
s32   msPFilePath;

extern Header* gaSeShmRegion;

/*----------------------------------------------------------------------------*/
/* run page file utility                                                      */
/*----------------------------------------------------------------------------*/
void PFileUtility::vRun(int argc, Stz* argv[])
{
  vTrace("PFile...\n"_Stz);

  msPFilePath = ""_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // parse arguments

  for (i8 i_Arg = 1; i_Arg < argc; i_Arg++) {

    if (argv[i_Arg]->bSame("--Create"_Stz)) {
      mbCreatePFile = true;
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
}

/*----------------------------------------------------------------------------*/
/* delete a page file on disk                                                 */
/*----------------------------------------------------------------------------*/
void PFileUtility::vDelete(void)
{
  vTrace("PFile::vDelete\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump info of page file                                                     */
/*----------------------------------------------------------------------------*/
void PFileUtility::vDump(void)
{
  vTrace("PFile::vDump\n"_Stz);
}
