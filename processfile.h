#ifndef PROCESSFILE_H
#define PROCESSFILE_H

#include <stdio.h>    // for FILE, size_t
#include <stdlib.h>   // for malloc/free
#include <string.h>   // for strdup if you use it

typedef struct Node {
    void* element;
    struct Node* nextElement;
} Node;

typedef struct List {
    Node* firstElement;
    int   numElements;
} List;

// … other structs you need publicly …
typedef struct Song {
    char* SongName;
    char* Artist;
} Song;
typedef struct Playlist {
    char*   PlaylistName;
    List*   Songs;
} Playlist;
typedef struct User {
    char* UserName;
    List* Playlists;
} User;

// Top-level process function
List* processFile(char* fileName);

// Clean-up function
void freeStuff(List* users);
void freeSong(void* element);


#endif