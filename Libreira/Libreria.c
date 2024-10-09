#include <stdio.h>
#include <string.h>

#define MAX_LIBRI 100
#define MAX_CATEGORIE 10
#define MAX_TITOLO 100
#define MAX_AUTORE 100
#define MAX_NOME_CATEGORIA 50
#define MAX_BUFF 5

// Struttura per rappresentare un libro
typedef struct {
    char titolo[MAX_TITOLO];
    char autore[MAX_AUTORE];
    int anno;
    float prezzo;
} Libro;

// Struttura per rappresentare una categoria
typedef struct {
    char nome[MAX_NOME_CATEGORIA];
    Libro libri[MAX_LIBRI];
    int numero_libri;
} Categoria;

// Funzione per aggiungere un libro a una categoria
void aggiungiLibro(Categoria *categoria, char titolo, char autore, int anno, float prezzo) {
    if (categoria->numero_libri < MAX_LIBRI) {
        Libro nuovo_libro;
        strcpy(nuovo_libro.titolo, titolo);
        strcpy(nuovo_libro.autore, autore);
        nuovo_libro.anno = anno;
        nuovo_libro.prezzo = prezzo;
        
        categoria->libri[categoria->numero_libri] = nuovo_libro;
        categoria->numero_libri++;
    } else {
        printf("Categoria piena, impossibile aggiungere altri libri.\n");
    }
}

// Funzione per stampare tutti i libri di una categoria
void stampaLibriCategoria(Categoria *categoria) {
    printf("Libri nella categoria '%s':\n", categoria->nome);
    for (int i = 0; i < categoria->numero_libri; i++) {
        Libro libro = categoria->libri[i];
        printf("Titolo: %s, Autore: %s, Anno: %d, Prezzo: %.2f\n",
               libro.titolo, libro.autore, libro.anno, libro.prezzo);
    }
}

// Funzione per cercare un libro in base al titolo
void cercaLibroPerTitolo(Categoria *categorie, int num_categorie, char *titolo_cercato) {
    for (int i = 0; i < num_categorie; i++) {
        Categoria categoria = categorie[i];
        for (int j = 0; j < categoria.numero_libri; j++) {
            if (strcmp(categoria.libri[j].titolo, titolo_cercato) == 0) {
                printf("Libro trovato!\nTitolo: %s, Autore: %s, Categoria: %s, Anno: %d, Prezzo: %.2f\n",
                       categoria.libri[j].titolo, categoria.libri[j].autore,
                       categoria.nome, categoria.libri[j].anno, categoria.libri[j].prezzo);
                return;
            }
        }
    }
    printf("Libro con il titolo '%s' non trovato.\n", titolo_cercato);
}

// Funzione per stampare tutti i libri di tutte le categorie
void stampaTuttiLibri(Categoria *categorie, int num_categorie) {
    for (int i = 0; i < num_categorie; i++) {
        stampaLibriCategoria(&categorie[i]);
        printf("\n");
    }
}

int main() {
    // Definizione di alcune categorie
    Categoria categorie[MAX_CATEGORIE];
    
    // Inizializzo alcune categorie
    strcpy(categorie[0].nome, "Narrativa");
    categorie[0].numero_libri = 0;

    strcpy(categorie[1].nome, "Saggistica");
    categorie[1].numero_libri = 0;

    strcpy(categorie[2].nome, "Scienze");
    categorie[2].numero_libri = 0;

    FILE *libriCSV = fopen("libreria_libri.csv", "r");
    if (libriCSV == NULL) {
        printf("Errore nell'aprire il file");
        return 1;
    }

    char buffer[MAX_BUFF]; // Buffer per memorizzare ogni riga
    int i = 0; //per salvare nel buffer nell'ordine giusto, titolo, autore, anno e prezzo
    // Legge il file riga per riga
    while (fgets(buffer, MAX_BUFF, libriCSV)) {
        // Rimuove il carattere di nuova riga se presente
        buffer[strcspn(buffer, "\n")] = 0;

        // Estrai ogni valore separato da virgola
        char *value = strtok(buffer, ",");
        i++;
        while (value != NULL) {
            value = strtok(NULL, ",");
            strcpy(&buffer[i], value);
        }
        aggiungiLibro(&categorie[i],&buffer[1],&buffer[2],(int)buffer[3], (float)buffer[4]);
        printf("\n"); // Nuova riga dopo aver letto tutti i valori di una riga
    }
    int stop;
    scanf("%d",&stop);

    int num_categorie = 3;
        //aggiungiLibro(categoria, titolo, autore, anno, prezzo);
    
    stampaLibriCategoria(&categorie[0]);
    
    cercaLibroPerTitolo(categorie, num_categorie, "1984");

    stampaTuttiLibri(categorie, num_categorie);
    fclose(libriCSV);
    return 0;
}
