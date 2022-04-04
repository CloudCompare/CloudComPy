#!/usr/bin/env python3

##########################################################################
#                                                                        #
#                              CloudComPy                                #
#                                                                        #
#  This program is free software; you can redistribute it and/or modify  #
#  it under the terms of the GNU General Public License as published by  #
#  the Free Software Foundation; either version 3 of the License, or     #
#  any later version.                                                    #
#                                                                        #
#  This program is distributed in the hope that it will be useful,       #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#  GNU General Public License for more details.                          #
#                                                                        #
#  You should have received a copy of the GNU General Public License     #
#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
#                                                                        #
#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
#                                                                        #
##########################################################################

import os
import sys
import math

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, dataDir, isCoordEqual
import cloudComPy as cc
import numpy as np

# --- QColor from Qt

c = cc.QColor()
if c.isValid():
    raise RuntimeError

c.setRed(80)
c.setGreen(100)
c.setBlue(120)
if not c.isValid():
    raise RuntimeError

c.setAlpha(160)
comp = (c.red(), c.green(), c.blue(), c.alpha())
cref= (80, 100, 120, 160)
for i in range(4):
    if not comp[i] == cref[i]:
        raise RuntimeError

c.setRedF(0.5)
c.setGreenF(0.6)
c.setBlueF(0.7)
c.setAlphaF(1.0)

refF = (0.5, 0.6, 0.7, 1.0)
compF = (c.redF(), c.greenF(), c.blueF(), c.alphaF())
for i in range(4):
    if not math.isclose(refF[i], compF[i], rel_tol=1e-04):
        raise RuntimeError

names = cc.QColor.colorNames()
if len(names) < 148:
    raise RuntimeError
if not 'turquoise' in names:
    raise RuntimeError

if not c.spec() == cc.QColorSpec.Rgb:
    raise RuntimeError
cHsl = c.convertTo(cc.QColorSpec.Hsl)
if not cHsl.spec() == cc.QColorSpec.Hsl:
    raise RuntimeError
cHsv = c.convertTo(cc.QColorSpec.Hsv)
if not cHsv.spec() == cc.QColorSpec.Hsv:
    raise RuntimeError
cCmyk = c.convertTo(cc.QColorSpec.Cmyk)
if not cCmyk.spec() == cc.QColorSpec.Cmyk:
    raise RuntimeError

compg = (c.red(), c.green(), c.blue(), c.alpha())
compl = (cHsl.red(), cHsl.green(), cHsl.blue(), cHsl.alpha())
compv = (cHsv.red(), cHsv.green(), cHsv.blue(), cHsv.alpha())
compk = (cCmyk.red(), cCmyk.green(), cCmyk.blue(), cCmyk.alpha())
cref = (128, 153, 179, 255)
for i in range(4):
    if compg[i] != cref[i] or compl[i] != cref[i] or compv[i] != cref[i] or compk[i] != cref[i]:
        raise RuntimeError

d = c.darker()
compdF = (d.redF(), d.greenF(), d.blueF(), d.alphaF())
refdF = (0.25, 0.3, 0.35, 1.0)
for i in range(4):
    if not math.isclose(refdF[i], compdF[i], rel_tol=1e-04):
        raise RuntimeError

cCmyk = cc.QColor.fromCmyk(120, 140, 160, 80)
compk = (cCmyk.red(), cCmyk.green(), cCmyk.blue(), cCmyk.alpha())
comyk = (cCmyk.cyan(),cCmyk.magenta(), cCmyk.yellow(), cCmyk.black())
refc = (120, 140, 160, 80)
refr = (93, 79, 65, 255)
for i in range(4):
    if not (comyk[i] == refc[i] and compk[i] == refr[i]):
        raise RuntimeError

cCmyk = cc.QColor.fromCmykF(0.5, 0.6, 0.7, 0.8)
comyk = (cCmyk.cyan(),cCmyk.magenta(), cCmyk.yellow(), cCmyk.black())
comykF = (cCmyk.cyanF(),cCmyk.magentaF(), cCmyk.yellowF(), cCmyk.blackF())
refc = (128, 153, 179, 204)
refF = (0.5, 0.6, 0.7, 0.8)
for i in range(4):
    if not (comyk[i] == refc[i] and math.isclose(comykF[i], refF[i], rel_tol=1e-04)):
        raise RuntimeError

cHsl = cc.QColor.fromHsl(60, 180, 200)
compl = (cHsl.red(), cHsl.green(), cHsl.blue(), cHsl.alpha())
cohsl = (cHsl.hslHue(), cHsl.hslSaturation(), cHsl.lightness(), cHsl.alpha())
refc = (60, 180,200, 255)
refr = (239, 239, 161, 255)
for i in range(4):
    if not (cohsl[i] == refc[i] and compl[i] == refr[i]):
        raise RuntimeError

cHsl = cc.QColor.fromHslF(0.5, 0.6, 0.7)
cohsl = (cHsl.hslHue(), cHsl.hslSaturation(), cHsl.lightness(), cHsl.alpha())
cohslF = (cHsl.hslHueF(), cHsl.hslSaturationF(), cHsl.lightnessF(), cHsl.alphaF())
refc = (180, 153, 179, 255)
refF = (0.5, 0.6, 0.7, 1.0)
for i in range(4):
    if not (cohsl[i] == refc[i] and math.isclose(cohslF[i], refF[i], rel_tol=1e-04)):
        raise RuntimeError

cHsv = cc.QColor.fromHsv(60, 180, 200)
compv = (cHsv.red(), cHsv.green(), cHsv.blue(), cHsv.alpha())
cohsv = (cHsv.hsvHue(), cHsv.hsvSaturation(), cHsv.value(), cHsv.alpha())
refc = (60, 180,200, 255)
refr = (200, 200, 59, 255)
for i in range(4):
    if not (cohsv[i] == refc[i] and compv[i] == refr[i]):
        raise RuntimeError

cHsv = cc.QColor.fromHsvF(0.5, 0.6, 0.7)
cohsv = (cHsv.hsvHue(), cHsv.hslSaturation(), cHsv.value(), cHsv.alpha())
cohsvF = (cHsv.hsvHueF(), cHsv.hsvSaturationF(), cHsv.valueF(), cHsv.alphaF())
refc = (180, 109, 179, 255)
refF = (0.5, 0.6, 0.7, 1.0)
for i in range(4):
    if not (cohsv[i] == refc[i] and math.isclose(cohsvF[i], refF[i], rel_tol=1e-04)):
        raise RuntimeError

if cHsv.hsvHue() != cHsv.hue() or cHsv.saturation() != cHsv.hsvSaturation():
    raise RuntimeError
if cHsv.hsvHueF() != cHsv.hueF() or cHsv.saturationF() != cHsv.hsvSaturationF():
    raise RuntimeError

cRgb = cc.QColor.fromRgb(160, 180, 200)
corgb = (cRgb.red(), cRgb.green(), cRgb.blue(), cRgb.alpha())
refc = (160, 180, 200, 255)
for i in range(4):
    if not (corgb[i] == refc[i]):
        raise RuntimeError

cRgb = cc.QColor.fromRgbF(0.5, 0.6, 0.7)
corgb = (cRgb.red(), cRgb.green(), cRgb.blue(), cRgb.alpha())
corgbF = (cRgb.redF(), cRgb.greenF(), cRgb.blueF(), cRgb.alphaF())
refc = (128, 153, 179, 255)
refcF = (0.5, 0.6, 0.7, 1.0)
for i in range(4):
    if not (corgb[i] == refc[i] and math.isclose(corgbF[i], refcF[i], rel_tol=1e-04)):
        raise RuntimeError

c = cc.QColor.fromRgb(160, 192, 224, 128)
if c.name() != '#a0c0e0':
    raise RuntimeError
if c.nameARGB() != '#80a0c0e0':
    raise RuntimeError
crgb = c.rgb()
crgba = c.rgba()
d =cc.QColor.fromRgbUInt(crgb)
if d.name() != '#a0c0e0':
    raise RuntimeError
if d.alpha() != 255:
    raise RuntimeError
e =cc.QColor.fromRgbUInt(crgba)
if e.name() != '#a0c0e0':
    raise RuntimeError
if e.alpha() != 255:
    raise RuntimeError
f = cc.QColor.fromRgba(crgb)
if f.name() != '#a0c0e0':
    raise RuntimeError
if f.alpha() != 255:
    raise RuntimeError
g = cc.QColor.fromRgba(crgba)
if g.name() != '#a0c0e0':
    raise RuntimeError
if g.alpha() != 128:
    raise RuntimeError

c = cc.QColor.fromRgba64(160*256, 192*256, 224*256, 128*256)
if (c.nameARGB() != '#80a0c0e0') and (c.nameARGB() != '#809fbfdf'):
    raise RuntimeError

c64 = cc.QRgba64.fromRgba64(160*256, 192*256, 224*256, 128*256)
if 'cloudComPy.QRgba64' not in c64.__repr__():
    raise RuntimeError
c = cc.QColor.fromRgba64Q(c64)
if (c.nameARGB() != '#80a0c0e0') and (c.nameARGB() != '#809fbfdf'):
    raise RuntimeError

cCmyk = cc.QColor.fromCmykF(0.5, 0.6, 0.7, 0.8)
comyk = cCmyk.getCmyk()
if comyk != [128, 153, 179, 204, 255]:
    raise RuntimeError
comykF = cCmyk.getCmykF()
refF = (0.5, 0.6, 0.7, 0.8, 1.0)
for i in range(5):
    if not (math.isclose(comykF[i], refF[i], rel_tol=1e-04)):
        raise RuntimeError

cHsl = cc.QColor.fromHslF(0.5, 0.6, 0.7)
cohsl = cHsl.getHsl()
if cohsl != [180, 153, 179, 255]:
    raise RuntimeError
cohslF = cHsl.getHslF()
refF = (0.5, 0.6, 0.7, 1.0)
for i in range(4):
    if not (math.isclose(cohslF[i], refF[i], rel_tol=1e-04)):
        raise RuntimeError

cHsv = cc.QColor.fromHsvF(0.5, 0.6, 0.7)
cohsv = cHsl.getHsv()
if (cohsv != [180, 104, 225, 255]) and (cohsv != [180, 104, 224, 255]):
    raise RuntimeError
cohsvF = cHsv.getHsvF()
refF = (0.5, 0.6, 0.7, 1.0)
for i in range(4):
    if not (math.isclose(cohsvF[i], refF[i], rel_tol=1e-04)):
        raise RuntimeError

cRgb = cc.QColor.fromRgbF(0.5, 0.6, 0.7)
corgb = cRgb.getRgb()
if corgb != [128, 153, 179, 255]:
    raise RuntimeError
corgbF = cRgb.getRgbF()
refF = (0.5, 0.6, 0.7, 1.0)
for i in range(4):
    if not (math.isclose(corgbF[i], refF[i], rel_tol=1e-04)):
        raise RuntimeError

if not cc.QColor.isValidColor('darkBlue'):
    raise RuntimeError
if not cc.QColor.isValidColor('#FFC53ED7'):
    raise RuntimeError

cRgb = cc.QColor.fromRgbF(0.4, 0.6, 0.2)
cl = cRgb.lighter()
corgbF = cl.getRgbF()
refF = [0.6, 0.9, 0.3, 1.0]
for i in range(4):
    if not (math.isclose(corgbF[i], refF[i], rel_tol=1e-04)):
        raise RuntimeError

cRgb = cc.QColor.fromRgbF(0.4, 0.6, 0.2)
c64 = cRgb.rgba64() 
comp64 = (c64.red(), c64.green(), c64.blue(), c64.alpha())
ref64 = (26214, 39321, 13107, 65535)
if comp64 != ref64:
    raise RuntimeError

cref = cc.QColor.fromCmyk(120, 80, 100, 160)
c= cc.QColor()
c.setCmyk(120, 80, 100, 160)
if c != cref:
    raise RuntimeError

cref = cc.QColor.fromCmykF(0.5, 0.6, 0.7, 0.8)
c= cc.QColor()
c.setCmykF(0.5, 0.6, 0.7, 0.8)
if c != cref:
    raise RuntimeError

cref = cc.QColor.fromHsl(120, 80, 100)
c= cc.QColor()
c.setHsl(120, 80, 100)
if c != cref:
    raise RuntimeError

cref = cc.QColor.fromHslF(0.5, 0.6, 0.7)
c= cc.QColor()
c.setHslF(0.5, 0.6, 0.7)
if c != cref:
    raise RuntimeError

cref = cc.QColor.fromHsv(120, 80, 100)
c= cc.QColor()
c.setHsv(120, 80, 100)
if c != cref:
    raise RuntimeError

cref = cc.QColor.fromHsvF(0.5, 0.6, 0.7)
c= cc.QColor()
c.setHsvF(0.5, 0.6, 0.7)
if c != cref:
    raise RuntimeError

c= cc.QColor()
c.setNamedColor('#5ABF3C')
if c.name() != '#5abf3c':
    raise RuntimeError

c.setNamedColor('turquoise')
if c.name() != '#40e0d0':
    raise RuntimeError

d = cc.QColor()
d.setRgbUInt(c.rgb())
if d != c:
    raise RuntimeError

d = cc.QColor()
d.setRgba64(c.rgba64())
if d != c:
    raise RuntimeError

c = cc.QColor.fromRgb(160, 180, 200)
d = cc.QColor()
d.setRgb(160, 180, 200)
if d != c:
    raise RuntimeError

c = cc.QColor.fromRgbF(0.5, 0.6, 0.7)
d = cc.QColor()
d.setRgbF(0.5, 0.6, 0.7)
if d != c:
    raise RuntimeError

c = cc.QColor.fromRgb(160, 180, 200)
d = c.toCmyk()
if d.name() != '#a0b4c8' or d.spec() != cc.QColorSpec.Cmyk:
    raise RuntimeError

c = cc.QColor.fromRgb(160, 180, 200)
d = c.toHsl()
if d.name() != '#a0b4c8' or d.spec() != cc.QColorSpec.Hsl:
    raise RuntimeError

c = cc.QColor.fromRgb(160, 180, 200)
d = c.toHsv()
if d.name() != '#a0b4c8' or d.spec() != cc.QColorSpec.Hsv:
    raise RuntimeError

c = cc.QColor.fromHsl(160, 180, 200)
d = c.toRgb()
if d.name() != '#a1efd5' or d.spec() != cc.QColorSpec.Rgb:
    raise RuntimeError
