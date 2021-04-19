# DataTrans.c DataTrans.h SerialPort.c SerialPort.h KNN.c KNN.h socket.c socket.h main.c
# Piserver
Piserver:DataTrans.o SerialPort.o KNN.o socket.o main.o
	g++ DataTrans.o SerialPort.o KNN.o socket.o main.o -o Pisever -pthread

DataTrans.o:DataTrans.cpp SerialPort.cpp KNN.cpp
	g++ DataTrans.cpp -c

SerialPort.o:SerialPort.cpp DataTrans.cpp
	g++ SerialPort.cpp -c

KNN.o:KNN.cpp
	g++ KNN.cpp -c

socket.o:socket.cpp
	g++ socket.cpp -c

main.o:main.cpp DataTrans.cpp SerialPort.cpp socket.cpp
	g++ main.cpp -c

clean:
	rm -fR *.o Piserver
