# Tomasulo's Algorithm Simulator

This project is a simulator for Tomasulo's Algorithm, an innovative solution devised by Robert Tomasulo to address hazards in Superscalar processors. The algorithm introduces an out-of-order issue and commit scheme, which offers two significant advantages:

<ol>
    <li> <strong>Improved IPC (Instructions per Clock/Cycle):</strong> By allowing instructions to execute out of order, the algorithm maximizes the utilization of functional units and enhances overall performance. </li>
    <li> <strong>Enhanced Hazard Control:</strong> Tomasulo's Algorithm introduces a way to prevent stalls created from the additional hazards that a Superscalar processor could create (Write after Read (WaR) and Write After Write (WaW)) in addition to the classical RaW (Read After Write) hazard. By effectively managing these hazards, the algorithm reduces execution errors and ensures accurate results.</li>
</ol>

To prevent stalls caused by false dependencies, Tomasulo's algorithm incorporates **register renaming** through reservation stations. These stations act as intermediate buffers, allowing instructions to operate independently of specific registers and promoting concurrent execution. The out-of-order issue and commit are granted by the Reorder buffer, which prevents an instruction to commit before every single instruction decodified before it.

In this project the Branch is always considered as taken, and its Branch value (if it should or not ACTUALLY branch) is checked only on WRITE\_RESULT stage (after the execution). If the branch shouldn't have been taken, all instructions added after the BRANCH in Reorder buffer is deleted, and new instructions (directly after the instruciton position) are read, decodified and issued.
## Creators

[Thiago de Campos Ribeiro Nolasco](https://github.com/Theeeccoo)

[Danielle Dias Vieira](https://github.com/DanielleDVieira)

[João Augusto dos Santos Silva](https://github.com/joaoaugustoss)

[Cecilia Capurucho Bouchardet](https://github.com/cecibou)

## Features
<ul>
    <li> Detailed simulation of Tomasulo's Algorithm, including out-of-order execution, hazard control and branch prediction.</li>
    <li> Visualization of the processor state, reservation stations, and functional units.</li>
    <li> Interactive interface for inputting parameters and observing simulation results.</li>
</ul>

## Installation
<ol>
    <li> Clone the repository: `git clone https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator.git`</li>
    <li> Navigate to the project directory: `cd Tomasulo-s-algorithm-simulator`</li>
    <li> Install the required dependencies (IF NEEDED): `sudo bash build`</li>
</ol>

## Usage
<ol>
    <li> Navigate to the code directory: `cd Cmulator`</li>
    <li> Add your instructions to be analysed at "input.txt" </li>
	<ol>
		<li> If you want, you can use the Examples pre-created by the authors </li>
        <li> It will be needed to change the File Path in "simulator.c" (at main) </li>
	</ol>
    <li> Run the simulator: `make all`</li>
    <li> Follow the on-screen instructions to input the necessary parameters.</li>
    <li> Observe the simulation results and analysis, including the processor state and execution timeline, a log file is created ("tables.txt").</li>
</ol>

## References
<ul>
    <li> Robert Tomasulo. "An Efficient Algorithm for Exploiting Multiple Arithmetic Units." IBM Journal of Research and Development, vol. 11, no. 1, 1967, pp. 25-33.</li>
    <li> Hennessy, John L., and David A. Patterson. "Computer Architecture: A Quantitative Approach." Morgan Kaufmann, 2018.</li>
</ul>
