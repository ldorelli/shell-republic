-> README.txt <-

GITHUB (READ-ONLY)
    git://github.com/ldorelli/shell-republic.git

SSC0140 - Sistemas Operacionais II
          Instituto de Ciencias Matematicas e de Computacao - ICMC
          Universidade de Sao Paulo - USP

6909543 - Denis Moreira dos Reis
6419011 - Luis Fernando Dorelli de Abreu

Topicos

1. Introducao a estrutura e decisoes de projeto
2. Funcionalidades Implementadas
3. Testes e Profiling

~~~~~ ( ^ .. ^ ) ~~~~~

1. Introducao a estrutura e decisoes de projeto

    O Shell foi implementdo dentro do paradigma de Programacao Orientada a
    Objetos. Basicamente, o funcionamento foi dividido em Parsing e Execu-
    cao, sendo que o Parsing e' responsavel por gerar linhas de comandos
    (CommandLine), cada uma contendo uma serie de comandos de uma mesma pi-
    peline, que poderiormente serao executadas pelo Executor.

    Algumas decisoes criticas merecem ser comentadas, mas antes, devemos
    ressaltar alguns pontos que nos levaram a elas.

    Em nossos estudos pre-producao, descobrimos que o unico sinal enviado 'a
    Shell, quando qualquer mudanca de estado acontece a um de seus filhos 
    gerados a patir da chamada de sistema fork acontece, e' o SIGCHLD.
    Alem disso, um Handler para manipulacao de qualquer sinal recebe como
    parametro apenas o sinal recebido (isso pois e' possivel tratar mais de
    um sinal utilizando o mesmo handler).
    Quando uma interrupcao e' gerada, o contexto e' empilhado e o programa
    salta par ao codigo do handler, nao importando a linha atualmente sendo
    executada.
    
    Possuindo uma lista para armazenar os jobs, para o tratamento de sinais,
    optamos pela existencia de uma flag que sinaliza a ocorrencia de algera-
    cao de estado em qualquer um dos filhos, sendo tal flag alterada em um
    handler responsavel por tratar o sinal SIGCHLD.
    A flag e' entao  utilizada para posterior atualizacao da lista, seguindo
    os motivos que seguem:

        I. Atualizar a lista de jobs durante um handler e' perigoso por pro-
           blema de concorrencia por esse recurso, pois a estrutura em ques-
           tao poderia estar sendo utilizada no exato momento em que um si-
           nal e' capturado pelo handler. Este problema poderia ser evitado
           bloqueando os sinais de SIGCHLD sempre que fossemos acessar 'a
           lista em outros pontos do codigo, com a chamada de sistema
           sigprocmask, porem...
       II. Um handler tambem pode ser interrompido por um sinal igual ao que
           foi recebido. Isso tambem pode ser evitado, utilizando a varia-
           vel sa_mask da estrutura de dados relacionada ao handler,
           porem...
      III. Como o unico parametro recebido pelo handler e' o sinal recebido
           (no caso, sempre o mesmo, ou seja, SIGCHLD), nao ha' informacoes
           adicionais suficientes para saber qual processo sofreu alteracao
           de estado e nem mesmo qual foi a alteracao ocorrida, de forma
           que a fila deva ser percorrida COMPLETAMENTE para que sejam veri-
           ficados TODOS os processos. Isso significa que, SEMPRE, para CADA
           sinal SIGCHLD recebido, toda a lista sera' percorrida, e
       IV. Para que seja possivel percorrer a lista de processos no handler,
           ela deveria ter acesso global, indo contra os principios de Pro-
           gramacao Orientada a Objetos
        V. Como o Shell e' uma aplicacao interativa , as informacoes sobre
           os dados dos jobs nao precisam estar 100% do tempo corretas, sen-
           do necessaria sua corretude apenas em momentos nos quais ha' in-
           teratividade com o usuario
       VI. Por fim, a utilizacao da flag ao inves de um contador de proces-
           sos alterados que, teoricamente, limitaria a varredura na lista,
           existe apenas para evitar o seguinte problema de concorrencia por
           recurso:

                -> Suponha uso de contador ao inves de flag
                filho 1 morre: contador <- contador + 1 (0 -> 1)
                filho 2 morre: contador <- contador + 1 (1 -> 2)
                Atualizacao da lista
                    var_temp <- contador (2)
                    filho 3 morre: contador <- contador + 1 (2 -> 3)
                    contador <- 0 (3 -> 0)
                    Atualiza var_temp filhos na lista (apenas 2 ao inves de
                                                                         3)

    
    Sobre os comandos Built-In:
    Todos os comandos herdam uma classe abstrata (Builtin) que possui um
    metodo run, para o qual e' passado o Executor que o convocou e a lista
    de parametros dados pelo usuario.

    Os comandos internos podem ou nao sofrer fork, dependendo da necessidade
    de gerar output.

    Uma ultima discussao esta relacionada ao recebimento de sinais durante
    uma leitura do stdin.
    Caso ocorra a chegada de um sinal durante a espera por um input (como
    no caso do uso de getline), a interrupcao causa uma anomalia: a partir
    daquele momento, o input esta configurado como EOF (end of file).
    Para resolver isso, sao possiveis duas abordagens:

        1. bloqueio dos sinais para a leitura; ou
        2. limpeza das flags associadas ao cin (cin.clear()).

    Adotamos a segunda opcao.

2. Funcionalidades implementadas

    Todas as funcionalidades solicitadas foram completamente implementadas.

3. Testes e Profiling

    Os tetes foram realizados baseados na comparacao de resultados obtidos
    pelo nosso shell e o bash (o nosso maior concorrente!)

    Segue uma lista de testes.

        I. Parsing

            touch Arquivo\ com\ espacos
            ls | grep "Arquivo com espacos"
            echo "test \" comma"
            echo 'test " comma'
            echo "teste ' comma"
            echo "test \\ comma"
            echo 'test \\ comma'

       II. Execucao simples

            cal
            vim
            <:q>
            ls
            ls -la
            ls -l -a

      III. Redirecionamento de entrada / saida

            echo CALENDARIO > teste.txt
            cal >> teste.txt
            cat < teste.txt

       IV. Pipeline

            cat < teste.txt | grep CAL
            cat < teste.txt | grep 1
            cat < teste.txt | grep 1 | grep 2
            cat < teste.txt | grep 1 | grep 2 | grep 3  > saida.txt
            less saida.txt
            <q>

       V. Execucao em background

           xeyes &
           xeyes&
           xeyes & xeyes &
           &&&&         xeyes       &&&&
           jobs
           jobs | less
           <ctrl + z>
           fg %6
           <q>
           jobs
           fg %1
           <ctrl + c>
           fg %2
           <ctrl + z>
           jobs | grep Stopped
           bg %2
           kill %2
           jobs
           kill %3
           jobs
           kill %4
           jobs | less
           <ctrl + z>
           jobs
           kill %5
           fg
           <q>
           xeyes | xeyes | xeyes | xeyes &
           jobs
           fg
           <ctrl + z>
           jobs
           bg
           jobs
           kill %1

      VI. Comandos Built-In ainda nao testados
           
           pwd
           pwd | less
           <q>
           mkdir Pasta\ 1
           cd Pasta\ 1
           pwd
           cd ..
           pwd
           rm -r Pasta\ 1
           ls
           ./shell
           quit
           exit

    Observacoes: nao tente dar ctrl + c no less. Nem no bash isso e' possi-
                 vel!

    Para profiling, compilamos os arquivos segundo o shellscript compile,
    que acompanha o source code, pois o gprof, utilizado para geracao do ar-
    quivo de profile, nao consegue capturar eventos ocorridos na biblioteca
    .so gerada para o trabalho.

    Como a shell e' uma aplicacao interativa e como o gprof NAO contabiliza
    tempos de operacoes IO e, tambem, como as operacoes de parsing e manu-
    tencao das filas de jobs usam um tempo desprezivelmente baixo de CPU,
    o relatorio do gprof, que utiliza 2 casas decimais para os segundos de
    processamento utilizado por cada regiao do programa nao ultrapassou
