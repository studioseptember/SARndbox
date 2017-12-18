apitrace trace --api gl ./bin/SARndbox -wts 32 24 -wo 15 -uhm -fpv -us -uhs -vruiVerbose -cp /tmp/control-socket -socket &

PID=$!
echo "PID = ${PID}";

sleep 2; 
#echo "waterAt 1 1" | socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 2;
echo "waterColumn 0 0 2 12 10" |socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 1;
#echo "waterAt 1 1" | socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 2; 
echo "removeWaterColumn 0 0" |socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 1;
#echo "waterAt 1 1" | socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 4; 
echo "waterAt 1 1" | socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 1; 
echo "waterColumn 0 0 32 24 0" |socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 1;
echo "removeWaterColumn 0 0" |socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 1; 

kill -SIGINT $PID
killall SARndbox


