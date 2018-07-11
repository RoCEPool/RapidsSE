/*============================================================================*/
/* JobNote.cpp -- the class which populates the job catalog                   */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

extern JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobNote::JobNote(Stz* aName,
                 Stz* aComment,
                 Stz* aCommand1,
                 Stz* aCommand2,
                 Stz* aSeed,
                 bool (*fPre)(TokenVec* pTokenVec),
                 void (*fPost)(void))
{
  vTrace("JobNote: %s %s %s %s\n"_Stz,
         aName, aComment, aCommand1, aSeed);
  
  // add the job class to the catalog
  i8 i_Job = gxJobCatalog.m_nClasses;
  
  gxJobCatalog.m_xJobClass[i_Job].m_aName     = aName;
  gxJobCatalog.m_xJobClass[i_Job].m_aComment  = aComment;
  gxJobCatalog.m_xJobClass[i_Job].m_aCommand1 = aCommand1;
  if (aCommand2)
    gxJobCatalog.m_xJobClass[i_Job].m_aCommand2 = aCommand2;
  gxJobCatalog.m_xJobClass[i_Job].m_aSeed     = aSeed;
  gxJobCatalog.m_xJobClass[i_Job].m_fPre      = fPre;
  gxJobCatalog.m_xJobClass[i_Job].m_fPost     = fPost;

  // we have one more class in the job catalog
  gxJobCatalog.m_nClasses++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
