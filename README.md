<h1 align="center" style="font-weight: 700">Chat_chat | 聊天軟體 </h1>

<br/>



<br>

## 設計沿革

此服務旨在提供使用者自行部署的小單位聊天室。
<br>
## 功能清單

- 伺服器 IP 位址輸入頁面
- 聊天頁面

<br>

## 建議體驗流程

- 1. 於伺服端電腦執行 server.exe，並查明 IP 位址，開放防火牆
- 2. 於客戶端電腦執行 client.exe，並連線至伺服端電腦 IP 位址，開放防火牆
- 3. 傳送訊息
- 4. [Demo](https://i.imgur.com/HQbrqeX.gif)
     
<br>

## 程式執行環境

- 編譯環境 ：
  1. Windows 7 以上作業系統
  2. 安裝 mingw [點我前往安裝頁面](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/sjlj/x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0.7z/download)
  3. 解壓縮後，將 mingw64/bin/ 設定為環境變數
  4. 執行 build.bat
- 執行 :
  1. server.exe 伺服端執行檔
  2. client.exe 客戶端執行檔
  3. client.exe 上限為五人，可以在 socket_configure.h 修改 connection_limit 變數
  4. 連接 port 號為 5555，可以在 socket_configure.h 修改
  5. 目前不支援中文訊息

