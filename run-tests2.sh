#apitrace trace --api gl ./bin/debug/SARndbox -uhm -fpv -us -uhs -vruiVerbose -cp /tmp/control-socket -socket &
./bin/debug/SARndbox -uhm -fpv -ns -nhs -vruiVerbose -cp /tmp/control-socket -socket &

PID=$!
echo "PID = ${PID}";

sleep 2; 
sleep 5;
sleep 2; 
sleep 1;
sleep 1; 

echo "killing ${PID}"

kill -SIGINT $PID
echo "killed ${PID}"
sleep 2
killall SARndbox


