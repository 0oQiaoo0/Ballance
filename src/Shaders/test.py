import socket

UDP_IP = '127.0.0.1'  # 服务器IP地址
UDP_PORT = 8811  # 服务器端口号

# 创建UDP套接字
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

local_ip = '127.0.0.1'  # 本地 IP 地址
local_port = 8888  # 本地端口号
sock.bind((local_ip, local_port))
while True:
    message = input("请输入消息：")
    
    # 发送消息到服务器
    sock.sendto(message.encode(), (UDP_IP, UDP_PORT))
    
    # 接收服务器的响应
    data, addr = sock.recvfrom(1024)
    print("来自服务器的响应：", data.decode())

# 关闭套接字
sock.close()
