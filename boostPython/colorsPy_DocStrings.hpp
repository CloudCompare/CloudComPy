//##########################################################################
//#                                                                        #
//#                              CloudComPy                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; either version 3 of the License, or     #
//#  any later version.                                                    #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#  You should have received a copy of the GNU General Public License     #
//#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
//#                                                                        #
//#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
//#                                                                        #
//##########################################################################

#ifndef COLORSPY_DOCSTRINGS_HPP_
#define COLORSPY_DOCSTRINGS_HPP_

const char* colorsPy_QColor_Spec_doc= R"(
The type of color specified, either RGB, HSV, CMYK or HSL.
See Qt documentation.)";

const char* Rgba64Py_QRgba64_doc= R"(
The QRgba64 struct contains a 64-bit RGB color.

QRgba64 is a 64-bit data-structure containing four 16-bit color channels: Red, green, blue and alpha.
QRgba64 can be used as a replacement for QRgb when higher precision is needed. 
In particular a premultiplied QRgba64 can operate on unpremultiplied QRgb without loss of precision except for alpha 0.
See Qt documentation.)";

const char* Rgba64Py_alpha8_doc= R"(
Returns the alpha channel as an 8-bit.
See Qt documentation.

:return: alpha 8 bits
:rtype: int)";

const char* Rgba64Py_alpha_doc= R"(
Returns the 16-bit alpha channel.
See Qt documentation.

:return: alpha 16 bits
:rtype: int))";

const char* Rgba64Py_blue8_doc= R"(
Returns the blue color component as an 8-bit.
See Qt documentation.

:return: blue 8 bits
:rtype: int)";

const char* Rgba64Py_blue_doc= R"(
Returns the 16-bit blue color component.
See Qt documentation.

:return: blue 16 bits
:rtype: int)";

const char* Rgba64Py_green8_doc= R"(
Returns the green color component as an 8-bit.
See Qt documentation.

:return: green 8 bits
:rtype: int)";

const char* Rgba64Py_green_doc= R"(
Returns the 16-bit green color component.
See Qt documentation.

:return: green 16 bits
:rtype: int)";

const char* Rgba64Py_isOpaque_doc= R"(
Returns whether the color is fully opaque.
See Qt documentation.

:return: True if the color is fully opaque.
:rtype: bool)";

const char* Rgba64Py_isTransparent_doc= R"(
Returns whether the color is transparent.
See Qt documentation.

:return: True if the color is transparent.
:rtype: bool)";

const char* Rgba64Py_premultiplied_doc= R"(
Returns the color with the alpha premultiplied.
See Qt documentation.

:return: color premultiplied.
:rtype: QRgba64)";

const char* Rgba64Py_red8_doc= R"(
Returns the red color component as an 8-bit.
See Qt documentation.

:return: red 8 bits
:rtype: int)";

const char* Rgba64Py_red_doc= R"(
Returns the 16-bit red color component.
See Qt documentation.

:return: red 16 bits
:rtype: int)";

const char* Rgba64Py_setAlpha_doc= R"(
Sets the alpha of this color to alpha.
See Qt documentation.

:param int alpha: alpha value.)";

const char* Rgba64Py_setBlue_doc= R"(
Sets the blue color component of this color to blue.
See Qt documentation.

:param int blue: blue value.)";

const char* Rgba64Py_setGreen_doc= R"(
Sets the green color component of this color to green.
See Qt documentation.

:param int green: green value.)";

const char* Rgba64Py_setRed_doc= R"(
Sets the red color component of this color to red.
See Qt documentation.

:param int red: red value.)";

const char* Rgba64Py_toArgb32_doc= R"(
Returns the color as a 32-bit ARGB value.
See Qt documentation.

:return: 32 bits ARGB color
:rtype: int)";

const char* Rgba64Py_toRgb16_doc= R"(
Returns the color as a 16-bit RGB value.
See Qt documentation.

:return: 16 bits RGB color
:rtype: int)";

const char* Rgba64Py_unpremultiplied_doc= R"(
Returns the color with the alpha unpremultiplied.
See Qt documentation.

:return: color unpremultiplied.
:rtype: QRgba64)";

const char* Rgba64Py_fromQuint64_py_doc= R"(
Return the color from a 64bit unsigned integer RGBA.
See Qt documentation.

:param int rgba: a 64bit unsigned integer RGBA.

:return: color.
:rtype: QRgba64)";

const char* Rgba64Py_toQuint64_py_doc= R"(
Returns the color as a 64bit unsigned integer RGBA.
See Qt documentation.

:return: 64bit unsigned integer RGBA.
:rtype: int)";

const char* Rgba64Py_fromArgb32_doc= R"(
Constructs a QRgba64 value from the 32bit ARGB value rgb.
See Qt documentation.

:param int rgb: 32bit ARGB value

:return: color.
:rtype: QRgba64)";

const char* Rgba64Py_fromRgba_doc= R"(
Constructs a QRgba64 value from the four 8-bit color channels red, green, blue and alpha.
See Qt documentation.

:param int red: 8bit red value.
:param int green: 8bit green value.
:param int blue: 8bit blue value.
:param int alpha: 8bit alpha value.

:return: color.
:rtype: QRgba64)";

const char* Rgba64Py_fromRgba64_doc= R"(
Constructs a QRgba64 value from the four 16-bit color channels red, green, blue and alpha.
See Qt documentation.

:param int red: 16bit red value.
:param int green: 16bit green value.
:param int blue: 16bit blue value.
:param int alpha: 16bit alpha value.

:return: color.
:rtype: QRgba64)";

const char* Rgba64Py_fromRgba64Q_doc= R"(
Constructs a QRgba64 value from a 64bit unsigned integer RGBA.
See Qt documentation.

:param int rgba: a 64bit unsigned integer RGBA.

:return: color.
:rtype: QRgba64)";

const char* colorsPy_QColor_doc= R"(
The Qt QColor class provides colors based on RGB, HSV or CMYK values.
See Qt documentation.

**Constructors**:

- default Constructor: invalid Color.

- copy Constructor

- Constructs a color with the RGB value r, g, b, and the alpha-channel (transparency) value a.

:param int r: red value (0 to 255)
:param int g: green value (0 to 255)
:param int b: blue value (0 to 255)
:param int a: alpha value (0 to 255)

- Constructs a color with the value color. The alpha component is ignored and set to solid.

:param int rgb: an unsigned int equivalent to an ARGB quadruplet on the format #AARRGGBB.

- Construct a Color with the value rgba64 (:py:class:`QRgba64`).

:param QRgba64 rgba64: an rgba64 value.

- Constructs a named color in the same way as :py:meth:`setNamedColor` using the given name.

The color is left invalid if the name cannot be parsed.

:param string colorName: the color name following the syntax in :py:meth:`setNamedColor`.
)";

const char* colorsPy_alpha_doc= R"(
Returns the alpha color component of this color.
See Qt documentation.

:return: alpha component int from [0..255]
:rtype: int
)";

const char* colorsPy_alphaF_doc= R"(
Returns the alpha color component of this color.
See Qt documentation.

:return: alpha component float from range [0., 1.]
:rtype: float
)";

const char* colorsPy_black_doc= R"(
Returns the black color component of this color.
See Qt documentation.

:return: black component int from [0..255]
:rtype: int
)";

const char* colorsPy_blackF_doc= R"(
Returns the black color component of this color.
See Qt documentation.

:return: black component float from range [0., 1.]
:rtype: float
)";

const char* colorsPy_blue_doc= R"(
Returns the blue color component of this color.
See Qt documentation.

:return: blue component int from [0..255]
:rtype: int
)";

const char* colorsPy_blueF_doc= R"(
Returns the blue color component of this color.
See Qt documentation.

:return: blue component float from range [0., 1.]
:rtype: float
)";

const char* colorsPy_convertTo_doc= R"(
Creates a copy of this color in the format specified by colorSpec.

See :py:class:`QColorSpec`

:param int spec: color spec

:return: converted color
:rtype: QColor
)";

const char* colorsPy_cyan_doc= R"(
Returns the cyan color component of this color.
See Qt documentation.

:return: cyan component int from [0..255]
:rtype: int
)";

const char* colorsPy_cyanF_doc= R"(
Returns the cyan color component of this color.
See Qt documentation.

:return: cyan component float from range [0., 1.]
:rtype: float
)";

const char* colorsPy_green_doc= R"(
Returns the green color component of this color.
See Qt documentation.

:return: green component int from [0..255]
:rtype: int
)";

const char* colorsPy_greenF_doc= R"(
Returns the green color component of this color.
See Qt documentation.

:return: green component float from range [0., 1.]
:rtype: float
)";

const char* colorsPy_magenta_doc= R"(
Returns the magenta color component of this color.
See Qt documentation.

:return: magenta component int from [0..255]
:rtype: int
)";

const char* colorsPy_magentaF_doc= R"(
Returns the magenta color component of this color.
See Qt documentation.

:return: magenta component float from range [0., 1.]
:rtype: float
)";

const char* colorsPy_red_doc= R"(
Returns the red color component of this color.
See Qt documentation.

:return: red component int from [0..255]
:rtype: int
)";

const char* colorsPy_redF_doc= R"(
Returns the red color component of this color.
See Qt documentation.

:return: red component float from range [0., 1.]
:rtype: float
)";

const char* colorsPy_darker_doc= R"(
Returns a darker (or lighter) color, but does not change this object.

If the factor is greater than 100, this functions returns a darker color.
Setting factor to 300 returns a color that has one-third the brightness.
If the factor is less than 100, the return color is lighter, 
but we recommend using the lighter() function for this purpose.
If the factor is 0 or negative, the return value is unspecified.

The function converts the current RGB color to HSV,
divides the value (V) component by factor and converts the color back to RGB.
See Qt documentation.

:param int,optional factor: default value 200

:return: new color object
:rtype: QColor
)";

const char* colorsPy_lighter_doc= R"(
Returns a lighter (or darker) color, but does not change this object.

If the factor is greater than 100, this functions returns a lighter color.
Setting factor to 150 returns a color that is 50% brighter.
If the factor is less than 100, the return color is darker,
but we recommend using the darker() function for this purpose.
If the factor is 0 or negative, the return value is unspecified.

The function converts the current RGB color to HSV,
multiplies the value (V) component by factor and converts the color back to RGB.
See Qt documentation.

:param int,optional factor: default value 150

:return: new color object
:rtype: QColor
)";

const char* colorsPy_getCmyk_doc= R"(
Get the cyan, magenta, yellow, black, and alpha-channel (transparency) components of the color's CMYK value.
See Qt documentation.

:return: a tuple (c, m, y, k, a), int values from [0..255]
:rtype: tuple
)";

const char* colorsPy_getCmykF_doc= R"(
Get the cyan, magenta, yellow, black, and alpha-channel (transparency) components of the color's CMYK value.
See Qt documentation.

:return: a tuple (c, m, y, k, a), float values from [0., 1.]
:rtype: tuple
)";

const char* colorsPy_getHsl_doc= R"(
Get the hue, saturation, lightness and alpha-channel (transparency) components of the color's HSL value.
See Qt documentation.

:return: a tuple (h, s, l, a), int values from [-1..359] for hue, [0..255] for saturation, lightness and alpha-channel
:rtype: tuple
)";

const char* colorsPy_getHslF_doc= R"(
Get the hue, saturation, lightness and alpha-channel (transparency) components of the color's HSL value.
See Qt documentation.

:return: a tuple (h, s, l, a), float values from [0., 1.]
:rtype: tuple
)";

const char* colorsPy_getHsv_doc= R"(
Get the hue, saturation, value and alpha-channel (transparency) components of the color's HSL value.
See Qt documentation.

:return: a tuple (h, s, v, a), int values from [-1..359] for hue, [0..255] for saturation, lightness and alpha-channel
:rtype: tuple
)";

const char* colorsPy_getHsvF_doc= R"(
Get the hue, saturation, value and alpha-channel (transparency) components of the color's HSL value.
See Qt documentation.

:return: a tuple (h, s, v, a), float values from [0., 1.]
:rtype: tuple
)";

const char* colorsPy_getRgb_doc= R"(
Get the the red, green, blue, and alpha-channel (transparency) components of the color's RGB value.
See Qt documentation.

:return: a tuple (r, g, b, a), int values from [0..255]
:rtype: tuple
)";

const char* colorsPy_getRgbF_doc= R"(
Get the the red, green, blue, and alpha-channel (transparency) components of the color's RGB value.
See Qt documentation.

:return: a tuple (r, g, b, a), float values from [0., 1.]
:rtype: tuple
)";

const char* colorsPy_hslHue_doc= R"(
Returns the HSL hue color component of this color. (-1 means achromatic)
See Qt documentation.

:return: the HSL hue color component of this color, value from [-1..359]
:rtype: int
)";

const char* colorsPy_hslHueF_doc= R"(
Returns the HSL hue color component of this color.
See Qt documentation.

:return: the HSL hue color component of this color, value from [0., 1.]
:rtype: float
)";

const char* colorsPy_hslSaturation_doc= R"(
Returns the HSL saturation color component of this color.
See Qt documentation.

:return: the HSL saturation color component of this color, value from [0..255]
:rtype: int
)";

const char* colorsPy_hslSaturationF_doc= R"(
Returns the HSL saturation color component of this color.
See Qt documentation.

:return: the HSL saturation color component of this color, value from [0., 1.]
:rtype: float
)";

const char* colorsPy_hsvHue_doc= R"(
Returns the hsv hue color component of this color. (-1 means achromatic)
See Qt documentation.

:return: the hsv hue color component of this color, value from [-1..359]
:rtype: int
)";

const char* colorsPy_hsvHueF_doc= R"(
Returns the hsv hue color component of this color.
See Qt documentation.

:return: the hsv hue color component of this color, value from [0., 1.]
:rtype: float
)";

const char* colorsPy_hsvSaturation_doc= R"(
Returns the hsv saturation color component of this color.
See Qt documentation.

:return: the hsv saturation color component of this color, value from [0..255]
:rtype: int
)";

const char* colorsPy_hsvSaturationF_doc= R"(
Returns the hsv saturation color component of this color.
See Qt documentation.

:return: the hsv saturation color component of this color, value from [0., 1.]
:rtype: float
)";

const char* colorsPy_hue_doc= R"(
Returns the HSV hue color component of this color. (-1 means achromatic)

The color is implicitly converted to HSV.
See Qt documentation.

:return: the hsv hue color component of this color, value from [-1..359]
:rtype: int
)";

const char* colorsPy_hueF_doc= R"(
Returns the HSV hue color component of this color.

The color is implicitly converted to HSV.
See Qt documentation.

:return: the hsv hue color component of this color, value from [0., 1.]
:rtype: float
)";

const char* colorsPy_isValid_doc= R"(
Returns True if the color is valid; otherwise returns False.
See Qt documentation.

:return: True if the color is valid.
:rtype: bool
)";

const char* colorsPy_isValidColor_doc= R"(
Returns True if the name is a valid color name and can be used to construct a valid QColor object, otherwise returns false.

It uses the same algorithm used in :py:meth:`setNamedColor`.
See Qt documentation.

:return: True if the color name is valid.
:rtype: bool
)";

const char* colorsPy_lightness_doc= R"(
Returns the lightness color component of this color.
See Qt documentation.

:return: lightness in [0..255]
:rtype: int
)";

const char* colorsPy_lightnessF_doc= R"(
Returns the lightness color component of this color.
See Qt documentation.

:return: lightness in [0., 1.]
:rtype: float
)";

const char* colorsPy_name_doc= R"(
Returns the name of the color in the format "#RRGGBB"; i.e. a "#" character followed by three two-digit hexadecimal numbers.
See Qt documentation.

:return: name of the color in the format "#RRGGBB".
:rtype: string
)";

const char* colorsPy_nameARGB_doc= R"(
Returns the name of the color in the format "#AARRGGBB"; i.e. a "#" character followed by four two-digit hexadecimal numbers.
See Qt documentation.

:return: name of the color in the format "#AARRGGBB".
:rtype: string
)";

const char* colorsPy_rgb_doc= R"(
Returns the RGB value of the color. The alpha value is opaque.
See Qt documentation.

:return: rgb value (unsigned int equivalent to an ARGB quadruplet on the format #AARRGGBB)
:rtype: int
)";

const char* colorsPy_rgba64_doc= R"(
Returns the RGB64 value of the color, including its alpha.

For an invalid color, the alpha value of the returned color is unspecified.
See Qt documentation.

:return: QRgba64 value
:rtype: QRgba64
)";

const char* colorsPy_rgba_doc= R"(
Returns the RGB value of the color, including its alpha.

unsigned int equivalent to an ARGB quadruplet on the format #AARRGGBB
rgb = blue + green*256 + red*256**2 + alpha*256**3 (blue, green, red, alpha: unsigned char)
For an invalid color, the alpha value of the returned color is unspecified.
See Qt documentation.

:return: rgb value (unsigned int equivalent to an ARGB quadruplet on the format #AARRGGBB)
:rtype: int
)";

const char* colorsPy_saturation_doc= R"(
Returns the HSV saturation color component of this color.

The color is implicitly converted to HSV.
See Qt documentation.

:return: saturation in [0..255]
:rtype: int
)";

const char* colorsPy_saturationF_doc= R"(
Returns the HSV saturation color component of this color.

The color is implicitly converted to HSV.
See Qt documentation.

:return: saturation in [0., 1.]
:rtype: float
)";

const char* colorsPy_setAlpha_doc= R"(
Sets the alpha of this color to alpha. Integer alpha is specified in the range 0-255.
See Qt documentation.

:param int alpha: alpha is specified in the range 0-255.
)";

const char* colorsPy_setAlphaF_doc= R"(
Sets the alpha of this color to alpha. float alpha is specified in the range [0., 1.]
See Qt documentation.

:param float alpha: alpha is specified in the range [0., 1.].
)";

const char* colorsPy_setBlue_doc= R"(
Sets the Blue of this color to Blue. Integer Blue is specified in the range 0-255.
See Qt documentation.

:param int Blue: Blue is specified in the range 0-255.
)";

const char* colorsPy_setBlueF_doc= R"(
Sets the Blue of this color to Blue. float Blue is specified in the range [0., 1.]
See Qt documentation.

:param float Blue: Blue is specified in the range [0., 1.].
)";

const char* colorsPy_setCmyk_py_doc= R"(
Sets the cyan, magenta, yellow, black, and alpha-channel (transparency) components of the color's CMYK value.

These components can be retrieved individually using the :py:meth:`cyan`, py:meth:`magenta`, py:meth:`yellow`, py:meth:`black` and py:meth:`alpha` functions.
See Qt documentation.

:param int c: cyan value (0 to 255)
:param int m: magenta value (0 to 255)
:param int y: yellow value (0 to 255)
:param int k: black value (0 to 255)
:param int,optional a: alpha value (0 to 255), default 255
)";

const char* colorsPy_setCmykF_py_doc= R"(
Sets the cyan, magenta, yellow, black, and alpha-channel (transparency) components of the color's CMYK value.

These components can be retrieved individually using the :py:meth:`cyan`, py:meth:`magenta`, py:meth:`yellow`, py:meth:`black` and py:meth:`alpha` functions.
See Qt documentation.

:param float c: cyan value from [0., 1.]
:param float m: magenta value from [0., 1.]
:param float y: yellow value from [0., 1.]
:param float k: black value  from [0., 1.]
:param float,optional a: alpha value from [0., 1.] default 1.
)";

const char* colorsPy_setGreen_doc= R"(
Sets the Green of this color to Green. Integer Green is specified in the range 0-255.
See Qt documentation.

:param int Green: Green is specified in the range 0-255.
)";

const char* colorsPy_setGreenF_doc= R"(
Sets the Green of this color to Green. float Green is specified in the range [0., 1.]
See Qt documentation.

:param float Green: Green is specified in the range [0., 1.].
)";

const char* colorsPy_setHsl_doc= R"(
Sets a HSL color value; h is the hue, s is the saturation, l is the lightness and a is the alpha component of the HSL color.

The saturation, lightness and alpha-channel values must be in the range 0-255, and the hue value must be greater than -1. [0..359]
See Qt documentation.

:param int h: hue
:param int s: saturation
:param int l: lightness
:param int,optional a: alpha, default value 255
)";

const char* colorsPy_setHslF_doc= R"(
Sets a HSL color value; h is the hue, s is the saturation, l is the lightness and a is the alpha component of the HSL color.

All the values must be in the range 0.0-1.0.
See Qt documentation.

:param int h: hue
:param int s: saturation
:param int l: lightness
:param int,optional a: alpha, default value 1.0
)";

const char* colorsPy_setHsv_doc= R"(
Sets a HSV color value; h is the hue, s is the saturation, v is the value and a is the alpha component of the HSV color.

The saturation, value and alpha-channel values must be in the range 0-255, and the hue value must be greater than -1. [0..359]
See Qt documentation.

:param int h: hue
:param int s: saturation
:param int v: value
:param int,optional a: alpha, default value 255
)";

const char* colorsPy_setHsvF_doc= R"(
Sets a HSV color value; h is the hue, s is the saturation, v is the value and a is the alpha component of the HSV color.

All the values must be in the range 0.0-1.0.
See Qt documentation.

:param int h: hue
:param int s: saturation
:param int v: value
:param int,optional a: alpha, default value 1.0
)";

const char* colorsPy_setNamedColor_doc= R"(
Sets the RGB value of this QColor to name, which may be in one of these formats:

 - #RGB (each of R, G, and B is a single hex digit)
 - #RRGGBB
 - #AARRGGBB (Since 5.2)
 - #RRRGGGBBB
 - #RRRRGGGGBBBB
 - A name from the list of colors defined in the list of SVG color keyword names provided by the World Wide Web Consortium;
   for example, "steelblue" or "gainsboro". These color names work on all platforms.
 - transparent - representing the absence of a color.

The color is invalid if name cannot be parsed.
See Qt documentation.

:param string colorName: color name. see :py:meth:`colorNames` for a list of valid keyword names
)";

const char* colorsPy_setRed_doc= R"(
Sets the Red of this color to Red. Integer Red is specified in the range 0-255.
See Qt documentation.

:param int Red: Red is specified in the range 0-255.
)";

const char* colorsPy_setRedF_doc= R"(
Sets the Red of this color to Red. float Red is specified in the range [0., 1.]
See Qt documentation.

:param float Red: Green is specified in the range [0., 1.].
)";

const char* colorsPy_setRgb_doc= R"(
Sets the RGB value to r, g, b and the alpha value to a.

All the values must be in the range 0-255.
See Qt documentation.

:param int r: red
:param int g: green
:param int b: blue
:param int,optional a: alpha
)";

const char* colorsPy_setRgbUInt_doc= R"(
Sets the RGB value to rgb (unsigned integer color code). The alpha value is set to opaque.
See Qt documentation.

:param int rgb: unsigned integer color code (ARGB quadruplet on the format #AARRGGBB)
)";

const char* colorsPy_setRgba64_doc= R"(
Sets the RGB64 value to rgba, including its alpha.
See Qt documentation.

:param QRgba64 rgba: 64-bit RGB color structure
)";

const char* colorsPy_setRgbF_doc= R"(
Sets the color channels of this color to r (red), g (green), b (blue) and a (alpha, transparency).

All values must be in the range 0.0-1.0.
See Qt documentation.

:param float r: red
:param float g: green
:param float b: blue
:param float,optional a: alpha
)";

const char* colorsPy_spec_doc= R"(
Returns how the color was specified.

See Qt documentation.

:return: spec value
:rtype: QColorSpec
)";

const char* colorsPy_toCmyk_doc= R"(
Creates and returns a CMYK QColor based on this color.
See Qt documentation.

:return: a CMYK QColor based on this color.
:rtype: QColor
)";

const char* colorsPy_toHsl_doc= R"(
Creates and returns an HSL QColor based on this color.
See Qt documentation.

:return: an HSL QColor based on this color.
:rtype: QColor
)";

const char* colorsPy_toHsv_doc= R"(
Creates and returns an HSV QColor based on this color.
See Qt documentation.

:return: an HSV QColor based on this color.
:rtype: QColor
)";

const char* colorsPy_toRgb_doc= R"(
Creates and returns an RGB QColor based on this color.
See Qt documentation.

:return: an RGB QColor based on this color.
:rtype: QColor
)";

const char* colorsPy_value_doc= R"(
Returns the value color component of this color.
See Qt documentation.

:return: value color component [0..255]
:rtype: int
)";

const char* colorsPy_valueF_doc= R"(
Returns the value color component of this color.
See Qt documentation.

:return: value color component [0., 1.]
:rtype: float
)";

const char* colorsPy_yellow_doc= R"(
Returns the yellow color component of this color.
See Qt documentation.

:return: yellow color component [0..255]
:rtype: int
)";

const char* colorsPy_yellowF_doc= R"(
Returns the yellow color component of this color.
See Qt documentation.

:return: yellow color component [0., 1.]
:rtype: float
)";

const char* colorsPy_colorNames_doc= R"(
Returns a list of strings containing the color names Qt knows about.
Static function.
See Qt documentation.

:return: list of Qt valid color names
:rtype: list
)";

const char* colorsPy_fromCmyk_doc= R"(
Static convenience function that returns a QColor constructed from the given CMYK color values:
c (cyan), m (magenta), y (yellow), k (black), and a (alpha-channel, i.e. transparency).
All the values must be in the range 0-255.
See Qt documentation.

:param int c: cyan
:param int m: magenta
:param int y: yellow
:param int k: black
:param int,optional a: alpha, default 255

:return: a CMYK QColor
:rtype: QColor
)";

const char* colorsPy_fromCmykF_doc= R"(
Static convenience function that returns a QColor constructed from the given CMYK color values:
c (cyan), m (magenta), y (yellow), k (black), and a (alpha-channel, i.e. transparency).
All the values must be in the range [0., 1.].
See Qt documentation.

:param float c: cyan
:param float m: magenta
:param float y: yellow
:param float k: black
:param float,optional a: alpha, default 1.

:return: a CMYK QColor
:rtype: QColor
)";

const char* colorsPy_fromHsl_doc= R"(
Static convenience function that returns a QColor constructed from the HSL color values:
h (hue), s (saturation), l (lightness), and a (alpha-channel, i.e. transparency).
The value of s, l, and a must all be in the range 0-255; the value of h must be in the range 0-359.
See Qt documentation.

:param int h: hue
:param int s: saturation
:param int l: lightness
:param int,optional a: alpha, default 255

:return: an HSL QColor
:rtype: QColor
)";

const char* colorsPy_fromHslF_doc= R"(
Static convenience function that returns a QColor constructed from the HSL color values:
h (hue), s (saturation), l (lightness), and a (alpha-channel, i.e. transparency).
All the values must be in the range 0.0-1.0.
See Qt documentation.

:param float h: hue
:param float s: saturation
:param float l: lightness
:param float,optional a: alpha, default 1.

:return: an HSL QColor
:rtype: QColor
)";

const char* colorsPy_fromHsv_doc= R"(
Static convenience function that returns a QColor constructed from the HSV color values:
h (hue), s (saturation), v (value), and a (alpha-channel, i.e. transparency).
The value of s, v, and a must all be in the range 0-255; the value of h must be in the range 0-359.
See Qt documentation.

:param int h: hue
:param int s: saturation
:param int v: value
:param int,optional a: alpha, default 255

:return: an HSV QColor
:rtype: QColor
)";

const char* colorsPy_fromHsvF_doc= R"(
Static convenience function that returns a QColor constructed from the HSV color values:
h (hue), s (saturation), v (value), and a (alpha-channel, i.e. transparency).
All the values must be in the range 0.0-1.0.
See Qt documentation.

:param float h: hue
:param float s: saturation
:param float v: value
:param float,optional a: alpha, default 255

:return: an HSV QColor
:rtype: QColor
)";

const char* colorsPy_fromRgb_doc= R"(
Static convenience function that returns a QColor constructed from the RGB color values,
r (red), g (green), b (blue), and a (alpha-channel, i.e. transparency).
All the values must be in the range 0-255.
See Qt documentation.

:param int r: red
:param int g: green
:param int b: blue
:param int,optional a: alpha, default 255

:return: an RGB QColor
:rtype: QColor
)";

const char* colorsPy_fromRgbUInt_doc= R"(
Static convenience function that returns a QColor constructed from
the given unsigned integer value rgb (ARGB quadruplet on the format #AARRGGBB).
The alpha component of rgb is ignored (i.e. it is automatically set to 255),
use the :py:meth:`fromRgba` function to include the alpha-channel specified by the given QRgb value.
See Qt documentation.

:param int rgb: ARGB quadruplet on the format #AARRGGBB).

:return: an RGB QColor
:rtype: QColor
)";


const char* colorsPy_fromRgba64_py_doc= R"(
Static convenience function that returns a QColor constructed from the RGBA64 color values,
r (red), g (green), b (blue), and a (alpha-channel, i.e. transparency).
All values: unsigned short = 16bit range 0..65535
See Qt documentation.

:param int r: red
:param int g: green
:param int b: blue
:param int,optional a: alpha, default 65535

:return: an RGB QColor
:rtype: QColor
)";

const char* colorsPy_fromRgba64Q_doc= R"(
Static convenience function that returns a QColor constructed from the given :py:class:`QRgba64` value rgba64.
See Qt documentation.

:param QRgba64 rgba64: an rgba64 value.

:return: an RGB QColor
:rtype: QColor
)";

const char* colorsPy_fromRgbF_doc= R"(
Static convenience function that returns a QColor constructed from the RGB color values,
r (red), g (green), b (blue), and a (alpha-channel, i.e. transparency).
All the values must be in the range 0.0-1.0.
See Qt documentation.

:param float r: red
:param float g: green
:param float b: blue
:param float,optional a: alpha, default 1.0

:return: an RGB QColor
:rtype: QColor
)";

const char* colorsPy_fromRgba_doc= R"(
Static convenience function that returns a QColor constructed from 
the given unsigned integer value rgb (ARGB quadruplet on the format #AARRGGBB).
Unlike the :py:meth:`fromRgbUInt` function, the alpha-channel specified by the given QRgb value is included.
See Qt documentation.

:param int rgb: ARGB quadruplet on the format #AARRGGBB).

:return: an RGB QColor
:rtype: QColor
)";

const char* colorsPy_equal_doc= R"(
Returns True if this color has the same RGB and alpha values as color; otherwise returns false.
See Qt documentation.

:return: True if this color has the same RGB and alpha values as color.
:rtype: bool
)";


const char* colorsPy_differ_doc= R"(
Returns True if this color a different RGB and alpha values as color; otherwise returns false.
See Qt documentation.

:return: True if this color has a different RGB and alpha values as color.
:rtype: bool
)";

//const char* colorsPy_NotDocumented_doc= R"(not documented yet :-()";

#endif /* COLORSPY_DOCSTRINGS_HPP_ */
