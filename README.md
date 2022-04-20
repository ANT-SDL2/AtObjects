# AtObjects

Low-level library containing the fundamental class implementations for interactive and renderable objects used in a 2D video game engine. Manager classes used to load and manage these objects are also implemented in this library.

## Classes
- **Object:** Base structure used to implement a game object within the application.
- **Interactable:** Structure used to describe an interactive 2D object within the application.
- **Texture:** Structure used to describe, load, and manipulate 2D graphics within the application.
- **Renderable:** Structure used to desribe a renderable and animated 2D object.
- **Animation:** Structure used to implement a 2D tile animation.
- **List:** Template class used to implement a generic linked list. Used extensively throughout the project with different classes.
- **ObjectManager:** Template class used to load and manage all game objects. 
- **TextureManager:** Class used to load and manage all texture objects.
- **API:** API exposing many of the classes and methods defined in the library to a Lua environment. Allows run-time interaction and manipulation of objects created by the library using scripts written in Lua.

## Dependencies:
- [AtUtility](https://github.com/atrapalis/AtUtility "AtUtility GitHub repository")
- Lua 5.2 binding library (lua52)
- OpenGL 3.2 (glu32, opengl32)
- SDL2 base and modules (SDL2, SDL2_image, SDL2_ttf)
