# traverse

      saves.c et saves.h apportent la fonctionnalité de sauvegarde de parties
      
      


L'ensemble des fonctions fonctionnent avec une structure de liste de sauvegarde : 

typedef struct saves_value{
  cell tab[10][10];
  char name[NAME_LENGTH];
  int  turn;
}saves_value;

typedef struct saves_list{
  saves_value value;
  struct saves_list *next;
}saves_list, *saves;


Listes des fonctions disponibles et utilisé : 

[ Fonctions de gestion du fichier de sauvegardes ]
FILE *open_saves_list(void);
	Permet l'ouverture du fichier de sauvegardes. Il faut impérativement utiliser close par la suite
void close_saves_list(FILE *file);
	Ferme un fichier /!\ OBLIGATOIRE
saves import_saves(FILE *file);
	Permet d'importer les sauvegardes qui ont déjà été enregistrées
FILE *export_saves(saves li,FILE *file);
	Exporte toutes la liste des sauvegardes 

[Fonctions de gestion de la liste des sauvegardes ]
saves newSavesList(void);
	Initialise une liste de sauvegardes
bool is_empty_savesList(saves li);
	Permet de vérifier que la liste de sauvegarde est vide
bool exist_save(saves li, char *name);
	Permet de vérifier qu'une élément appartient à la liste de sauvegarde (pour éviter les doublés)
saves delete_save(saves li, char name[10]);
	Permet de détruire une sauvegarde
saves add_save(saves li, saves_value newValue);
	Permet de rajouter une sauvegarde
saves pop_front_savesList(saves li);
	Inutile 
saves deleteAll_savesList(saves li);
	OBLIGATOIRE en fin de programme
	
[Fonctions de gestion des listes de Pions qui ont été joués sur une case ]
pion_list newPionList(void);
	Initialise une liste de Pion (a utiliser dans une case)
bool is_empty_pionList(pion_list li);
	Vérifie qu'une liste de pion est vide
pion_list add_pion(pion_list li, cell_pion_list_value newValue);
	Permet de rajoute le passage d'un pion 
pion_list pop_front_pionList(pion_list li);
	Inutile
pion_list deleteAll_pionList(pion_list li);
	OBLIGATOIRE si non utilisation de sauvegardes

[Modalité d'utilisation des fonctions]

Toutes les fonctions modifiant des listes doivent être utilisés sous cette forme :
	nom_liste  = fonction(paramètres);
	
	Sinon aucuns changements sera fait


Les fonctions de listes Pions seront utilisé à chaque coup




Le fichier saves.h contient la structure des tableaux celle-ci sera déplacé dans le fichier jeu.h



