# projeto-pif-av2

## Equipe
 José Vitor 
 
 Flávio Barbosa
 
 Pedro Henrique
 
 David Alves
 
 Nestor França

## Mecânicas principais do jogo Campo Minado   
 1. Tabuleiro (Grid)
 O jogo ocorre em um grid bidimensional (matriz) de tamanho configurável (ex.: 8x8, 10x10, 16x16).

 Cada célula pode estar em um dos seguintes estados:

 Oculta: Não revelada (inicialmente todas estão assim).

 Revelada: Mostra se é segura (número) ou uma mina.

 Marcada: O jogador suspeita que há uma mina ali (bandeira).

 1.1. Mineração do Campo
 Um número pré-definido de minas (N) é distribuído aleatoriamente pelo tabuleiro.

 Ex.: Em um tabuleiro 8x8, pode-se ter 10 minas.

 Regra de mineração:

 As minas não podem ser colocadas na primeira célula clicada pelo jogador (para evitar derrota imediata).

 1.2. Sistema de Números nas Células
 Células seguras revelam um número que indica quantas minas estão adjacentes a ela (em todas as 8 direções possíveis).

 Se uma célula não tem minas adjacentes, ela é revelada como vazia e revela automaticamente todas as células vizinhas (recursão).

 1.3. Interações do Jogador
 O jogador pode realizar duas ações principais:

 Revelar Célula (Clique Esquerdo)

 Se for uma mina → Fim de jogo (derrota).

 Se for segura → Mostra o número ou abre células vazias automaticamente.

 Marcar Célula (Clique Direito ou Tecla Específica)

 Coloca uma bandeira onde o jogador suspeita de uma mina.

 Impede cliques acidentais na célula marcada.

 1.4. Condições de Vitória e Derrota
 Vitória: Todas as células sem minas são reveladas.

 Derrota: O jogador revela uma célula com mina.

 1.5. Temporizador e Contador de Minas
 Temporizador: Registra o tempo decorrido desde o início da partida.

 Contador de Minas: Mostra quantas minas faltam ser marcadas (total de minas - bandeiras colocadas).

## Fluxo do Jogo
 1.Inicialização:

 O tabuleiro é gerado com minas em posições aleatórias.

 Todas as células começam ocultas.

 2.Jogabilidade:

 O jogador interage com o tabuleiro até vencer ou perder.

 3.Fim de Jogo:

 Em caso de derrota, todas as minas são reveladas.

 Em caso de vitória, o tempo final é registrado 


## Instruções para compilar e executar 
  Pré-requisitos

Estar em um ambiente Linux (ou WSL com Ubuntu).

Ter o GCC e o make instalados no sistema.

Estar dentro do diretório do projeto.

Passos:

-Como instalar o GCC e Make: bash: sudo apt update sudo apt install build-essential

Ou escreva esse codigo aqui que ja vai instalar tudo e preparar seu ambiente: sudo apt update && sudo apt install -y build-essential

-Acesse o diretório do projeto usando o WSL Ubuntu:

cd "/mnt/c/Users/pedro/OneDrive/Área de Trabalho/teste_campominado"

Execute o jogo: ./build/game

