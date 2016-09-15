CPP=g++
AR=ar

CFLAGS=-std=gnu++11

PYTHON_VER=3.5
PYTHON_CONFIG=python${PYTHON_VER}-config

# For some bizarre reasons, config --ldflags will work for
#  the Mac the first time, and then subsequently fail to
#  include the path to the libraries
PYTHON_PREFIX=$(shell ${PYTHON_CONFIG} --prefix)
PYTHON_CFLAGS=$(shell ${PYTHON_CONFIG} --cflags)
PYTHON_LDFLAGS=$(shell ${PYTHON_CONFIG} --ldflags)

#WRAPPER_OBJS=wconCpp_PythonWrapper.o wrapperInternal.o \
	wconCpp_wrapperWCONWorms.o \
	wconCpp_wrapperMeasurementUnit.o
WRAPPER_OBJS=wconCpp_wrapObj_WCONWorms.o \
	wconCpp_wrapObj_MeasurementUnit.o
#COMMON_HEADERS=wconCpp_PythonWrapper.h wrapperTypes.h
COMMON_HEADERS=wconCpp_wrapObj_WCONWorms.h \
	wconCpp_wrapObj_MeasurementUnit.h

WRAPPER_LIB=libWconCpp.a libWconCpp.so
WRAPPER_LIB_LDFLAGS=-L. -lWconCpp

#all: driver ${WRAPPER_LIB} 
#all: driverOO ${WRAPPER_LIB}
all: ${WRAPPER_LIB} 

driverOO: driverOO.o ${WRAPPER_LIB}
	$(CPP) -o driverOO driverOO.o ${WRAPPER_LIB_LDFLAGS}

driver: driver.o ${WRAPPER_LIB}
	$(CPP) -o driver driver.o ${WRAPPER_LIB_LDFLAGS}

libWconCpp.a: ${WRAPPER_OBJS}
	$(AR) rcs libWconCpp.a ${WRAPPER_OBJS} 

libWconCpp.so:	${WRAPPER_OBJS}
	$(CPP) -shared -o libWconCpp.so ${WRAPPER_OBJS} ${PYTHON_LDFLAGS}

driverOO.o: driverOO.cpp
	$(CPP) $(CFLAGS) -c driverOO.cpp

driver.o: driver.cpp
	$(CPP) $(CFLAGS) -c driver.cpp

%.o: %.cpp ${COMMON_HEADERS}
	$(CPP) $(CFLAGS) -fPIC -c $< ${PYTHON_CFLAGS}

clean:
	rm -f *~ *.o *.a *.so driver
