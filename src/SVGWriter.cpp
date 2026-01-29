#include "SVGWriter.h"
#include "svg.h"
#include <string>
#include <vector>
#include <iostream>

struct CSVGWriter::SImplementation{
    std::shared_ptr<CDataSink> Sink;
    TSVGPixel Width;
    TSVGPixel Height;
};

CSVGWriter::CSVGWriter(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->Sink = sink;
    DImplementation->Width = width;
    DImplementation->Height = height;

}

CSVGWriter::~CSVGWriter(){
    
}

bool CSVGWriter::Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){
    //example: <circle cx="50" cy="50" r="10" fill="red" />
    std::string svgtext = "<circle cx=\"" + std::to_string(center.DX) + "\" cy=\"" + std::to_string(center.DY) + "\" r=\"" + std::to_string(radius) + "\"";
    for (auto &attr : style){
        svgtext += " " + attr.first + "=\"" + attr.second+ "\"";
    }
    svgtext += "/>\n";
    //std::vector<char> is a dynamic array of bytes
    std::vector<char> buffer(svgtext.begin(), svgtext.end());
    return DImplementation->Sink->Write(buffer);;

}

bool CSVGWriter::Rectange(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){
    //example: <rect x="10" y="20" width="50" height="30" fill="blue" stroke="black"/>
    std::string svgtext = "<rect x=\"" + std::to_string(topleft.DX) + "\" y =\"" + std::to_string(topleft.DY) + "\" width=\"" + std::to_string(size.DWidth) + "\" height=\"" + std::to_string(size.DHeight) + "\"";
    for (auto &attr : style){
        svgtext += " " + attr.first + "=\"" + attr.second + "\"";
    }
    svgtext += "/>\n";
    
    std::vector<char> buffer(svgtext.begin(), svgtext.end());
    return DImplementation->Sink->Write(buffer);;

}

bool CSVGWriter::Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style){
    //example: <line x1="0" y1="0" x2="100" y2="100" stroke="black" stroke-width="2"/>
    std::string svgtext = "<line x1=\"" + std::to_string(start.DX) + "\" y1=\"" + std::to_string(start.DY) +"\" x2=\"" + std::to_string(end.DX) + "\" y2=\"" + std::to_string(end.DY) + "\"";
    for (auto &attr : style){
        svgtext += " " + attr.first + "=\"" + attr.second + "\"";
    }
    svgtext += "/>\n";
    std::vector<char> buffer(svgtext.begin(), svgtext.end());
    return DImplementation->Sink->Write(buffer);;
    
}

bool CSVGWriter::SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style){
    //example: <path d="M 10 10 L 20 20 L 30 15" stroke="black" fill="none"/>
    if (points.empty()) return false;
    std::string svgtext = "<path d=\"";
    //first point -> M x y
    svgtext += "M " + std::to_string(points[0].DX) + " " + std::to_string(points[0].DY);
    for (size_t i = 1; i < points.size(); i++){
        svgtext += " L " + std::to_string(points[i].DX) + " " + std::to_string(points[i].DY);
    }
    svgtext += "\"";
    for (auto &attr : style){
        svgtext += " " + attr.first + "=\"" + attr.second + "\"";

    }
    svgtext += "/>\n";
    std::vector<char> buffer(svgtext.begin(), svgtext.end());
    return DImplementation->Sink->Write(buffer);

}

bool CSVGWriter::GroupBegin(const TAttributes &attrs){
    //example: <g fill="red" stroke="black">
    std::string svgtext = "<g";
    for (auto &attr : attrs){
        svgtext += " " + attr.first + "=\"" + attr.second + "\"";
    }
    svgtext += ">\n";
    std::vector<char> buffer(svgtext.begin(), svgtext.end());

    return DImplementation->Sink->Write(buffer);;

}

bool CSVGWriter::GroupEnd(){
    //</g>
    std::string svgtext = "</g>\n";
    std::vector<char> buffer(svgtext.begin(), svgtext.end());
    return DImplementation->Sink->Write(buffer);
}