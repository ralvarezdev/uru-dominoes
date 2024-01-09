# Blackjack
This repository contains the source code of a Dominoes Minigame that can be Executed in the Terminal, developed by:

## Developers
Ramón Álvarez <a href="https://github.com/ralvarezdev">(ralvarezdev)</a>.

## Programming Languages
Only C++.

## Installation

<ol>
<li>Clone this repository to your local machine using the following command:

```git clone https://github.com/ralvarezdev/dominoes```
</li>

<li>Change your Current Working Directory to where the Repository was Cloned</li>

<li>There are Two Ways to Compile the Program:</li>

<ol>
<li>With <strong>G++</strong> (You must have Installed <strong>MinGW Compiler</strong>). Use the following command:

<details>
<summary>Command for Windows</summary>

```g++ -o bin\Dominoes.exe src\main.cpp src\lib\cards.cpp src\lib\cards.h src\lib\input.cpp src\lib\input.h src\lib\terminal.cpp src\lib\terminal.h```

</details>

<details>
<summary>Command for Linux</summary>

```g++ -o bin/Dominoes.exe src/main.cpp src/lib/cards.cpp src/lib/cards.h src/lib/input.cpp src/lib/input.h src/lib/terminal.cpp src/lib/terminal.h```

</details></li>

<li>With <strong>CMake</strong> (You must have Installed <strong>CMake and Make</strong>). First, Get to the <code>bin</code> Folder inside the Repository. Use the following command:

<details>
<summary>Command for Windows</summary>

```cmake -S ..\ -G "MinGW Makefiles" -B .\ && make```

</details>

<details>
<summary>Command for Linux</summary>

```cmake -S ../ -G "MinGW Makefiles" -B ./ && make```

</details></li></ol>

<li>Run the program by clicking the <code>Dominoes.exe</code>.</li>
</ol>

<strong>For a Better User Experience, Resize the Terminal Window to Fullscreen</strong>