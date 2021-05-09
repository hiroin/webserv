# webserv
メモ
testerに
```
Test Put http://localhost:1234/put_test/file_should_exist_after with a size of 1000
Test Put http://localhost:1234/put_test/file_should_exist_after with a size of 10000000
Test multiple workers(20) doing multiple times(5): Put on /put_test/multiple_same with size 1000000
Test multiple workers(20) doing multiple times(5): Post on /directory/youpi.bla with size 100000000
```
というのがあるので、PutとかPostのように大文字でないmethodも許容しないと駄目かも。

