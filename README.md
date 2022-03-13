Dominique Michel et Lea Masselles

# Comment utiliser ?
Compiler le projet en utilisant cmake
```bash
42sh$ cmake -B build_folder -DCMAKE_BUILD_TYPE=Debug
```
Lancer le projet dans le dossier res
```bash
42sh$ cd res && cp ../build_folder/myOpenGl . && ./myOpenGl
```

# Les deplacements
- *WASD* pour se deplacer
- *ESPACE* pour aller vers le haut
- *CTRL* pour aller vers le bas
- *MOLETTE DE LA SOURIS* pour controler la vitesse de deplacement
  
- *ENTER* pour activer / désactiver les feux d'artifices (old)
- *P* pour afficher la position de la caméra
- *O* pour ctiver / désactiver le SSAO
- *F* pour activer / désactiver le depth of field (variable dans le shader dop_pass.glsl)
  focusSettings = vec3(-30, 15, 5) où
  x = position du point focal
  y = demi-largeur de la zone claire
  z = largeur de la zone de transition flou / claire
