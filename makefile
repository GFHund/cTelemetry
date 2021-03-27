objects = App.o	\
DiagramView.o \
DiagramWidget.o \
TrackView.o \
TrackViewWidget.o \
ChangeDiagramEvent.o \
DataView.o \
FileManager.o \
ConverterInterface.o \
F1_2020_Converter.o \
DbFile.o \
EventManager.o \
EventParam.o \
DiagramDataSet.o \
TrackDataSet.o \
vector.o \
FileNotFoundException.o FileOpenErrorException.o SQLErrorException.o \
NotFoundException.o sqlite3.o 
objects_converter = main.o ConverterInterface.o F1_2020_Converter.o SampleGenerator.o FileNotFoundException.o SQLErrorException.o sqlite3.o 
objects_splitter = SplitOneRound.o sqlite3.o  
objects_show_laps = ShowLaps.o sqlite3.o  
CPPFLAGS += `wx-config --cxxflags` 
#-g
#-std=c++11

all: cTelemetry.exe cTelemetryConverter.exe SplitOneRound.exe ShowLaps.exe
	 
cTelemetry.exe: $(objects)
	$(CXX) -o cTelemetry $(objects) `wx-config --libs core base aui xrc`
cTelemetryConverter.exe: $(objects_converter)
	$(CXX) -o cTelemetryConverter $(objects_converter)
SplitOneRound.exe: $(objects_splitter)
	$(CXX) -o SplitOneRound $(objects_splitter)
ShowLaps.exe: $(objects_show_laps)
	$(CXX) -o ShowLaps $(objects_show_laps)
App.o: src/App.cpp
main.o: src/main.cpp
DiagramView.o: src/DiagramView.cpp
DiagrammWidget.o: src/CustomWidgets/DiagramWidget.cpp
TrackView.o: src/TrackView.cpp
TrackViewWidget.o: src/CustomWidgets/TrackViewWidget.cpp
ChangeDiagramEvent.o: src/CustomEvents/ChangeDiagramEvent.cpp
DataView.o: src/DataView.cpp
FileManager.o: src/data/FileManager.cpp
ConverterInterface.o: src/data/ConverterInterface.cpp
F1_2020_Converter.o: src/data/F1_2020_Converter.cpp
SampleGenerator.o: src/data/SampleGenerator.cpp
DbFile.o: src/data/DbFile.cpp
EventManager.o: src/EventSystem/EventManager.cpp
EventParam.o: src/EventSystem/EventParam.cpp
DiagramDataSet.o: src/data/DiagramDataSet.cpp
TrackDataSet.o: src/data/TrackDataSet.cpp
vector.o: src/data/vector.cpp
FileNotFoundException.o: src/data/Exceptions/FileNotFoundException.cpp
FileOpenErrorException.o: src/data/Exceptions/FileOpenErrorException.cpp
SQLErrorException.o: src/data/Exceptions/SQLErrorException.cpp
NotFoundException.o: src/data/Exceptions/NotFoundException.cpp

SplitOneRound.o: src/utilityPrograms/SplitOneRound.cpp
ShowLaps.o: src/utilityPrograms/ShowLaps.cpp

sqlite3.o: src/vendor/sqlite3/sqlite3.c
	gcc $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm $(objects) cTelemetry
	
%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -I src/
%.o: src/CustomWidgets/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
%.o: src/CustomEvents/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
%.o: src/data/%.cpp
	$(CXX) $(CPPFLAGS2) $(CXXFLAGS) -c $<
%.o: src/EventSystem/%.cpp
	$(CXX) $(CPPFLAGS2) $(CXXFLAGS) -c $<
%.o: src/data/Exceptions/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
%.o: src/utilityPrograms/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<