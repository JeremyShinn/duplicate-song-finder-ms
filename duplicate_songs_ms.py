import zmq
import time

def remove_duplicates(filepath):
    duplicates_removed = 0
    data = []

    with open(filepath, "r") as file:
        data = file.readlines()

    with open(filepath, "w") as file:
        i = 1
        file.write(data[0])
        num_users = int(data[0].strip("\n"))
        for user in range(num_users):
            num_playlists = int(data[i].strip("\n").split(";")[1])
            file.write(data[i])
            i += 1
            for playlist in range(num_playlists):
                playlist_data = data[i].strip("\n").split(";")
                songs = []
                num_songs = int(playlist_data[1])
                new_num = num_songs
                i += 1
                for song in range(num_songs):
                    if data[i] in songs:
                        new_num -= 1
                        duplicates_removed += 1
                    else:
                        songs.append(data[i])
                    i += 1
                file.write(playlist_data[0] + ";" + str(new_num) + "\n")
                file.writelines(songs)
        file.truncate()
    return duplicates_removed

print("Preparing connection...", end=" ")
socket = zmq.Context().socket(zmq.REP)
socket.bind("tcp://*:5555")
print("Success!\n")

while True:
    message = socket.recv()
    file_path = message.decode()
    print(f"Message received with the following file path: {file_path}\n")
    time.sleep(5)
    print(f"Removing duplicate songs from: {file_path}\n")
    duplicates = str(remove_duplicates(file_path))
    time.sleep(5)
    print(f"{duplicates} duplicates removed!\n")
    time.sleep(5)
    print(f"Sending the following message back: {duplicates}")
    socket.send_string(duplicates)




