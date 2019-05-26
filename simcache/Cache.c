// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
// Nome: Ronei, Gleider
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cache.h"

void SimulateCache()
{
	g_Misses = 0;
	g_Hits = 0;

	g_MissComp = 0;
	g_MissCapac = 0;
	g_MissConf = 0;

	for (int cacheId = 0; cacheId < 1; cacheId++)
	{
		int blockMask = (g_Cache[cacheId].m_BSize - 1);
		int blockBits = (int)log2((float)g_Cache[cacheId].m_BSize);
		int setBits = (int)log2((float)g_Cache[cacheId].m_NSets);

		int setMask = ((g_Cache[cacheId].m_NSets - 1) << blockBits);
		int tagMask = -1 ^ setMask ^ blockMask;

		g_Cache[cacheId].m_Set = (stCacheSet*)malloc(sizeof(stCacheSet) * g_Cache[cacheId].m_NSets);
		memset(g_Cache[cacheId].m_Set, 0, sizeof(stCacheSet) * g_Cache[cacheId].m_NSets);

		for (int setId = 0; setId < g_Cache[cacheId].m_NSets; setId++)
		{
			g_Cache[cacheId].m_Set[setId].m_Block = (stCacheBlock*)malloc(sizeof(stCacheBlock) * g_Cache[cacheId].m_Assoc);
			memset(g_Cache[cacheId].m_Set[setId].m_Block, 0, sizeof(sizeof(stCacheBlock) * g_Cache[cacheId].m_Assoc));
		}

		FILE* out = fopen("MemoryAccess.txt", "wt");

		for (int addressId = 0; addressId < g_MemorySize; addressId++)
		{
			int address = g_Memory[addressId];

			int set = (address & setMask) >> blockBits;
			int tag = ((address & tagMask) >> blockBits) >> setBits;

			if (out)
				fprintf(out, "Addr %03d: tag %d set %d: ", address, tag, set);

			int blockId;
			for (blockId = 0; blockId < g_Cache[cacheId].m_Assoc; blockId++)
			{
				if (g_Cache[cacheId].m_Set[set].m_Block[blockId].m_Valid == 1)
				{
					if (g_Cache[cacheId].m_Set[set].m_Block[blockId].m_Tag == tag)
					{
						g_Hits++;

						if (out)
							fprintf(out, "HIT\n");

						break;
					}
				}
			}

			if (blockId == g_Cache[cacheId].m_Assoc)
			{
				g_Misses++;

				if (out)
					fprintf(out, "MISS\n");

				for (blockId = 0; blockId < g_Cache[cacheId].m_Assoc; blockId++)
					if (g_Cache[cacheId].m_Set[set].m_Block[blockId].m_Valid == 0)
						break;

				if (blockId == g_Cache[cacheId].m_Assoc)
				{
					if (g_Cache[cacheId].m_Repl == 'R' || g_Cache[cacheId].m_Repl == 'r')
					{
						blockId = (rand() % g_Cache[cacheId].m_Assoc);
						g_MissConf++; // Miss Conflito ou Capacidade?
					}
					else // Outras políticas de substituição.
						blockId = 0;
				}
				else
					g_MissComp++;

				g_Cache[cacheId].m_Set[set].m_Block[blockId].m_Valid = 1;
				g_Cache[cacheId].m_Set[set].m_Block[blockId].m_Tag = tag;
			}
		}
	}
}
