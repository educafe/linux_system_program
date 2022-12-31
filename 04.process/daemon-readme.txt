systemctl 명령을 이용하여 daemon 프로그램을 실행시키는 실습에서는,
반드시,
/tmp/mydaemon.log 파일을 삭제한 후 실행해야 한다.
또한,
/run/mydaemon.pid 파일의 존재 유무에 따른 에러를 확인할 필요가 있다