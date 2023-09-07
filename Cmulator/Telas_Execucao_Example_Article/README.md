# Demonstração de execução no simulador

Abaixo será demonstrado, com um exemplo, as telas do passo a passo da execução do conjunto de instruções RISC-V. Cada tela é mostrada no terminal ao pressionar a tecla Enter após a execução do código. É possível observar nas telas abaixo quando uma instrução vai para cada tabela, ou quando sai e cada estado da instrução no Reorder Buffer no passo a passo de sua execução.

- Conjunto de Instruções que será executado:

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/01.png" width=700>

- As 6 primeiras instruções são carregadas no Reorder Buffer. É possível observar que como a previsão é de que sempre haverá desvio, a instrução 'BNE R1, R5, bar #1' está após o label 'foo'.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/02.png" width=800>

- As instruções vão para as Estações de Reserva para as Unidades Funcionais livres. Como há apenas uma unidade funcional de Branch, logo a instrução 'BNE R1, R5, bar #1' aguarda até que a unidade funcional fique livre.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/03.png" width=800>

- As instruções que não possuem dependência são enviadas para execução. É possível observar nas colunas Qk e Qj em Reservation Station, de qual linha do Reorder Buffer as instruções tem dependências verdadeiras. É possível observar também qual o endereço de memória será acessado pela instrução LW na coluna Address em Reservation Station.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/04.png" width=800>

- Após a execução as instruções 'SRA R2, R5, 2' e 'ADD R22, R6, R3' tem seu valor calculado e demonstrado na coluna Value do Reorder Buffer. Como já foram executadas, as unidades funcionais que antes elas estavam ficam livres. Há o adiantamento de dados da instrução 'SRA R2, R5, 2' para as instruções 'LW R5, 6(R2)', 'MUL R1, R2, R5' e 'BEQ R1, R2, foo #0', assim é possível observar que as colunas Qj e Qk, no Reservation Station, que possuiam a dependência verdadeira da linha 0 do Reorder Buffer teve seu valor removido e o registrador R2 aparece nas colunas Vj e Vk também no Reservation Station. 

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/05.png" width=800>

- Como o Commit é feito em ordem, apenas a instrução 'SRA R2, R5, 2' é commitada, pois apesar de 'ADD R22, R6, R3' já ter sido executada, antes dela há a instrução 'LW R5, 6(R2)' para ser commitada. A instrução 'SRA R2, R5, 2' escreve no registrador R2, então com o Commit, na tabela Register Status o registrador R2 recebe o 0 que é a linha onde a instrução está no Reorder Buffer. 

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/06.png" width=800>

- Como a instrução 'SRA R2, R5, 2' já foi commitada e há mais instruções na fila para entrar no Reorder Buffer, a instrução SRA é retirada do Reorder Buffer e a instrução 'REM R16, R2, R3' entra. Lembrando que a instrução REM entra porque há a previsão de desvio para o label 'bar'.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/07.png" width=800>

- Como há uma unidade funcional livre para a instrução 'REM R16, R2, R3', ela é enviada para a estação de reserva MULT2.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/08.png" width=800>

- As instruções 'LW R5, 6(R2)' e 'REM R16, R2, R3' são enviadas para execução, pois não possuem dependência verdadeira nas colunas Qj e Qk em Reservation Station.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/09.png" width=800>

- Após a execução das instruções 'LW R5, 6(R2)' e 'REM R16, R2, R3' seu cálculo é mostrado na coluna Value em Reorder Buffer. Como LW escreve em R5 e a instrução 'MUL R1, R2, R5' precisa ler o valor de R5, LW faz o adiantamento de dados para MUL e sua dependência verdadeira em Qk é removida.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/10.png" width=800>

- As instruções 'LW R5, 6(R2)' e 'ADD R22, R6, R3' são commitadas em ordem e é possível observar na tabela Register Status que os registradores R5 e R22 recebem respectivamente os valores 1 e 2, que são as linhas em que as instruções que escrevem nestes registradores estão no Reorder Buffer.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/11.png" width=800>

- As instruções 'LW R5, 6(R2)' e 'ADD R22, R6, R3' são removidas do Reorder Buffer, pois há as instruções 'SLT R30, R16, R15' e 'SLTI R4, R2, 5' que estão na fila para entrar no Reorder Buffer. As instruções 'SLT R30, R16, R15' e 'SLTI R4, R2, 5' entram. Como as instruções 'LW R5, 6(R2)' e 'ADD R22, R6, R3' são removidas do Reorder Buffer, suas referências nos registradores R5 e R22 na tabela Register Status também são removidas.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/12.png" width=800>

- A instrução 'SLT R30, R16, R15' é enviada para a unidade funcional Comparison, pois a mesma está livre. E a instrução 'SLTI R4, R2, 5' permanece no status Waiting, pois só há uma unidade funcional Comparison.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/13.png" width=800>

- As instruções 'MUL R1, R2, R5' e 'SLT R30, R16, R15' são enviadas para execução, pois não possuem dependência verdadeira de nenhuma instrução.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/14.png" width=800>

- Após a execução 'MUL R1, R2, R5' e 'SLT R30, R16, R15' são retiradas das unidades funcionais em que estavam e a instrução 'MUL R1, R2, R5' faz adiantamento de dados para a instrução 'BEQ R1, R2, foo #0'.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/15.png" width=800>

- A instrução 'MUL R1, R2, R5' tem seu Commit realizado e escrita no registrador R1 e sua posição do Reorder Buffer colocado em Register Status no registrador R1.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/16.png" width=800>

- A instrução 'MUL R1, R2, R5' apesar de já ter seu Commit realizado, não é removida do Reorder Buffer, pois não há nenhuma instrução na fila para entrar no Reorder Buffer. Como há uma unidade funcional Comparison livre a instrução 'SLTI R4, R2, 5' é enviada para ela em Reservation Station.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/17.png" width=800>

- As instruções 'BEQ R1, R2, foo #0' e 'SLTI R4, R2, 5' são enviadas para execução, pois não possuem dependências verdadeiras.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/18.png" width=800>

- Ao executar a instrução 'BEQ R1, R2, foo #0' o resultado da execução foi que não era para realizar o desvio, ou seja, a previsão de que sempre haverá o desvio foi incorreta. Então todas as instruções que estão no Reorder Buffer depois da instrução 'BEQ R1, R2, foo #0' são descartadas. Como não é calculado de fato a instrução, pois não há acesso ao valor do registrador, é apenas para ilustração do algoritmo de Tomasulo, a informação de que se é ou não para ocorrer o desvio é por meio do último campo da instrução, onde #0 não é para ocorrer o desvio e #1 é para ocorrer desvio.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/19.png" width=800>

- A instrução 'BEQ R1, R2, foo #0' é commitada.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/20.png" width=800>

- Como não ocorreu o desvio, as próximas instruções na fila são as que estão logo após a instrução 'BEQ R1, R2, foo #0', conforme a entrada com o conjunto de instruções (primeira imagem deste README). Mesmo que algumas instruções já tenham sido executadas anteriormente, como foram descartadas, deverão ser executadas novamente. É possível observar que as instruções 'DIV R10, R4, R7', 'OR R9, R20, R11' e 'AND R1, R2, R7' não entram no Reorder Buffer, isso porque há a previsão de que sempre terá o desvio após uma instrução de desvio.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/21.png" width=800>

- As instruções são enviadas para suas respectivas unidades funcionais que estejam livres.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/22.png" width=800>

- As instruções XOR, SUB e BNE são enviadas para execução, pois não possuem dependências verdadeiras.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/23.png" width=800>

- As instruções XOR, SUB e BNE são executadas e removidas do Reservation Station, e seus cálculos realizados são mostrados na coluna Value no Reorder Buffer. A instrução 'SUB R2, R2, R3' faz adiantamento de dados para a instrução 'REM R16, R2, R3', devido a dependência verdadeira no registrador R2.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/24.png" width=800>

- As instruções XOR, SUB e BNE são commitadas em ordem e é possível observar na tabela Register Status que os registradores R10 e R2 recebem respectivamente os valores 3 e 4, que são as linhas em que as instruções que escrevem nestes registradores estão no Reorder Buffer.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/25.png" width=800>

- As instruções XOR, SUB e BNE não são removidas da tabela Reorder Buffer, pois não há mais nenhuma intrução na fila para entrar. A instrução 'REM R16, R2, R3' é enviada para execução pois não possui mais nenhuma dependência verdadeira.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/26.png" width=800>

- A instrução 'REM R16, R2, R3' é executada e faz adiantamento de dados para a instrução 'SLT R30, R16, R15'.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/27.png" width=800>

- A instrução 'REM R16, R2, R3' é commitada e escreve no registrador R16 na tabela Register Status sua posição na tabela Reorder Buffer.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/28.png" width=800>

- A instrução 'SLT R30, R16, R15' é enviada para execução.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/29.png" width=800>

- A instrução 'SLT R30, R16, R15' é executada e é possível visualizar seu cálculo na coluna Value da tabela Reorder Buffer.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/30.png" width=800>

- A instrução 'SLT R30, R16, R15' é commitada e escreve no registrador R30 na tabela Register Status sua posição na tabela Reorder Buffer.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/31.png" width=800>

- A instrução 'SLTI R4, R2, 5' é enviada para a estação de reserva de sua unidade funcional, pois a mesma ficou livre.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/32.png" width=800>

- A instrução 'SLTI R4, R2, 5' é enviada para execução, pois não há nenhuma dependência visto que só falta ela para executar do conjunto de instruções de entrada.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/33.png" width=800>

- A instrução 'SLTI R4, R2, 5' é executada e tem seu cálculo mostrado na coluna Value da tabela Reorder Buffer.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/34.png" width=800>

- A instrução 'SLTI R4, R2, 5' é commitada e escreve no registrador R4 na tabela Register Status sua posição na tabela Reorder Buffer.

<img src="https://github.com/Theeeccoo/Tomasulo-s-algorithm-simulator/blob/master/Cmulator/Telas_Execucao_Example_Article/35.png" width=800>



Conforme as telas apresentadas é possível observar que o Reorder Buffer é uma fila circular. E que para cada unidade funcional há apenas 1 lugar na fila da estação de reserva.
