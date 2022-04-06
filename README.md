# Lunar Client Qt
Lunar Client Qt is a lightweight, cross-platform and open sourced launcher for Lunar Client written in C++ using the Qt framework.  
  
![alt text](https://i.imgur.com/Ih9BGOn.png)

## Discord
Add me on Discord (nils#5281) if you have any questions or thoughts about the project. For any suggestions/issues open an issue here on the github page.

## Agents
One of the primary features of this launcher is the ability to easily attach Java premain agents to your game, 
this allows you to modify the game on launch, which lets you write modifications that won't break when Lunar Client recieves an update. 
  
Here are repositories containing some useful java agents that you might want to use: https://github.com/Nilsen84/lunar-client-agents 
and https://github.com/Youded-byte/youded-lunar-client-agents.
If you're familiar with java bytecode feel free to write your own!

## Launching without launcher
A useful feature of this launcher is the ability to with the click of a button immediately get into lunar client without the hassle of being faced 
with a launcher interface at every launch. Launch Lunar Client Qt with the --nogui argument and you will be launched into lunar client using the 
options you have used in the previous launch. It can be added to a shortcut of the program with ease.

## Helper program support
It is common for people to use external programs to enhance their minecraft experience. These programs range from chat log readers to memory readers and
programs using JNI to interact with minecraft's JVM. This launcher includes built-in support to launch such programs alongside lunar client.

## Performance Improvements
This launcher is able to launch lunar client very quickly. The option to disable cosmetics may speed this process up even further. The ability to use
a custom JRE and JVM arguments opens up more possibilities for performance improvements. You can refer to [this](https://github.com/Youded-byte/Java-Optimisations-MC/blob/main/README.md) for advice.

## Build instructions
Make sure you have Qt6 or Qt5 and CMake 3.21+ installed.
```
git clone https://github.com/Youded-byte/lunar-client-qt.git
cd lunar-client-qt
mkdir build && cd build
cmake ..
make
```

**NOTE**  
If you're on MacOS, you wanna start off by installing Qt using [Brew](https://brew.sh/): ```brew install qt@6```  
You also want to replace the `cmake ..` command with `cmake -DCMAKE_PREFIX_PATH=/usr/local/opt/qt6/ ..`  
Besides that, follow the same instructions above.