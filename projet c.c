#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
//caracterstique de voiture
typedef struct DATE
{
 int JOUR;
 int MOIS;
 int ANNEE;
} date;

typedef struct {
    int matricule;
    int serie ;
    char marque[70];
    char modele[70] ;
    char couleur[50];
    char etat[50] ; //excellent, bon, usé
    char optionssupp[100] ;//Toit ouvrant, jantes spéciales
    float prixparjour;
    int nbreplace ;
    int available ;
    date datedelocation;
    date daterendrevoiture;


} Car;
void afficherDescriptionVoiture(const char *nomFichier) {
   int matricule;
   printf("matricule:");
   scanf("d", &matricule);

    FILE *fichier = fopen(nomFichier, "rb");

    if (fichier != NULL) {
        Car voiture;
        int found = 0;

        while (fread(&voiture, sizeof(Car), 1, fichier)) {
            if (voiture.matricule == matricule) {
                found = 1;
                printf("Description de la voiture (Matricule : %d) :\n", voiture.matricule);
                printf("Marque : %c\n", voiture.marque);
                printf("Serie : %d\n", voiture.serie);
                printf("Modele : %c\n", voiture.modele);
                printf("Couleur : %c\n", voiture.couleur);
                printf("État : %c\n", voiture.etat);
                printf("optionsSupp : %c\n", voiture.optionssupp);
                printf("Prix par jour : %f\n", voiture.prixparjour);
                printf("Nombre de places : %d\n", voiture.nbreplace);

                break;
            }
        }

        if (!found) {
            printf("voiture introuvable.\n", matricule);
        }

        fclose(fichier);
    } else {
        printf("fichier vide.\n");
    }
}
void ajouternouvelleVoiture(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "ab");
    if (fichier != NULL) {
        Car nouvelleVoiture;

        printf("Matricule : ");
        scanf("%d", &nouvelleVoiture.matricule);

        printf("Serie : ");
        scanf("%d", &nouvelleVoiture.serie);

        printf("Marque : ");
        scanf("%s", nouvelleVoiture.marque);

        printf("Modele : ");
        scanf("%s", nouvelleVoiture.modele);

        printf("Couleur : ");
        scanf("%s", nouvelleVoiture.couleur);

        printf("État (excellent, bon, use) : ");
        scanf("%s", nouvelleVoiture.etat);

        printf("Options supplémentaires : ");
        scanf("%s", nouvelleVoiture.optionssupp);

        printf("Prix par jour : ");
        scanf("%f", &nouvelleVoiture.prixparjour);

        printf("Nombre de places : ");
        scanf("%d", &nouvelleVoiture.nbreplace);

        nouvelleVoiture.available = 1;

        fwrite(&nouvelleVoiture, sizeof(Car), 1, fichier);
        printf("La voiture a été ajoutée avec succès.\n");

        fclose(fichier);
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }
}

void modifierVoiture(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rb+");

    if (fichier != NULL) {
        int mat;
        Car voiture;
        printf("Matricule : ");
        scanf("%d", &mat);

        while (fread(&voiture, sizeof(Car), 1, fichier)) {
            if (voiture.matricule == mat) {
                printf("Voiture trouvée !\n");
                printf("Nouvelle couleur : ");
                scanf(" %c", &voiture.couleur);

                printf("Nouvel état (excellent, bon, use) : ");
                scanf(" %s", voiture.etat);

                printf("Nouveau prix par jour : ");
                scanf("%f", &voiture.prixparjour);

                fseek(fichier, -sizeof(Car), SEEK_CUR); // Se déplacer en arrière d'un enregistrement
                fwrite(&voiture, sizeof(Car), 1, fichier);
                printf("La voiture a été modifiée avec succès.\n");
                break;
            }
        }

        fclose(fichier);
    } else {
        printf("Fichier vide ou inaccessible.\n");
    }
}
void supprimerVoitureEnPanne(int matricule, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rb+");

    if (fichier != NULL) {
        Car voiture;

        int found = 0;
        long positionDebut, positionFin;
        positionDebut = ftell(fichier);

        while (fread(&voiture, sizeof(Car), 1, fichier)) {
            if (voiture.matricule == matricule && strcmp(voiture.etat, "P") == 0) {
                found = 1;
                positionFin = ftell(fichier);
                fseek(fichier, positionDebut, SEEK_SET);

                while (ftell(fichier) != positionFin) {
                    fread(&voiture, sizeof(Car), 1, fichier);
                    fwrite(&voiture, sizeof(Car), 1, fichier);
                }

                ftruncate(fileno(fichier), positionFin - sizeof(Car));
                printf("La voiture en panne a été supprimée.\n");
                break;
            }
            positionDebut = ftell(fichier);
        }

        if (!found) {
            printf("Aucune voiture en panne correspondant à ce matricule.\n");
        }

        fclose(fichier);
    } else {
        printf("fichier vide.\n");
    }
}

void louerVoiture(const char *nomFichier) {
    int matricule;
    printf("Entrez le matricule de la voiture à louer : ");
    scanf("%d", &matricule);

    FILE *fichier = fopen(nomFichier, "rb+");

    if (fichier != NULL) {
        Car voiture;

        while (fread(&voiture, sizeof(Car), 1, fichier)) {
            if (voiture.matricule == matricule && voiture.available == 1) {
                printf("Voiture trouvée !\n");
                voiture.available = 0; // Mettre la voiture comme indisponible

                printf("Entrez la date de location (Jour Mois Année) : ");
                scanf("%d %d %d", &voiture.datedelocation.JOUR, &voiture.datedelocation.MOIS, &voiture.datedelocation.ANNEE);

                printf("Entrez la date de retour (Jour Mois Année) : ");
                scanf("%d %d %d", &voiture.daterendrevoiture.JOUR, &voiture.daterendrevoiture.MOIS, &voiture.daterendrevoiture.ANNEE);

                 voiture.available= 0;

                fseek(fichier, -sizeof(Car), SEEK_CUR);
                fwrite(&voiture, sizeof(Car), 1, fichier);
                printf("La voiture a été louée avec succès.\n");
                break;
            }
        }

        fclose(fichier);
    } else {
        printf("fichier vide.\n");
    }
}
void afficherHistoriqueLocations() {
    FILE *fichier = fopen("historique_locations.bin", "rb");

    if (fichier != NULL) {
        printf("Historique des locations :\n");

          Car voiture;

        while (fread(&voiture, sizeof(Car), 1, fichier)) {
            printf("Matricule de la voiture : %d\n", voiture.matricule);
            printf("Date de location : %d/%d/%d\n", voiture.datedelocation.JOUR,
                   voiture.datedelocation.MOIS, voiture.datedelocation.ANNEE);

            if (voiture.daterendrevoiture.JOUR != 0 && voiture.daterendrevoiture.MOIS != 0 &&
                voiture.daterendrevoiture.ANNEE != 0) {
                printf("Date de retour : %d/%d/%d\n", voiture.daterendrevoiture.JOUR,
                      voiture.daterendrevoiture.MOIS, voiture.daterendrevoiture.ANNEE);
            } else {
                printf("Voiture non encore retournée.\n");
            }

            printf("\n");
        }

        fclose(fichier);
    } else {
        printf("fichier vide.\n");
    }
}
void retourVoitureReclamation(const char *nomFichier) {
    int matricule;
    printf("Entrez le matricule de la voiture à retourner suite à une réclamation : ");
    scanf("%d", &matricule);

    FILE *fichier = fopen(nomFichier, "rb+");

    if (fichier != NULL) {
        Car voiture;

        while (fread(&voiture, sizeof(Car), 1, fichier)) {
            if (voiture.matricule == matricule && voiture.available == 0) {
                printf("Voiture trouvée !\n");
                voiture.available = 1;

                printf("Entrez le nouvel état de la voiture suite à la réclamation (excellent, bon, usé) : ");
                scanf(" %c", &voiture.etat);

                fseek(fichier, -sizeof(Car), SEEK_CUR);
                fwrite(&voiture, sizeof(Car), 1, fichier);
                printf("La voiture a été retournée suite à la réclamation.\n");
                break;
            }
        }

        fclose(fichier);
    } else {
        printf("fichier vide.\n");
    }
}
int  main (){
    int mat;
    int choix = 0;
    char nom_du_fichier[] = "nom_du_fichier.txt";
    while (choix != 8) {
      do{
    printf("\nGESTION DE LOCATION DES VOITURES\n");
    printf("\n* 1 *\tAfficher la description d’une voiture.");
    printf("\n* 2 *\tAjouter une nouvelle voiture.");
    printf("\n* 3 *\t Modifier la description et l’état d’une voiture.");
    printf("\n* 4 *\tSupprimer une voiture qui est en panne.");
    printf("\n* 5 *\t Louer une voiture.");
    printf("\n* 6 *\t Afficher l’historique des locations.");
    printf("\n* 7 *\tRetour d’une voiture en cas de réclamation.");
    printf("\n* 8 *\tQuitter le programme");
    printf("\n\nSaisissez votre choix : ");
    fflush(stdin);
    scanf("%d", &choix);
}while( (choix<1)||(choix>8) );
  FILE *fichier = fopen("nom_du_fichier.txt", "rb+");

    if (fichier != NULL) {
 switch(choix)
 {
          case 1 :

           afficherDescriptionVoiture(nom_du_fichier);
            break;
          case 2:
                  ajouternouvelleVoiture(nom_du_fichier);
                  break;
            case 3:
                modifierVoiture(nom_du_fichier);
                break;
            case 4:
               printf("mat:");
               scanf("%d" , &mat);
               supprimerVoitureEnPanne(mat, nom_du_fichier);
                break;
            case 5:
               louerVoiture(nom_du_fichier);
                break;

            case 6:
                afficherHistoriqueLocations(nom_du_fichier);
                break;
            case 7:
                retourVoitureReclamation(nom_du_fichier);
                break;
            case 8:
                printf("Programme quitte.\n");
                break;
            default:
                printf("Choix invalide. Veuillez choisir parmi les options fournies.\n");
        }
        fclose(fichier);
    } else {
            printf("Erreur lors de l'ouverture du fichier.\n");
        }
}
    return 0;
}

