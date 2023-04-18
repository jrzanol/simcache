// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cache.h"

stCache g_Cache;

int g_MemorySize = 0;
unsigned int* g_Memory = NULL;

int g_Misses = 0;
int g_Hits = 0;

int g_MissComp = 0;
int g_MissCapac = 0;
int g_MissConf = 0;

void SimulateCache()
{
	g_Misses = 0;
	g_Hits = 0;

	g_MissComp = 0;
	g_MissCapac = 0;
	g_MissConf = 0;

	// Calcula a quantidade de bits do Deslocamento e do Índice.
	const int offsetBits = (int)log2((float)g_Cache.m_BSize);
	const int indiceBits = (int)log2((float)g_Cache.m_NSets);

	// Cálcula a máscara de bits do Índice.
	const int indiceMask = (g_Cache.m_NSets - 1);

	// Cálculo do tamanho total da Cache.
	const size_t cacheSize = (g_Cache.m_NSets * g_Cache.m_Assoc);

	g_Cache.m_Block = (stCacheBlock*)malloc(cacheSize * sizeof(stCacheBlock));
	if (g_Cache.m_Block)
	{
		memset(g_Cache.m_Block, 0, cacheSize * sizeof(stCacheBlock));

		for (int addressId = 0; addressId < g_MemorySize; addressId++)
		{
			const int address = g_Memory[addressId];

			const int indice = (address >> offsetBits) & indiceMask;
			const int tag = (address >> offsetBits) >> indiceBits;

			// Corrige o Índice em relação a Associatividade.
			const int idMemory = (indice * g_Cache.m_Assoc);

			int blockId;
			for (blockId = 0; blockId < g_Cache.m_Assoc; blockId++)
			{
				if (g_Cache.m_Block[idMemory + blockId].m_Valid == 1)
				{
					if (g_Cache.m_Block[idMemory + blockId].m_Tag == tag)
					{
						g_Hits++;
						break;
					}
				}
			}

			if (blockId == g_Cache.m_Assoc)
			{
				g_Misses++;

				for (blockId = 0; blockId < g_Cache.m_Assoc; blockId++)
					if (g_Cache.m_Block[idMemory + blockId].m_Valid == 0)
						break;

				if (blockId == g_Cache.m_Assoc)
				{
					blockId = (rand() % g_Cache.m_Assoc);

					size_t checkCapac;
					for (checkCapac = 0; checkCapac < cacheSize; checkCapac++)
						if (g_Cache.m_Block[checkCapac].m_Valid == 0)
							break;

					if (checkCapac < cacheSize)
						g_MissConf++;
					else
						g_MissCapac++;
				}
				else
					g_MissComp++;

				g_Cache.m_Block[idMemory + blockId].m_Valid = 1;
				g_Cache.m_Block[idMemory + blockId].m_Tag = tag;
			}
		}

		free(g_Cache.m_Block);
	}
}
