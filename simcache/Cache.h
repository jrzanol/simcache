// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
// Nome: Ronei, Gleider
//

#pragma once

void SimulateCache();
void MemoryGenerate();

typedef struct _stCache
{
	char m_Atived;

	char m_Name[16];
	int m_NSets;
	int m_BSize;
	int m_Assoc;
	char m_Repl;
} stCache;

stCache g_Cache[4];

int g_MemorySize;
unsigned int* g_Memory;
