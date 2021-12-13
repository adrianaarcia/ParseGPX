# ParseGPX
Parse GPX files to extract the location information for easier use in other applications.

## Introduction
Global Positioning System devices are used in a wide variety of applications, from ride hailing services to fitness tracking to exergaming to migratory animal tracking to illegal fishing monitoring. Many GPS devices record a GPS track, which is a record of the position of the device over time. GPS tracks are often stores in GPX files, which are a specific kind of XML file. This project aims to process GPX files to extract the location information for easier use in other applications.

## Overview
ParseGPX extracts GPS tracking information from GPX files read from standard input.

GPX, and XML in general, has many features, but for this application I allow some assumpstions that the files being processed are somewhat more restricted than general GPX/XML files.

In general, an XML file contains a prolog followed by XML elements. XML elements are delimited by start and end tags (I do not consider the special syntax for empty elements), where each tag starts with a `<` character and ends with a `>` character. Between those delimiters, a start tag contains a sequence of non-whitespace characters giving the tag type, and optionally a sequence of unique attributes, separated from the tag type by whitespace and given as a whitespace-separated list of attribute names followed by an equal sign `=` and a quoted attribute value. The end tags have only a tag type, preceded by a forward slash `/`. XML elements may contain text or other XML elements. Elements must nest properly, so any child elements must have their start tag and end tag both inside their parent element. The prolog of an XML file contains tag-like items that start and end with `<?` and `?>` or `<!` and `!>`

A GPX file is a specific kind of XML file with specific kinds of elements structured in a particular way. In particular, a GPX file contains `trkpt` elements that have a `lat` and `lon` attributes whose values give the latitude and longitude of the tracked object at some point in time. The `trkpt` elements contain `ele` and `time` elements whose text gives the elevation of the object and the time of the measurements respectively.

This program aims to extract the values of the `lat` and `lon` attributes of the `trkpt` elements along with the text of the `ele` and `time` elements contained in the `trkpt` elements: for each `trkpt`, I output a comma-separated list of the `lat` and `lon` values and `ele` and `time` text in that order. The contents of each piece of data is copied to the output. The data for each `trkpt` is written to standard output, one `trkpt` per line, with a newline at the end of each, and no other output.

## Input Specifications
Proper input should obey these rules:
* the input will be well-formed XML with one exception: tag types are not case-sensitive
* tag starts and ends nest properly
* no whitespace between the tag start `<` and the tag type or end-type character `/`
* no whitespace between the tag end-type character `/` and the tag type
* attribute values are always quoted with `"` or `'`, and the beginning and ending quote will always match
* the tag end character `>` does not mean "tag end" inside a quoted value
* the tag start character `<` appears nowhere except at the start of a tag
* quotes only appear surrounding attribute values or inside values quoted by the other type of quote
* end tag `>`, element end `/`, and attribute name/value pairing `=` symbols appear only in their special contexts or inside quoted attribute values
* there will be no newline characters in the attribute values or the text of the `ele` or `time` elements inside `trkpt` elements;
* `trkpt` elements always have `lat` and `lon` attributes that appear in that order (along with possibly other attributes mixed in before or after or between them);
* `trkpt` elements always contain a single `ele` element and a single `time` element in that order as children and never as deeper descendants (along with possibly other descendant elements before, after, or between the `ele` and `time` child elements);
* `trkpt` elements will not contain other `trkpt` elements;
* `ele` and `time` elements inside `trkpt` elements will not contain other elements;
* `ele` and `time` elements may appear outside `trkpt` elements, but will not be extracted;
* there are no CDATA, processing, comment elements, or self-closing tags.

## Example
If the input is
```
<?xml version="1.0" encoding="UTF-8"?>
<gpx creator="StravaGPX" version="1.1" xmlns="http://www.topografix.com/GPX/1/1" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
 <metadata>
  <time>2018-08-24T13:49:45Z</time>
 </metadata>
 <trk>
  <name>Morning Ride</name>
  <type>1</type>
  <trkseg>
   <trkpt lat="41.3078680" lon="-72.9342120">
    <ele>20.0</ele>
    <time>2018-08-24T13:49:45Z</time>
   </trkpt>
   <trkpt lat="41.3078680" lon="-72.9342120">
    <ele>20.0</ele>
    <time>2018-08-24T13:49:46Z</time>
   </trkpt>
   <trkpt lat="41.3078810" lon="-72.9342590">
    <ele>20.0</ele>
    <time>2018-08-24T13:49:49Z</time>
   </trkpt>
  </trkseg>
 </trk>
</gpx>
``` 
then the output must be
```
41.3078680,-72.9342120,20.0,2018-08-24T13:49:45Z
41.3078680,-72.9342120,20.0,2018-08-24T13:49:46Z
41.3078810,-72.9342590,20.0,2018-08-24T13:49:49Z
```
