import zmq

print("Attempting to connect...", end=" ")
socket = zmq.Context().socket(zmq.REQ)
socket.connect("tcp://localhost:5555")
print("Success!\n")

message = "Data.txt"
print(f"Sending the following file path: {message}")
socket.send_string(message)
message = socket.recv()
print(f"Received the following message: {message.decode()}")

zmq.Context().destroy()