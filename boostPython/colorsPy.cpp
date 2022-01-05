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

#include "colorsPy.hpp"

#include <boost/python.hpp>

#include <QColor>

#include "colorsPy_DocStrings.hpp"
#include <vector>
#include "pyccTrace.h"

using namespace boost::python;

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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_darker_overloads, darker, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_lighter_overloads, lighter, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_setCmyk_overloads, setCmyk, 4, 5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_setCmykF_overloads, setCmykF, 4, 5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_setHsl_overloads, setHsl, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_setHslF_overloads, setHslF, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_setHsv_overloads, setHsv, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_setHsvF_overloads, setHsvF, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QColor_setRgbF_overloads, setRgbF, 3, 4)

BOOST_PYTHON_FUNCTION_OVERLOADS(QColor_fromCmyk_overloads, QColor::fromCmyk, 4, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(QColor_fromCmykF_overloads, QColor::fromCmykF, 4, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(QColor_fromHsl_overloads, QColor::fromHsl, 3, 4)
BOOST_PYTHON_FUNCTION_OVERLOADS(QColor_fromHslF_overloads, QColor::fromHslF, 3, 4)
BOOST_PYTHON_FUNCTION_OVERLOADS(QColor_fromHsv_overloads, QColor::fromHsv, 3, 4)
BOOST_PYTHON_FUNCTION_OVERLOADS(QColor_fromHsvF_overloads, QColor::fromHsvF, 3, 4)
BOOST_PYTHON_FUNCTION_OVERLOADS(QColor_fromRgbF_overloads, QColor::fromRgbF, 3, 4)
BOOST_PYTHON_FUNCTION_OVERLOADS(setRgb_py_overloads, setRgb_py, 4, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(fromRgb_py_overloads, fromRgb_py, 3, 4)
BOOST_PYTHON_FUNCTION_OVERLOADS(fromRgba64_py_overloads, fromRgba64_py, 3, 4)

void export_colors()
{
    enum_<QColor::Spec>("QColorSpec")
        .value("Invalid", QColor::Invalid)
        .value("Rgb", QColor::Rgb)
        .value("Hsv", QColor::Hsv)
        .value("Cmyk", QColor::Cmyk)
        .value("Hsl", QColor::Hsl)
        ;

    class_<QRgba64>("QRgba64")
        .def("alpha8", &QRgba64::alpha8)
        .def("alpha", &QRgba64::alpha)
        .def("blue8", &QRgba64::blue8)
        .def("blue", &QRgba64::blue)
        .def("green8", &QRgba64::green8)
        .def("green", &QRgba64::green)
        .def("isOpaque", &QRgba64::isOpaque)
        .def("isTransparent", &QRgba64::isTransparent)
        .def("premultiplied", &QRgba64::premultiplied)
        .def("red8", &QRgba64::red8)
        .def("red", &QRgba64::red)
        .def("setAlpha", &QRgba64::setAlpha)
        .def("setBlue", &QRgba64::setBlue)
        .def("setGreen", &QRgba64::setGreen)
        .def("setRed", &QRgba64::setRed)
        .def("toArgb32", &QRgba64::toArgb32)
        .def("toRgb16", &QRgba64::toRgb16)
        .def("unpremultiplied", &QRgba64::unpremultiplied)
        .def("fromQuint64", &fromQuint64_py)
        .def("toQuint64", &toQuint64_py)
        .def("fromArgb32", &QRgba64::fromArgb32)
            .staticmethod("fromArgb32")
        .def("fromRgba", &QRgba64::fromRgba)
            .staticmethod("fromRgba")
        .def("fromRgba64", &QRgba64fromRgba64_py)
            .staticmethod("fromRgba64")
        .def("fromRgba64Q", &QRgba64fromRgba64Q_py)
            .staticmethod("fromRgba64Q")
        ;

    class_<QColor>("QColor", colorsPy_QColor_doc)
        .def(init<int, int, int, int>())
        .def(init<unsigned int>())
        .def(init<QRgba64>())
        .def(init<const QString&>())
        .def("alpha", &QColor::alpha)
        .def("alphaF", &QColor::alphaF)
        .def("black", &QColor::black)
        .def("blackF", &QColor::blackF)
        .def("blue", &QColor::blue)
        .def("blueF", &QColor::blueF)
        .def("convertTo", &QColor::convertTo)
        .def("cyan", &QColor::cyan)
        .def("cyanF", &QColor::cyanF)
        .def("darker", &QColor::darker, QColor_darker_overloads(colorsPy_NotDocumented_doc))
        .def("getCmyk", &getCmyk_py)
        .def("getCmykF", &getCmykF_py)
        .def("getHsl", &getHsl_py)
        .def("getHslF", &getHslF_py)
        .def("getHsv", &getHsv_py)
        .def("getHsvF", &getHsvF_py)
        .def("getRgb", &getRgb_py)
        .def("getRgbF", &getRgbF_py)
        .def("green", &QColor::green)
        .def("greenF", &QColor::greenF)
        .def("hslHue", &QColor::hslHue)
        .def("hslHueF", &QColor::hslHueF)
        .def("hslSaturation", &QColor::hslSaturation)
        .def("hslSaturationF", &QColor::hslSaturationF)
        .def("hsvHue", &QColor::hsvHue)
        .def("hsvHueF", &QColor::hsvHueF)
        .def("hsvSaturation", &QColor::hsvSaturation)
        .def("hsvSaturationF", &QColor::hsvSaturationF)
        .def("hue", &QColor::hue)
        .def("hueF", &QColor::hueF)
        .def("isValid", &QColor::isValid)
        .def("lighter", &QColor::lighter, QColor_lighter_overloads(colorsPy_NotDocumented_doc))
        .def("lightness", &QColor::lightness)
        .def("lightnessF", &QColor::lightnessF)
        .def("magenta", &QColor::magenta)
        .def("magentaF", &QColor::magentaF)
        .def("name", &name_py)
        .def("nameARGB", &nameARGB_py)
        .def("red", &QColor::red)
        .def("redF", &QColor::redF)
        .def("rgb", &QColor::rgb)
        .def("rgba64", &QColor::rgba64)
        .def("rgba", &QColor::rgba)
        .def("saturation", &QColor::saturation)
        .def("saturationF", &QColor::saturationF)
        .def("setAlpha", &QColor::setAlpha)
        .def("setAlphaF", &QColor::setAlphaF)
        .def("setBlue", &QColor::setBlue)
        .def("setBlueF", &QColor::setBlueF)
        .def("setCmyk", &QColor::setCmyk, QColor_setCmyk_overloads(colorsPy_NotDocumented_doc))
        .def("setCmykF", &QColor::setCmykF, QColor_setCmykF_overloads(colorsPy_NotDocumented_doc))
        .def("setGreen", &QColor::setGreen)
        .def("setGreenF", &QColor::setGreenF)
        .def("setHsl", &QColor::setHsl, QColor_setHsl_overloads(colorsPy_NotDocumented_doc))
        .def("setHslF", &QColor::setHslF, QColor_setHslF_overloads(colorsPy_NotDocumented_doc))
        .def("setHsv", &QColor::setHsv, QColor_setHsv_overloads(colorsPy_NotDocumented_doc))
        .def("setHsvF", &QColor::setHsvF, QColor_setHsvF_overloads(colorsPy_NotDocumented_doc))
        .def("setNamedColor", &setNamedColor_py)
        .def("setRed", &QColor::setRed)
        .def("setRedF", &QColor::setRedF)
        .def("setRgb", &setRgb_py, setRgb_py_overloads(colorsPy_NotDocumented_doc))
        .def("setRgbUInt", &setRgbUInt_py)
        .def("setRgba64", &QColor::setRgba64)
        .def("setRgbF", &QColor::setRgbF, QColor_setRgbF_overloads(colorsPy_NotDocumented_doc))
        .def("spec", &QColor::spec)
        .def("toCmyk", &QColor::toCmyk)
        .def("toHsl", &QColor::toHsl)
        .def("toHsv", &QColor::toHsv)
        .def("toRgb", &QColor::toRgb)
        .def("value", &QColor::value)
        .def("valueF", &QColor::valueF)
        .def("yellow", &QColor::yellow)
        .def("yellowF", &QColor::yellowF)
        .def("colorNames", &colorNames_py)
            .staticmethod("colorNames")
        .def("fromCmyk", &QColor::fromCmyk, QColor_fromCmyk_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromCmyk")
        .def("fromCmykF", &QColor::fromCmykF, QColor_fromCmykF_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromCmykF")
        .def("fromHsl", &QColor::fromHsl, QColor_fromHsl_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromHsl")
        .def("fromHslF", &QColor::fromHslF, QColor_fromHslF_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromHslF")
        .def("fromHsv", &QColor::fromHsv, QColor_fromHsv_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromHsv")
        .def("fromHsvF", &QColor::fromHsvF, QColor_fromHsvF_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromHsvF")
        .def("fromRgb", &fromRgb_py, fromRgb_py_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromRgb")
        .def("fromRgbUInt", &fromRgbUInt_py)
            .staticmethod("fromRgbUInt")
        .def("fromRgba64", &fromRgba64_py, fromRgba64_py_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromRgba64")
        .def("fromRgba64Q", &fromRgba64Q_py)
            .staticmethod("fromRgba64Q")
        .def("fromRgbF", &QColor::fromRgbF, QColor_fromRgbF_overloads(colorsPy_NotDocumented_doc))
            .staticmethod("fromRgbF")
        .def("fromRgba", &QColor::fromRgba)
            .staticmethod("fromRgba")
        .def("isValidColor", &isValidColor_py)
            .staticmethod("isValidColor")
        .def(self == self)
        .def(self != self)
        ;
}



