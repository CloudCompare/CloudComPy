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

#include "cloudComPy.hpp"

#include <QColor>

#include "colorsPy_DocStrings.hpp"
#include <vector>
#include "pyccTrace.h"

std::vector<QString> colorNames_py()
{
    QStringList colors = QColor::colorNames();
    CCTRACE("colors.size: " << colors.size());
    std::vector<QString> names;
    names.resize(colors.size());
    for (int i = 0; i < colors.size(); ++i)
    {
        names[i] = colors[i];
    }
    return names;
}

std::vector<int> getCmyk_py(QColor& self)
{
    int c, y, m, k, a;
    self.getCmyk(&c, &y, &m, &k, &a);
    std::vector<int> vec = {c, y, m, k, a};
    return vec;
}

std::vector<double> getCmykF_py(QColor& self)
{
    double c, y, m, k, a;
    self.getCmykF(&c, &y, &m, &k, &a);
    std::vector<double> vec = {c, y, m, k, a};
    return vec;
}

std::vector<int> getHsl_py(QColor& self)
{
    int h, s, l, a;
    self.getHsl(&h, &s, &l, &a);
    std::vector<int> vec = {h, s, l, a};
    return vec;
}

std::vector<double> getHslF_py(QColor& self)
{
    double h, s, l, a;
    self.getHslF(&h, &s, &l, &a);
    std::vector<double> vec = {h, s, l, a};
    return vec;
}

std::vector<int> getHsv_py(QColor& self)
{
    int h, s, v, a;
    self.getHsv(&h, &s, &v, &a);
    std::vector<int> vec = {h, s, v, a};
    return vec;
}

std::vector<double> getHsvF_py(QColor& self)
{
    double h, s, v, a;
    self.getHsvF(&h, &s, &v, &a);
    std::vector<double> vec = {h, s, v, a};
    return vec;
}

std::vector<int> getRgb_py(QColor& self)
{
    int r, g, b, a;
    self.getRgb(&r, &g, &b, &a);
    std::vector<int> vec = {r, g, b, a};
    return vec;
}

std::vector<double> getRgbF_py(QColor& self)
{
    double r, g, b, a;
    self.getRgbF(&r, &g, &b, &a);
    std::vector<double> vec = {r, g, b, a};
    return vec;
}

QString name_py(QColor& self)
{
    return self.name();
}

QString nameARGB_py(QColor& self)
{
    return self.name(QColor::HexArgb);
}

void setNamedColor_py(QColor& self, const QString& colorName)
{
    self.setNamedColor(colorName);
}

void setRgb_py(QColor& self, int r, int g, int b, int a = 255)
{
    self.setRgb(r, g, b, a);

}

void setRgbUInt_py(QColor& self, unsigned int rgb)
{
    self.setRgb(rgb);

}

QColor fromRgbUInt_py(unsigned int rgb)
{
    return QColor::fromRgb(rgb);
}

QColor fromRgb_py(int r, int g, int b, int a = 255)
{
    return QColor::fromRgb(r, g, b, a);
}

QColor fromRgba64_py(ushort r, ushort g, ushort b, ushort a = USHRT_MAX)
{
    return QColor::fromRgba64(r, g, b, a);
}

QColor fromRgba64Q_py(QRgba64 rgba64)
{
    return QColor::fromRgba64(rgba64);
}

bool isValidColor_py(const QString &name)
{
    return QColor::isValidColor(name);
}

QRgba64 fromQuint64_py(QRgba64& self, unsigned long long rgba64)
{
    self = rgba64;
    return self;
}

unsigned long long toQuint64_py(QRgba64& self)
{
    return quint64(self);
}

QRgba64 QRgba64fromRgba64Q_py(unsigned long long c)
{
    return QRgba64::fromRgba64(c);
}

QRgba64 QRgba64fromRgba64_py(int r, int g, int b, int a)
{
    return QRgba64::fromRgba64(r, g, b, a);
}

void export_colors(py::module &m0)
{
    py::enum_<QColor::Spec>(m0, "QColorSpec", colorsPy_QColor_Spec_doc)
        .value("Invalid", QColor::Invalid)
        .value("Rgb", QColor::Rgb)
        .value("Hsv", QColor::Hsv)
        .value("Cmyk", QColor::Cmyk)
        .value("Hsl", QColor::Hsl)
        .export_values();

    py::class_<QRgba64>(m0, "QRgba64", Rgba64Py_QRgba64_doc)
        .def(py::init<>())
        .def("alpha8", &QRgba64::alpha8, Rgba64Py_alpha8_doc)
        .def("alpha", &QRgba64::alpha, Rgba64Py_alpha_doc)
        .def("blue8", &QRgba64::blue8, Rgba64Py_blue8_doc)
        .def("blue", &QRgba64::blue, Rgba64Py_blue_doc)
        .def("green8", &QRgba64::green8, Rgba64Py_green8_doc)
        .def("green", &QRgba64::green, Rgba64Py_green_doc)
        .def("isOpaque", &QRgba64::isOpaque, Rgba64Py_isOpaque_doc)
        .def("isTransparent", &QRgba64::isTransparent, Rgba64Py_isTransparent_doc)
        .def("premultiplied", &QRgba64::premultiplied, Rgba64Py_premultiplied_doc)
        .def("red8", &QRgba64::red8, Rgba64Py_red8_doc)
        .def("red", &QRgba64::red, Rgba64Py_red_doc)
        .def("setAlpha", &QRgba64::setAlpha, Rgba64Py_setAlpha_doc)
        .def("setBlue", &QRgba64::setBlue, Rgba64Py_setBlue_doc)
        .def("setGreen", &QRgba64::setGreen, Rgba64Py_setGreen_doc)
        .def("setRed", &QRgba64::setRed, Rgba64Py_setRed_doc)
        .def("toArgb32", &QRgba64::toArgb32, Rgba64Py_toArgb32_doc)
        .def("toRgb16", &QRgba64::toRgb16, Rgba64Py_toRgb16_doc)
        .def("unpremultiplied", &QRgba64::unpremultiplied, Rgba64Py_unpremultiplied_doc)
        .def("fromQuint64", &fromQuint64_py, Rgba64Py_fromQuint64_py_doc)
        .def("toQuint64", &toQuint64_py, Rgba64Py_toQuint64_py_doc)
        .def_static("fromArgb32", &QRgba64::fromArgb32, Rgba64Py_fromArgb32_doc)
        .def_static("fromRgba", &QRgba64::fromRgba, Rgba64Py_fromRgba_doc)
        .def_static("fromRgba64", &QRgba64fromRgba64_py, Rgba64Py_fromRgba64_doc)
        .def_static("fromRgba64Q", &QRgba64fromRgba64Q_py, Rgba64Py_fromRgba64Q_doc)
        ;

    py::class_<QColor>(m0, "QColor", colorsPy_QColor_doc)
        .def(py::init<>())
        .def(py::init<int, int, int, int>())
        .def(py::init<unsigned int>())
        .def(py::init<QRgba64>())
        .def(py::init<const QString&>())
        .def("alpha", &QColor::alpha, colorsPy_alpha_doc)
        .def("alphaF", &QColor::alphaF, colorsPy_alphaF_doc)
        .def("black", &QColor::black, colorsPy_black_doc)
        .def("blackF", &QColor::blackF, colorsPy_blackF_doc)
        .def("blue", &QColor::blue, colorsPy_blue_doc)
        .def("blueF", &QColor::blueF, colorsPy_blueF_doc)
        .def("convertTo", &QColor::convertTo, colorsPy_convertTo_doc)
        .def("cyan", &QColor::cyan, colorsPy_cyan_doc)
        .def("cyanF", &QColor::cyanF, colorsPy_cyanF_doc)
        .def("darker", &QColor::darker, py::arg("factor")=200, colorsPy_darker_doc)
        .def("getCmyk", &getCmyk_py, colorsPy_getCmyk_doc)
        .def("getCmykF", &getCmykF_py, colorsPy_getCmykF_doc)
        .def("getHsl", &getHsl_py, colorsPy_getHsl_doc)
        .def("getHslF", &getHslF_py, colorsPy_getHslF_doc)
        .def("getHsv", &getHsv_py, colorsPy_getHsv_doc)
        .def("getHsvF", &getHsvF_py, colorsPy_getHsvF_doc)
        .def("getRgb", &getRgb_py, colorsPy_getRgb_doc)
        .def("getRgbF", &getRgbF_py, colorsPy_getRgbF_doc)
        .def("green", &QColor::green, colorsPy_green_doc)
        .def("greenF", &QColor::greenF, colorsPy_greenF_doc)
        .def("hslHue", &QColor::hslHue, colorsPy_hslHue_doc)
        .def("hslHueF", &QColor::hslHueF, colorsPy_hslHueF_doc)
        .def("hslSaturation", &QColor::hslSaturation, colorsPy_hslSaturation_doc)
        .def("hslSaturationF", &QColor::hslSaturationF, colorsPy_hslSaturationF_doc)
        .def("hsvHue", &QColor::hsvHue, colorsPy_hsvHue_doc)
        .def("hsvHueF", &QColor::hsvHueF, colorsPy_hsvHueF_doc)
        .def("hsvSaturation", &QColor::hsvSaturation, colorsPy_hsvSaturation_doc)
        .def("hsvSaturationF", &QColor::hsvSaturationF, colorsPy_hsvSaturationF_doc)
        .def("hue", &QColor::hue, colorsPy_hue_doc)
        .def("hueF", &QColor::hueF, colorsPy_hueF_doc)
        .def("isValid", &QColor::isValid, colorsPy_isValid_doc)
        .def("lighter", &QColor::lighter, py::arg("factor")=150, colorsPy_lighter_doc)
        .def("lightness", &QColor::lightness, colorsPy_lightness_doc)
        .def("lightnessF", &QColor::lightnessF,colorsPy_lightnessF_doc )
        .def("magenta", &QColor::magenta, colorsPy_magenta_doc)
        .def("magentaF", &QColor::magentaF, colorsPy_magentaF_doc)
        .def("name", &name_py, colorsPy_name_doc)
        .def("nameARGB", &nameARGB_py, colorsPy_nameARGB_doc)
        .def("red", &QColor::red, colorsPy_red_doc)
        .def("redF", &QColor::redF, colorsPy_redF_doc)
        .def("rgb", &QColor::rgb, colorsPy_rgb_doc)
        .def("rgba64", &QColor::rgba64, colorsPy_rgba64_doc)
        .def("rgba", &QColor::rgba, colorsPy_rgba_doc)
        .def("saturation", &QColor::saturation, colorsPy_saturation_doc)
        .def("saturationF", &QColor::saturationF, colorsPy_saturationF_doc)
        .def("setAlpha", &QColor::setAlpha, colorsPy_setAlpha_doc)
        .def("setAlphaF", &QColor::setAlphaF, colorsPy_setAlphaF_doc)
        .def("setBlue", &QColor::setBlue, colorsPy_setBlue_doc)
        .def("setBlueF", &QColor::setBlueF, colorsPy_setBlueF_doc)
        .def("setCmyk", &QColor::setCmyk,
             py::arg("c"), py::arg("m"), py::arg("y"), py::arg("k"), py::arg("a")=255,
             colorsPy_setCmyk_py_doc)
        .def("setCmykF", &QColor::setCmykF,
             py::arg("c"), py::arg("m"), py::arg("y"), py::arg("k"), py::arg("a")=1.0,
             colorsPy_setCmykF_py_doc)
        .def("setGreen", &QColor::setGreen, colorsPy_setGreen_doc)
        .def("setGreenF", &QColor::setGreenF, colorsPy_setGreenF_doc)
        .def("setHsl", &QColor::setHsl,
             py::arg("h"), py::arg("s"), py::arg("l"), py::arg("a")=255,
             colorsPy_setHsl_doc)
        .def("setHslF", &QColor::setHslF,
             py::arg("h"), py::arg("s"), py::arg("l"), py::arg("a")=1.0,
             colorsPy_setHslF_doc)
        .def("setHsv", &QColor::setHsv,
             py::arg("h"), py::arg("s"), py::arg("v"), py::arg("a")=255,
             colorsPy_setHsv_doc)
        .def("setHsvF", &QColor::setHsvF,
             py::arg("h"), py::arg("s"), py::arg("v"), py::arg("a")=1.0,
             colorsPy_setHsvF_doc)
        .def("setNamedColor", &setNamedColor_py, colorsPy_setNamedColor_doc)
        .def("setRed", &QColor::setRed, colorsPy_setRed_doc)
        .def("setRedF", &QColor::setRedF, colorsPy_setRedF_doc)
        .def("setRgb", &setRgb_py,
              py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a")=255,
             colorsPy_setRgb_doc)
        .def("setRgbUInt", &setRgbUInt_py, colorsPy_setRgbUInt_doc)
        .def("setRgba64", &QColor::setRgba64, colorsPy_setRgba64_doc)
        .def("setRgbF", &QColor::setRgbF,
             py::arg("r"), py::arg("g"), py::arg("g"), py::arg("a")=1.0,
             colorsPy_setRgbF_doc)
        .def("spec", &QColor::spec, colorsPy_spec_doc)
        .def("toCmyk", &QColor::toCmyk, colorsPy_toCmyk_doc)
        .def("toHsl", &QColor::toHsl, colorsPy_toHsl_doc)
        .def("toHsv", &QColor::toHsv, colorsPy_toHsv_doc)
        .def("toRgb", &QColor::toRgb, colorsPy_toRgb_doc)
        .def("value", &QColor::value, colorsPy_value_doc)
        .def("valueF", &QColor::valueF, colorsPy_valueF_doc)
        .def("yellow", &QColor::yellow, colorsPy_yellow_doc)
        .def("yellowF", &QColor::yellowF, colorsPy_yellowF_doc)
        .def_static("colorNames", &colorNames_py, colorsPy_colorNames_doc)
        .def_static("fromCmyk", &QColor::fromCmyk,
             py::arg("c"), py::arg("m"), py::arg("y"), py::arg("k"), py::arg("a")=255,
             colorsPy_fromCmyk_doc)
        .def_static("fromCmykF", &QColor::fromCmykF,
             py::arg("c"), py::arg("m"), py::arg("y"), py::arg("k"), py::arg("a")=1.0,
             colorsPy_fromCmykF_doc)
        .def_static("fromHsl", &QColor::fromHsl,
             py::arg("h"), py::arg("s"), py::arg("l"), py::arg("a")=255,
             colorsPy_fromHsl_doc)
        .def_static("fromHslF", &QColor::fromHslF,
             py::arg("h"), py::arg("s"), py::arg("l"), py::arg("a")=1.0,
             colorsPy_fromHslF_doc)
        .def_static("fromHsv", &QColor::fromHsv,
             py::arg("h"), py::arg("s"), py::arg("v"), py::arg("a")=255,
             colorsPy_fromHsv_doc)
        .def_static("fromHsvF", &QColor::fromHsvF,
             py::arg("h"), py::arg("s"), py::arg("v"), py::arg("a")=1.0,
             colorsPy_fromHsvF_doc)
        .def_static("fromRgb", &fromRgb_py,
             py::arg("r"), py::arg("g"), py::arg("g"), py::arg("a")=255,
             colorsPy_fromRgb_doc)
        .def_static("fromRgbUInt", &fromRgbUInt_py, colorsPy_fromRgbUInt_doc)
        .def_static("fromRgba64", &fromRgba64_py,
             py::arg("r"), py::arg("g"), py::arg("g"), py::arg("a")=USHRT_MAX,
             colorsPy_fromRgba64_py_doc)
        .def_static("fromRgba64Q", &fromRgba64Q_py, colorsPy_fromRgba64Q_doc)
        .def_static("fromRgbF", &QColor::fromRgbF,
             py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a")=1.0,
             colorsPy_fromRgbF_doc)
        .def_static("fromRgba", &QColor::fromRgba, colorsPy_fromRgba_doc)
        .def_static("isValidColor", &isValidColor_py, colorsPy_isValidColor_doc)
        .def(py::self == py::self)
        .def(py::self != py::self)
        ;
}



