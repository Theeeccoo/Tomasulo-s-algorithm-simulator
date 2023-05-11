# Tomasulo's Algorithm Simulator

This project is a simulator for Tomasulo's Algorithm, an innovative solution devised by Robert Tomasulo to address hazards in superscalar processors. The algorithm introduces an out-of-order issue and commit scheme, which offers two significant advantages:

<ol>
    <li> <strong>Improved IPC (Instructions per Clock/Cycle):</strong> By allowing instructions to execute out of order, the algorithm maximizes the utilization of functional units and enhances overall performance. </li>
    <li> <strong>Enhanced Hazard Control:</strong> Tomasulo's Algorithm introduces two additional hazards, WAR (Write After Read) and WAW (Write After Write), in addition to the classical RAW (Read After Write) hazard. By effectively managing these hazards, the algorithm reduces execution errors and ensures accurate results.</li>
</ol>

To prevent stalls caused by false dependencies, Tomasulo's algorithm incorporates **register renaming** through reservation stations. These stations act as intermediate buffers, allowing instructions to operate independently of specific registers and promoting concurrent execution.

## Creators
<ul>
    <li> [Cecilia Capurucho Bouchardet](https://github.com/cecibou)</li>
    <li> [Danielle Dias Vieira](https://github.com/DanielleDVieira)</li>
    <li> [João Augusto dos Santos Silva](https://github.com/joaoaugustoss)</li>
    <li> [Thiago de Campos Ribeiro Nolasco](https://github.com/Theeeccoo)</li>
</ul>

## Features
<ul>
    <li> Detailed simulation of Tomasulo's Algorithm, including out-of-order execution and hazard control.</li>
    <li> Visualization of the processor state, reservation stations, and functional units.</li>
    <li> Interactive interface for inputting parameters and observing simulation results.</li>
</ul>

## Installation
<ol>
    <li> Clone the repository: `git clone https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator.git`</li>
    <li> Navigate to the project directory: `cd Tomasulo-s-algorithm-simulator`</li>
    <li> Install the required dependencies: `sudo bash build`</li>
</ol>

## Usage
<ol>
    <li> Navigate to the code directory: `cd Cmulator`</li>
    <li> Run the simulator: `make all`</li>
    <li> Follow the on-screen instructions to input the necessary parameters.</li>
    <li> Observe the simulation results and analysis, including the processor state and execution timeline.</li>
</ol>

## References
<ul>
    <li> Robert Tomasulo. "An Efficient Algorithm for Exploiting Multiple Arithmetic Units." IBM Journal of Research and Development, vol. 11, no. 1, 1967, pp. 25-33.</li>
    <li> Hennessy, John L., and David A. Patterson. "Computer Architecture: A Quantitative Approach." Morgan Kaufmann, 2018.</li>
</ul>
