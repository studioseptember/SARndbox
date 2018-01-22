NUM=`ls /tmp/.ICE-unix/`

export SESSION_MANAGER=local/sarndbox:@/tmp/.ICE-unix/$NUM,unix/sarndbox:/tmp/.ICE-unix/$NUM
export DISPLAY=:0



killall node
sleep 1;
killall chrome
sleep 1;

killall SARndbox
sleep 1;

xrandr --output HDMI-0 --scale 0.75x1

sleep 10;

./bin/debug/SARndbox -wo 15 -uhm -fpv -ns -nhs -vruiVerbose -cp /tmp/control-socket -socket &
sleep 1
cd ~/src/node-sandbox; ~/Downloads/node-v8.9.4-linux-x64/bin/node index.js &
sleep 1
google-chrome-unstable &
sleep 5
google-chrome-unstable --app-id=cgigkgkdfoolaeobhmpmcnkobgnmlkio &

sleep 10

killall chrome

sleep 5


google-chrome-unstable &
sleep 5
google-chrome-unstable --app-id=cgigkgkdfoolaeobhmpmcnkobgnmlkio &
sleep 5
