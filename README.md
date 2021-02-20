# webserv

## jpgファイルのダウンロード
```
cd prototype_jpg
make clean
make
./server
```
別のターミナルを起動して
```
curl -O http://127.0.0.1:5000/1.jpg -v -0
```
1.jpgがダウンロードされる