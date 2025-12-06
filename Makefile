CXX = g++
CXXFLAGS = -std=c++17 -Wall
OBJS = main.o DomainTrie.o HashMap.o AVLTree.o UserDriver.o AdminDriver.o
TARGET = dnsfirewall

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
