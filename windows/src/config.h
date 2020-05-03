#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FonctionsDuJeu.h"
/* Prototype de config */

typedef struct config_type
{
	int theme;
	bool fullscreen;
	bool fps;
} config_type;

/* Fonctions de gestion du fichier de configs */
FILE *open_config(void);
void close_config(FILE *file);
config_type import_config(FILE *file);
FILE *export_config(config_type config, FILE *file);

#endif