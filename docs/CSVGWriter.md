# CSVGWriter
CSVGWriter is a class that generates SVG content. It creates shapes, lines, and groups that is written into a string or data sink.

## Overview
- Supports basic SVG shapes: Circle, Rectangle, Line, and Groups
- Writes SVG content in XML format
- Handles nested groups for structured SVG drawings
- Works with CDataSink

## Constructor and Destructor
CSVGWriter(std::shared_ptr<CDataSink> sink, TSVGPixel width, TSVGPixel height)
- constructs CSVGWriter instance
- parameters: 
    - sink is a shared pointer to CDataSink object and it's where SVG output will be written
    - width: The width of SVG canvas in pixels
    - height: The height of the SVG canvas in pixels
~CSVGWriter()
- Destructor. Cleans up internal resources

## Public Methods
bool Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style)
- writes SVG <circle> element to sink
- parameters:
    - center is the center point of circle
    - radius is the radius of circle
    - style is the optional style attributes
- returns true if writing successful, else false

bool Rectange(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style)
- writes SVG <rect> element to sink
- parameters:
    - topleft is the top left corner of rectangle
    - size is the width and height of rectangle
    - style is otional style attributes
- returns true if writing successful, else false

bool Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style)
- writes SVG <line> element
- parameters:
    - start is the starting point of line
    - end is the ending point of line
    - style is the optional style attributes
- returns true if writing successful, else false

bool Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style)
- begins SVG <g> group element with optional attributes
- parameters:
    - attrs is the attributes for group
- returns true if writing successful, else false
