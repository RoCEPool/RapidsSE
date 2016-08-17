//
//  Api.cpp
//  RapidsSE
//
//  Created by alka yamarti on 16/08/16.
//  Copyright © 2016 Borrui Data Technology. All rights reserved.
//

#include "Api.hpp"

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
void vRapidsSE_PoolOpen(void)
{
    RapidsSE::vPoolOpen();
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
void vRapidsSE_SEitfDelete(SEitf* pSEitf)
{
  RapidsSE::vSEitfDelete(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
SEitf* pRapidsSE_SEitfNew(void)
{
  SEitf* pSEitf = RapidsSE::pSEitfNew();
  pSEitf->vInit();
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* get list of catalogs                                                       */
/*----------------------------------------------------------------------------*/
char* pRapidsSE_GetCatalogs(SEitf* pSEitf)
{
  s256 sResponse;
  s256 sCommand;
  
  char xArray[C_16KB];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1],
    &xArray[C_1KB * 2],
    &xArray[C_1KB * 3],
    &xArray[C_1KB * 4],
    &xArray[C_1KB * 5],
    &xArray[C_1KB * 6],
    &xArray[C_1KB * 7],
    &xArray[C_1KB * 8],
    &xArray[C_1KB * 9],
    &xArray[C_1KB * 10],
    &xArray[C_1KB * 11],
    &xArray[C_1KB * 12],
    &xArray[C_1KB * 13],
    &xArray[C_1KB * 14],
    &xArray[C_1KB * 15]
  };

  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "tenants");
  pSEitf->bDecode(pWords);
  vTrace("cd tenants\n");
  
  i8 iStatus;
  while (! pSEitf->bPopResponse(iStatus,sResponse))
    ;
  vTrace("rsp: %s\n", sResponse.a());
  
  String::vCopy(pWords[0], "cd ten");
  pSEitf->bDecode(pWords);
  vTrace("cd ten\n");
  
  while (! pSEitf->bPopResponse(iStatus,sResponse))
    ;
  vTrace("rsp: %s\n", sResponse.a());
  
  String::vCopy(pWords[0], "cd catalogs");
  pSEitf->bDecode(pWords);
  vTrace("cd catalogs\n");
  
  while (! pSEitf->bPopResponse(iStatus,sResponse))
    ;
  vTrace("rsp: %s\n", sResponse.a());
  
  String::vCopy(pWords[0], "ls");
  pSEitf->bDecode(pWords);
  vTrace("ls\n");
  
  while (! pSEitf->bPopResponse(iStatus,sResponse))
    ;
  
  vTrace("rsp: %s\n", sResponse.a());
  
 return sResponse.a();
}
