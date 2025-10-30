# mpi-transformation

## Descrição

Este projeto implementa um programa de computação paralela usando MPI (Message Passing Interface): realizar transformações em um conjunto de dados distribuído entre múltiplos processos aplicando funções matemáticas.

Cada processo recebe um subconjunto de dados, aplica uma transformação (por exemplo, calcular o quadrado de cada número) e então os resultados são coletados e combinados para produzir o resultado final.

## Pré-requisitos

MPI instalado (por exemplo OpenMPI). Para isso você pode executar o script `install-mpi.sh` fornecido neste repositório.

```sh
$ chmod +x libs/install-mpi.sh
$ ./libs/install-mpi.sh
```
Verifique a instalação com:

```sh
mpicc --version
```

## Compilação

O projeto já traz um `Makefile`. Você pode compilar a aplicação usando:

```sh
make compile
```

Ou, para executar as etapas padrão (limpar e compilar):

```sh
make
```

O alvo `compile` usa `mpicc` para produzir o binário `main` a partir de `main.c`.

## Execução

Você pode executar o binário gerado diretamente com `mpirun`/`mpiexec` ou usar o alvo `run` do `Makefile`.

Exemplo usando `mpirun`:

```sh
mpirun -np 5 ./main
```

Exemplo usando o `Makefile` (substitua `NP` e `N` conforme necessário):

```sh
make run
```

## Demonstração

Execute o comando abaixo para compilar e rodar o programa com 5 processadores:


