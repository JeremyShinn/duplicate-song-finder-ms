#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#include "processfile.h"

void addSong(List* songs){
    printf("Enter a new title: ");

    // Flush leftover newline from previous input (e.g. scanf)
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // flush stdin

    char* title = (char*)calloc(50, sizeof(char));
    fgets(title, 50, stdin);
    title[strcspn(title, "\n")] = 0;

    printf("Enter a new artist: ");

    char* artist = (char*)calloc(50, sizeof(char));
    fgets(artist, 50, stdin);
    artist[strcspn(artist, "\n")] = 0;

    Song* newSong = (Song*)calloc(1,sizeof(Song));

    newSong->SongName = (char*)malloc(strlen(title) + 1);
    strcpy(newSong->SongName, title);

    free(title);

    newSong->Artist = (char*)malloc(strlen(artist) + 1);
    strcpy(newSong->Artist, artist);

    free(artist);

    if (songs->firstElement == NULL) {
        songs->firstElement = calloc(1, sizeof(Node));
        songs->firstElement->element     = newSong;
        songs->firstElement->nextElement = NULL;
        songs->numElements = 1;
        return;
    }

    Node* node = songs->firstElement;
    while(node->nextElement != NULL){
        node = node->nextElement;
    }
    Node* newNode = (Node*)calloc(1, sizeof(Node));
    newNode->element = newSong;
    newNode->nextElement = NULL;
    node->nextElement = newNode;
    songs->numElements++;
    return;
}

void* getElement(List* list, int index){
    Node* node = list->firstElement;
    int i;
    for(i = 1; i <= list->numElements; i++){
        if(i == index){
            return node->element;
        }
        node = node->nextElement;
    }
    return NULL;
}

void deleteElement(List* list, int index){
    Node* current = list->firstElement;
    Node* next = current->nextElement;
    if(index == 1){
        list->firstElement = next;
        free(current);
        list->numElements--;
        return;
    }
    else{
        int i;
        for(i = 2; i <= list->numElements; i++){
            if(i == index){
                current->nextElement = next->nextElement;
                free(next);
                break;
            }
            current = next;
            next = next->nextElement;
        }
    }
    list->numElements --;
    return;
}

void editSong(Song* selectedSong){
    printf("\nTitle: %s\n", selectedSong->SongName);
    printf("(Leave blank to remain the same)\n");
    printf("Enter a new title: ");

    // Flush leftover newline from previous input (e.g. scanf)
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // flush stdin

    char* title = (char*)calloc(50, sizeof(char));
    fgets(title, 50, stdin);
    title[strcspn(title, "\n")] = 0;

    printf("\nArtist: %s\n", selectedSong->Artist);
    printf("(Leave blank to remain the same)\n");
    printf("Enter a new artist: ");

    char* artist = (char*)calloc(50, sizeof(char));
    fgets(artist, 50, stdin);
    artist[strcspn(artist, "\n")] = 0;

    if (strcmp(title, "") != 0) {
        free(selectedSong->SongName);
        selectedSong->SongName = (char*)malloc(strlen(title) + 1);
        strcpy(selectedSong->SongName, title);
    }
    free(title);

    if (strcmp(artist, "") != 0) {
        free(selectedSong->Artist);
        selectedSong->Artist = (char*)malloc(strlen(artist) + 1);
        strcpy(selectedSong->Artist, artist);
    }
    free(artist);
    return;
}

void selectSong(List* songs, int songNumber){
    Song* song = (Song*)getElement(songs, songNumber);
    printf("\nWhat would you like to do with \"%s\"?\n\n", song->SongName);
    printf("1. Edit song\n");
    printf("2. Delete song\n\n");
    printf("Select an option: ");
    int input = 0;
    scanf("%d", &input);
    if(input == 1){
        editSong(song);
        return;
    }
    if(input == 2){
        deleteElement(songs, songNumber);
        freeSong(song);
        return;
    }
    return;
}

//print the user info
void printPlaylist(Playlist* Playlist){
    int songNumber = 0;
    List* songs = Playlist->Songs;
    while(songNumber != songs->numElements + 2){
        printf("\n%s playlist:\n\n", Playlist->PlaylistName);
        int i;
        for(i = 1; i <= songs->numElements; i++){
            Song* song = (Song*) getElement(songs, i);
            printf("%d: Title: %s, Artist: %s\n", i, song->SongName, song->Artist);
        }
        printf("%d. Add song\n", songs->numElements + 1);
        printf("%d. Exit\n", songs->numElements + 2);
        printf("\nEnter song number: ");
        scanf("%d", &songNumber);
        if (songNumber > 0 && songNumber <= songs->numElements){
            selectSong(songs, songNumber);
        }
        if (songNumber == songs->numElements + 1){
            addSong(songs);
        }
    }
    return;
}

//print the user info
void printUser(User* user){
    int playlistNumber = 0;
    List* playlists = user->Playlists;
    while(playlistNumber != playlists->numElements + 1){
        printf("\n%s's Playlists:\n\n", user->UserName);
        int i;
        for(i = 1; i <= playlists->numElements; i++){
            Playlist* playlist = (Playlist*) getElement(playlists, i);
            printf("%d: %s\n", i, playlist->PlaylistName);
        }
        printf("%d. Exit\n", playlists->numElements + 1);
        printf("\nEnter playlist number: ");
        scanf("%d", &playlistNumber);
        if (playlistNumber > 0 && playlistNumber <= playlists->numElements){
            Playlist* selectedPlaylist = (Playlist*) getElement(playlists, playlistNumber);
            printPlaylist(selectedPlaylist);
        }
    }
    return;
}

//main lol
int main() {
    char* fileName = "Data.txt";
    List* users = processFile(fileName);
    int userNumber = 0;
    while (userNumber != users->numElements + 1){
        int i;
        printf("\nUsers:\n\n");
        for(i = 1; i <= users->numElements; i++){
            User* user = (User*) getElement(users, i);
            printf("%d. %s\n", i, user->UserName);
        }
        printf("%d. Exit program\n", users->numElements + 1);
        printf("\nEnter user number: ");
        scanf("%d", &userNumber);
        if (userNumber > 0 && userNumber <= users->numElements){
            User* selectedUser = (User*) getElement(users, userNumber);
            printUser(selectedUser);
        }
    }

    freeStuff(users);
	return 0;
}