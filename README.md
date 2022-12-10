<p align="center">
  <a href="https://github.com/umutsevdi/unix-assistant">
  <h3 align="center"> Unix Assistant</h3>
  </a>

<p align="center">  
A voice assistant tool on OLED displays for running shell commands
  <br/>
    <i>Developed by <a href="https://github.com/umutsevdi"> Umut Sevdi</a>, <a href="https://github.com/Selimshady">Yavuz Selim Cagan</a>, <a href="https://github.com/semihyazici">Semih Yazici</a> and <a href="https://github.com/Oguzhanercan">Oguzhan Ercan</a>.</i>

<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#project_definition">Project Definition</a></li>
    <li><a href="#system_architecture">System Architecture</a></li>
    <li><a href="#installation">Installation</a></li>
    <li><a href="LICENSE">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

<p id="project_definition">

## 1- Project Definition

Unix Assistant is a voice controlled UNIX Shell that is displayed on the OLED screens.
Works on Raspberry Pi or any embedded device running UNIX like operating systems.

In our project we used Raspberry Pi Zero running [Rasbian](https://www.raspberrypi.com/software/).

By clicking to the button, our software records the voice and converts it into a
valid UNIX shell command. After that our software executes the command and displays
it's output on OLED screen.
<img src="screenshot/program1.png" width=512px>

<p id="system_architecture">

## 2- System Architecture

Our software is written in C, C++ and Python. Voice recording and processing is done
in Python. Everything related to the OLED display is done in C and C++.

After successfully processing the voice data, the text is transferred through sockets
to the C program. The C program then executes the incoming command in a seperate
process and it's result is transferred to the main program via pipes. When the command
and it's result is obtained, they are both printed to the OLED screen on the current
cursor position.

- For button control we used [RPi.GPIO](https://pypi.org/project/RPi.GPIO/).
- For voice control and processing we used [PyAudio](https://pypi.org/project/PyAudio/).
- To control the OLED display we used [SSD1306_OLED_RPI](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI)
  and [bcm2835](https://www.airspayce.com/mikem/bcm2835/).

<p align="center"><img src="screenshot/diagram.png" alt="diagram" width=512px></p>

<p id="installation">

### 3- Installation

1- Clone the repo

```sh
   git clone https://github.com/umutsevdi/unix-assistant.git
```

2- Download the dependencies to the Raspberry Pi.

3- Compile the C++ program.

```sh
    cd app/
    make all
    sudo bin/test
```

4- Run the Python program.

```sh
    python main.py
```

## 4- License

Distributed under the MIT License. See `LICENSE` for more information.

<p id="contact">

## 5- Contact

You can contact any developer of this project for any suggestion or information.

Project: [umutsevdi/unix-assistant](https://github.com/umutsevdi/unix-assistant)

<i>Developed by <a href="https://github.com/umutsevdi"> Umut Sevdi</a>, <a href="https://github.com/Selimshady">Yavuz Selim Cagan</a>, <a href="https://github.com/semihyazici">Semih Yazici</a> and <a href="https://github.com/Oguzhanercan">Oguzhan Ercan</a>.</i>
