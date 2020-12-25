objects = App.o	DiagramView.o DiagramWidget.o
CPPFLAGS += `wx-config --cxxflags` 

all: $(objects)
	g++ -o cTelemetry $(objects) `wx-config --libs core base aui xrc` 
App.o: src/App.cpp
DiagramView.o: src/DiagramView.cpp
DiagrammWidget.o: src/CustomWidgets/DiagramWidget.cpp

clean:
	rm $(objects)
	
%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -I src/
%.o: src/CustomWidgets/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<