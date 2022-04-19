# AtObjects

Low-level library containing extensive class definitions and implementations required for a video game engine written in C++.

Contains the following classes:
- **Animation:** Structure used to describe a 2D tile animation.
- **Interactable:** Structure used to describe an interactive 2D object within the application.
- **List:** Template class used to implement a generic linked list. Used extensively throughout the project with different classes.
- **Object:** Structure used to describe a basic game object within the application.
- **ObjectManager:** Template class used to load and manage all game objects. 
- **Renderable:** Structure used to desribe a renderable and animated 2D object.
- **Texture:** Structure used to describe, load, and manipulate 2D graphics within the application.
- **TextureManager:** Class used to load and manage all texture objects.
- **Lua API:** Extensive API for many of the classes defined in the library. Allows run-time interaction and manipulation of objects handled by the library using external scripts written in Lua.
