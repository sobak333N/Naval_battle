import socket
import os

print('I STARTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT')


socket_path = '/tmp/unix_start2.server'
try:
    print('start2 ' , socket_path)
    os.unlink(socket_path)
except OSError:
    if os.path.exists(socket_path):
        raise
server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
server.bind(socket_path)
server.listen(1)
print('Server is listening for incoming connections...')
connection, client_address = server.accept()
try:
    # print('Connection from', client_address)
    # while True:
    text = '_'
    while True and text[-1]!='F':
        data = connection.recv(1024)
        if not data:
            break
        print('tmp data:', data.decode())
    # if not data:
        # break
        text += data.decode()
        
    print('Received data:',text)
    
    answer = '''<html>
<head><title>GAME STARTED!</title></head>
<body>\n'''
    # left = 0 
    # right = 0
    left = 0
    right = 0
    while text[right]!=' ':
        right+=1

    answer+= '<a>'
    answer+= 'YOU PLAY WITH '
    answer+= text[left:right]
    answer+= '</a>\n'
    answer+='<br>'
    i = 0
    count = 0
    cur = right

    while i < 10 and cur < len(text):
        answer+= '<a>'
        count = 0
        while count < 20 and cur < len(text):
            if text[cur] == '0':
                answer+='_'
            elif text[cur] == '1':
                answer+='#'
            if count == 10:
                answer+='|||'
            cur+=1
            count+=1
        i+=1
        answer+='</a>\n'
        answer+='<br>'


    answer+='</pre><hr></body>\n</html>\n'
    # print('html code', answer)

    # response = 'Hello from the server!'
    connection.sendall(answer.encode())
finally:
    connection.close()
    os.unlink(socket_path)