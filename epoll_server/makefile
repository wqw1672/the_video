.PHONY: clean all
CC  = gcc
CXX = g++ 
CPPFLAGS = -Wall -Wno-deprecated
LDFLAGS  = -lpthread -lrt -lsqlite3

SRCS 	:= EpollBase.cpp EpollTcpServer.cpp QQEpollTcpServer.cpp CSinDB.cpp  TcpSocket.cpp TcpServer.cpp HostAddress.cpp QQPacket.cpp PThreadCond.cpp PThreadMutex.cpp ShmFIFO.cpp PThreadPool.cpp SocketBase.cpp BasePacket.cpp QQSendThread.cpp PThreadBase.cpp StreamBase.cpp

BIN = FrontServer BackServer# client
all: $(BIN)

FrontServer: $(SRCS) FrontServer.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@  $(LDFLAGS)

BackServer: $(SRCS) BackServer.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@  $(LDFLAGS)
	
#client: $(SRCS) client1.cpp
#	$(CXX) $(CPPFLAGS) $^ -o $@  $(LDFLAGS) 
	
%.o:%.cpp  
	$(CXX) $(CPPFLAGS) -c $<  	
    
%.o:%.c 
	$(CC) -c $<

clean:
	-rm -rf $(BIN)  *.o
