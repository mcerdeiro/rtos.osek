/* Copyright 2008, 2009, Mariano Cerdeiro
 *
 * This file is part of OpenSEK.
 *
 * OpenSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *             
 * Linking OpenSEK statically or dynamically with other modules is making a
 * combined work based on OpenSEK. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of OpenSEK give
 * you permission to combine OpenSEK program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with OpenSEK solely through the OpenSEK defined interface. 
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for OpenSEK and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of OpenSEK are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 * 
 * OpenSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenSEK. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/** \brief OpenSEK GetResource Implementation File
 **
 ** This file implements the GetResource API
 **
 ** \file GetResource.c
 **
 **/

/** \addtogroup OpenSEK
 ** @{ */
/** \addtogroup OpenSEK_Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * KLi          Kang Li
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090130 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080909 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/
#include "Osek_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
#if (OSEK_MEMMAP == ENABLE)
#define OpenSEK_START_SEC_CODE
#include "MemMap.h"
#endif

StatusType GetResource
(
	ResourceType ResID
)
{
	/* \req OSEK_SYS_3.13 The system service StatusType
	 ** GetResource ( ResourceType ResID ) shall be defined */

	/* \req OSEK_SYS_3.13.2: Possible return values in Standard mode is E_OK */
	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	if (ResID > RESOURCES_COUNT)
	{
		/* \req OSEK_SYS_3.13.3-1/2 Extra possible return values in Extended mode are
		 ** E_OS_ID, E_OS_ACCESS */
		ret = E_OS_ID;
	}
	else if ( ( TasksVar[GetRunningTask()].Resources & ( 1 << ResID ) ) ||
				 ( ( TasksConst[GetRunningTask()].ResourcesMask & ( 1 << ResID ) ) == 0 ) )
	{
		/* \req OSEK_SYS_3.13.3-2/2 Extra possible return values in Extended mode are
		 ** E_OS_ID, E_OS_ACCESS */
		ret = E_OS_ACCESS;
	}
	else
#endif
	{

		IntSecure_Start();

		/* \req OSEK_SYS_3.13.1 This call serves to enter critical sections in
		 ** the code that are assigned to the resource referenced by ResID */
		if ( TasksVar[GetRunningTask()].ActualPriority < ResourcesPriority[ResID])
		{
			TasksVar[GetRunningTask()].ActualPriority = ResourcesPriority[ResID];
		}

		/* mark resource as set */
		TasksVar[GetRunningTask()].Resources |= ( 1 << ResID );

		IntSecure_End();

	}

#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
		(HOOK_ERRORHOOK == ENABLE) )
	/* \req OSEK_ERR_1.3-6/xx The ErrorHook hook routine shall be called if a
	 ** system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-6/xx The hook routine ErrorHook is not called if a
	 ** system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_GetResource);
		SetError_Param1(ResID);
		SetError_Ret(ret);
		SetError_Msg("GetResource returns != E_OK");
		SetError_ErrorHook();
	}
#endif

	return ret;
}

#if (OSEK_MEMMAP == ENABLE)
#define OpenSEK_STOP_SEC_CODE
#include "MemMap.h"
#endif

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

