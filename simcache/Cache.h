// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
// Nome: Ronei, Gleider
//

#pragma once

void SimulateCache();
void MemoryGenerate();

typedef enum _enCacheType {
	UNKNOWN = -1,
	INSTRUCTION_L1 = 0,
	INSTRUCTION_L2 = 1,
	DATA_L1 = 2,
	DATA_L2 = 3
} enCacheType;

typedef struct _stCacheBlock
{
	int m_Valid;
	int m_Tag;
} stCacheBlock;

typedef struct _stCacheSet
{
	stCacheBlock* m_Block;
} stCacheSet;

typedef struct _stCache
{
	char m_Atived;

	char m_Name[16];
	int m_NSets;
	int m_BSize;
	int m_Assoc;
	char m_Repl;

	stCacheSet* m_Set;
} stCache;

stCache g_Cache[4];

int g_MemorySize;
unsigned int* g_Memory;

int g_Misses;
int g_Hits;

int g_MissComp;
int g_MissCapac;
int g_MissConf;
