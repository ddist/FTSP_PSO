# Particle Swarm Optimization (PSO) for the Field Technician Scheduling Problem (FTSP)
Implements a flexible PSO solver for the FTSP. Includes the necesary config files to use ParamILS for parameter tuning. 

The implementation follows [this](https://drive.google.com/open?id=0B8z7-_lhSCqvdmxFcU82cmpZTE0) paper, and together make up for the final evaluation of the Advance AI Course.

#### Topologies
- Global
- Ring
- FDR
- Any combination of the above

#### Heuristics
- Priority first
- Most urgent first
(see paper)

### REQUIREMENTS

- GCC 4.8.X
- C++11 support
- Ruby 2.2.0+

### INSTRUCTIONS

1) Navigate to the project folder
2) Run make in the shell
3) Execute as ./pso <params> 

Example 

>  ./pso -seed 1 -inst data/FTSP_C_1_1.txt -type C -heuristic 2 -maxIter 1000 -topologies 123 -uMin 0.0 -uMax 80.0 -vMin -30.0 -vMax 30.0 -wMin 0.4 -wMax 0.9 
>  -nParticles 40 -cp 0.5 -cg 0.5 -cl 1.5 -cn 2.0 -uq 3.0

4) For parameter tuning with ParamILS, execute as ./paramils <params>

Example

>  ./paramils -numRun 0 -userunlog 1 -maxEvals 500 -pruning 0 -validN 0 -scenariofile scenario_pso_ftsp.txt > tuner/ParamILS_0.out


### INSTANCES

1) The name of each instance consists of the problem's name (FTSP), the geographic distribution (R, C, RC, and RAD), the number to the case or dimension of the instances generated (from 1 to 13), and the number of the instance (from 1 to 20). 
For example, the instance FTSP_RC_12_19.txt refers to the RC geographical distributions of tasks, case 12 (ie, 500 tasks and 83 technicians - see Table 3 of the paper), and instance 19 of the case 12. 


2) Using the paperÕs nomenclature, the instances are arranged as presented below:
<blockquote><br>
Number of tasks (n)<br>
Number of technicians (m)<br><br>
Priority of tasks (w_i)<br><br>
Skill of technicians to perform each task (s_ik)<br><br>
Processing time of each task (p_i)<br>
Beginning of time windows of tasks (e_i)<br>
End of time windows of tasks (l_i)<br><br>
Beginning of time windows of technicians (a_k)<br>
End of time windows of technicians (b_k)<br><br>
Parameter M<br>
Parameter MW<br>
Parameter MZ<br><br>
Geographic coordinates of locations of the base and jobs<br>
</blockquote>
3) Travel times are equal to Euclidean distances (we use full precision in our experiments).

4) For example, in FTSP_R_1_1.txt, we have:

n = 16
m = 2

w_1 = 6

s_11 = 1 and s_42 = 0

p_16 = 85
e_16 = 630
l_16 = 750

a_2 = 660
b_2 = 1200

M = 1189
MW = 2
MZ = 1080

c_13 = 14.26902
