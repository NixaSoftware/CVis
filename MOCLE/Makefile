# Projeto Final da disciplina de Computacao Paralela
#
# Makefile
#
# Arquivo responsavel pela compilacao, execucao e
# limpeza dos arquivos gerados ao compilar/executar os benchmarks.
#
PROGRAM_NAME=mocle
CC=g++
CFLAGS=
# INCLUDE=-I /home/lasid/programs/auxiliares/boost_1_55_0
# LIBS2 = -L /home/lasid/programs/auxiliares/boost_1_55_0/stage/lib/

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
./TPartitionSolution.cpp \
./cluster.cpp \
./clusteringTypes.cpp \
./combiner.cpp \
./partitionCombiner.cpp \
./similarities.cpp \
./tPartition.cpp \
./tPatterns.cpp \
./validationIndices.cpp  \
./MO/global.cpp \
./MO/momethod.cpp \
./MO/momhconstrainedsolution.cpp \
./MO/nondominatedset.cpp \
./MO/nsgaii.cpp \
./MO/paretobase.cpp \
./MO/solution.cpp \
./MO/solutionsset.cpp \
./MO/tbnode.cpp \
./MO/tbtree.cpp \
./MO/tlistset.cpp \
./MO/tmotools.cpp \
./MO/tquadnode.cpp \
./MO/tquadtree.cpp \
./MO/trandomgenerator.cpp \
./MO/weights.cpp

OBJS = \
./TPartitionSolution.o \
./cluster.o \
./clusteringTypes.o \
./combiner.o \
./partitionCombiner.o \
./similarities.o \
./tPartition.o \
./tPatterns.o \
./validationIndices.o \
./MO/global.o \
./MO/momethod.o \
./MO/momhconstrainedsolution.o \
./MO/nondominatedset.o \
./MO/nsgaii.o \
./MO/paretobase.o \
./MO/solution.o \
./MO/solutionsset.o \
./MO/tbnode.o \
./MO/tbtree.o \
./MO/tlistset.o \
./MO/tmotools.o \
./MO/tquadnode.o \
./MO/tquadtree.o \
./MO/trandomgenerator.o \
./MO/weights.o

LIBS := -lboost_system -lboost_filesystem

all:
	$(CC) $(CFLAGS) $(INCLUDE) -o "mocle" $(CPP_SRCS) $(LIBS2) $(LIBS)
#g++ -I /home/lasid/programs/auxiliares/boost_1_55_0 -o mocle partitionCombiner.cpp

# c++ -I path/to/boost_1_55_0 example.cpp -o example

#$(CC) $(CFLAGS) $(INCLUDE) -o "mocle" $(OBJS) $(USER_OBJS) $(LIBS)
#run:    ${PROGRAM_NAME}
#	./bin/bench

#clean:
#	rm -rf ./bin/* ./tests/mpi/*.out ./tests/openmp/*.out ./tests/pthreads/*.out
