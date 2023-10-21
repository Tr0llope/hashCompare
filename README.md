# hashCompare
L'utilité de ce programme est la suivante: hashCompare permet de générer une rainbow table à partir d'un dictionnaire puis de comparer des hashs avec cette table pour en retrouver le mot de passe équivalent.
## Contenu
Voici le contenu du projet:
```
.
├── README.md
└── src
    ├── Dockerfile
    ├── hash.c
    ├── hashes_input.txt
    ├── hash.h
    ├── dico.txt
    ├── main.c
    ├── Makefile
    ├── rainbow_table.txt
    ├── t3c_struct.c
    └── t3c_struct.h

```
Les fichiers txt sont présents à titre d'exemple, vous pouvez utiliser les votres pourvu que vous respectiez leur format.

## Compilation
La compilation se fait via le Dockerfile.
Voici les commandes a exécuter pour créer un environnement d'exécution du programme:
```
docker build -t hash_compare .

docker run -it hash_compare
```
## Exécution
Une fois dans le conteneur, vous avez accès au programme de comparaison de hashs.
Il propose deux modes pouvant être lancés successivement de cette manière:
#### Mode Génération: 
En utilisant l'entrée standard:
```
cat dico.txt | ./hashCompare -G rainbow_table.txt
```
Avec seulement des arguments:
```
./hashCompare -G rainbow_table.txt dico.txt
```
où rockyou.txt est un dictionnaire de mots de passe stockés en clair et rainbow_table.txt le fichier contenant les condensats générés.

#### Mode Comparaison: 
En utilisant l'entrée standard:
```
cat hashes_input.txt | ./hashCompare -L rainbow_table.txt
```
Avec seulement des arguments:
```
./hashCompare -L rainbow_table.txt hashes_input.txt
```
où hashes_input.txt est le fichier qui contient les hashs dont on souhaite connaitre la correspondance en clair.
