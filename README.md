# Projet-Dames

Il s’agit ici de realiser un programme qui joue aux Dames.

Les regles de jeu considerees sont les suivantes : 
  - Un pion n’avance que vers l’avant et d’une seule case. 
  - Une dame se deplace dans tous les sens et aussi loin qu’elle peut.
  - Une prise par un pion ne menace qu’un voisin immediat. 
  - La premiere prise par un pion ne va que vers l’avant, mais les prises suivantes peuvent aller dans tous les sens. 
  - Une dame prend d’emblee dans tous les sens et a toutes distances. Elle atterrit immediatement
    derriere la piece prise. 
  - Des qu’une prise est possible, elle est obligatoire et il faut continuer la sequence de prises tant qu’il en reste. 
    Ceci est une simplification par rapport a la regle classique du ”souffler n’est pas jouer”.
    
Le programme propose met en place un parcours exhaustif sous forme de minimax
sans aucune optimisation du type alpha-beta. L'evaluation d’une situation finale
est rudimentaire et se limite au d´ecompte des pieces dans l’un et l’autre des camps,
sans tenir compte de la fragilite potentielle de certaines pieces. La partie est terminee
lorsqu’un camp n’a plus de pieces ou ne peut plus faire de mouvements.
