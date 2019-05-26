// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
// Nome: Ronei, Gleider
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cache.h"

int main(int argc, const char* argv[])
{
	srand(_time32(0));

	printf("SimCache\n");
	printf("--------\n\n");

	g_MemorySize = 0;
	g_Memory = NULL;

	// ./simcache -config <config file> <benchmark>

	if (argc == 2 && strcmp(argv[1], "-memgen") == 0)
	{
		MemoryGenerate();

		printf("Memória gerada!\n");
		return EXIT_SUCCESS;
	}

	FILE* in;

	if (argc < 3 || (in = fopen(argv[2], "rt")) == NULL)
	{
		// Definir os valores padrões.
		g_Cache[INSTRUCTION_L1].m_Atived = 1;
		g_Cache[INSTRUCTION_L1].m_BSize = 4;
		g_Cache[INSTRUCTION_L1].m_NSets = 16;
		g_Cache[INSTRUCTION_L1].m_Assoc = 16;
		g_Cache[INSTRUCTION_L1].m_Repl = 'R';
		g_Cache[INSTRUCTION_L1].m_Name[0] = 'A';

		g_Cache[DATA_L1].m_Atived = 1;
		g_Cache[DATA_L1].m_BSize = 4;
		g_Cache[DATA_L1].m_NSets = 256;
		g_Cache[DATA_L1].m_Assoc = 1;
		g_Cache[DATA_L1].m_Repl = 'R';
		g_Cache[DATA_L1].m_Name[0] = 'B';
	}
	else
	{
		char command[256];

		fgets(command, sizeof(command), in);
		fclose(in);

		memset(g_Cache, 0, sizeof(g_Cache));

		// -cache:il1 il1:64:64:1:l -cache:il2 none -cache:dl1 dl1:64:64:1:l -cache:dl2 none -tlb:itlb none -tlb:dtlb none

		char *ptr = command;

		while ((ptr = strchr(ptr, '-')) != NULL)
		{
			if (strncmp(ptr + 1, "cache", 5) == 0)
			{
				char* cacheTypePtr = strchr(ptr, ':');
				if (cacheTypePtr)
				{
					enCacheType cacheIndex = UNKNOWN;
					char* cacheType = ++cacheTypePtr;

					if (strncmp(cacheType, "il1", 3) == 0)
						cacheIndex = INSTRUCTION_L1;
					else if (strncmp(cacheType, "il2", 3) == 0)
						cacheIndex = INSTRUCTION_L2;
					else if (strncmp(cacheType, "dl1", 3) == 0)
						cacheIndex = DATA_L1;
					else if (strncmp(cacheType, "dl2", 3) == 0)
						cacheIndex = DATA_L2;

					if (cacheIndex != UNKNOWN)
					{
						char* cacheDefPtr = strchr(cacheTypePtr, ' ');
						if (cacheDefPtr)
						{
							if (strncmp(++cacheDefPtr, "none", 4) != 0)
							{
								if (sscanf(cacheDefPtr, "%[^:]:%d:%d:%d:%c", g_Cache[cacheIndex].m_Name, &g_Cache[cacheIndex].m_NSets, &g_Cache[cacheIndex].m_BSize, &g_Cache[cacheIndex].m_Assoc, &g_Cache[cacheIndex].m_Repl) == 5)
								{
									g_Cache[cacheIndex].m_Atived = 1;
								}
								else
									printf("Erro de leitura da definição da cache: %s.\n", ptr);
							}
						}
						else
							printf("Erro de leitura da definição: %s.\n", ptr);
					}
					else
						printf("Nenhum tipo de cache definido: %s.\n", ptr);
				}
				else
					printf("Erro de leitura do comando: %s.\n", ptr);
			}
			else
				printf("Erro no comando: %s.\n", ptr);

			ptr++; // next "-" character
		}
	}

	in = fopen("memory.dat", "rb");
	if (in == NULL)
	{
		printf("Falha ao ler a memória!\n");
		return EXIT_FAILURE;
	}

	fseek(in, 0, SEEK_END);
	long fsize = ftell(in);
	fseek(in, 0, SEEK_SET);

	if ((fsize % sizeof(unsigned int)) != 0)
	{
		printf("Arquivo de memória inválido!\n");

		fclose(in);
		return EXIT_FAILURE;
	}

	g_Memory = (unsigned int*)malloc(fsize);
	if (g_Memory == NULL)
	{
		printf("Erro ao alocar memória!\n");
		
		fclose(in);
		return EXIT_FAILURE;
	}

	g_MemorySize = (fsize / sizeof(unsigned int));

	fread(g_Memory, sizeof(unsigned int), g_MemorySize, in);
	fclose(in);

	SimulateCache();

	for (int cacheId = 0; cacheId < 4; cacheId++)
	{
		if (!g_Cache[cacheId].m_Set)
			continue;

		for (int setId = 0; setId < g_Cache[cacheId].m_NSets; setId++)
			free(g_Cache[cacheId].m_Set[setId].m_Block);

		free(g_Cache[cacheId].m_Set);
		g_Cache[cacheId].m_Set = NULL;
	}
	
	free(g_Memory);
	g_Memory = NULL;

	printf("Access: %d\n", g_MemorySize);
	printf("Misses: %d\n", g_Misses);
	printf("Hits: %d\n\n", g_Hits);

	printf("Miss Compulsorio: %d\n", g_MissComp);
	printf("Miss Capacidade: %d\n", g_MissCapac);
	printf("Miss Conflito: %d\n\n", g_MissConf);

	system("pause >nul");
	return EXIT_SUCCESS;
}

