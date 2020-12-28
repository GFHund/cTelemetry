objects = App.o	DiagramView.o DiagramWidget.o DataView.o FileManager.o F1_2020_Converter.o DbFile.o FileNotFoundException.o FileOpenErrorException.o SQLErrorException.o sqlite3.o
CPPFLAGS += `wx-config --cxxflags` 
#-g
#-std=c++11

all: $(objects)
	$(CXX) -o cTelemetry $(objects) `wx-config --libs core base aui xrc` 
App.o: src/App.cpp
DiagramView.o: src/DiagramView.cpp
DiagrammWidget.o: src/CustomWidgets/DiagramWidget.cpp
DataView.o: src/DataView.cpp
FileManager.o: src/data/FileManager.cpp
F1_2020_Converter.o: src/data/F1_2020_Converter.cpp
DbFile.o: src/data/DbFile.cpp
FileNotFoundException.o: src/data/Exceptions/FileNotFoundException.cpp
FileOpenErrorException.o: src/data/Exceptions/FileOpenErrorException.cpp
SQLErrorException.o: src/data/Exceptions/SQLErrorException.cpp

sqlite3.o: src/vendor/sqlite3/sqlite3.c
	gcc $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm $(objects) cTelemetry
	
%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -I src/
%.o: src/CustomWidgets/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
%.o: src/data/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
%.o: src/data/Exceptions/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<