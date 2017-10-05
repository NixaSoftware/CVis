# Clustering
#
# Makefile
#


PROGRAM_NAME=clustering
CC=g++
CFLAGS=
# INCLUDE=-I /home/lasid/programs/auxiliares/boost_1_55_0
# LIBS2 = -L /home/lasid/programs/auxiliares/boost_1_55_0/stage/lib/

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
./averagelink.cpp \
./bootstrap.cpp \
./centroidlink.cpp \
./clustering.cpp \
./clusteringTypes.cpp \
./completelink.cpp \
./cpartition.cpp \
./dbscan.cpp \
./kmeans.cpp \
./main.cpp \
./similarities.cpp \
./singlelink.cpp \
./snn.cpp \
./sota.cpp \
./validationIndex.cpp \
./variability.cpp



OBJS = \
./averagelink.o \
./bootstrap.o \
./centroidlink.o \
./clustering.o \
./clusteringTypes.o \
./completelink.o \
./cpartition.o \
./dbscan.o \
./kmeans.o \
./main.o \
./similarities.o \
./singlelink.o \
./snn.o \
./sota.o \
./validationIndex.o \
./variability.o

LIBS := -lboost_system -lboost_filesystem

all:
	$(CC) $(CFLAGS) $(INCLUDE) -o "clustering" $(CPP_SRCS) $(LIBS2) $(LIBS)
