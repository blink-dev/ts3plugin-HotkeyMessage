# Hotkey Message (ts3-plugin)

Hotkey Message is ts3 plugin to send messages to clients with hotkeys.
It has ability to send two messages with one hotkey, like a switch message.
Its mostly usable for teamspeak music bots



### Compliling
```
git clone https://github.com/blink-dev/ts3plugin-HotkeyMessage.git
```
Qt in the minor version of the client is required, e.g.
```sh
mkdir build64 & pushd build64

# For Visual Studio 2017 x64
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_PREFIX_PATH="D:/Dev/Qt/Qt5.6.2_x64/5.6/msvc2015_64/" ..

# For Visual Studio 2019 x64
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="D:/Dev/Qt/Qt5.6.2_x64/5.6/msvc2015_64/" ..
```
License
----

MIT