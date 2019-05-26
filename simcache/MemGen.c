// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
// Nome: Ronei, Gleider
//

#include <stdio.h>
#include <stdlib.h>

#include "Cache.h"

void MemoryGenerate()
{
	FILE* out = fopen("memory.dat", "wb");
	if (out)
	{
		for (int i = 0; i < 100; i++)
		{
			unsigned int address = (rand() % 10);
			fwrite(&address, sizeof(address), 1, out);
		}

		for (int i = 0; i < 100; i++)
		{
			unsigned int address = (rand() % 100);
			fwrite(&address, sizeof(address), 1, out);
		}

		fclose(out);
	}
}
