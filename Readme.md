Type de jeu : rpg linéaire, combats au tour par tour (tout comme pokemon en fait)

Le jeu commence dans un village avec des pnj qui t'expliquent l'histoire
    Les pnj ont leur anim, ils se déplacent d'UNE case toutes les qlq secondes,
    et ont une limite quant à jusqu'où ils peuvent aller
    chaque pnj à un dialogue attribué qui se déclenche lorsque le joueur intéragit avec lui
    Il y a quelques types de pnj différents (pour pas avoir à en dessiner 3000) avec chacun son animation

On peut rentrer dans les maisons (certaines), il y en a genre 4-5
    et dans chaque maison il y a quelques pnj aussi

Le joueur apprend qu'il doit partir à l'aventure pour X raisons
donc il sort du village et arrive dans une "zone à mobs"
    dans une zone à mobs il y a ... des mobs
    chaque ennemi a une animation et ne bouge pas (contrairement aux pnj)
    Mais si le joueur arrive dans une certaine zone autour de cet ennemi alors il commence à poursuivre le joueur
    les ennemis sont + rapides que le joueur pour toujours l'attraper
    si le joueur entre en contact avec un ennemi alors un combat se déclenche avec cet ennemi

Le 2ème "état possible" du jeu est donc la phase de combat
    Le joueur est face à l'ennemi (en vue de côté, joueur à gauche ennemi à droite)
    tout à droite de l'ecran il y a les différentes actions (attaques) que le joueur peut faire
    et tout en bas de l'écran la boîte de dialogue qui décrit ce qui se passe (kaki inflige 5 points de dgt !)
    entre l'écran avec les 2 persos (qui prend la plupart de l'ecran) et la boite de dialogue y'a les points de vie des deux persos
    
    Lorsque le tour du joueur arrive il peut choisir une de ses 4 attaques, kaki fera donc son annimation en fonction de cette attaque
    et l'ennemi subira un certain nombre de points de dégâts en fonction de l'atq du joueur, la capacité utilisée etc etc etc
    Ensuite arrive le tour de l'ennemi, qui fait une attaque au hasard (il a aussi une animation)
    chaque type de mob à les 2 (3 pour les boss ?) mêmes attaques
    Et le joueur perd un certain nombre de pv en fonction du niveau de l'ennemi (et sa DEF ? jsp s'il y a une stat DEF)

    Si les pv du joueur arrivent à 0 il meurt, et respawn au point de spawn le plus proche
    Si l'ennemi meurt le combat se termine, l'ennemi disparaît de la map et le joueur reçoit un certain nombre de points d'EXP
    
Ces points d'EXP font augmenter la "jauge d'exp" en dessous des PV du joueur (comme pokemon)
    lorsque la jauge est pleine le joueur augmente de niveau
    Ça lui augmente un peu toutes ses stats, et il reçoit quelques points "bonus", qu'il peut répartir entre ses stats

    Exemple :
        Kaki passe niveau 3 ! elle gagne +10 PVmax, +3 AtqMelee, +3 AtqMagie, +3 VIT
        elle gagne aussi 5 points bonus, le joueur choisit de se rajouter +4 AtqMelee et +1 VIT par exemple
        et donc le perso a gagné +7 AtqMelee, +3 AtqMagie, +4 VIT
        (bon voilà c'est un exemple, les stats seront sûrement complètement différentes mais c'est pour expliquer les points bonus)

    Après un certain nombre de niveaux, le joueur peut apprendre une nouvelle attaque, au choix aussi
    Ce choix déterminera le build vers lequel le joueur veut s'orienter

    Exemple :
        Kaki passe niveau 10 ! Le joueur a le choix entre "Boule de feu" et "Coup de hache"
        Si il choisit "Boule de feu" alors il aura choisi la voie "sorcier", sinon la voie "guerrier"
        et il ne pourra plus changer de voie jusqu'à la fin, il restera sorcier
        [...]
        Kaki passe niveau 15 ! Le joueur a le choix entre "Glaciation" et "Vol d'âme" (??)
        Ça le fait donc choisir entre "sorcier de glace" et "démoniste"

        et donc à chaque sauvegarde (parce que tu pourras recommencer plusieurs parties sans perdre les anciennes) t'as un build différent
        exemples :
            Sorcier < Démoniste < Nécromancien
            Guerrier < Assassin < Ninja

    et donc il y aurait une attaque pour chaque spécialisation +1 pour la dernière quand t'es super badass
    (donc ça fait 22 attaques (donc anims) différentes à faire x^x)

Les ennemis disparaissent une fois vaincus donc on peut pas les "farmer" dans les zones à mobs
Les zones à mobs sont séparées par des villages avec des pnj, dialogues tout ça tout ça et ça fait progresser l'histoire
    (qui sera pas trop longue pour pas que ça devienne chiant)
Et chaque zone à mobs à un thème différent (plaine, désert, neige etc etc) avec des obstacles différents

À partir du 2ème village tu débloques une zone de farm, où le joueur pourra farmer des mobs dans des hautes herbes comme pokemon
    Comme ça il gagne plein d'EXP et peut améliorer de ouf son perso avant de retourner à l'aventure
    Donc les zones à mobs deviennent très dures pour forcer le joueur à farmer ;
    elles ne servent pas à farm mais à mettre au défi le joueur
    Et il y aura un "mini-boss" ou "gros mob" à la fin de chaque zone à mobs

Je sais pas combien de zones il y aura, mais je dirais qu'il y aura au total 10 mobs différents ? Donc 25 animations d'attaque + 10 pour la map
    Bref donc bonne chance aux graphistes, de toute façon vous aurez le temps, le projet va durer loooongtemps

Et à la fin un boss final, bref tout ça on verra parce que ce sera dans longtemps

Les maps seront définies dans un fichier .kaki dans les fichiers du jeu, et le programme saura convertir le fichier en une vraie map
    Par contre il faut que la map soit chargée, (pas comme le premier kaki project qui était "vide")
    Genre si tu regardes pokemon y'a des trucs partout, des arbres des chemins des barrières etc etc
    Donc y'aura un travail de level design à faire

*** Idées de trucs à faire ***

- Le perso est personnalisable
- Squelette de dragon géant