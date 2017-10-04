################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../averagelink.cpp \
../bootstrap.cpp \
../centroidlink.cpp \
../clustering.cpp \
../clusteringTypes.cpp \
../completelink.cpp \
../cpartition.cpp \
../dbscan.cpp \
../kmeans.cpp \
../main.cpp \
../similarities.cpp \
../singlelink.cpp \
../snn.cpp \
../sota.cpp \
../validationIndex.cpp \
../variability.cpp 

OBJS += \
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

CPP_DEPS += \
./averagelink.d \
./bootstrap.d \
./centroidlink.d \
./clustering.d \
./clusteringTypes.d \
./completelink.d \
./cpartition.d \
./dbscan.d \
./kmeans.d \
./main.d \
./similarities.d \
./singlelink.d \
./snn.d \
./sota.d \
./validationIndex.d \
./variability.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


