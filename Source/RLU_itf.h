/*============================================================================*/
/* RLU_itf.h -- Interface for RLU                                             */
/*============================================================================*/

i8 iDEV_MAP(RLU* aRLU, void* aDev, i8 eDO, void** aaCrs, void* aArg, i8 iArg);

i8 iDEV_MAP_DEL_ENT   (RLU* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_DUMP      (RLU* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_ENTRIES   (RLU* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_INS_ENT   (RLU* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_TRV_BEGIN (RLU* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_TRV_NEXT  (RLU* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
