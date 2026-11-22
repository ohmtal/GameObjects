# Ohmtal Game Objects 

when I work on different Projects recreate some "wheels" over and over again. 
So i decided to create a reuable library. Starting with 2D Objects from ElfFlux.

Goal is i can only need to interface rendering and script bindings if needed 
and do not need to recreate/copy the logic behind again. 

## Porting Status namespace [OGO]

- Common 
    - [X] Math - only a BIT and some defines so far using cmath
    - [X] Data Structs - borrowed from OhmFlux and BaseElf
        
    
- 2D - Compile passed : only no function tests so far
    - [X] SceneObject2D
    - [X] SceneContainer2D
    - [X] SpriteObject
