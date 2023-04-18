// SimCache: Cache Simulator
// 
// AOCII - Trabalho 2
//

#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cache.h"

int main(int argc, const char* argv[])
{
	if (argc != 7) {
		printf("Numero de argumentos incorreto. Utilize:\n");
		printf("./cache_simulator <nsets> <bsize> <assoc> <substituição> <flag_saida> arquivo_de_entrada\n");
		exit(EXIT_FAILURE);
	}

	int nsets = atoi(argv[1]);
	int bsize = atoi(argv[2]);
	int assoc = atoi(argv[3]);
	const char* subst = argv[4];
	int flagOut = atoi(argv[5]);
	const char* arquivoEntrada = argv[6];

	g_Cache.m_NSets = nsets;
	g_Cache.m_BSize = bsize;
	g_Cache.m_Assoc = assoc;
	g_Cache.m_Repl = toupper(subst[0]);
	
	srand(_time32(0));

	FILE* in = fopen(arquivoEntrada, "rb");
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

	// Convert Big Endian to Little Endian.
	for (int i = 0; i < g_MemorySize; i++)
	{
		unsigned int num = g_Memory[i];

		g_Memory[i] = ((num >> 24) & 0xff) |
			((num << 8) & 0xff0000) |
			((num >> 8) & 0xff00) |
			((num << 24) & 0xff000000);
	}

	SimulateCache();

	free(g_Memory);
	g_Memory = NULL;

	if (flagOut == 0)
	{
		printf("Access: %d\n", g_MemorySize);
		printf("Misses: %.4f\n", g_Misses / (float)g_MemorySize);
		printf("Hits: %.4f\n\n", g_Hits / (float)g_MemorySize);

		printf("Miss Compulsorio: %.2f\n", g_MissComp / (float)g_Misses);
		printf("Miss Capacidade: %.2f\n", g_MissCapac / (float)g_Misses);
		printf("Miss Conflito: %.2f\n\n", g_MissConf / (float)g_Misses);
	}
	else
		printf("%d %.4f %.4f %.2f %.2f %.2f", g_MemorySize, g_Hits / (float)g_MemorySize, g_Misses / (float)g_MemorySize,
			g_MissComp / (float)g_Misses, g_MissCapac / (float)g_Misses, g_MissConf / (float)g_Misses);

	return EXIT_SUCCESS;
}

