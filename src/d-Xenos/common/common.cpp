#include "common.h"


void* snet_malloc(int i_size, unsigned int* startAddress)
{
	unsigned char *align_buf = NULL;
	unsigned int curAddr = *startAddress;
	curAddr = (curAddr + 63) & (0xFFFFFFC0);
	align_buf = (unsigned char*)curAddr;
	curAddr += i_size;
	*startAddress = curAddr;
	//printf("L2:%d, MSM:%d, DDR3:%d\n", g_startHeapAddress-HEAP_STRUCTURE_ADDR, g_startMSMCAddress-MSMC_STRUCTURE_ADDR, g_startDDR3Address-DDR3_STRUCTURE_ADDR);
	return (void*)align_buf;
}

void snet_free( void *p )
{
	return;
}

void SetCoreReadyFlag(sensenet_t* s)
{
	*(s->p_coreready + s->ui_core - 1) = 1;
	CACHE_wbInvL1d((void*)(s->p_coreready + s->ui_core - 1), sizeof(int8_t), CACHE_WAIT);
}

void WaitAllCoreReady(sensenet_t* s)
{
	int starttotal = 0;
	int i;
	while (starttotal != CORES_WORKING)
	{
		CACHE_invL1d((void*)(s->p_coreready), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);
		starttotal = 0;
		for (i = 0; i < CORES_WORKING; i++)
		{
			starttotal += *(s->p_coreready + i);
		}
	}

	for (i = 0; i < CORES_WORKING; i++)
	{
		*(s->p_coreready + i) = 0;
	}
	CACHE_wbInvL1d((void*)(s->p_coreready), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);
}

void WaitCoreStartFlag(sensenet_t* s)
{
	CACHE_invL1d((void*)(s->p_corestart), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);
	while ((*(s->p_corestart + s->ui_core - 1)) != 1)
	{
		CACHE_invL1d((void*)(s->p_corestart), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);
	}
}

void SetAllCoreStart(sensenet_t* s)
{
	int i;
	for (i = 0; i < CORES_WORKING; i++)
	{
		*(s->p_corestart + i) = 1;
	}
	CACHE_wbInvL1d((void*)(s->p_corestart), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);
}

bool IsAllCoreDone(sensenet_t* s)
{
	CACHE_invL1d((void*)(s->p_corestart), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);
	for (int i = 0; i < CORES_WORKING; i++)
	{
		if (*(s->p_corestart + i) != 0)
		{
			return false;
		}
	}
	return true;
}

void ResetCoreStartFlag(sensenet_t* s)
{
	*(s->p_corestart + s->ui_core - 1) = 0;
	CACHE_wbInvL1d((void*)(s->p_corestart), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);

	*(s->p_corestart + s->ui_core - 1) = 0;
	CACHE_wbInvL1d((void*)(s->p_corestart), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);

	*(s->p_corestart + s->ui_core - 1) = 0;
	CACHE_wbInvL1d((void*)(s->p_corestart), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);

	*(s->p_corestart + s->ui_core - 1) = 0;
	CACHE_wbInvL1d((void*)(s->p_corestart), sizeof(int8_t) * CORES_WORKING, CACHE_WAIT);
}
