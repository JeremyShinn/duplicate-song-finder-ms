#define _POSIX_C_SOURCE 200809L

#include <stdio.h>     // for getline, fopen, printf
#include <stdlib.h>    // for calloc, free, atoi
#include <string.h>    // for strtok, strdup

#include "processfile.h"

//free all the lists
void freeSong(void* element) {
    Song* song = (Song*)element;
    free(song->SongName);
    free(song->Artist);
    free(song);
}

// free a List of Songs
void freeSongList(List* list) {
    Node* current = list->firstElement;
    while (current) {
        Node* next = current->nextElement;
        freeSong(current->element);
        free(current);
        current = next;
    }
    free(list);
}

// free a single Playlist
void freePlaylist(void* element) {
    Playlist* playlist = (Playlist*)element;
    free(playlist->PlaylistName);
    freeSongList(playlist->Songs);
    free(playlist);
}

// free a List of Playlists
void freePlaylistList(List* list) {
    Node* current = list->firstElement;
    while (current) {
        Node* next = current->nextElement;
        freePlaylist(current->element);
        free(current);
        current = next;
    }
    free(list);
}

// free a single User
void freeUser(void* element) {
    User* user = (User*)element;
    free(user->UserName);
    freePlaylistList(user->Playlists);
    free(user);
}

// free the top‐level List of Users
void freeStuff(List* users) {
    Node* current = users->firstElement;
    while (current) {
        Node* next = current->nextElement;
        freeUser(current->element);
        free(current);
        current = next;
    }
    free(users);
}

//populate songs
Song* populateSong(FILE* dataFile, char* line, size_t lineBuffer){
    Song* song = (Song*)calloc(1, sizeof(Song));
    getline(&line, &lineBuffer, dataFile);

    char* songName = strtok(line, ";");
    char* artist = strtok(NULL, ";");

    song->SongName = (char*) malloc(sizeof(char) * (strlen(songName) + 1));
    songName[strcspn(songName, "\n")] = 0;
    strcpy(song->SongName, songName);

    song->Artist = (char*) malloc(sizeof(char) * (strlen(artist)) + 1);
    artist[strcspn(artist, "\n")] = 0;
    strcpy(song->Artist, artist);

    return song;
}

Node* populateSongs(FILE* dataFile, char* line, size_t lineBuffer, int numSongs){
    Node* firstSong = (Node*)calloc(1, sizeof(Node));
    firstSong->element = populateSong(dataFile, line, lineBuffer);
    firstSong->nextElement = NULL;

    Node* currentSong = firstSong;
    Node* nextSong = NULL;

    int i;
    for(i = 0; i < numSongs - 1; i++){
        nextSong = (Node*)calloc(1, sizeof(Node));
        nextSong->element = populateSong(dataFile, line, lineBuffer);
        nextSong->nextElement = NULL;
        currentSong->nextElement = nextSong;
        currentSong = nextSong;
    }
    return firstSong;
}

//populate playlists
Playlist* populatePlaylist(FILE* dataFile, char* line, size_t lineBuffer){
    Playlist* playlist = (Playlist*)calloc(1, sizeof(Playlist));
    playlist->Songs = (List*)calloc(1, sizeof(List));
    getline(&line, &lineBuffer, dataFile);

    char* playlistName = strtok(line, ";");
    char* numSongs = strtok(NULL, ";");

    playlist->PlaylistName = (char*) malloc(sizeof(char) * (strlen(playlistName) + 1));
    strcpy(playlist->PlaylistName, playlistName);

    playlist->Songs->numElements = atoi(numSongs);

    playlist->Songs->firstElement = populateSongs(dataFile, line, lineBuffer, playlist->Songs->numElements);
    return playlist;
}

Node* populatePlaylists(FILE* dataFile, char* line, size_t lineBuffer, int numPlaylists){
    Node* firstPlaylist = (Node*)calloc(1, sizeof(Node));
    firstPlaylist->element = populatePlaylist(dataFile, line, lineBuffer);
    firstPlaylist->nextElement = NULL;

    Node* currentPlaylist = firstPlaylist;
    Node* nextPlaylist = NULL;

    int i;
    for(i = 0; i < numPlaylists - 1; i++){
        nextPlaylist = (Node*)calloc(1, sizeof(Node));
        nextPlaylist->element = populatePlaylist(dataFile, line, lineBuffer);
        nextPlaylist->nextElement = NULL;
        currentPlaylist->nextElement = nextPlaylist;
        currentPlaylist = nextPlaylist;
    }
    return firstPlaylist;
}

//populate the users
User* populateUser(FILE* dataFile, char* line, size_t lineBuffer){
    User* user = (User*)calloc(1, sizeof(User));
    user->Playlists = (List*)calloc(1, sizeof(List));
    getline(&line, &lineBuffer, dataFile);

    char* userName = strtok(line, ";");
    char* numPlaylists = strtok(NULL, ";");

    user->UserName = (char*) malloc(sizeof(char) * (strlen(userName) + 1));
    strcpy(user->UserName, userName);

    user->Playlists->numElements = atoi(numPlaylists);

    user->Playlists->firstElement = populatePlaylists(dataFile, line, lineBuffer, user->Playlists->numElements);
    return user;
}

Node* populateUsers(FILE* dataFile, char* line, size_t lineBuffer, int numUsers){
    Node* firstUser = (Node*)calloc(1, sizeof(Node));
    firstUser->element = populateUser(dataFile, line, lineBuffer);
    firstUser->nextElement = NULL;

    Node* currentUser = firstUser;
    Node* nextUser = NULL;

    int i;
    for(i = 0; i < numUsers - 1; i++){
        nextUser = (Node*)calloc(1, sizeof(Node));
        nextUser->element = populateUser(dataFile, line, lineBuffer);
        nextUser->nextElement = NULL;
        currentUser->nextElement = nextUser;
        currentUser = nextUser;
    }
    return firstUser;
}

//process the file
List* processFile(char* fileName){
    //open the file in read only
	FILE* dataFile = fopen(fileName, "r");
    //error handle
	if (!dataFile) {
		printf("Couldn't find the file: %s\n", fileName);
		return NULL;
	}

    //now that we successfully opened it, let them know we're processing it
    printf("Processing: %s\n", fileName);
    
    //initialize user list
    List* users = (List*)calloc(1, sizeof(List));
    users->numElements = 0;
    users->firstElement = NULL;

    //make the line to store the line we will take in and the lineBuffer
    char* line = NULL;
	size_t lineBuffer = 0;

    //get the number of users
    getline(&line, &lineBuffer, dataFile);
    users->numElements = atoi(line);

    //populate the users
    users->firstElement = populateUsers(dataFile, line, lineBuffer, users->numElements);

    //free stuff
    free(line);
    fclose(dataFile);
    return users;
}