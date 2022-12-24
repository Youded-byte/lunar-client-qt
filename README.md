![Github all releases](https://img.shields.io/github/downloads/Nilsen84/lunar-client-qt/total)
![GPLv3](https://img.shields.io/github/license/Nilsen84/lunar-client-qt)
![Stars](https://img.shields.io/github/stars/Nilsen84/lunar-client-qt)
![Forks](https://img.shields.io/github/forks/Nilsen84/lunar-client-qt)
![CI](https://img.shields.io/github/workflow/status/Nilsen84/lunar-client-qt/Build%20and%20Release)

# Lunar Client Qt
Lunar Client Qt is a lightweight, cross-platform and open sourced launcher for Lunar Client written in C++ using the Qt framework.  
  
![alt text](https://i.imgur.com/FTnzJvi.png)

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
It is common for people to use external programs such as overlays, proxies and clients to enhance their minecraft experience. This launcher includes built-in support to launch such programs alongside lunar client.

## Performance Improvements
This launcher is able to launch lunar client very quickly. The option to disable cosmetics may speed this process up even further. The ability to use
a custom JRE and JVM arguments opens up more possibilities for performance improvements. You can refer to [this](https://github.com/Youded-byte/Java-Optimisations-MC/blob/main/README.md) for advice.


## Bugs
Bugs and crashes are bound to happen as changes outside our control happen. If a crash happens it is likely due to old files being left over by lunar.
Try first to clear your `%userprofile%\.lunarclient\offline\multiver` folder to remove old files, then launch your desired version on the official launcher.
If your game still does not work, open an issue, stating your game version, forge, fabric or optifine option and add explanation, like copying the error.log and latest.log file.

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
If you're on MacOS, you want to start off by installing Qt using [Brew](https://brew.sh/): ```brew install qt@6```  
You also want to replace the `cmake ..` command with `cmake -DCMAKE_PREFIX_PATH=/usr/local/opt/qt6/ ..`  
Besides that, follow the same instructions above.
