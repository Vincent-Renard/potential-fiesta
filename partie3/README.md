# Sprint prog parallele

## Composition du groupe
  - Romain Guidez
  - Vincent Renard

## Partie 1
Nous avons réalisé les fonctions Partitionnement en parallèle et SommePrefixe.
## Partie 2
Tentative vaine
## Partie 3
Seul le sum left est calculé localement , le proc-root connait la taille du vecteur contenu sur le proc_i donc sum_right[proc_i] = send[proc_i]-sum_left[proc_i]
chaque proc envoie sum_left[proc] element (les elements plus petits que le pivot)
