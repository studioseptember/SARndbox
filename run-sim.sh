xrandr --output HDMI-0 --scale 0.75x1

killall node
sleep 1;
killall google-chrome-unstable
sleep 1;

killall SARndbox
sleep 5;

./bin/debug/SARndbox -wo 15 -uhm -fpv -us -uhs -vruiVerbose -cp /tmp/control-socket -socket &
sleep 1
cd ~/src/node-sandbox; ~/Downloads/node-v8.9.4-linux-x64/bin/node index.js &
sleep 1
google-chrome-unstable --app-id=cgigkgkdfoolaeobhmpmcnkobgnmlkio &
