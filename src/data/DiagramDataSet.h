#ifndef __DIAGRAM_DATA_SET__
#define __DIAGRAM_DATA_SET__

#include <vector>
#include <string>
class DiagramDataSet{
    private: 
        std::vector< std::pair<float,float> > mData;
        float mMaxXValue;
        float mMinXValue;
        float mMaxYValue;
        float mMinYValue;
        std::string mName;
        class Iterator{
            private:
            std::vector< std::pair<float,float> >::iterator mStart;
            std::vector< std::pair<float,float> >::iterator mEnd;
            public: 
            Iterator(std::vector< std::pair<float,float> >::iterator start, std::vector< std::pair<float,float> >::iterator end);

            bool isEnd();
            void next();

            float getX();
            float getY();

            bool hasNext();
            float getNextX();
            float getNextY();
        }; 
    public:
        DiagramDataSet(std::string name, std::vector< std::pair<float,float> > data);
        Iterator getIterator();

        float getMaxXValue();
        float getMinXValue();
        float getMaxYValue();
        float getMinYValue();
        std::string getName();
    
};

#endif