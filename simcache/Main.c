// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
// Nome: Ronei, Gleider
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cache.h"

int main(int argc, const char* argv[])
{
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

	if (argc != 3 || strcmp(argv[1], "-config") != 0)
	{
		printf("Argumento incorretos!\n");
		return EXIT_FAILURE;
	}

	const char* configFileName = argv[2];

	FILE* in = fopen(configFileName, "rt");
	if (in == NULL)
	{
		printf("Arquivo de configuração não encontrado!\n");
		return EXIT_FAILURE;
	}

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
				int cacheIndex = -1;
				char* cacheType = ++cacheTypePtr;

				if (strncmp(cacheType, "il1", 3) == 0)
					cacheIndex = 0;
				else if (strncmp(cacheType, "il2", 3) == 0)
					cacheIndex = 1;
				else if (strncmp(cacheType, "dl1", 3) == 0)
					cacheIndex = 2;
				else if (strncmp(cacheType, "dl2", 3) == 0)
					cacheIndex = 3;

				if (cacheIndex != -1)
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

	free(g_Memory);
	g_Memory = NULL;

	return EXIT_SUCCESS;
}

