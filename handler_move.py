import socket
import os

print('I STARTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT')


socket_path = '/tmp/unix_handler_move1.server'
try:
    print('handling move1 ' , socket_path)
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
    
    # if text[-2] not in nums: 
    print('Received data:',text)
    answer = '''<html>
<head><title>MOVE!</title></head>
<body>\n'''
    # left = 0 
    # right = 0
    left = 0
    right = 0
    while text[right]!=' ':
        right+=1

    if(text[-2] != 'A'):
        answer+= '<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">'
        answer+= 'YOU PLAY WITH '
        answer+= text[left:right]
        answer+= '</a>\n'
        answer+='<br>'
    if text[-2] == 'A':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">YOU ARE NOT PLAYING WITH'
        answer+=text[left:right]
        answer+= '</a>'
        answer+='<br>'
    if text[-2] == 'B':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">YOU HAVE ALLREADY SHOT IN THIS POINT</a><br>'
    if text[-2] == 'C':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">IT IS NOT UR TURN TO MOVE ! WAIT FOR MOVE FROM YOUR OPPONENT</a><br>'
    if text[-2] == 'K':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">YOU KILL ENEMYS SHIP</a><br>'
    if text[-2] == 'E':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">U HAVE KILLED ALL EMEMYS SHIPS !</a><br>'
        answer+='<a>U WON!</a><br>'
    if text[-2] == 'L':
        answer+='<a>ALL YOUR SHIPS WERE KILLED !</a><br>'
        answer+='<a>U LOST!</a><br>'
    i = 0
    count = 0
    cur = right+1
    answer+='<table border="1">'
    answer+='<caption>FIELD</caption>'
    answer+='<tr height="20">'
    answer+='<td width="20"></td>'
    for i in range(10):
        answer+='<td width="50">'
        answer+=str(i)
        answer+='</td>'
    answer+='<td width="20"></td>'
    for i in range(10):
        answer+='<td width="50">'
        answer+=str(i)
        answer+='</td>'
    answer+='</tr>'
    i = 0
    while i < 10 and cur < len(text):
        answer+='<tr height="50">'
        answer+='<td width ="20" bgcolor="#FFFFFF">'
        answer+=str(i)
        answer+='</td>'
        # answer+= '<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">'
        count = 0
        while count < 10 and cur < len(text):
            answer+='<td width ="50" bgcolor="'
            if text[cur] == '0':
                answer+='#E0FFFF"'
            elif text[cur] == '1':
                answer+='#181721"'
            elif text[cur] == '2':
                answer+='#848CE9"'
            elif text[cur] == '3':
                answer+='#F25015"'
            answer+='></td>'
            cur+=1
            count+=1
        count = 0
        answer+='<td width ="20" bgcolor="#FFFFFF"></td>'
        while count < 10 and cur < len(text):
            answer+='<td width ="50" bgcolor="'
            if text[cur] == '0':
                answer+='#E0FFFF"'
            elif text[cur] == '1':
                answer+='#181721"'
            elif text[cur] == '2':
                answer+='#848CE9"'
            elif text[cur] == '3':
                answer+='#F25015"'
            answer+='></td>'
            cur+=1
            count+=1
        
        i+=1
        answer+='</tr>\n'

    answer+='</table>\n'

    answer+='</pre><hr></body>\n</html>\n'
    # print('html code', answer)

    # response = 'Hello from the server!'
    connection.sendall(answer.encode())
    # connection.close()
    # os.unlink(socket_path)
finally:
    connection.close()
    os.unlink(socket_path)