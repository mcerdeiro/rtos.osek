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

#ifndef _OSEK_ARCH_H_
#define _OSEK_ARCH_H_
/** \brief OpenSEK Architecture Dependent Header File
 **
 ** This file is included form os.h and defines macros
 ** and types which depends on the architecture.
 **
 ** \file win/Osek_Arch.h
 ** \arch posix
 **
 **/

/** \addtogroup OpenSEK
 ** @{ */
/** \addtogroup OpenSEK_Global
 ** @{ */


/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * v0.1.0 20080725 MaCe	initial version
 */  

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
/** \brief Enable All Interrupts Arch */
#define EnableAllInterrupts_Arch()	ResumeAllInterrupts_Arch()

/** \brief Disable All Interrupts Arch */
#define DisableAllInterrupts_Arch() SuspendAllInterrupts_Arch()

/** \brief Resume All Interrupts Arch */
#define ResumeAllInterrupts_Arch()						\
	{																\
		InterruptState = ((InterruptStateType)1U);	\
		ScheduleInterrupts();								\
	}

/** \brief Suspend All Interrupts Arch */
#define SuspendAllInterrupts_Arch()						\
	{																\
		InterruptState = ((InterruptStateType)0U);	\
	}

/** \brief Resume OS Interrupts Arch */
#define ResumeOSInterrupts_Arch()													\
	{																							\
		InterruptMask &= (InterruptFlagsType)~(OSEK_OS_INTERRUPT_MASK);	\
	}


/** \brief Suspend OS Interrupts Arch */
#define SuspendOSInterrupts_Arch()					\
	{															\
		InterruptMask |= OSEK_OS_INTERRUPT_MASK;	\
	}

/*==================[typedef]================================================*/
/** \brief Interrupt type definition */
typedef unsigned int InterruptFlagsType;

/** \brief Interrupt state type definition */
typedef unsigned char InterruptStateType;

/*==================[external data declaration]==============================*/
/** \brief Interrupt Mask
 **
 ** This variable mask the interrupts. Interrupts which are masked are
 ** not going to be executed until the mask is cleared.
 **/
extern InterruptFlagsType InterruptMask;

/** \brief Interrupt State
 **
 ** If this variable is set the intterupts are enable, if it is 0
 ** interrupts are disable.
 **/
extern InterruptStateType InterruptState;

/*==================[external functions declaration]=========================*/
extern void ScheduleInterrupts(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_ARCH_H_ */

