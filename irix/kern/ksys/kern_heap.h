/**************************************************************************
 *									  *
 * 		 Copyright (C) 1990-1996 Silicon Graphics, Inc.		  *
 *									  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *									  *
 **************************************************************************/

/*
 * This is a kernel heap allocator specific private file. This is
 * used to define the heap allocator specific data structures 
 * Typically no other file (other thand idbg.c) need to include 
 * this file. If you are forced to include this file, you are
 * reaching for some variables that's out of your reach. 
 */
#ifndef __KSYS_KERN_HEAP_H__
#define __KSYS_KERN_HEAP_H__

#ident	"$Revision: 1.18 $"

#include <sys/types.h>


/*
 * data structure for a dynamic memory arena
 */

#define LOG2GRAIN	3
#define GRAIN		(1 << LOG2GRAIN)

/*
 * Small block header.  Linkage when not on free list.
 */
struct blk {
	unsigned short prevblksz;
	unsigned short thisblksz;
	/* nodenum used only in NUMA, but this structure must
	 * be 8 bytes for kmem_alloc to return 8-byte aligned memory.
	 */
	__uint32_t nodenum;
};

#endif /* __KSYS_KERN_HEAP_H__ */
