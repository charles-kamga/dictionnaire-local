#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_TABLE 27

typedef struct element{
   char *cle;
   char *valeur;
   struct element *suivant;
} element;

typedef element *table_hachage[TAILLE_TABLE];

int HachageAlphabetique(const char *mot){

   if (mot[0] == '\0')
      return 0;

   char c = mot[0];
   if (c >= 'a' && c <= 'z'){
      c = c - 'a' + 'A';
   }

   if (c >= 'A' && c <= 'Z'){
      return c - 'A';
   }
   return 26;
}

// int hachage(const char *mot){
//    int somme = 0;
//    for (int i = 0; mot[i] != '\0'; i++){
//       somme += mot[i];
//    }
//    return somme % TAILLE_TABLE;
// }

void triAlphabetique(char *tableau[], int taille){
   for (int i = 0; i < taille - 1; i++){
      for (int j = i + 1; j < taille; j++){
         if (strcmp(tableau[i], tableau[j]) > 0){
            char *temp = tableau[i];
            tableau[i] = tableau[j];
            tableau[j] = temp;
         }
      }
   }
}

void inserer(table_hachage table, const char *cle, const char *valeur){
   int index = HachageAlphabetique(cle);
   element *nouveau = malloc(sizeof(element));
   if (!nouveau)
      return;

   nouveau->cle = strdup(cle);
   nouveau->valeur = strdup(valeur);
   nouveau->suivant = table[index];
   table[index] = nouveau;
}

char *chercher(table_hachage table, const char *cle){
   int index = HachageAlphabetique(cle);
   element *courant = table[index];
   while (courant != NULL){
      if (strcmp(courant->cle, cle) == 0){
         return courant->valeur;
      }
      courant = courant->suivant;
   }
   return NULL;
}

void supprimer(table_hachage table, const char *cle){
   int index = HachageAlphabetique(cle);
   element *courant = table[index];
   element *precedent = NULL;

   while (courant != NULL){
      if (strcmp(courant->cle, cle) == 0){
         if (precedent == NULL){
            table[index] = courant->suivant;
         }
         else{
            precedent->suivant = courant->suivant;
         }
         free(courant->cle);
         free(courant->valeur);
         free(courant);
         printf("Mot supprimé : %s\n", cle);
         return;
      }
      precedent = courant;
      courant = courant->suivant;
   }
   printf("Mot non trouvé : %s\n", cle);
}

void lireDansFichier(table_hachage table, const char *nom_fichier){
   FILE *fichier = fopen(nom_fichier, "r");
   if (fichier == NULL){
      perror("Erreur d'ouverture du fichier");
      return;
   }

   char cle[100], valeur[100];
   // for(int i = 0; i < n; i++){
   //    fscanf(fichier, "%s %s",cle, valeur);
   //    inserer(table, cle, valeur);
   // }
   while (fscanf(fichier, "%s %s", cle , valeur) == 2){
      inserer(table, cle, valeur);
   }
   fclose(fichier);
}

void insererDansFicier(const char *nom_fichier, const char *cle, const char *valeur) {
   FILE *fichier = fopen(nom_fichier, "a");
   if (fichier == NULL) {
      perror("Erreur d'ouverture du fichier");
      return;
   }
   fprintf(fichier, "%s %s\n", cle, valeur);
   fclose(fichier);
}

void afficher(table_hachage table){
   printf("\n====CONTENU DU DICTIONNAIRE =====\n");
   for (int i = 0; i < TAILLE_TABLE; i++){
      element *courant = table[i];
      while (courant != NULL){
         printf("%s → %s\n", courant->cle, courant->valeur);
         courant = courant->suivant;
      }
   }
   printf("\n");
}

void motsParDefaut(table_hachage table){
   inserer(table, "bonjour", "môné");
   inserer(table, "attente", "ngwe̱ń");
   inserer(table, "merci", "na som");
   inserer(table, "oui", "ee");
   inserer(table, "non", "kèm");
   inserer(table, "ecole", "esukulu");
   inserer(table, "maman", "mama");
   inserer(table, "chien", "mbo̱");
   inserer(table, "chat", "singi");
   inserer(table, "eau", "madíbá");
   inserer(table, "feu", "wea");
   inserer(table, "ami", "mwenja");
   inserer(table, "nuit", "budu");
   inserer(table, "jour", "buña");
   inserer(table, "argent", "mbolo");
}

int main(){
   table_hachage dictionnaire_local = {NULL};
   table_hachage dicoFichier = {NULL};
   int choix;
   char mot[100], trad[100];

   motsParDefaut(dictionnaire_local);

   do{
      printf("\n===== DICTIONNAIRE ==\n");
      printf("1. Ajouter un mot\n");
      printf("2. Rechercher un mot\n");
      printf("3. Supprimer un mot\n");
      printf("4. Afficher le dictionnaire_local\n");
      printf("5. Afficger le tri alphabetique\n");
      printf("6. Lire dans fichier\n");
      printf("7. Inserer dans le fichier\n");
      printf("8. Quitter\n");
      printf("Choix : ");
      scanf("%d", &choix);
      getchar();

      switch (choix){
      case 1:
         printf("Mot en français : ");
         scanf("%s", mot);
         printf("Traduction locale : ");
         scanf("%s", trad);
         inserer(dictionnaire_local, mot, trad);
         break;

      case 2:
         printf("Mot à rechercher : ");
         scanf("%s", mot);
         char *result = chercher(dictionnaire_local, mot);
         if (result)
            printf("Traduction : %s\n", result);
         else
            printf("Mot non trouvé.\n");
         break;

      case 3:
         printf("Mot à supprimer : ");
         scanf("%s", mot);
         supprimer(dictionnaire_local, mot);
         break;

      case 4:
         afficher(dictionnaire_local);
         break;
      
      case 5:
         printf("Affiche alphabetiquement les mots du dictionnaire_local\n");
         char *mots[TAILLE_TABLE];
         int count = 0;
         for (int i = 0; i < TAILLE_TABLE; i++){
            element *courant = dictionnaire_local[i];
            while (courant != NULL){
               mots[count++] = courant->cle;
               courant = courant->suivant;
            }
         }
         triAlphabetique(mots, count);
         printf("Mots triés par ordre alphabétique :\n");
         printf("======================================\n");
         for (int i = 0; i < count; i++){
            printf("%s\n", mots[i]);
         }
         printf("========================================\n");
         break;

      case 6:
         int nbMot;
         // printf("Lire combient de mots ? : ");
         // scanf("%d",&nbMot);
         printf("Mots lues à partir du fichier : \n");
         lireDansFichier(dicoFichier, "titi.txt");
         afficher(dicoFichier);
         break;
      
      case 7:
         printf("Entrer le mot : ");
         scanf("%s",mot);
         printf("Entrer sa traduction : ");
         scanf("%s",trad);
         insererDansFicier("titi.txt", mot, trad);
      break;
      
      case 8:
         printf("Quitté !\n");
         break;

      default:
         printf("Choix invalide.\n");
      }
   } while (choix != 8);
   return 0;
}
