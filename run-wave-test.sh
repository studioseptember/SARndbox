#apitrace trace --api gl ./bin/debug/SARndbox -uhm -fpv -us -uhs -vruiVerbose -cp /tmp/control-socket -socket &
./bin/debug/SARndbox -uhm -fpv -ns -nhs -vruiVerbose -cp /tmp/control-socket -socket &

PID=$!
echo "PID = ${PID}";

sleep 2; 
echo "waterColumn 0 0 0.05 1 5" |socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 5;
echo "waterAt 0.5 0.5
reset" | socat - UNIX-CONNECT:/tmp/control-socket; 
sleep 1; 

echo "killing ${PID}"

kill -SIGINT $PID
echo "killed ${PID}"
sleep 2
killall SARndbox


