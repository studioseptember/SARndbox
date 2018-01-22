#apitrace trace --api gl ./bin/debug/SARndbox -uhm -fpv -us -uhs -vruiVerbose -cp /tmp/control-socket -socket &
./bin/debug/SARndbox -uhm -fpv -ns -nhs -vruiVerbose -cp /tmp/control-socket -socket &

PID=$!
echo "PID = ${PID}";

sleep 2; 
echo "rainColumn 0.9 0.9 10 12" |socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 5;
echo "waterAt 0.5 0.5
reset" | socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 1; 

echo "killing ${PID}"

kill -SIGINT $PID
echo "killed ${PID}"
sleep 2
killall SARndbox


