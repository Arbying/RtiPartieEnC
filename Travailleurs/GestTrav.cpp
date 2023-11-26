#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

void clearScreen() {
    system("clear");
}

int authenticate(MYSQL *conn, const char *username, const char *password) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[256];
    int isAuth = 0;

    sprintf(query, "SELECT * FROM Employes WHERE Nom = '%s' AND MDP = '%s' AND Type = 1", username, password);
    if (mysql_query(conn, query) == 0) {
        res = mysql_store_result(conn);
        if (res != NULL && (row = mysql_fetch_row(res)) != NULL) {
            isAuth = 1;
        }
        mysql_free_result(res);
    }
    return isAuth;
}

void showMenu() {
    printf("\nMenu Principal:\n");
    printf("1. Consulter les employés\n");
    printf("2. Ajouter un employé\n");
    printf("3. Modifier un employé\n");
    printf("4. Voir et modifier les types de contrat\n");
    printf("5. Quitter\n");
    printf("Entrez votre choix : ");
}

void consultEmployees(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    if (mysql_query(conn, "SELECT * FROM Employes")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    res = mysql_use_result(conn);
    printf("Liste des Employes:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s - %s\n", row[0], row[1]); // Affiche l'ID et le nom

    mysql_free_result(res);
    printf("\nAppuyez sur une touche pour continuer...\n");
    getchar(); getchar(); // Pour attendre une saisie utilisateur
}

void addEmployee(MYSQL *conn) {
    char nom[50], mdp[50], actif[10];
    int type;
    char query[256];

    printf("Entrez le nom de l'employé: ");
    scanf("%49s", nom);
    printf("Entrez le mot de passe: ");
    scanf("%49s", mdp);
    printf("Entrez le type (numéro): ");
    scanf("%d", &type);
    printf("Est-il actif? (oui/non): ");
    scanf("%9s", actif);

    sprintf(query, "INSERT INTO Employes (Nom, MDP, Type, Actif) VALUES ('%s', '%s', %d, '%s')", nom, mdp, type, actif);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    } else {
        printf("Employé ajouté avec succès.\n");
    }
}

void modifyEmployee(MYSQL *conn) {
    int id;
    char nom[50], mdp[50], actif[10];
    int type;
    char query[256];

    printf("Entrez l'ID de l'employé à modifier: ");
    scanf("%d", &id);
    printf("Entrez le nouveau nom: ");
    scanf("%49s", nom);
    printf("Entrez le nouveau mot de passe: ");
    scanf("%49s", mdp);
    printf("Entrez le nouveau type: ");
    scanf("%d", &type);
    printf("Est-il actif? (oui/non): ");
    scanf("%9s", actif);

    sprintf(query, "UPDATE Employes SET Nom='%s', MDP='%s', Type=%d, Actif='%s' WHERE IdEmploye=%d", nom, mdp, type, actif, id);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    } else {
        printf("Employé modifié avec succès.\n");
    }
}

void manageContractTypes(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    int choice;
    char type[50];
    char query[256];

    if (mysql_query(conn, "SELECT * FROM FonctionsTrav")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    res = mysql_use_result(conn);
    printf("Types de contrats existants:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s - %s\n", row[0], row[1]);

    mysql_free_result(res);

    printf("\nVoulez-vous ajouter un nouveau type de contrat? (1 pour oui, 0 pour non): ");
    scanf("%d", &choice);
    if (choice) {
        printf("Entrez le nom du nouveau type de contrat: ");
        scanf("%49s", type);
        sprintf(query, "INSERT INTO FonctionsTrav (Fonction) VALUES ('%s')", type);
        if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
        } else {
            printf("Nouveau type de contrat ajouté.\n");
        }
    }
}

int main() {
    MYSQL *conn;
    char username[50], password[50];

    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0)) {
        fprintf(stderr, "Erreur de connexion : %s\n", mysql_error(conn));
        exit(1);
    }

    clearScreen();

    printf("Login : ");
    scanf("%49s", username);
    printf("Mot de passe : ");
    scanf("%49s", password);

    if (authenticate(conn, username, password)) {
        int choice;
        do {
            showMenu();
            scanf("%d", &choice);
            clearScreen();
            switch (choice) {
                case 1: consultEmployees(conn); break;
                case 2: addEmployee(conn); break;
                case 3: modifyEmployee(conn); break;
                case 4: manageContractTypes(conn); break;
                case 5: printf("Fin du programme.\n"); break;
                default: printf("Choix invalide.\n"); break;
            }
        } while (choice != 5);
    } else {
        printf("Authentification échouée.\n");
    }

    mysql_close(conn);
    return 0;
}
