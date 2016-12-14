# Introduction

WCON (Worm tracker Commons Object Notation) is a text-based data interchange format for *C. elegans* trackers.  It is a constrained subset of [JSON](http://www.json.org/).  It is designed to be both human and machine readable and to facilitate data exchange and inter-operability for worm tracking data that is independent of platform and the language used for implementation.  Our hope is that working in a common format will also encourage code sharing and development within the community.

WCON has a minimal core description so that the basic data from a tracking experiment can be parsed by any software that is WCON-compatible, regardless of resolution or whether the file is from a single- or multi-worm tracker.  At the same time, we recognise that most groups that develop and use worm trackers have unique features they are interested in.  We have included recommendations for how such custom features should be included while preserving interchange of the basic tracking data.

## An example of a WCON file

The following is an example of a very small WCON file consisting of a single animal tracked at two time points.

```JSON
{
    "units":{"t":"seconds", "x":"mm", "y":"mm"},
    "metadata":{"strain":"N2", "who":"Rex Kerr"},
    "data":[
        {"id":"1", "t":0.0, "x":[17.2, 17.3, 17.9, 18.6, 18.8], "y":[2, 2.8, 3.3, 3.7, 4.6]},
        {"id":"1", "t":0.3, "x":[16.4, 16.9, 17.5, 18.1, 18.4], "y":[1.8, 2.4, 3, 3.4, 4.3]}
    ]
}
```

The key features of the format include a required *units* section so that it is clear what time and space values mean, an optional *metadata* section that can specify commonly-varied parameters such as strain or name of experimenter, and a *data* section that consists of entries that specify the animal (by *id*), the time point (or an array thereof), and the spine along the worm at that timepoint given by separate *x* and *y* coordinates.  (A single x and y value is permitted also, if for example the tracker does not have any shape information.)

Since WCON files are valid JSON, any of a wide number of packages that read JSON can be used to import the data.

## Formal schema

A formal schema, in JSON, specifying most of the below is available as [`wcon_schema.json`](wcon_schema.json).

## Basic implementation

### WCON must be JSON

A WCON file **must** be valid JSON.  This allows general-purpose JSON parsers to read WCON files.  Such parsers are typically much faster and much better debugged than ad-hoc parsers.  WCON data can then be extracted from the JSON data in memory.

We recommend either `.json` or `.wcon` as a file extension.

### Nomenclature

A JSON _object_ is a set of key-value pairs.  The key is always a string; the value can be anything.  This structure is useful for identifying data by name.

A JSON _array_ is a list of values.  The values can be anything, and need not be the same as each other.  However, in the WCON specification, all arrays do contain the same type of value.

A particular value may be _required_ or _optional_.  Optional values may simply be left out when not present (the whole key-value pair, if part of an object).  Values may also be _single-valued_ or _arrayed_.  A single value is just a single JSON entity of appropriate type.  An arrayed value is zero or more such values inside a JSON array.  If there is only one value, an array need not be used even if it is permitted.  If a value is required, and can be arrayed, then the array must not be empty.

### File structure

A WCON file contains a single JSON object with a minimum of two key-value pairs: `units` and `data`.  The value for `units` is an object that defines the temporal and spatial dimensions of the experiment as strings.  `data` specifies the tracking information, including time and position information, for the animal or animals tracked.

#### Example

A WCON file with single-valued `t` and arrayed `x` and `y`:

```JSON
{
    "units":{"t":"s", "x":"mm", "y":"mm"},
    "data":[
        { "id":1, "t":1.3, "x":[15.11, 16.01], "y":[24.89, 24.63] },
        { "id":2, "t":1.3, "x":[22.01, 22.35], "y":[8.06, 8.96] },
        { "id":1, "t":1.4, "x":[15.21, 16.09], "y":[24.85, 24.58] }
    ]
}
```

#### Units

Any numeric quantity followed on a per-animal or per-timepoint basis must have its units defined in an object:

```JSON
{
    "units": {
        "t": "s",
        "x": "mm",
        "y": "mm"
    },
    "data":[]
}
```

We recommend that this be done not just for values specified in the base WCON format but also inside custom data blocks.

The WCON readers and writers provided in the Tracker Commons package internally represent times in seconds and distances
in millimeters; when it is practical, we encourage other labs to adopt these to reduce the risk of loss of precision or inaccurate values due to unit conversions.  Seconds can be specified as one of `"s"`, `"second"`, or `"seconds"`), and millimetres are specified as one of `"mm"`, `"millimetre"`, or `"millimetres"`, or the American English versions "millimeter" or "millimeters".

For compound units, `*` can be used for multiplication, `/` for division, and `^` for exponentiation.  For instance, a unit of acceleration could be written `"mm^2/s"`.  Use `"1"` for a unitless quantity (or `"%"`, as appropriate).  `""` is treated as `"1"`.

Other numerical values may be used in the expressions: for instance, `"0.04*s"` can be used for frame indices in a 25 FPS sample.

`"units"` is required and must be single-valued.  Units must be specified for all quantities used.  If the data set is non-empty, that means there must be units at least for `"t"`, `"x"`, and `"y"` since those are required.  It is permitted to specify units for variables that do not exist.  An empty `"data"` section is also permitted, but WCON readers are allowed to fail on this degenerate case if units for `"t"`, `"x"`, and `"y"` are not supplied.

#### Data

The `"data"` entry contains data for one or more animals at one or more timepoints.  It may be arrayed or single-valued.

A data entry is an object with four required keys.

1. Each worm is identified by a numerical `id` so that multiple worms can be distinguished in one data set.  The `id` must be single-valued (JSON numbers or strings).

2. The time of a recording is specified by `t` (a JSON number).  It may be single-valued, for a single timepoint, or arrayed for a series of timepoints.

3. The body position of an animal is given by `x` and `y`.  These are typically arrayed and represent points along the midline of the body.  They must be the same size.  If `t` is arrayed, then `x` and `y` must be arrays of arrays, with the i<sup>th</sup> index of `x` and `y` having the same size.  If tracking is done at low resolution and the worm is defined using a single xy-coordinate, this is accommodated by simply using this single point for `x` and `y` at each time.

A data array may contain the same ID multiple times with different timepoints.

A WCON file with arrayed t and corresponding nested arrays for `x` and `y`.

```JSON
{
    "units":{"t":"s", "x":"mm", "y":"mm"},
    "data":[
        { "id":1,
          "t":[
               1.3,
               1.4,
               1.5
          ],
          "x":[
               [12.15, 13.01],
               [12.09, 12.95],
               [12.07, 12.92]
          ],
          "y":[
               [2.34, 2.74],
               [2.37, 2.76],
               [2.39, 2.77]
          ]
        }
    ]
}
```

Note that even if some xy-coordinate arrays are nested, it is still possible to repeat an `id` in the same file if the same worm has more tracks later.  Allowing repeats in `id` is convenient for multi-worm tracking data as tracks can be joined simply by updating `id` without having to copy track data.

#### Unknown Contents

**Any software that reads WCON should ignore any unrecognised key-value pair**. This is essential to make WCON flexible and allows the inclusion of custom features and additional information not required by the basic format. Below we specify a method for including custom features and other additions that are included in the accompanying software examples.

#### A Word about Numbers

All numeric values may be floating-point.  However, JSON does not support infinite or not-a-number values permitted by IEEE754 standards that cover almost every floating-point number you might use on a computer.  Since WCON must be valid JSON, custom extensions to JSON (i.e. _not JSON_ but almost) like using `infinity` unquoted to denote a floating-point positive infinity, must be avoided.

If possible, the best solution is to simply leave out non-finite numbers.  However, in some cases (e.g. with arrayed `t`) it may be more practical to indicate missing data inline.  If you wish to embed not-a-number values, use `null`.  Avoid positive or negative infinity; this is not physical data anyway, so can just be replaced with `null`.

## Including custom features

Even seemingly primitive features such as speed can have many variants (centroid vs. midbody vs. head; calculated over a window vs. frame-by-frame; etc.).  Therefore, having a standard feature called "speed" is not sufficiently flexible for wide use, and in any case it is impractical to specify in advance all quantities that may be of interest to all researchers.  Instead, WCON files allow custom features by using group-specific tags that are easily identifiable by `@` followed by a unique identifier.

For software developed in worm labs, use the lab's strain designation as a unique identifier.  A current list of lab strain designations (the first code in capital letters) is available from the [CGC](http://cbs.umn.edu/cgc/lab-code) or [WormBase](https://www.wormbase.org/resources/laboratory).  If a single group has multiple incompatible feature sets, the strain designation can be followed by a suffix separated from the lab code by a non-letter character (e.g. `"@CF Rex"` or `"@OMG_1.5"`).  For groups without a strain designation, choose an identifier that is clearly not a lab strain identifier (i.e. avoid an identifier which is two or three capital letters).

If a feature could be interpreted by other programs and has units that are supported,
it is preferable to list the feature by name and let the WCON reader convert the units
if needed.  However, it is also possible to store features in other less-accessible
forms, such as an array; this may be more compact or faster to read if one does not
anticipate that any other software could make use of the feature.

Here is an example of a WCON file with three custom features with units:

```JSON
{
    "units":{
        "t":"s", "x":"mm", "y":"mm",
        "speed":"mm/s",
        "curvature":"1/mm",
        "width":"mm"
    },
    "data":[
        { "id":1,
          "t":1.3,
          "x":[12.11, 11.87],
          "y":[5.72, 5.01],
          "@OMG":{ "speed":0.34, "curvature":1.5, "width":0.103 }
        }
    ]
}
```

Here is the same file but with an unconverted feature vector instead:

```JSON
{
    "units":{
        "t":"s", "x":"mm", "y":"mm"
    },
    "@OMG": { "feature_order":["speed", "curvature", "width"] },
    "data":[
        { "id":1,
          "t":1.3,
          "x":[12.11, 11.87],
          "y":[5.72, 5.01],
          "@OMG":[0.34, 1.5, 0.103]
        }
    ]
}
```

Note that the custom `"@OMG"` tag is used both at the top level (in this case, to specify for the custom reader the order of the parameters), and within each data entry.

For features that describe an entire plate rather than properties of individual worms (such as density or aggregate number), the values should be specified in a custom block outside of `data`:

```JSON
{
  "units":{
      "t":"s", "x":"mm", "y":"mm",
      "speed":"mm/s",
      "curvature":"1/mm",
      "width":"mm",
      "density":"1/mm^2",
      "aggregate number":"1"
  },
  "@OMG": {
      "plate_features":{
        "density":0.035,
        "aggregate number":8
      }
   },
  "data":[
      { "id":1,
        "t":1.3,
        "x":[12.11, 11.87],
        "y":[5.72, 5.01],
        "@OMG":{ "speed":0.34, "curvature":1.5, "width":0.103 }
      }
  ]
}
```

This example presents only a small number of possibilities for how a custom reader could store information within a WCON file.  The driving principle is that the custom readers and writers can do whatever they need to, so long as it is valid JSON, and any units that ought to be automatically converted are specified in the `"units"` block.

## Common additions handled in accompanying code

It is expected that users will take advantage of the flexibility of the WCON specification to create derivatives.  The following examples are handled by the accompanying code.  We recommend that general-purpose code written in other languages also handle these cases.

### Units

We recommend sticking to seconds and millimetres for time and distance and denoting those with `"s"` and `"mm"` to make automated parsing as easy as possible.  However, we recognize that these units may not always be natural, so the following should be recognized as common variants for units.

A WCON parser should, if it's going to interpret units, handle the following SI prefixes:

| Prefix   | Abbreviations | Value     |
| -------- | ------------- | --------- |
| centi    | c       | 10<sup>-2</sup> |
| milli    | m       | 10<sup>-3</sup> |
| micro    | u µ μ   | 10<sup>-6</sup> |
| nano     | n       | 10<sup>-9</sup> |
| kilo     | k       | 10<sup>3</sup>  |
| mega     | M       | 10<sup>6</sup>  |
| giga     | G       | 10<sup>9</sup>  |

_Note: JSON must be unicode, so the micro-symbol must be encoded as unicode.  There are two choices: "micro symbol" `U+00B5` or "greek mu" `U+03BC`.  Both should be handled.  Some software does not understand unicode, however, so writing `u` is preferred._

The following units should be handled (including both variants of meter/metre and celsius/centigrade):

| Unit       | Abbreviations |
| ---------- | ------------- |
| second     | s sec         |
| minute     | min           |
| hour       | h             |
| day        | d             |
| metre      | m             |
| meter      | m             |
| inch       | in            |
| micron     | *(none)*      |
| fahrenheit | F             |
| centigrade | C             |
| kelvin     | K             |
| celsius    | C             |
| percent    | %             |

Abbreviated and full versions must not be mixed.  For instance, both `"ms"` and `"milliseconds"` are okay, but `"msecond"` and `"millis"` are not.  Words may be pluralized or not.  Capitalization is significant: `"mm"` and `"Mm"` are not the same (millimetre vs megametre)!

If a numeric quantity is dimensionless, specify the units as an empty string `""` or the string `"1"`.

Compound units can be built with the operators `*` (multiplication), `/` (division), and `^` (exponentiation, with integer powers only).  Scalar factors can be included also (e.g. `"7*day"` if the units are weeks, or `"in/72"` if the units are points).  To write a reciprocal, a dimensionless `1` can be inserted.  For instance, a frequency might be represented by `"1/s"`.

Tracker Commons software will automatically convert units to the standard internal representations (e.g. inches to mm) for all fields specified in the `units` block.  It _will_ look inside anything with a custom tag.  It will _not_ look inside the `settings` block in `metadata`, nor will it look inside unknown fields.

As an example, inside

```JSON
{
    "units":{"t":"s", "x":"12*in", "y":"12*in", "e":"min", "q":"%"},
    "metadata":{
        "q":45,
        "@XJ":{ "foo": { "e": 2 }, "yes": "I think so"},
        "settings":{"q": 4, "r": 5}
    },
    "data": [{ "id":1, "t":0, "x":1, "y":2, "@XJ": {"e": 3, "f":{"p": 4}}}]
}
```

the `q` inside `metadata` would be converted from percent to a fraction, the `e` inside `foo` inside `@XJ` inside `metadata` would be converted to seconds, the `q` inside `settings` inside `metadata` would not be converted, the `e` in `data` would be converted as would the `e` in `@XJ` in `data`, and the `p` inside `f` inside `data` would not be converted.

### Experiment and software metadata

Information about experimental conditions and software versions is critical for reproducibility across labs.  JSON's flexibility makes it straightforward to include experimental conditions directly in tracking files.  This has the advantage that the data and corresponding metadata will not be separated, making the usefulness of tracking data more durable.  If further tags are required, they can of course be added, but where appropriate we recommend using the following tags.  We also recommend that additional tags be inside a custom tag for the lab in question to document who to ask to understand the contents.

An example WCON file with a complete metadata section is given below.

```JSON
{
    "metadata":{
        "lab":{ "location":"Ivory Tower, room 512", "name":"Wiggly Things Lab" },
        "who":["J. Smith", "Suzie Q."],
        "timestamp":"2012-04-23T18:25:43.511Z",
        "temperature": 20,
        "humidity": 40,
        "arena":{ "type":"petri", "size":35 },
        "food":"OP50",
        "media":"NGM",
        "sex":"hermaphrodite",
        "stage":"dauer",
        "age":38.4,
        "strain":"CB4856",
        "protocol":[
            "dauer induction by method in J. Doe, 'Get Dauers', J. of Stuff, v1 p234",
            "worm transferred to arena 1-2 minutes before recording started"
        ],
        "software":{
            "name":"Suzie's Worm Knower",
            "version":"1.1.3",
            "featureID":"@suzq"
        },
        "settings":"Note to self: hardware/software config goes here (any valid JSON)"
    },
    "units":{"t":"s", "x":"mm", "y":"mm", "temperature":"C", "humidity":"%", "size":"mm", "age":"h"},
    "data":[
        { "id":1, "t":1.3, "x":-5.3, "y":6.4, "@suzq":[true, true, false, true] }
    ]
}
```

#### Metadata fields in detail

All entries in the metadata object are optional.  Custom tags may be included in the metadata object.

| Field | Description |
|-------|-------------|
| **lab** | This is a JSON object that specifies the laboratory in which the work was done.  Valid fields include _location_ (to specify physical location or address), _name_ (to indicate the name of the laboratory as necessary), and "PI" (to indicate the principal investigator of that lab).  Since it is conceivable that an experiment could be done jointly in more than one lab (e.g. in shared space), this field may be arrayed. |
| **who** | This is an arrayed set of JSON strings that specify the name(s) of the experimenter(s). |
| **timestamp** | This should specify the time corresponding to `t = 0` in the data, using the [ISO-8601 combined date/time format](https://en.wikipedia.org/wiki/ISO_8601).  Fractional seconds and time zones are optional.  The field is single-valued. |
| **temperature** | The temperature at which the experiment takes place.  The units should be specified in the `units` block, but are assumed to be Celsius.  The field is single-valued. |
| **humidity** | The relative humidity at which the experiment takes place.  Since this is normally expressed in percentage, but dimentionless units do not default to percentage, take care to specify units of `"%"` in the `units` block.  The field is single-valued. |
| **arena** | This is a JSON object that specifies the place in which the worms are being recorded.  Subfields include _type_ which is a string description of the arena (e.g. "plate" or "slide"), _size_ which is either a single value or an array of two values which indicates the diameter or extent in each relevant direction, and _orientation_ which is a string describing how the plate or slide is oriented.  For `orientation`, use `"toward"` or `"away"` to indicate that the surface of a plate is pointing towards the camera or away from it (in the latter case one would be imaging through the agar).  The field is single-valued. |
| **food** | The food, if any, present during the experiment, as a JSON string.  If no food is present, and you wish others to know, write `""` or `"none"` rather than leaving the entry absent so it can be distinguished from the case where food is present but the metadata entry is not provided.  The field is single-valued. |
| **media** | The media on which the animal is placed, as a JSON string.  The field is single-valued. |
| **sex** | The sex of the animals, as a JSON string.  If there is a mixed population, it cannot be conveniently indicated here; instead, a custom tag should be used to specify on an animal-by-animal basis.  The field is single-valued. |
| **stage** | The stage of the animals.  If left blank, `"adult"` is normally assumed. |
| **age** |  The age of the animals, with time units as specified in `units`.  If used in aging studies, the value should be the total age of the animals, not "days of adulthood".  If the animals underwent an extended period of larval or dauer arrest, but are now a different stage, it is preferable to explain the details as text in the `protocol` section and leave this field blank.  The field is single-valued. |
| **strain** | The name of the strain as a string.  It is recommended to just have the strain designation using standard nomenclature, not extended information about the genotype.  The field is single-valued. |
| **protocol** | A text description of the protocol for the experiment.  This may be arrayed. |
| **software** |  A JSON object that specifies relevant features of the software used to capture and/or analyze the data.  Valid subfields are _name_ (the name of the software package), _version_ (a string containing the version number), and _featureID_ (an array of strings that state which custom tags the software produces, as a convenience for those wishing to know what to expect).  This field may be arrayed; the first value is presumed to be the software that captured the data, while later entries represent subsequent post-processing steps. |
| **settings** |  An arbitrary JSON value that contains any data necessary to describe parameters of the software.  It is recommended but not required that if multiple distinct software analysis steps were used that this contain an array with one value for each step.  However, the Tracker Commons software will not do anything with this information so you are free to do whatever is most useful to allow reproduction and/or comprehension of the acquisition and analysis that gave rise to this data.  Units will NOT be converted inside this block. |

### Worm orientation

To completely define a worm's orientation, it is necessary to know which part of the skeleton corresponds to the head and which side of the worm is dorsal or ventral.  Use `"head":"L"` to indicate that the head is the first xy-coordinate, `"head":"R"` to indicate that the head is the last xy-coordinate, and, if you wish to be explicit, `"head":"?"` to indicate that the orientation is uncertain.  `"ventral":"CW"` indicates that the ventral side is clockwise from the first xy-coordinate (whether or not the first coordinate is the head), `"ventral":"CCW"` that the ventral side is counterclockwise from the first point, and (if desired) `"ventral":"?"` that the dorsoventral orientation is unknown.  One or both of head and ventral can be arrayed if orientation changes within an object but in that case it must have one entry for each entry in `t`.  Single-valued entries for either head or ventral indicate that the single value applies for all times in the current object.

Knowing the head position and the ventral side, it is possible to infer whether a worm is crawling with its left or right side touching the agar surface, but only if it is known whether the image is recorded looking onto the agar surface or looking through the agar.  To allow this determination to be made, please use the `orientation` field in the `arena` field of the metadata.

An example WCON file with orientation information:

```JSON
{
    "units":{"t":"s", "x":"mm", "y":"mm"},
    "data":[
        { "id":1, "t":1.3, "x":[7.2, 7.8], "y":[0.5, -0.3],
          "head":"L",
          "ventral":"CCW"
        }
    ]
}
```

### Variable origin and centroid position

The origin used to define the worm's xy-coordinates can change over time to reduce file size by reducing the magnitude of the numbers in the worm coordinates.  For example, the origin at each time could be the centroid of the worm skeleton or the position of its head in plate coordinates.  Using this convention, as worms move, the scale of their skeleton coordinates does not increase as they move away from an arbitrary plate frame of reference.  The origin coordinates are defined by `ox` and `oy`.  If `t` is an _n_ length array, `ox` and `oy` can be either _n_ length arrays or single valued.  If they are single valued, the same origin is used for all skeletons in the current object.

It is often the case that the centroid of an animal is known with considerably greater accuracy than any other position.  The centroid positions can be specified by `cx` and `cy`, and must be given for every time point.  If `ox` and `oy` are not specified, it is assumed that the value of `cx` and `cy` is used for the origin; if `ox` and `oy` are specified, the values of `cx` and `cy` are relative to the origin.  If you wish to specify absolute numbers for `cx` and `cy` as well as `x` and `y`, use `"ox":0, "oy":0` to explicity set the origin to `(0,0)`.

Note that you must specify units for `cx`, `cy`, `ox`, and `oy` if you use them.  Also, note that they must be used in pairs: a `cx` without a `cy` is uninterpretable.

Here is an example WCON file using origin and centroid points:

```JSON
{
    "units":{
        "t":"s", "x":"mm", "y":"mm",
        "cx":"mm", "cy":"mm", "ox":"mm", "oy":"mm"
    },
    "data":[
        {"id":1, "t":1.3, "x":[7.2, 8.1], "y":[0.5, 0.3],
         "ox":32.4, "oy":9.2, "cx":7.676, "cy": 0.384}
    ]
}
```

### Splitting large WCON files into chunks

For very long tracking experiments, it may be convenient to split a single experiment across multiple WCON files.  To make it easier to reconstruct tracks across files, we support a `files` entry in the main WCON object, that obeys the following rules:

1. The variable part of the file name (for example a numerical suffix) is listed under `this`
2. To find another file, take the current file name, delete the last instance of the variable part, and replace it with the string in `prev` or `next`
3. There must be at least one `next`, and they occur in temporal order.  If the current file is the last in a series, the single value of `next` can be `null`, or it can be an empty array, or the `next` field can simply be missing.
4. There must be at least one `prev`, and they occur in **reverse** temporal order.  If the current file is the first in a series, the single value of `prev` can be `null`, an empty array, or the field can be missing.

If a WCON file is split into chunks, it is assumed that use of animal `id` values is consistent.

In this example, we imagine a directory containing files labelled `filename_0.wcon`, `filename_1.wcon`, `filename_2.wcon`, and `filename_3.wcon`.  Then `filename_2.wcon` could look like this:

```JSON
{
    "files":{
        "this":"_2",
        "prev":["_1", "_0"],
        "next":["_3"]
     },
    "units":{"t":"s", "x":"mm", "y":"mm"},
    "data":[
        { "id":1, "t":1.3, "x":[7.2, 8.1], "y":[0.5, -0.1] }
    ]
}
```

# Zipped files

Compressing JSON files typically space savings of an order of magnitude or more.  For this reason it is recommended that implementations allow files to be loaded and saved as [Zip archives](https://en.wikipedia.org/wiki/Zip_(file_format)).  If so, files must end in `".zip"`.  It is further recommended (but not required) that they end in `".wcon.zip"`.

A Zip archive can contain one or more WCON files:

- If an archive is loaded containing zero files, an error is raised.
- If an archive contains exactly one file, this file is loaded.
- If an archive contains more than one file, one file (not necessarily the first) is selected for loading.  If this file contains links via the "files" object in the specification (see above), then all linked files in the archive will be loaded.  For this reason chunks may be archived together into one `.wcon.zip` file for convenience.
