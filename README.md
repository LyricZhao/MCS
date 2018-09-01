## fakeCOMSOL v1.0 - Microfluidic Chip Simulation UI

####  Introduction

- A MCS(Microfluidic Chip Simulation) User Interface, core by TA.
- Developed by LyricZ.
- A summer programming training project, 2018 Summer.

#### Features

- [x] OOP Design Style
- [x] Efficient
- [x] Cross-Platform
- [x] Concentration Computing
- [x] Colorful Display
- [x] Simulated Annealing Auto Design
- [x] Pipe Width Changing
- [x] Multithread Timer

#### Environments

- Local Compiling Environement
  - macOS 10.13.4
  - QT 5.10.1
  - Clang 902.0.39.1

#### MainWindow

- A simple welcome interface with QPixmap (the fonts are created by PhotoShop and inserted in).

  ![](http://otxp6khet.bkt.clouddn.com/mcs/mcs1.png)

- Using simple QMenuBar.

  ![](http://otxp6khet.bkt.clouddn.com/mcs/mcs2.png)

#### Computing Mode

- Click "Compute - New Computation" to create a new computation.

  ![](http://otxp6khet.bkt.clouddn.com/mcs/mcs3.png)

  - If the input has something wrong, a warning dialog will be showed.

- Computing Interface

  ![](http://otxp6khet.bkt.clouddn.com/mcs/mcs4.png)

  - The pipeline is draw by QPainter.
  - The color of the pipeline is decided by its flow speed.
  - The output includes both speed and concentration.
  - The timer is implemented by inheriting the QThread, so that it will not have an impact on the UI fluency.
  - Just click on the pipe to change its existence and press Space to change its width, which will be strictly limited in a range.

#### Auto Design Mode

![](http://otxp6khet.bkt.clouddn.com/mcs/mcs5.png)

- Here I implement a simple Chip Auto Design using Simulated Annealling Algorithm.
- Click the Start Button to start iteration, when reaching the convergence point, the process will be stoped by itself.
- The SA is implemented using QThread.

#### About the code

- The code is designed in OOP Style, there are many codes reusing, such as the computing mode and auto design mode are using the same class, the only difference is the parameter.

- The class diagram is as the following one.

  ![](http://otxp6khet.bkt.clouddn.com/mcs/cd.png)

#### Happy Creating Your Chip!