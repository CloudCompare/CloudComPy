import os
import sys
import math

import numpy as np
try:
    import quaternion
except:
    print("Test skipped")
    sys.exit()


os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces
import cloudComPy as cc

# --- from ccGLMatrix to quaternion

#---quaternion01-begin
tr1 = cc.ccGLMatrixd()
tr1.initFromParameters(math.pi/3., (1., 1., 1.), (0,0,0)) # 60°
q1 = quaternion.as_quat_array(tr1.toQuaternion())
rot1 = quaternion.as_rotation_matrix(q1)
#---quaternion01-end

# --- direct build of a quaternion

#---quaternion02-begin
def normalized_quaternion(theta, ux, uy, uz):
    norm = math.sqrt(ux*ux + uy*uy +uz*uz)
    q1 = math.cos(theta/2.)
    q2 = ux*math.sin(theta/2.)/norm
    q3 = uy*math.sin(theta/2.)/norm
    q4 = uz*math.sin(theta/2.)/norm
    return np.quaternion(q1, q2, q3 ,q4)
#---quaternion02-end

qa = normalized_quaternion(math.pi/3., 1., 1., 1.) # 60°
rota = quaternion.as_rotation_matrix(qa)

# --- same result

qeps = qa-q1
if qeps.abs() > 1.e-7:
    raise RuntimeError

# --- product of quaternions: composition of rotations

qb = qa*qa                                         # 120°
rotb = quaternion.as_rotation_matrix(qb)

# --- from quaternion to ccGLMatrix

#---quaternion03-begin
trb = cc.ccGLMatrixd.FromQuaternionAndTranslation(qb.components)
datb = trb.getParameters1()
angle = datb.alpha_rad*180/math.pi
axis = datb.axis3D
#---quaternion03-end

if not math.isclose(angle, 120):
    raise RuntimeError

# ---

qc = qb*qb                                         # 240°
rotc = quaternion.as_rotation_matrix(qc)

qd = qb*qc                                         # 360°
rotd = quaternion.as_rotation_matrix(qd)

# --- check simple precision

tr2 = cc.ccGLMatrix()
tr2.initFromParameters(math.pi/3., (1., 1., 1.), (0,0,0))
q2 = quaternion.as_quat_array(tr2.toQuaternion())
rot2 = quaternion.as_rotation_matrix(q2)

qeps2 = qa-q2
if qeps2.abs() > 1.e-7:
    raise RuntimeError


qb2 = q2*q2
trb2 = cc.ccGLMatrix.FromQuaternionAndTranslation(qb2.components)
datb2 = trb2.getParameters1()

angle2 = datb2.alpha_rad*180/math.pi
if not math.isclose(angle2, 120, rel_tol=1.e-7):
    raise RuntimeError


