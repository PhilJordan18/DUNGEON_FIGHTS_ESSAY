#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Variables globales u

#define ROUGE "\033[0;31m"
#define VERT "\033[0;32m"
#define BLEU "\033[0;34m"
#define JAUNE "\033[0;33m"
#define VIOLET "\033[0;35m"
#define CYAN "\033[0;36m"
#define NOIR "\033[0;30m"
#define BLANC "\033[0;37m"
#define GRIS_FONCE "\033[0;90m"
#define GRIS_CLAIR "\033[0;97m"
#define NEUTRE "\033[0m"

#define LONGUEUR_MAX_ENREGISTREMENT 1024
#define NB_INIT_APOTRES 10

#define DIFFICULTE_MOYENNE 1.45
#define DIFFICULTE_SUPERIEURE 2.0

#define LONGUEUR_DONJON 8
#define LARGEUR_DONJON 8

#define NB_MAX_APOTRES_PAR_SALLES 2
#define NB_MAX_APOTRES 43

// Définitions des différentes structures
typedef struct {
   char nom[50];
   double hp;
   double ap;
   double mp;
   double dp;
   double pointsXP;
   double precision;
   double chance;
}Apotres;

typedef struct {
    char nom[50];
    int hp;
    int ap;
    int mp;
    int dp;
    int gainXP;
    int precision;
    int chance;
} Joueur;

typedef struct {
    char nom[50];
    int ap;
    int mp;
    int chance;
} Equipement;

typedef struct {
    int nbMonstres; // Nombre de monstres par salle dans le donjon
    Apotres *monstres[NB_MAX_APOTRES_PAR_SALLES]; // Tableau qui pointe vers la structure d'apotre
}Salle;

Salle donjon [LONGUEUR_DONJON][LARGEUR_DONJON]; // Créer un tableau 2D de type salle pour en avoir autant sur la hauteur que sur la largeur 8x8


// Prototypes des fonctions

//Fonctions utilitaires
void displayMenu();
void afficherJoueur(const Joueur *joueur);
void displayOptions();
int validerChoixMenu(int choix);
int choisirDifficulte();
void afficherDifficulte();

// Création des monstres, joueur et du donjon
void creerJoueur(Joueur **joueur, int *taille);
void creerMontsres(const char *nomFichier, Apotres **apotres, int *nbApotres, int difficulte);
void importerEquipements(const char *nomFichier, Equipement **equipements, int *nbEquipements);
void attribuerEquipements(Joueur *joueur, Apotres *apotres, int nbApotres, Equipement *equipements, int nbEquipements);
void remplirDonjon(Apotres *apotres, int nbApotres);
void afficherDonjon();

// Fonctions pour les actions et intercations le long du jeu
void deplacerJoueur(Joueur *joueur, const int *abscissesX, int *ordonneesY);


void validerQuitter();

int main(void) {
    int choix = 0;
    Joueur *joueur  = NULL;
    Equipement *equipements  = NULL;
    Apotres apotres1[NB_MAX_APOTRES];
    Apotres *apotres  = NULL;
    int taille = 0;
    int nbEquipements = 0;
    int nbApotres = 0;
    int difficulte;
    displayMenu();
    choix = validerChoixMenu(choix);
    switch (choix) {
        case 1:
            creerJoueur(&joueur, &taille);
            afficherDifficulte();
            difficulte = choisirDifficulte();
            creerMontsres("apotres.csv", &apotres, &nbApotres, difficulte);
            importerEquipements("equipments.csv", &equipements, &nbEquipements);
            attribuerEquipements(joueur, apotres, nbApotres, equipements, nbEquipements);
            remplirDonjon(apotres1, NB_MAX_APOTRES);
            afficherDonjon();
            int abscissesX = 0, ordonneesY = 0;
            deplacerJoueur(joueur, &abscissesX, &ordonneesY);
            displayOptions();
            break;
        case 2:
            break;
        case 3:
            validerQuitter();
            break;
    }
    // Libérer la mémoire
    if (joueur != NULL) {free(joueur);}
    if (apotres != NULL) {free(apotres);}
    if (equipements != NULL) {free(equipements);}

    return 0;
}

void validerQuitter() {
    char reponse[50];

    do {
        printf("%s ËTES-VOUS SÛR DE VOULOIR QUITTER ?? (OUI/NON) %s ", ROUGE, NEUTRE);
        if (scanf("%49s", reponse) != 1) {
            // Vider le buffer en cas de saisie incorrecte
            while (getchar() != '\n');
            continue;
        }

        // Mettre la réponse en majuscules
        for (int i = 0; reponse[i]; i++) {
            reponse[i] = toupper(reponse[i]);
        }

    } while (!(strcmp(reponse, "OUI") == 0 || strcmp(reponse, "O") == 0 || strcmp(reponse, "NON") == 0 || strcmp(reponse, "N") == 0));

    if (strcmp(reponse, "OUI") == 0 || strcmp(reponse, "O") == 0) {
        printf("%s FERMETURE DE L'APPLICATION...%s", GRIS_FONCE, NEUTRE);
        exit(EXIT_SUCCESS);
    } else {
        // Fonction pour rejouer
    }
}


void displayMenu() {
    printf("%s  __________  ____  _   _ _   _  ____ _____ ___  _   _ \n"
 " / / / / / / |  _ \\| | | | \\ | |/ ___| ____/ _ \\| \\ | |\n"
 "/ / / / / /  | | | | | | |  \\| | |  _|  _|| | | |  \\| |\n"
 "\\ \\ \\ \\ \\ \\  | |_| | |_| | |\\  | |_| | |__| |_| | |\\  |\n"
 " \\_\\_\\_\\_\\_\\_|____/_\\___/|_| \\_|\\____|_____\\___/|_| \\_|\n"
 "| __ )  / \\|_   _|_   _| |   | ____/ ___|  \\ \\ \\ \\ \\   \n"
 "|  _ \\ / _ \\ | |   | | | |   |  _| \\___ \\   \\ \\ \\ \\ \\  \n"
 "| |_) / ___ \\| |   | | | |___| |___ ___) |  / / / / /  \n"
 "|____/_/   \\_\\_|   |_| |_____|_____|____/  /_/_/_/_/   %s", ROUGE, NEUTRE);
    printf("\n\n\n\n\n");


    printf(""
           "                -----------------------------------------------\n"
 "                    %s  1. COMMENCER UNE NOUVELLE PARTIE %s\n"
 "                -----------------------------------------------\n"
 "                     %s  2. CHARGER LA DERNIÈRE PARTIE %s\n"
 "                -----------------------------------------------\n"
 "                      %s          3. QUITTER %s\n"
 "                -----------------------------------------------",
 VIOLET, NEUTRE, VIOLET,NEUTRE, VIOLET,NEUTRE);
    printf("\n\n\n\n");

}

int validerChoixMenu(int choix){
    do{
        printf("\n%sSAISIR UN NOMBRE ENTRE 1 & 3: %s", VERT, NEUTRE);
        if (scanf("%d", &choix) != 1){
            while (getchar() != '\n'){
                choix = 0;
            }
        }
    }while(choix < 1 || choix > 3);

    return choix;
}

void creerJoueur(Joueur **joueur, int *taille) {
    srand(time(NULL));
    if (*taille == 0) {
        *joueur = (Joueur *)malloc(sizeof(Joueur));
    } else {
        *joueur = (Joueur *) realloc(*joueur, (*taille + 1) * sizeof(Joueur));
    }
    if (*joueur == NULL) {
        printf("%s Erreur: impossible d'allouer la mémoire!%s", ROUGE, NEUTRE);
    }

    printf("\n\n\n\t\t\t%sENTREZ VOTRE NOM: %s", VERT, NEUTRE);
    scanf("%s", (*joueur)[*taille].nom);
    fflush(stdin);

    (*joueur)[*taille].ap = rand() % 101 + 150;
    (*joueur)[*taille].mp = rand() % 99 ;
    (*joueur)[*taille].gainXP = 0;
    (*joueur)[*taille].precision = rand() % 50 + 40;
    (*joueur)[*taille].chance = rand() % 50;
    (*joueur)[*taille].hp = rand() % 101 + 250;
    (*joueur)[*taille].dp = rand() % 51;

    (*taille)++;
}

void afficherDifficulte() {
    printf("%s", ROUGE);
    printf("                  ____________________\n");
    printf("                 /                    \\\n");
    printf("                 |      %sNIVEAUX%s      |\n", JAUNE, ROUGE);
    printf("                 |     %sDIFFICULTÉ%s     |\n", JAUNE, ROUGE);
    printf("                 \\______%s____________/\n\n", NEUTRE);

    printf("%s                   %s▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄%s\n", VERT, GRIS_CLAIR, VERT);
    printf("                  %s►%s 1. %sFACILE%s        %s◄%s\n", VERT, NEUTRE, CYAN, NEUTRE, VERT, NEUTRE);
    printf("                   %s▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n\n", VERT);

    printf("%s                   %s▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄%s\n", JAUNE, GRIS_CLAIR, JAUNE);
    printf("                  %s►%s 2. %sINTERMÉDIAIRE%s  %s◄%s\n", JAUNE, NEUTRE, VIOLET, NEUTRE, JAUNE, NEUTRE);
    printf("                   %s▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n\n", JAUNE);

    printf("%s                   %s▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄%s\n", ROUGE, GRIS_CLAIR, ROUGE);
    printf("                  %s►%s 3. %sDIFFICILE%s      %s◄%s\n", ROUGE, NEUTRE, BLANC, NEUTRE, ROUGE, NEUTRE);
    printf("                   %s▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀%s\n\n", ROUGE, NEUTRE);

    choisirDifficulte();
}



int choisirDifficulte() {
    int difficulte = 0;

    difficulte = validerChoixMenu(difficulte);

    switch (difficulte) {
        case 1:
            difficulte = 0;
            printf("\t\t\t\t\t %s NIVEAU FACILE CHOISIT %s", BLEU, NEUTRE);
            break;
        case 2:
            difficulte = 1;
            printf("\t\t\t\t\t %s NIVEAU MOYEN CHOISIT %s", VERT, NEUTRE);
            break;
        case 3:
            difficulte = 2;
            printf("\t\t\t\t\t %s NIVEAU DIFFICILE CHOISIT %s", ROUGE, NEUTRE);
            break;
    }
    return difficulte;
}

void creerMontsres(const char *nomFichier, Apotres **apotres, int *nbApotres, int difficulte) {
    FILE *ap = fopen(nomFichier, "r");
    *nbApotres = 0;
    int capacite = NB_INIT_APOTRES;
    char ligne[LONGUEUR_MAX_ENREGISTREMENT];
    double multiplicateur;

    if (!ap) {
        printf("%s Erreur impossible d'ouvrir le fichier %s %s\n",ROUGE, nomFichier, NEUTRE);
        exit(EXIT_FAILURE);
    }
    // Allouer de l'espace pour accueillir les apotres
    *apotres = malloc(capacite * sizeof(Apotres));
    if (*apotres == NULL) {
        printf("%s Erreur: impossible d'allouer la mémoire!%s", ROUGE, NEUTRE);
    }
    // Passer l'en-tête
    fgets(ligne, sizeof(ligne), ap);
    while(fgets(ligne, sizeof(ligne), ap) != NULL) {
        if (*nbApotres == capacite) {
            capacite *=2;
            *apotres = realloc(*apotres, capacite * sizeof(Apotres));
            if (*apotres == NULL) {
                printf("%s Erreur: impossible d'allouer la mémoire!%s", ROUGE, NEUTRE);
            }
        }
        char *token = strtok(ligne, ";");

        strncpy((*apotres)[*nbApotres].nom, token, sizeof((*apotres)[*nbApotres].nom) - 1); // Nom
        (*apotres)[*nbApotres].nom[sizeof((*apotres)[*nbApotres].nom) - 1] = '\0';
        (*apotres)[*nbApotres].hp = atoi(token);
        (*apotres)[*nbApotres].mp = atoi(token);
        (*apotres)[*nbApotres].pointsXP = atoi(token);

        (*apotres)[*nbApotres].ap = rand() % 101 + 150;
        (*apotres)[*nbApotres].dp = rand() % 51;
        (*apotres)[*nbApotres].chance = rand() % 51;
        (*apotres)[*nbApotres].precision = rand() % 50 + 40;

        // Ajout de l'apôtre
        (*nbApotres)++;
    }
    fclose(ap);

    // Ajout manuel pour le dernier apotre qui sera le BOSS de mon jeu
    strncpy((*apotres)[*nbApotres].nom, "VOID", sizeof((*apotres)[*nbApotres].nom) - 1); // Nom
    (*apotres)[*nbApotres].nom[sizeof((*apotres)[*nbApotres].nom) - 1] = '\0';
    (*apotres)[*nbApotres].hp = 400;
    (*apotres)[*nbApotres].mp = 100;
    (*apotres)[*nbApotres].pointsXP = 5000;
    (*apotres)[*nbApotres].ap = 200;
    (*apotres)[*nbApotres].dp = 80;
    (*apotres)[*nbApotres].chance = 80;
    (*apotres)[*nbApotres].precision = 100;
    (*nbApotres)++;

    switch (difficulte) {
        case 1:
            multiplicateur = DIFFICULTE_MOYENNE;
            break;
        case 2:
            multiplicateur = DIFFICULTE_SUPERIEURE;
            break;
        default:
            multiplicateur = 1.0;
    }

    for (int i = 0; i < *nbApotres; i++) {
        (*apotres)[i].hp *= multiplicateur;
        (*apotres)[i].ap *= multiplicateur;
        (*apotres)[i].mp *= multiplicateur;
        (*apotres)[i].dp *= multiplicateur;
        (*apotres)[i].pointsXP *= multiplicateur;
        (*apotres)[i].precision *= multiplicateur;
        (*apotres)[i].chance *= multiplicateur;
    }
}

void importerEquipements(const char *nomFichier, Equipement **equipements, int *nbEquipements) {
    FILE *equipFile = fopen(nomFichier, "r");
    *nbEquipements = 0;
    int capacite = 10; // Capacité initiale pour les équipements
    char ligne[LONGUEUR_MAX_ENREGISTREMENT];

    if (!equipFile) {
        printf("%s Erreur impossible d'ouvrir le fichier %s %s\n", ROUGE, nomFichier, NEUTRE);
        exit(EXIT_FAILURE);
    }

    // Allouer de l'espace pour accueillir les équipements
    *equipements = malloc(capacite * sizeof(Equipement));
    if (*equipements == NULL) {
        printf("%s Erreur: impossible d'allouer la mémoire!%s", ROUGE, NEUTRE);
        exit(EXIT_FAILURE);
    }

    // Passer l'en-tête
    fgets(ligne, sizeof(ligne), equipFile);

    while (fgets(ligne, sizeof(ligne), equipFile) != NULL) {
        if (*nbEquipements == capacite) {
            capacite *= 2;
            *equipements = realloc(*equipements, capacite * sizeof(Equipement));
            if (*equipements == NULL) {
                printf("%s Erreur: impossible d'allouer la mémoire!%s", ROUGE, NEUTRE);
                exit(EXIT_FAILURE);
            }
        }

        char *token = strtok(ligne, ";");
        strncpy((*equipements)[*nbEquipements].nom, token, sizeof((*equipements)[*nbEquipements].nom) - 1);
        (*equipements)[*nbEquipements].nom[sizeof((*equipements)[*nbEquipements].nom) - 1] = '\0';

        token = strtok(NULL, ";");
        (*equipements)[*nbEquipements].ap = atoi(token);

        token = strtok(NULL, ";");
        (*equipements)[*nbEquipements].mp = atoi(token);

        token = strtok(NULL, ";");
        (*equipements)[*nbEquipements].chance = atoi(token);

        (*nbEquipements)++;
    }

    fclose(equipFile);
}

void attribuerEquipements(Joueur *joueur, Apotres *apotres, int nbApotres, Equipement *equipements, int nbEquipements) {

    // Attribuer un équipement aléatoire au joueur
    int indiceEquipement = rand() % nbEquipements;
    joueur->ap += equipements[indiceEquipement].ap;
    joueur->mp += equipements[indiceEquipement].mp;
    joueur->chance += equipements[indiceEquipement].chance;
    afficherJoueur(joueur);
    printf("%sÉquipement: \t\t\t\t%s%s\n", JAUNE, equipements[indiceEquipement].nom, NEUTRE);
    printf("%s---------------------------------------------------------------%s\n",JAUNE, NEUTRE);

    // Attribuer des équipements aléatoires aux apôtres
    for (int i = 0; i < nbApotres; i++) {
        indiceEquipement = rand() % nbEquipements;
        apotres[i].ap += equipements[indiceEquipement].ap;
        apotres[i].mp += equipements[indiceEquipement].mp;
        apotres[i].chance += equipements[indiceEquipement].chance;
    }
}

void afficherJoueur(const Joueur *joueur) {
    printf("%s", JAUNE);
    printf("                                              __     _____  ____            \n"
 "                                              \\ \\   / / _ \\/ ___|           \n"
 " _____ _____ _____ _____ _____ _____ _____ ____\\ \\ / / | | \\___ \\           \n"
 "|_____|_____|_____|_____|_____|_____|_____|_____\\ V /| |_| |___) |          \n"
 "  ____    _    ____      _    ____ _____  __  ___\\_/__\\___/|____/_ ___ ___  \n"
 " / ___|  / \\  |  _ \\    / \\  / ___|_   _|/_/_|  _ \\|_ _/ ___|_   _|_ _/ _ \\ \n"
 "| |     / _ \\ | |_) |  / _ \\| |     | || ____| |_) || |\\___ \\ | |  | | | | |\n"
 "| |___ / ___ \\|  _ <  / ___ \\ |___  | ||  _|_|  _ < | | ___) || |  | | |_| |\n"
 " \\____/_/___\\_\\_| \\_\\/_/   \\_\\____| |_||_____|_| \\_\\___|____/ |_| |___\\__\\_\\\n"
 "| | | | ____/ ___|                                                          \n"
 "| | | |  _| \\___ \\ _____ _____ _____ _____ _____ _____ _____ _____          \n"
 "| |_| | |___ ___) |_____|_____|_____|_____|_____|_____|_____|_____|         \n"
 " \\___/|_____|____/                                                          ");

    printf("\n\n---------------------------------------------------------------\n\n");
    printf("       \t\t\t\t\t%s", joueur->nom);
    printf("\n\n---------------------------------------------------------------\n");
    printf("Points de Vie: \t\t\t\t%d\n", joueur->hp);
    printf("Attaque: \t\t\t\t\t%d\n", joueur->ap);
    printf("Magie: \t\t\t\t\t\t%d\n", joueur->mp);
    printf("Précision: \t\t\t\t\t%d\n", joueur->precision);
    printf("Chance: \t\t\t\t\t%d\n", joueur->chance);
    printf("Défense: \t\t\t\t\t%d\n", joueur->dp);
    printf("Gain XP: \t\t\t\t\t%d\n", joueur->gainXP);

    printf("%s", NEUTRE);
}

void displayOptions() {
    printf("%s", ROUGE);
    printf("                           _____\n");
    printf("                          /     \\\n");
    printf("                         | ╔═══╗ |\n");
    printf("                         | ║ %sOPTIONS%s ║ |\n", JAUNE, ROUGE);
    printf("                         | ╚═══╝ |\n");
    printf("                          \\______/%s\n\n", NEUTRE);

    printf("%s                     %s▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄%s\n", VIOLET, GRIS_CLAIR, VIOLET);
    printf("                    %s►%s 1. %sATTAQUER%s     %s◄%s\n", VIOLET, NEUTRE, VERT, NEUTRE, VIOLET, NEUTRE);
    printf("                     %s▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n\n", VIOLET);

    printf("                     %s▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n", CYAN);
    printf("                    %s►%s 2. %sFUIRE%s         %s◄\n", CYAN, NEUTRE, JAUNE, NEUTRE, CYAN);
    printf("                     %s▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n\n", CYAN);

    printf("                     %s▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n", VERT);
    printf("                    %s►%s 3. %sSE SOIGNER%s   %s◄\n", VERT, NEUTRE, CYAN, NEUTRE, VERT);
    printf("                     %s▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n\n", VERT);

    printf("                     %s▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n", JAUNE);
    printf("                    %s►%s 4. %sNE RIEN FAIRE%s%s◄\n", JAUNE, NEUTRE, ROUGE, NEUTRE, JAUNE);
    printf("                     %s▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀%s\n\n", JAUNE, NEUTRE);
}

void remplirDonjon(Apotres *apotres, int nbApotres) {
    int indexApotres;
    
    for (int i = 0; i < LONGUEUR_DONJON; i++) {
        int nbApotresParLigne = 0;
        switch (i) {
            case 0:
                nbApotresParLigne = 2;
                break;
            case 1:
                nbApotresParLigne = 4;
                break;
            case 2:
                nbApotresParLigne = 6;
                break;
            case 3:
                nbApotresParLigne = 8;
                break;
            case 4:
                nbApotresParLigne = 10;
                break;
            case 5:
                nbApotresParLigne = 2;
                break;
            case 6:
                nbApotresParLigne = 10;
                break;
            case 7:
                donjon[i][0].nbMonstres = 1;
                donjon[i][0].monstres[0] = &apotres[nbApotres - 1]; // Dernier monstre est le boss
                break;
//                continue; // Passer à la prochaine ligne
        }

        for (int j = 0; j < LARGEUR_DONJON; j++) {
            if (nbApotresParLigne > 0) {
                donjon[i][j].nbMonstres = (rand() % NB_MAX_APOTRES_PAR_SALLES) + 1; // Il n'y aura que deux monstres max par salle
                nbApotresParLigne -= donjon[i][j].nbMonstres;

                for (int k = 0; k < donjon[i][j].nbMonstres; ++k) {
                    donjon[i][j].monstres[k] = &apotres[indexApotres];
                    indexApotres = (indexApotres + 1) % nbApotres;
                }
            } else {
                donjon[i][j].nbMonstres = 0;
            }
        }
    }

}

void afficherDonjon() {
    printf(" %s", BLANC);
    printf("  ---> ");
    for (char col = 'A'; col < 'A' + LARGEUR_DONJON; col++) {
        printf(" %c ", col);
    }
    printf(" %s\n", BLANC);

    for (int i = 0; i < LONGUEUR_DONJON; i++) {
        printf("%s %d ---> %s", VERT, i + 1, NEUTRE);
        for (int j = 0; j < LARGEUR_DONJON; j++) {
            printf("[%d]", donjon[i][j].nbMonstres);
        }
        printf("\n");
    }

}

void deplacerJoueur(Joueur *joueur, const int *abscissesX, int *ordonneesY) {
    char choixCol;
    int choixLigne;
    int indexCol;
    int indexLigne;
    char coordonnees[4];

    do {
        printf("%s ENTREZ LES COORDONNÉES DE LA SALLE QUE VOUS SOUHAITER EXPLORER (Ex. A3):%s ", BLANC, NEUTRE);
        if (fgets(coordonnees, sizeof(coordonnees), stdin) == NULL || coordonnees[0] == '\n') {
            printf("%s ERREUR DE LECTURE DES COORDONNÉES. VEUILLEZ RÉESSAYER. %s\n", ROUGE, NEUTRE);
            continue;
        }

        if (strlen(coordonnees) != 3 || !isalpha(coordonnees[0]) || !isdigit(coordonnees[1]) || coordonnees[2] != '\n') {
            printf("%s COORDONNÉES INVALIDES. VEUILLEZ ENTRER DES COORDONNÉES VALIDES (Ex. A3). %s\n", ROUGE, NEUTRE);
            continue;
        }

        choixCol = toupper(coordonnees[0]);
        choixLigne = coordonnees[1] - '0';
        indexCol = choixCol - 'A';
        indexLigne = choixLigne - 1;

        if (indexLigne != *abscissesX) {
            printf("%s VOUS NE POUVEZ PAS ACCÉDER À CETTE LIGNE POUR LE MOMENT. VEUILLEZ CHOISIR UNE SALLE SUR LA LIGNE %d. %s\n", ROUGE, *abscissesX + 1, NEUTRE);
        }
    } while (indexCol < 0 || indexCol >= LARGEUR_DONJON || indexLigne < 0 || indexLigne >= LONGUEUR_DONJON || indexLigne != *abscissesX);

    *ordonneesY = indexCol;
    printf("%s VOUS ÊTES DANS LA SALLE%s %s%c%s%s%d%s\n", VIOLET, NEUTRE, BLANC, choixCol, NEUTRE, JAUNE, choixLigne, NEUTRE);

    if (donjon[indexLigne][indexCol].nbMonstres > 0) {
        printf("%s IL Y A %d APÔTRES DANS CETTE SALLE, AFFRONTEZ LES!!! %s\n", ROUGE, donjon[indexLigne][indexCol].nbMonstres, NEUTRE);
        combattreDansSalle(joueur, &donjon[indexLigne][indexCol]);
    } else {
        printf("IL N'Y AUCUN APÔTRE DANS CETTE SALLE, CHERCHEZ LES AILLEURS!!\n");
    }
}




