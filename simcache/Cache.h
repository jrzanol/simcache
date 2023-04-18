// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
//

#pragma once

void SimulateCache();
void MemoryGenerate();

typedef struct _stCacheBlock
{
	int m_Valid;
	int m_Tag;
} stCacheBlock;

typedef struct _stCache
{
	int m_NSets;
	int m_BSize;
	int m_Assoc;
	char m_Repl;

	stCacheBlock* m_Block;
} stCache;

extern stCache g_Cache;

extern int g_MemorySize;
extern unsigned int* g_Memory;

extern int g_Misses;
extern int g_Hits;

extern int g_MissComp;
extern int g_MissCapac;
extern int g_MissConf;
