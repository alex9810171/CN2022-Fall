# NTU CSIE Computer Networks - 2022 Fall
## Socket Programming Project
- Name: Alex Hsu
- ID: R10525073
- E-mail: r10525073@g.ntu.edu.tw

## Environment
- gcc 9.4.0
- nodejs 10.19.0
- npm 6.14.4

## Instruction to run the code - Phase 1
1. Run the program:
```
./server
./client
./webserver
(for Linux, Ubuntu system)
```

## Instruction to run the code - Phase 2
1. Install nodejs and npm:
```
sudo apt install nodejs
sudo apt install npm
(for Linux, Ubuntu system)
```
2. Install npm package:
```
npm init --yes
npm install express socket.io fs
```

3. Run the program:
```
node server
```

## Note
- Our website: <https://140.112.26.143:1025/>
- Using IPv4 TCP connect
- socket() should use PF_INET, others should use AF_INET
- using 2048 key.
- <https://stackoverflow.com/questions/73725572/>

## Reference
- TCP Socket Programming 學習筆記: <http://zake7749.github.io/2015/03/17/SocketProgramming/>
- Linux c socket, client, server 透過網路傳送文字訊息: <https://wenchiching.wordpress.com/2009/10/11/linux-c-socketclientserver-%E9%80%8F%E9%81%8E%E7%B6%B2%E8%B7%AF%E5%82%B3%E9%80%81%E6%96%87%E5%AD%97%E8%A8%8A%E6%81%AF/>
- Linux環境用c++建立Socket連線: <https://snsd0805.github.io/jekyll/update/2019/05/27/%E7%AD%86%E8%A8%98-Linux%E7%92%B0%E5%A2%83%E7%94%A8c++%E5%BB%BA%E7%AB%8BSocket%E9%80%A3%E7%B7%9A.html>
- Linux平台C语言Socket编程练习之TCP套接字: <https://hackfun.org/2016/11/03/Linux%E5%B9%B3%E5%8F%B0C%E8%AF%AD%E8%A8%80Socket%E7%BC%96%E7%A8%8B%E7%BB%83%E4%B9%A0%E4%B9%8BTCP%E5%A5%97%E6%8E%A5%E5%AD%97/>
- Making a simple HTTP webserver in C: <https://bruinsslot.jp/post/simple-http-webserver-in-c/>
- 查port: `netstat -tulpn`
- 解決websocket連線問題: <https://stackoverflow.com/questions/44575978>