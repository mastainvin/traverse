#include "config.h"
#include "FonctionsDuJeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_config(void)
{
	FILE *file = fopen("./../data/config.txt", "r+");
	// On vérifie qu'il n'y a pas eu d'erreur lors de l'ouverture
	if (file == NULL)
	{
		printf("ERREUR D'OUVERTURE DU FICHIER 'config.txt'\n");
		exit(1);
	}
	return file;
}

void close_config(FILE *file)
{
	fclose(file);
}

config_type import_config(FILE *file)
{
	config_type new_config;
	// On récupére les trois paramètres de configuration
	if (fscanf(file, "fullscreen = %d\ntheme = %d\nfps = %d\n", &new_config.fullscreen, &new_config.theme, &new_config.fps) != 3)
	{
		printf("Erreur lecture fichier conifg");
		exit(1);
	}
	return new_config;
}

FILE *export_config(config_type config, FILE *file)
{
	// On réouvre le fichier pour supprimer son contenu
	fclose(file);
	file = fopen("./../data/config.txt", "w+");
	if (file == NULL)
	{
		printf("ERREUR D'OUVERTURE DU FICHIER 'config.txt'\n");
		exit(1);
	}

	// Ecris la nouvelle configuration dans le fichier
	fprintf(file, "fullscreen = %d\ntheme = %d\nfps = %d\n", config.fullscreen, config.theme, config.fps);
	return file;
}